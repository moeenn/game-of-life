#include "raylib.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint16_t win_width;
  uint16_t win_height;
  const char *win_title;
  uint16_t fps;
  Color bg_norm;
  uint16_t grid_size;
  Color line_color;
} GameConfig;

#define MATRIX_SIZE 100

typedef struct {
  float grid[MATRIX_SIZE][MATRIX_SIZE];
} GameState;

typedef struct {
  Vector2 top_left;
  Vector2 bottom_right;
} CellPosition;

Color game_float_to_color(float color) {
  unsigned char val = (unsigned char)255 * color;
  struct Color c = {val, val, val, 255};
  return c;
}

CellPosition game_get_cell_position(uint16_t win_size, uint16_t grid_size,
                                    uint16_t x, uint16_t y) {
  uint16_t cell_size = win_size / grid_size;

  struct Vector2 top_left = {
      .x = (float)x * cell_size,
      .y = (float)y * cell_size,
  };

  struct Vector2 bottom_right = {
      .x = top_left.x + cell_size,
      .y = top_left.y + cell_size,
  };

  CellPosition p = {
      .top_left = top_left,
      .bottom_right = bottom_right,
  };

  return p;
}

void game_fill_cell(Vector2 top_left, Vector2 bottom_right, float color) {
  DrawRectangle(top_left.x, top_left.y, top_left.x - bottom_right.x,
                top_left.y - bottom_right.y, game_float_to_color(color));
}

void game_draw_grid_lines(GameConfig *c) {
  uint16_t cell_size = c->win_width / c->grid_size;

  for (uint16_t i = 0; i < c->win_width; i++) {
    if (i % cell_size == 0) {
      // draw vertical and horizontal lines respectively
      DrawLine(i, 0, i, c->win_height, c->line_color);
      DrawLine(0, i, c->win_width, i, c->line_color);
    }
  }
}

void game_render_cells(GameConfig *c, GameState *s) {
  for (uint16_t y = 0; y < MATRIX_SIZE; y++) {
    for (uint16_t x = 0; x < MATRIX_SIZE; x++) {
      float cell = s->grid[y][x];
      if (cell != 0.0f) {
        CellPosition position =
            game_get_cell_position(c->win_width, c->grid_size, x, y);
        game_fill_cell(position.top_left, position.bottom_right, cell);
      }
    }
  }
}

int main() {
  GameConfig config = {
      .win_width = 900,
      .win_height = 900,
      .win_title = "Game",
      .fps = 60,
      .bg_norm = BLACK,
      .grid_size = MATRIX_SIZE,
      .line_color = {30, 30, 30, 255},
  };

  // TODO: initialize with size and default value
  GameState state = {
      .grid = {{0}},
  };

  // TODO: remove after testing
  state.grid[3][3] = 0.8;
  state.grid[10][10] = 0.3;

  InitWindow(config.win_width, config.win_height, config.win_title);
  SetTargetFPS(config.fps);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(config.bg_norm);
    game_draw_grid_lines(&config);
    game_render_cells(&config, &state);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}