use ggez::{Context, ContextBuilder, GameResult, timer};
use ggez::graphics::{self, Color, DrawMode, Mesh};
use rand::Rng;
use ggez::event::{self, EventHandler};
use ggez::input::keyboard::{KeyCode, KeyInput};
use glam::{Vec2, vec2};

#[derive(Clone)]
struct Projectile {
    pos: Vec2,
    segment: usize,
    progress: f32, // 1.0 = outer, 0.0 = inner
}

impl Projectile {
    fn new(segment: usize, web_points_outer: &[Vec2]) -> Self {
        Projectile {
            pos: web_points_outer[segment],
            segment,
            progress: 1.0,
        }
    }
}
use std::f32::consts::PI;

const NUM_SEGMENTS: usize = 16;
const INNER_RADIUS: f32 = 100.0;
const OUTER_RADIUS: f32 = 300.0;
const ENEMY_SPEED: f32 = 100.0;
const SPAWN_INTERVAL: f32 = 2.0; // Spawn enemy every 2 seconds
const COLLISION_RADIUS: f32 = 15.0; // Collision detection radius
const PROJECTILE_SPEED: f32 = 400.0;

#[derive(Clone)]
struct Enemy {
    pos: Vec2,
    segment: usize,
    progress: f32, // 0.0 = inner, 1.0 = outer
}

impl Enemy {
    fn new(segment: usize, web_points_inner: &[Vec2]) -> Self {
        Enemy {
            pos: web_points_inner[segment],
            segment,
            progress: 0.0,
        }
    }
}

struct GameState {
    player_pos: Vec2,
    player_segment: usize,
    web_points_inner: Vec<Vec2>,
    web_points_outer: Vec<Vec2>,
    enemies: Vec<Enemy>,
    projectiles: Vec<Projectile>,
    spawn_timer: f32,
    game_over: bool,
}

impl GameState {
    fn new() -> Self {
        let mut web_points_inner = Vec::with_capacity(NUM_SEGMENTS);
        let mut web_points_outer = Vec::with_capacity(NUM_SEGMENTS);
        
        // Calculate points for the web structure
        for i in 0..NUM_SEGMENTS {
            let angle = (i as f32 * 2.0 * PI) / NUM_SEGMENTS as f32;
            
            // Calculate inner ring points
            let x = 400.0 + angle.cos() * INNER_RADIUS;
            let y = 300.0 + angle.sin() * INNER_RADIUS;
            web_points_inner.push(vec2(x, y));
            
            // Calculate outer ring points
            let x = 400.0 + angle.cos() * OUTER_RADIUS;
            let y = 300.0 + angle.sin() * OUTER_RADIUS;
            web_points_outer.push(vec2(x, y));
        }

        GameState {
            player_pos: Vec2::new(400.0 + OUTER_RADIUS, 300.0), // Start at rightmost outer point
            player_segment: 0,
            web_points_inner,
            web_points_outer,
            enemies: Vec::new(),
            projectiles: Vec::new(),
            spawn_timer: 0.0,
            game_over: false,
        }
    }

    fn draw_web(&self, ctx: &mut Context) -> GameResult<Vec<Mesh>> {
        let mut meshes = Vec::new();
        
        // Draw the segments
        for i in 0..NUM_SEGMENTS {
            // Draw lines from inner to outer points
            let line = Mesh::new_line(
                ctx,
                &[self.web_points_inner[i], self.web_points_outer[i]],
                2.0,
                Color::BLUE,
            )?;
            meshes.push(line);
            
            // Draw inner ring segments
            let next_i = (i + 1) % NUM_SEGMENTS;
            let inner_segment = Mesh::new_line(
                ctx,
                &[self.web_points_inner[i], self.web_points_inner[next_i]],
                2.0,
                Color::GREEN,
            )?;
            meshes.push(inner_segment);
            
            // Draw outer ring segments
            let outer_segment = Mesh::new_line(
                ctx,
                &[self.web_points_outer[i], self.web_points_outer[next_i]],
                2.0,
                Color::RED,
            )?;
            meshes.push(outer_segment);
        }
        
        Ok(meshes)
    }
}

impl EventHandler for GameState {
    fn update(&mut self, ctx: &mut Context) -> GameResult {
        if self.game_over {
            return Ok(());
        }

        let dt = timer::delta(ctx).as_secs_f32();

        // Check collisions
        for enemy in &self.enemies {
            let distance = (enemy.pos - self.player_pos).length();
            if distance < COLLISION_RADIUS {
                self.game_over = true;
                return Ok(());
            }
        }
        
        // Update spawn timer
        self.spawn_timer += dt;
        if self.spawn_timer >= SPAWN_INTERVAL {
            self.spawn_timer = 0.0;
            let mut rng = rand::thread_rng();
            let segment = rng.gen_range(0..NUM_SEGMENTS);
            self.enemies.push(Enemy::new(segment, &self.web_points_inner));
        }
        
        // Update projectile positions
        for projectile in &mut self.projectiles {
            projectile.progress -= PROJECTILE_SPEED * dt / (OUTER_RADIUS - INNER_RADIUS);
            
            let start = self.web_points_outer[projectile.segment];
            let end = self.web_points_inner[projectile.segment];
            projectile.pos = start.lerp(end, 1.0 - projectile.progress);
        }

        // Check projectile-enemy collisions
        let mut destroyed_enemies = Vec::new();
        let mut destroyed_projectiles = Vec::new();
        
        for (proj_idx, projectile) in self.projectiles.iter().enumerate() {
            for (enemy_idx, enemy) in self.enemies.iter().enumerate() {
                let distance = (enemy.pos - projectile.pos).length();
                if distance < COLLISION_RADIUS {
                    destroyed_enemies.push(enemy_idx);
                    destroyed_projectiles.push(proj_idx);
                }
            }
        }

        // Remove destroyed entities
        for &idx in destroyed_enemies.iter().rev() {
            self.enemies.remove(idx);
        }
        for &idx in destroyed_projectiles.iter().rev() {
            self.projectiles.remove(idx);
        }

        // Remove projectiles that reached the inner ring
        self.projectiles.retain(|proj| proj.progress > 0.0);

        // Update enemy positions
        for enemy in &mut self.enemies {
            enemy.progress += ENEMY_SPEED * dt / (OUTER_RADIUS - INNER_RADIUS);
            enemy.progress = enemy.progress.min(1.0);
            
            let start = self.web_points_inner[enemy.segment];
            let end = self.web_points_outer[enemy.segment];
            enemy.pos = start.lerp(end, enemy.progress);
        }
        
        // Remove enemies that reached the outer ring
        self.enemies.retain(|enemy| enemy.progress < 1.0);
        
        Ok(())
    }

    fn draw(&mut self, ctx: &mut Context) -> GameResult {
        let mut canvas = graphics::Canvas::from_frame(ctx, Color::BLACK);
        
        // Draw the web structure
        let web_meshes = self.draw_web(ctx)?;
        for mesh in web_meshes {
            canvas.draw(&mesh, graphics::DrawParam::default());
        }
        
        // Draw the player (triangular ship)
        let angle = (self.player_segment as f32 * 2.0 * PI) / NUM_SEGMENTS as f32;
        let direction = Vec2::new(angle.cos(), angle.sin());
        let perp = Vec2::new(-direction.y, direction.x);
        
        // Create U-shaped player like original Tempest
        let base_width = 16.0;
        let arm_length = 20.0;
        let arm_width = 3.0;
        
        // Calculate the base points of the U shape
        let base_left = self.player_pos - perp * (base_width/2.0);
        let base_right = self.player_pos + perp * (base_width/2.0);
        
        // Calculate the arm points
        let left_outer = base_left + direction * arm_length;
        let left_inner = base_left + direction * arm_length - perp * arm_width;
        let right_outer = base_right + direction * arm_length;
        let right_inner = base_right + direction * arm_length + perp * arm_width;
        
        let player_points = [
            base_left,
            left_outer,
            left_inner,
            right_inner,
            right_outer,
            base_right,
        ];
        
        let player = graphics::Mesh::new_polygon(
            ctx,
            DrawMode::fill(),
            &player_points,
            Color::WHITE,
        )?;
        
        canvas.draw(&player, graphics::DrawParam::default());
        
        // Draw projectiles
        for projectile in &self.projectiles {
            let projectile_mesh = graphics::Mesh::new_circle(
                ctx,
                DrawMode::fill(),
                projectile.pos,
                5.0,
                0.1,
                Color::CYAN,
            )?;
            canvas.draw(&projectile_mesh, graphics::DrawParam::default());
        }

        // Draw enemies
        for enemy in &self.enemies {
            // Create Tempest-style "flipper" enemy shape
            let angle = (enemy.segment as f32 * 2.0 * PI) / NUM_SEGMENTS as f32;
            let direction = Vec2::new(angle.cos(), angle.sin());
            let perp = Vec2::new(-direction.y, direction.x);
            
            let size = 12.0;
            let width = 8.0;
            
            // Calculate points for angular flipper shape
            let enemy_points = [
                enemy.pos + direction * size,  // front point
                enemy.pos + perp * width - direction * (size/2.0),  // left wing
                enemy.pos - direction * (size/2.0),  // back point
                enemy.pos - perp * width - direction * (size/2.0),  // right wing
            ];
            
            let enemy_mesh = graphics::Mesh::new_polygon(
                ctx,
                DrawMode::fill(),
                &enemy_points,
                Color::YELLOW,
            )?;
            canvas.draw(&enemy_mesh, graphics::DrawParam::default());
        }
        
        // Draw game over text if needed
        if self.game_over {
            let text = graphics::Text::new("Game Over!");
            let dest = Vec2::new(350.0, 250.0);
            canvas.draw(&text, graphics::DrawParam::default().dest(dest).color(Color::RED));
        }

        canvas.finish(ctx)?;
        Ok(())
    }

    fn key_down_event(&mut self, _ctx: &mut Context, input: KeyInput, _repeat: bool) -> GameResult {
        if self.game_over {
            return Ok(());
        }

        match input.keycode {
            Some(KeyCode::Space) => {
                self.projectiles.push(Projectile::new(self.player_segment, &self.web_points_outer));
            },
            Some(KeyCode::Left) => {
                self.player_segment = (self.player_segment + 1) % NUM_SEGMENTS;
                let new_pos = self.web_points_outer[self.player_segment];
                self.player_pos = new_pos;
            },
            Some(KeyCode::Right) => {
                self.player_segment = (self.player_segment + NUM_SEGMENTS - 1) % NUM_SEGMENTS;
                let new_pos = self.web_points_outer[self.player_segment];
                self.player_pos = new_pos;
            },
            _ => (),
        }
        Ok(())
    }
}

fn main() -> GameResult {
    let (ctx, event_loop) = ContextBuilder::new("tempest", "Your Name")
        .window_setup(ggez::conf::WindowSetup::default().title("Tempest"))
        .window_mode(ggez::conf::WindowMode::default().dimensions(800.0, 600.0))
        .build()?;

    let state = GameState::new();
    event::run(ctx, event_loop, state)
}
