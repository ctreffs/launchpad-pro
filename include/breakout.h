//
//  breakout.h
//  simulator
//
//  Created by Christian Treffs on 08.11.19.
//  Copyright © 2019 Focusrite Audio Engineering Ltd. All rights reserved.
//

#ifndef breakout_h
#define breakout_h

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

#define COLOR_PLATE 0x00ff00

// KEYBINDINGS
#define MOVE_RIGHT 19 // index 19 app_button_event
#define MOVE_LEFT 10 // index 10 app_button_event

#define GRID_BOTTOM_LEFT 11
#define GRID_BOTTOM_RIGHT 18
#define BOTTOM_LEFT 1

// We have a 8x9 grid
#define WIDTH 8
#define HEIGHT 9

#define EMPTY 0
#define BALL 1
#define PLATE 2

#define TEST 255

typedef unsigned char FIELD;
typedef unsigned char BOARD[];

#define BOARD_SIZE 72 // WIDTH * HEIGHT
unsigned char board[BOARD_SIZE]; // DEFINE BOARD ARRAY

void debug_board(const BOARD board);

void make_board(BOARD board);

int get_index(const int x, const int y);
int get_next_x(const int x);
int get_next_y(const int y);

void set_field(BOARD board, const int x, const int y, const FIELD value);
FIELD get_field(const BOARD board, const int x, const int y);

void set_plate(BOARD board, const int middle);
int get_plate(const BOARD board);

void move_plate_left(BOARD board);
void move_plate_right(BOARD board);

void advanceRunningLight(BOARD board);

void draw(const BOARD board);

void update_game_state();

#endif /* breakout_h */
