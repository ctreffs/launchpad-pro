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
    
    for (y = 0; y < HEIGHT ; y++){
        for (x = 0; x < WIDTH ; x++) {
            i = y * WIDTH + x;
            PRINTF("%d ", board[i]);
        }
        PRINTF("\n");
    }
    PRINTF("\n");
}


void make_board(BOARD board) {
    frame_count = 0;
    game_speed = 300;
    
    int i;
    
    // initialize board empty
    for (i = 0; i < BOARD_SIZE ; i++){
        board[i] = EMPTY;
    }
    
    // set plate
    set_plate(board, 4);
    
}

int get_index(const int x, const int y) {
    return y * WIDTH + x;
}

void set_field(BOARD board, const int x, const int y, const FIELD value) {
    board[get_index(x, y)] = value;
}

int get_next_x(const int x) {
    return (x+1) % WIDTH;
}

int get_next_y(const int y) {
    return (y+1) % HEIGHT;
}

FIELD get_field(const BOARD board, const int x, const int y) {
    return board[get_index(x, y)];
}

void set_plate(BOARD board, const int middle) {
    set_field(board, middle-1, HEIGHT-1, PLATE);
    set_field(board, middle+0, HEIGHT-1, PLATE);
    set_field(board, middle+1, HEIGHT-1, PLATE);
}

int get_plate(const BOARD board) {
    int x = 0;
    
    while (get_field(board, x, HEIGHT-1) != PLATE) {
        ++x;
    }
    return ++x;
}

void move_plate_left(BOARD board) {
    FIELD middle = get_plate(board);
    if (middle > 1) {
        set_plate(board, middle-1);
        set_field(board, middle+1, HEIGHT-1, EMPTY);
    }
}

void move_plate_right(BOARD board) {
    FIELD middle = get_plate(board);
    if (middle < WIDTH-2) {
        set_plate(board, middle+1);
        set_field(board, middle-1, HEIGHT-1, EMPTY);
    }
}

void increase_game_speed() {
    game_speed += 10;
}

void decrease_game_speed() {
    long newSpeed = game_speed - 10;
    if (newSpeed <= 0) {
        game_speed = 0;
        return;
    }
    game_speed = newSpeed;
}

// TODO: remove since this is only debug
int x = 0;
void advanceRunningLight(BOARD board) {
    board[get_index(x, 2)] = EMPTY;
    x = get_next_x(x);
    board[get_index(x, 2)] = TEST;
}

int game_state_needs_update() {
    if (frame_count == game_speed) {
        frame_count = 0;
        return 1;
    }
    frame_count++;
    return 0;
}

// update game state
void update_game_state() {
    advanceRunningLight(board);
}

void draw(const BOARD board) {
    // clear screen
    display_fill_all(COLOR_BLACK);
    
    // draw plate
    FIELD plate_middle = get_plate(board);
    display_plot_led(BOTTOM_LEFT + plate_middle-1, COLOR_PLATE);
    display_plot_led(BOTTOM_LEFT + plate_middle+0, COLOR_PLATE);
    display_plot_led(BOTTOM_LEFT + plate_middle+1, COLOR_PLATE);
    
    // draw running light
  
    int x = 0;
    for (x = 0; x < WIDTH ; x++){
        
        switch (board[get_index(x, 2)]) {
        case TEST:
            display_plot_led(61+x, COLOR_BLUE);
            break;
        default:
            break;
        }
    }
}
