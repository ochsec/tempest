use ggez::{Context, ContextBuilder, GameResult};
use ggez::graphics::{self, Color, DrawMode, Mesh};
use ggez::event::{self, EventHandler};
use ggez::input::keyboard::{KeyCode, KeyInput};
use glam::{Vec2, vec2};
use std::f32::consts::PI;

const NUM_SEGMENTS: usize = 16;
const INNER_RADIUS: f32 = 100.0;
const OUTER_RADIUS: f32 = 300.0;

struct GameState {
    player_pos: Vec2,
    player_segment: usize,
    web_points_inner: Vec<Vec2>,
    web_points_outer: Vec<Vec2>,
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
    fn update(&mut self, _ctx: &mut Context) -> GameResult {
        Ok(())
    }

    fn draw(&mut self, ctx: &mut Context) -> GameResult {
        let mut canvas = graphics::Canvas::from_frame(ctx, Color::BLACK);
        
        // Draw the web structure
        let web_meshes = self.draw_web(ctx)?;
        for mesh in web_meshes {
            canvas.draw(&mesh, graphics::DrawParam::default());
        }
        
        // Draw the player
        let player = graphics::Mesh::new_circle(
            ctx,
            DrawMode::fill(),
            self.player_pos,
            10.0,
            0.1,
            Color::WHITE,
        )?;
        
        canvas.draw(&player, graphics::DrawParam::default());
        canvas.finish(ctx)?;
        Ok(())
    }

    fn key_down_event(&mut self, _ctx: &mut Context, input: KeyInput, _repeat: bool) -> GameResult {
        match input.keycode {
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
