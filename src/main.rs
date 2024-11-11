use ggez::{Context, ContextBuilder, GameResult};
use ggez::graphics::{self, Color};
use ggez::event::{self, EventHandler};
use ggez::input::keyboard::{KeyCode, KeyInput};
use glam::Vec2;

struct GameState {
    player_pos: Vec2,
}

impl GameState {
    fn new() -> Self {
        GameState {
            player_pos: Vec2::new(400.0, 300.0),
        }
    }
}

impl EventHandler for GameState {
    fn update(&mut self, _ctx: &mut Context) -> GameResult {
        Ok(())
    }

    fn draw(&mut self, ctx: &mut Context) -> GameResult {
        let mut canvas = graphics::Canvas::from_frame(ctx, Color::BLACK);
        
        // Draw a simple rectangle for now as the player
        let rect = graphics::Mesh::new_rectangle(
            ctx,
            graphics::DrawMode::fill(),
            graphics::Rect::new(self.player_pos.x, self.player_pos.y, 30.0, 30.0),
            Color::WHITE,
        )?;
        
        canvas.draw(&rect, graphics::DrawParam::default());
        canvas.finish(ctx)?;
        Ok(())
    }

    fn key_down_event(&mut self, _ctx: &mut Context, input: KeyInput, _repeat: bool) -> GameResult {
        match input.keycode {
            Some(KeyCode::Left) => self.player_pos.x -= 5.0,
            Some(KeyCode::Right) => self.player_pos.x += 5.0,
            Some(KeyCode::Up) => self.player_pos.y -= 5.0,
            Some(KeyCode::Down) => self.player_pos.y += 5.0,
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
