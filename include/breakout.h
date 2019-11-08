//
//  breakout.h
//  simulator
//
//  Created by Christian Treffs on 08.11.19.
//  Copyright © 2019 Focusrite Audio Engineering Ltd. All rights reserved.
//

#ifndef breakout_h
#define breakout_h

// COLORS
#define RED 0xff0000;
#define GREEN 0x00ff00;
#define GREEN 0x00ff00;


// We have a 8x9 grid
#define WIDTH 8
#define HEIGTH 9

#define EMPTY 0
#define BALL 0
#define PLATE 2

typedef unsigned int FIELD;
typedef unsigned int BOARD[];

#define BOARD_SIZE 72 // WIDTH * HEIGHT


void debug_board(const BOARD board);

void make_board(BOARD board);

void set_field(BOARD board, const int x, const int y, const FIELD value);
FIELD get_field(const BOARD board, const int x, const int y);

void set_plate(BOARD board, const int middle);
int get_plate(const BOARD board);

void move_plate_left(BOARD board);
void move_plate_right(BOARD board);

void draw(const BOARD board);

#endif /* breakout_h */
