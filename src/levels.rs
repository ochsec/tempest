#[derive(Clone, Copy)]
pub enum LevelShape {
    Circle,      // Standard circular layout
    Square,      // Square/diamond layout
    Plus,        // Plus-sign shape
    V,           // V-shaped layout
    Triangle,    // Triangle layout
    Cross,       // X-shaped layout
    Star,        // Star shape
    Spiral,      // Spiral layout
    Pentagon,    // Pentagon layout
    Octagon,     // Octagon layout
}

pub struct LevelConfig {
    pub num_segments: usize,
    pub inner_radius: f32,
    pub outer_radius: f32,
    pub enemy_speed: f32,
    pub spawn_interval: f32,
    pub enemies_to_destroy: usize,
    pub shape: LevelShape,
}


impl LevelConfig {
    pub fn get_level(level: usize) -> Self {
        match level {
            0 => Self::level_1(),
            1 => Self::level_2(),
            2 => Self::level_3(),
            3 => Self::level_4(),
            4 => Self::level_5(),
            5 => Self::level_6(),
            6 => Self::level_7(),
            7 => Self::level_8(),
            8 => Self::level_9(),
            _ => Self::level_10(),
        }
    }

    fn level_1() -> Self {
        LevelConfig {
            num_segments: 16,
            inner_radius: 80.0,
            outer_radius: 250.0,
            enemy_speed: 100.0,
            spawn_interval: 2.0,
            enemies_to_destroy: 20,
            shape: LevelShape::Circle,
        }
    }

    fn level_2() -> Self {
        LevelConfig {
            num_segments: 32,
            inner_radius: 100.0,
            outer_radius: 270.0,
            enemy_speed: 120.0,
            spawn_interval: 1.8,
            enemies_to_destroy: 25,
            shape: LevelShape::Square,
        }
    }

    fn level_3() -> Self {
        LevelConfig {
            num_segments: 24,
            inner_radius: 120.0,
            outer_radius: 290.0,
            enemy_speed: 140.0,
            spawn_interval: 1.6,
            enemies_to_destroy: 30,
            shape: LevelShape::V,
        }
    }

    // ... Similar patterns for levels 4-10 with increasing difficulty
    // Level 4
    fn level_4() -> Self {
        LevelConfig {
            num_segments: 40,
            inner_radius: 140.0,
            outer_radius: 310.0,
            enemy_speed: 160.0,
            spawn_interval: 1.4,
            enemies_to_destroy: 35,
            shape: LevelShape::Triangle,
        }
    }

    // Levels 5-10 follow similar patterns with increasingly challenging configurations
    fn level_5() -> Self {
        LevelConfig {
            num_segments: 48,
            inner_radius: 160.0,
            outer_radius: 330.0,
            enemy_speed: 180.0,
            spawn_interval: 1.2,
            enemies_to_destroy: 40,
            shape: LevelShape::Cross,
        }
    }

    fn level_6() -> Self {
        LevelConfig {
            num_segments: 56,
            inner_radius: 180.0,
            outer_radius: 350.0,
            enemy_speed: 200.0,
            spawn_interval: 1.0,
            enemies_to_destroy: 45,
            shape: LevelShape::Star,
        }
    }

    fn level_7() -> Self {
        LevelConfig {
            num_segments: 64,
            inner_radius: 200.0,
            outer_radius: 370.0,
            enemy_speed: 220.0,
            spawn_interval: 0.9,
            enemies_to_destroy: 50,
            shape: LevelShape::Spiral,
        }
    }

    fn level_8() -> Self {
        LevelConfig {
            num_segments: 72,
            inner_radius: 220.0,
            outer_radius: 390.0,
            enemy_speed: 240.0,
            spawn_interval: 0.8,
            enemies_to_destroy: 55,
            shape: LevelShape::Pentagon,
        }
    }

    fn level_9() -> Self {
        LevelConfig {
            num_segments: 80,
            inner_radius: 240.0,
            outer_radius: 410.0,
            enemy_speed: 260.0,
            spawn_interval: 0.7,
            enemies_to_destroy: 60,
            shape: LevelShape::Circle,
        }
    }

    fn level_10() -> Self {
        LevelConfig {
            num_segments: 96,
            inner_radius: 260.0,
            outer_radius: 430.0,
            enemy_speed: 280.0,
            spawn_interval: 0.6,
            enemies_to_destroy: 70,
            shape: LevelShape::Octagon,
        }
    }
}
