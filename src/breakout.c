//
//  breakout.c
//  simulator
//
//  Created by Christian Treffs on 08.11.19.
//  Copyright © 2019 Focusrite Audio Engineering Ltd. All rights reserved.
//

#include "breakout.h"

void debug_board(const BOARD board) {
    int x, y, i;
    
    for (y = 0; y < HEIGTH ; y++){
        for (x = 0; x < WIDTH ; x++) {
            i = y * WIDTH + x;
            printf("%d ", board[i]);
        }
        printf("\n");
        
    }
}


void make_board(BOARD board) {
    int i;
    
    // initialize board empty
    for (i = 0; i < BOARD_SIZE ; i++){
        board[i] = EMPTY;
    }
    
}

void set_field(BOARD board, const int x, const int y, const FIELD value) {
    int i = y * WIDTH * x;
    board[i] = value;
}

