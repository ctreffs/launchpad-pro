//
//  breakout.h
//  simulator
//
//  Created by Christian Treffs on 08.11.19.
//  Copyright © 2019 Focusrite Audio Engineering Ltd. All rights reserved.
//

#ifndef breakout_h
#define breakout_h

#include <stdbool.h>
#define NULL ((void*)0)

#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...) { }
#endif

// COLORS
#define COLOR_BLACK 0x000000
#define COLOR_WHITE 0xffffff
#define COLOR_RED   0xff0000
#define COLOR_GREEN 0x00ff00
#define COLOR_BLUE  0x0000ff

#define COLOR_PLATE   0x00ff00
#define COLOR_BALL    0x0000ff
#define COLOR_CONTROL 0xff0000

// KEYBINDINGS
#define MOVE_RIGHT 19 // index 19 app_button_event
#define MOVE_LEFT 10 // index 10 app_button_event
#define FRAME_LIMIT_INC 92 // these are flipped to invert mapping - it's more intuitive
#define FRAME_LIMIT_DEC 91

#define GRID_BOTTOM_LEFT 11
#define GRID_BOTTOM_RIGHT 18
#define BOTTOM_LEFT 1

#define WIDTH 10
#define HEIGHT 10

#define EMPTY 0
#define WALL 1
#define CONTROL 2
#define PLATE 3
#define BALL 4


typedef unsigned char FIELD;
typedef unsigned char BOARD[];

typedef struct Vec2
{
    int x;
    int y;
} Vec2;


#define BOARD_SIZE 100 // WIDTH * HEIGHT
unsigned char board[BOARD_SIZE]; // DEFINE BOARD ARRAY
long frame_count;
long game_speed;

void debug_board(const BOARD board);

void make_board(BOARD board);

bool is_empty_field_vec(const BOARD board, const Vec2 vec);
bool is_empty_field(const BOARD board, const int index);
bool is_vec_on_board(const Vec2 vec);
bool is_index_out_of_bounds(const int index);
int get_index(const int x, const int y);
int get_index_vec(const Vec2 vec);
int get_next_x(const int x);
int get_next_y(const int y);

void set_field(BOARD board, const int x, const int y, const FIELD value);
FIELD get_field(const BOARD board, const int x, const int y);

void set_plate(BOARD board, const int middle);
int get_plate(const BOARD board);

void move_plate_left(BOARD board);
void move_plate_right(BOARD board);

void simulateBall(BOARD board);
void increase_game_speed();
void decrease_game_speed();

int get_draw_index(const int index);
void draw(const BOARD board);

int game_state_needs_update();
void update_game_state();


void get_new_direction(BOARD board, Vec2 *direction, Vec2 current_position);
void bounceQuadrant(Vec2* o, int u, int d, int r, Vec2* q);
void diagonalBounce(Vec2* o, int u, int d, int r, Vec2* q);
int is_neighbor(BOARD board, Vec2 pos);
void bounce(Vec2* dir, int n[8]);

#endif /* breakout_h */
