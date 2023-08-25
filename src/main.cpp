#include "raylib.h"
#include <array>
#include <assert.h>
#include <iostream>

namespace game {
struct Config {
  uint win_width;
  uint win_height;
  const char *win_title;
  uint fps;
  Color bg_norm;
  uint grid_size;
  Color line_color;
};

const uint MATRIX_SIZE = 100;
typedef std::array<std::array<float, MATRIX_SIZE>, MATRIX_SIZE> Matrix;

struct State {
  Matrix grid;
};

struct CellPosition {
  Vector2 top_left;
  Vector2 bottom_right;
};

Color float_to_color(float);
CellPosition get_cell_position(uint, uint, uint, uint);
void fill_cell(Vector2, Vector2, float);
void draw_grid_lines(const Config &);
void render_cells(const Config &, const State &);
}; // namespace game

Color game::float_to_color(float color) {
  unsigned char val = (unsigned char)255 * color;
  return Color{val, val, val, 255};
}

game::CellPosition game::get_cell_position(uint win_size, uint grid_size,
                                           uint x, uint y) {
  uint cell_size = win_size / grid_size;

  Vector2 top_left = Vector2{
      .x = (float)x * cell_size,
      .y = (float)y * cell_size,
  };

  Vector2 bottom_right = Vector2{
      .x = top_left.x + cell_size,
      .y = top_left.y + cell_size,
  };

  return game::CellPosition{
      .top_left = top_left,
      .bottom_right = bottom_right,
  };
}

void game::fill_cell(Vector2 top_left, Vector2 bottom_right, float color) {
  DrawRectangle(top_left.x, top_left.y, top_left.x - bottom_right.x,
                top_left.y - bottom_right.y, game::float_to_color(color));
}

void game::draw_grid_lines(const Config &c) {
  uint cell_size = c.win_width / c.grid_size;

  for (uint i = 0; i < c.win_width; i++) {
    if (i % cell_size == 0) {
      // draw vertical and horizontal lines respectively
      DrawLine(i, 0, i, c.win_height, c.line_color);
      DrawLine(0, i, c.win_width, i, c.line_color);
    }
  }
}

void game::render_cells(const game::Config &c, const game::State &s) {
  for (uint y = 0; y < s.grid.size(); y++) {
    for (uint x = 0; x < s.grid[y].size(); x++) {
      float cell = s.grid[y][x];
      if (cell != 0.0f) {
        game::CellPosition position =
            game::get_cell_position(c.win_width, c.grid_size, x, y);
        game::fill_cell(position.top_left, position.bottom_right, cell);
      }
    }
  }
}

int main() {
  game::Config config = {
      .win_width = 900,
      .win_height = 900,
      .win_title = "Game",
      .fps = 60,
      .bg_norm = BLACK,
      .grid_size = game::MATRIX_SIZE,
      .line_color = Color{30, 30, 30, 255},
  };

  // TODO: initialize with size and default value
  game::State state = {
      .grid = {},
  };

  // TODO: remove after testing
  state.grid[3][3] = 0.8;
  state.grid[10][10] = 0.3;

  InitWindow(config.win_width, config.win_height, config.win_title);
  SetTargetFPS(config.fps);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(config.bg_norm);
    game::draw_grid_lines(config);
    game::render_cells(config, state);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}