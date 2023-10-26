#include "game.h"

int main() {
  srand(time(0));

  GameConfig config = {
      .win_width = 900,
      .win_height = 900,
      .win_title = "Game",
      .fps = 15,
      .bg_norm = BLACK,
      .grid_size = MATRIX_SIZE,
      .line_color = {30, 30, 30, 255},
      .fill_color = {160, 160, 160, 255},
      .init_active = 400,
  };

  GameState state = {
      .grid = {{false}},
      .next = {{false}},
  };

  game_initialize_random_cells(&state, config.init_active);

  InitWindow(config.win_width, config.win_height, config.win_title);
  SetTargetFPS(config.fps);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(config.bg_norm);
    game_draw_grid_lines(&config);
    game_render_cells(&config, &state);
    game_update_matrix_state(&state);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
