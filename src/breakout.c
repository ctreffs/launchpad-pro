//
//  breakout.c
//  simulator
//
//  Created by Christian Treffs on 08.11.19.
//  Copyright © 2019 Focusrite Audio Engineering Ltd. All rights reserved.
//

#include "app.h"
#include "breakout.h"

void debug_board(const BOARD board) {
    int x, y, i;
    
    for (y = 0; y < HEIGTH ; y++){
        for (x = 0; x < WIDTH ; x++) {
            i = y * WIDTH + x;
            PRINTF("%d ", board[i]);
        }
        PRINTF("\n");
    }
    PRINTF("\n");
}


void make_board(BOARD board) {
    int i;
    
    // initialize board empty
    for (i = 0; i < BOARD_SIZE ; i++){
        board[i] = EMPTY;
    }
    
    // set plate
    set_plate(board, 4);
    
}

void set_field(BOARD board, const int x, const int y, const FIELD value) {
    int i = y * WIDTH + x;
    board[i] = value;
}

FIELD get_field(const BOARD board, const int x, const int y) {
    int i = y * WIDTH + x;
    return board[i];
}

void set_plate(BOARD board, const int middle) {
    set_field(board, middle-1, HEIGTH-1, PLATE);
    set_field(board, middle+0, HEIGTH-1, PLATE);
    set_field(board, middle+1, HEIGTH-1, PLATE);
}

int get_plate(const BOARD board) {
    int x = 0;
    
    while (get_field(board, x, HEIGTH-1) != PLATE) {
        ++x;
    }
    return ++x;
}

void move_plate_left(BOARD board) {
    FIELD middle = get_plate(board);
    if (middle > 1) {
        set_plate(board, middle-1);
        set_field(board, middle+1, HEIGTH-1, EMPTY);
    }
}

void move_plate_right(BOARD board) {
    FIELD middle = get_plate(board);
    if (middle < WIDTH-2) {
        set_plate(board, middle+1);
        set_field(board, middle-1, HEIGTH-1, EMPTY);
    }
}

void draw(const BOARD board) {
    // clear screen
    display_fill_all(COLOR_BLACK);
    
    // draw plate
    FIELD plate_middle = get_plate(board);
    
    display_plot_led(BOTTOM_LEFT + plate_middle-1, COLOR_PLATE);
    display_plot_led(BOTTOM_LEFT + plate_middle+0, COLOR_PLATE);
    display_plot_led(BOTTOM_LEFT + plate_middle+1, COLOR_PLATE);
    //display_plot_led(plate_middle+1, COLOR_PLATE);
    //display_plot_led(plate_middle, COLOR_PLATE);
    //display_plot_led(plate_middle+1, COLOR_PLATE);
}
