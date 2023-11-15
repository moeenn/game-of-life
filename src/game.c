#include "game.h"

/**
 * not using raylibs GetRandomValue because it produces zero values on some 
 * systems 
 */
int random_int(int min, int max) {
  int v = rand();
  return (v % max) + min;  
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

void game_fill_cell(Vector2 top_left, Vector2 bottom_right, Color color) {
  DrawRectangle(top_left.x, top_left.y, top_left.x - bottom_right.x,
                top_left.y - bottom_right.y, color);
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

/** draw a fix-sized grid on the screen with lines as separators */
void game_render_cells(GameConfig *c, GameState *s) {
  for (uint16_t y = 0; y < MATRIX_SIZE; y++) {
    for (uint16_t x = 0; x < MATRIX_SIZE; x++) {
      bool cell = s->grid[y][x];
      if (cell) {
        CellPosition position =
            game_get_cell_position(c->win_width, c->grid_size, x, y);
        game_fill_cell(position.top_left, position.bottom_right, c->fill_color);
      }
    }
  }
}

/** count number of neighbors for a specified cell */
int game_count_cell_neighbors(GameState *state, uint16_t x, uint16_t y) {
  bool x_before = true;
  bool x_after = true;
  bool y_before = true;
  bool y_after = true;

  if (x == 0)
    x_before = false;
  if ((x - 1) > MATRIX_SIZE)
    x_after = false;
  if (y == 0)
    y_before = false;
  if ((y - 1) > MATRIX_SIZE)
    y_after = false;

  int n_count = 0;
  if (x_before == true && state->grid[x - 1][y] == true)
    n_count++;
  if (x_after == true && state->grid[x + 1][y] == true)
    n_count++;
  if (y_before == true && state->grid[x][y - 1] == true)
    n_count++;
  if (y_after == true && state->grid[x][y + 1] == true)
    n_count++;
  if (x_before == true && y_before == true && state->grid[x - 1][y - 1] == true)
    n_count++;
  if (x_after == true && y_after == true && state->grid[x + 1][y + 1] == true)
    n_count++;
  if (x_after == true && y_before == true && state->grid[x + 1][y - 1] == true)
    n_count++;
  if (x_before == true && y_after == true && state->grid[x - 1][y + 1] == true)
    n_count++;

  return n_count;
}

/** set n number of random cells as alive on the grid matrix */
void game_initialize_random_cells(GameState *state, int count) {
  int i, x, y;
  for (i = 0; i < count; i++) {
    x = random_int(0, MATRIX_SIZE);
    y = random_int(0, MATRIX_SIZE);
    state->grid[x][y] = true;
  }
}

/**
 * rules:
 * - If the cell is alive, then it stays alive if it has either 2 or 3 live
 * neighbors
 * - If the cell is dead, then it springs to life only in the case that it has 3
 * live neighbors
 * - If cell is alive but has 5 or more alive neighbors, it dies from congestion
 */
void game_update_cell_status(GameState *state, uint16_t x, uint16_t y) {
  int n_neighbors = game_count_cell_neighbors(state, x, y);
  bool is_alive = state->grid[x][y];

  if (is_alive == true && (n_neighbors == 2 || n_neighbors == 3)) {
    state->next[x][y] = true;
  } else if (is_alive == true && n_neighbors >= 5) {
     state->next[x][y] = false;
  } else if (is_alive == false && n_neighbors >= 3) {
    state->next[x][y] = true;
  } else {
    state->next[x][y] = false;
  }
}

void game_update_matrix_state(GameState *state) {
  uint16_t x, y;
  memcpy(state->next, state->grid, sizeof(bool) * MATRIX_SIZE * MATRIX_SIZE);

  for (x = 0; x < MATRIX_SIZE; x++) {
    for (y = 0; y < MATRIX_SIZE; y++) {
      game_update_cell_status(state, x, y);
    }
  }

  memcpy(state->grid, state->next, sizeof(bool) * MATRIX_SIZE * MATRIX_SIZE);
}
