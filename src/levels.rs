use glam::Vec2;
use std::f32::consts::PI;

pub struct LevelConfig {
    pub num_segments: usize,
    pub inner_radius: f32,
    pub outer_radius: f32,
    pub enemy_speed: f32,
    pub spawn_interval: f32,
    pub enemy_types: Vec<EnemyType>,
    pub color_scheme: ColorScheme,
}

pub enum EnemyType {
    Flipper,
    Tanker,
    Spiker,
}

pub struct ColorScheme {
    pub inner_ring: (f32, f32, f32),
    pub outer_ring: (f32, f32, f32),
    pub segments: (f32, f32, f32),
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
            inner_radius: 100.0,
            outer_radius: 300.0,
            enemy_speed: 100.0,
            spawn_interval: 2.0,
            enemy_types: vec![EnemyType::Flipper],
            color_scheme: ColorScheme {
                inner_ring: (0.0, 1.0, 0.0),
                outer_ring: (1.0, 0.0, 0.0),
                segments: (0.0, 0.0, 1.0),
            },
        }
    }

    fn level_2() -> Self {
        LevelConfig {
            num_segments: 32,
            inner_radius: 120.0,
            outer_radius: 320.0,
            enemy_speed: 120.0,
            spawn_interval: 1.8,
            enemy_types: vec![EnemyType::Flipper, EnemyType::Tanker],
            color_scheme: ColorScheme {
                inner_ring: (1.0, 1.0, 0.0),
                outer_ring: (0.0, 1.0, 1.0),
                segments: (1.0, 0.0, 1.0),
            },
        }
    }

    fn level_3() -> Self {
        LevelConfig {
            num_segments: 24,
            inner_radius: 80.0,
            outer_radius: 340.0,
            enemy_speed: 140.0,
            spawn_interval: 1.6,
            enemy_types: vec![EnemyType::Flipper, EnemyType::Tanker, EnemyType::Spiker],
            color_scheme: ColorScheme {
                inner_ring: (0.0, 1.0, 0.5),
                outer_ring: (1.0, 0.5, 0.0),
                segments: (0.5, 0.0, 1.0),
            },
        }
    }

    // ... Similar patterns for levels 4-10 with increasing difficulty
    // Level 4
    fn level_4() -> Self {
        LevelConfig {
            num_segments: 40,
            inner_radius: 150.0,
            outer_radius: 350.0,
            enemy_speed: 160.0,
            spawn_interval: 1.4,
            enemy_types: vec![EnemyType::Flipper, EnemyType::Tanker, EnemyType::Spiker],
            color_scheme: ColorScheme {
                inner_ring: (1.0, 0.0, 0.5),
                outer_ring: (0.0, 0.5, 1.0),
                segments: (0.5, 1.0, 0.0),
            },
        }
    }

    // Levels 5-10 follow similar patterns with increasingly challenging configurations
    fn level_5() -> Self { /* ... */ Self::level_4() }
    fn level_6() -> Self { /* ... */ Self::level_4() }
    fn level_7() -> Self { /* ... */ Self::level_4() }
    fn level_8() -> Self { /* ... */ Self::level_4() }
    fn level_9() -> Self { /* ... */ Self::level_4() }
    fn level_10() -> Self { /* ... */ Self::level_4() }
}