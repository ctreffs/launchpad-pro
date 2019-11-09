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
    int x, y;
    
    for (y = 0; y < HEIGHT ; y++){
        for (x = 0; x < WIDTH ; x++) {
            PRINTF("%d ", board[get_index(x, HEIGHT-1-y)]);
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
    return (y * WIDTH) + x;
}


int get_x(const int index) {
    return index % WIDTH;
}

int get_y(const int index) {
    return index / WIDTH;
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

int is_index_out_of_bounds(const int index) {
    if (index >= BOARD_SIZE || index <= 0 ) {
        return 1;
    }
    return 0;
}

int get_next_index(const int index, const Vec2 direction) {
    int x = get_x(index);
    int y = get_y(index);
    
    Vec2 d = direction;
    
    x = x + d.x;
    y = y + d.y;
    
    return get_index(x, y);
}

FIELD get_field(const BOARD board, const int x, const int y) {
    return board[get_index(x, y)];
}

void set_plate(BOARD board, const int middle) {
    set_field(board, middle-1, 0, PLATE);
    set_field(board, middle+0, 0, PLATE);
    set_field(board, middle+1, 0, PLATE);
}

int get_plate(const BOARD board) {
    int x = 0;
    
    while (get_field(board, x, 0) != PLATE) {
        ++x;
    }
    return ++x;
}

void move_plate_left(BOARD board) {
    FIELD middle = get_plate(board);
    if (middle > 1) {
        set_plate(board, middle-1);
        set_field(board, middle+1, 0, EMPTY);
    }
}

void move_plate_right(BOARD board) {
    FIELD middle = get_plate(board);
    if (middle < WIDTH-2) {
        set_plate(board, middle+1);
        set_field(board, middle-1, 0, EMPTY);
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
int idx = 90;
Vec2 dir = { 1, -1};
void advanceRunningLight(BOARD board) {
    board[idx] = EMPTY;
    idx = get_next_index(idx, dir);
    if (is_index_out_of_bounds(idx) == 1) {
        idx = 90;
    }
    
    board[idx] = TEST;
    
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
    
    int i = 0;
    // DRAW SCANLINE
    for (i = 0; i < BOARD_SIZE ; i++){
        
        switch (board[i]) {
            case EMPTY:
                display_plot_led(i, COLOR_BLACK);
                break;
            case BALL:
                display_plot_led(i, COLOR_BALL);
                break;
            case PLATE:
                display_plot_led(i, COLOR_PLATE);
                break;
            case TEST:
                display_plot_led(i, COLOR_BLUE);
                break;
            default:
                break;
                
        }
        
    }
}
