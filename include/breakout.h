//
//  breakout.h
//  simulator
//
//  Created by Christian Treffs on 08.11.19.
//  Copyright © 2019 Focusrite Audio Engineering Ltd. All rights reserved.
//

#ifndef breakout_h
#define breakout_h

// We have a 8x9 grid
#define WIDTH 8
#define HEIGTH 9



#define EMPTY 0
#define BALL 0
#define PLATE 2

typedef unsigned char* BOARD;

#define BOARD_SIZE 72 // WIDTH * HEIGHT
BOARD board[BOARD_SIZE];

void make_board(BOARD* board);
void debug_board(const BOARD* board);

#endif /* breakout_h */
