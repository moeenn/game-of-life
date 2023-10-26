#ifndef __GAME_H__
#define __GAME_H__

#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/**
 * structs and typedefs
 *
 */
typedef struct {
  uint16_t win_width;
  uint16_t win_height;
  const char *win_title;
  uint16_t fps;
  Color bg_norm;
  uint16_t grid_size;
  Color line_color;
  Color fill_color;
  int init_active;
} GameConfig;

#define MATRIX_SIZE 100
typedef bool CellMatrix[MATRIX_SIZE][MATRIX_SIZE];

typedef struct {
  CellMatrix grid;
  CellMatrix next;
} GameState;

typedef struct {
  Vector2 top_left;
  Vector2 bottom_right;
} CellPosition;

/**
 * function signatures
 *
 */
CellPosition game_get_cell_position(uint16_t win_size, uint16_t grid_size,
                                    uint16_t x, uint16_t y);
void game_fill_cell(Vector2 top_left, Vector2 bottom_right, Color color);
void game_draw_grid_lines(GameConfig *c);
void game_render_cells(GameConfig *c, GameState *s);
int game_count_cell_neighbors(GameState *state, uint16_t x, uint16_t y);
void game_initialize_random_cells(GameState *state, int count);
void game_update_cell_status(GameState *state, uint16_t x, uint16_t y);
void game_update_matrix_state(GameState *state);

#endif