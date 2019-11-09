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
    game_speed = 240;
    
    int i;
    
    // initialize board empty
    for (i = 0; i < BOARD_SIZE ; i++){
        board[i] = EMPTY;
    }
    
    // set plate
    set_plate(board, 5);
    
    // walls
    //set_field(board, 0, 0, WALL);
    //set_field(board, WIDTH-1, 0, WALL);
    set_field(board, 0, HEIGHT-1, WALL);
    set_field(board, WIDTH-1, HEIGHT-1, WALL);
    
    // controls
    set_field(board, 0, 1, CONTROL);
    set_field(board, WIDTH-1, 1, CONTROL);
    
    
    make_bricks(board, 4);
}

void make_bricks(BOARD board, const unsigned int brickSize) {
    
    int idx;
    int brickIdx = BRICK_MIN;
    
    int x, y;
    
    
    for (y = 5; y < HEIGHT; y++) {
        idx = get_index(0, y);
        
        for (x = 0; x < WIDTH; x++) {
            board[idx] = brickIdx + (x / brickSize);
            idx = get_next_index(idx, (Vec2){ 1, 0});
        }
        
        brickIdx += (WIDTH / brickSize) + 1;
    }
    
}

int get_index_vec(const Vec2 vec) {
    return get_index(vec.x, vec.y);
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

bool is_empty_field_vec(const BOARD board, const Vec2 vec) {
    return is_empty_field(board, get_index_vec(vec));
}

bool is_empty_field(const BOARD board, const int index) {
    if (is_index_out_of_bounds(index)) {
        return false;
    }
    
    switch (board[index]) {
        case EMPTY:
            return true;
        default:
            return false;
    }
    
}

bool is_vec_on_board(const Vec2 vec) {
    if (vec.x > WIDTH-1 || vec.x < 0) {
        return false;
    }
    
    if (vec.y > HEIGHT-1 || vec.y < 0) {
        return false;
    }
    
    return true;
}

bool is_index_out_of_bounds(const int index) {
    if (index >= BOARD_SIZE || index <= 0 ) {
        return true;
    }
    return false;
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
    if (middle > 2) {
        set_plate(board, middle-1);
        set_field(board, middle+1, 0, EMPTY);
    }
}

void move_plate_right(BOARD board) {
    FIELD middle = get_plate(board);
    if (middle < WIDTH-3) {
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

int idx = 15;
Vec2 dir = { -1, 1};
Vec2 pos = { 0, 0};
void simulateBall(BOARD board) {
    board[idx] = EMPTY;
    pos.x = get_x(idx);
    pos.y = get_y(idx);
    Vec2 col = get_new_direction(board, &dir, pos);
    
    col.x = pos.x + col.x;
    col.y = pos.y + col.y;
    
    if (col.x != pos.x || col.y != pos.y) {
        // COLLISION!
        PRINTF("COL x:%d y:%d\n", col.x, col.y);
        if (is_vec_on_board(col)) {
            board[get_index_vec(col)] = COLLISION;
        }
        
    }
    
    
    idx = get_next_index(idx, dir);
    board[idx] = BALL;
    
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
    simulateBall(board);
}

void draw(const BOARD board) {
    
    int i = 0;
    // DRAW SCANLINE
    for (i = 0; i < BOARD_SIZE ; i++){
        if (board[i] == EMPTY) {
            display_plot_led(i, COLOR_BLACK);
        } else if (board[i] == BALL) {
            display_plot_led(i, COLOR_BALL);
        } else if (board[i] == PLATE) {
            display_plot_led(i, COLOR_PLATE);
        } else if (board[i] == CONTROL) {
            display_plot_led(i, COLOR_CONTROL);
        } else if (board[i] == COLLISION) {
            display_plot_led(i, COLOR_COLLISION);
        } else if (board[i] >= BRICK_MIN && board[i] < BRICK_MAX) {
            switch ((board[i] + get_y(i)) % 3) {
                case 0:
                    display_plot_led(i, COLOR_RED);
                    break;
                case 1:
                    display_plot_led(i, COLOR_BLUE);
                    break;
                case 2:
                    display_plot_led(i, COLOR_GREEN);
                    break;
                default:
                    break;
            }
        }
    }
}

// MARK: - collision

Vec2 get_new_direction(BOARD board, Vec2 *direction, Vec2 current_position) {
    int x = current_position.x;
    int y = current_position.y;
    
    int neighbors[8];
    
    neighbors[0] = is_neighbor(board, (Vec2){ x,   y+1 });
    neighbors[1] = is_neighbor(board, (Vec2){ x+1, y+1 });
    neighbors[2] = is_neighbor(board, (Vec2){ x+1, y });
    neighbors[3] = is_neighbor(board, (Vec2){ x+1, y-1 });
    neighbors[4] = is_neighbor(board, (Vec2){ x,   y-1 });
    neighbors[5] = is_neighbor(board, (Vec2){ x-1, y-1 });
    neighbors[6] = is_neighbor(board, (Vec2){ x-1, y });
    neighbors[7] = is_neighbor(board, (Vec2){ x-1, y+1 });
    
    return bounce(direction, neighbors);
}


int is_neighbor(BOARD board, Vec2 pos) {
    
    if (is_vec_on_board(pos)) {
        // empty yes/no
        if (is_empty_field_vec(board, pos)) {
            // empty
            return 0;
        } else {
            // no empty
            return 1;
        }
    } else {
        // out of bounds
        return 1;
    }
}


/// Callculate new direction.
/// @param dir The direction of travel.
/// @param n The neighbors.
Vec2 bounce(Vec2* dir, int n[8]) {
    if (dir -> x == 1 && dir -> y == 1) {
        if (bounceQuadrant(dir, n[0], n[1], n[2], &(Vec2){1,1})) {
            return (Vec2) { 1,1 };
        }
    } else if (dir -> x == 1 && dir -> y == -1) {
        if (bounceQuadrant(dir, n[2], n[3], n[4], &(Vec2){-1,1})) {
            return (Vec2) { 1, -1};
        }
        
    } else if (dir -> x == -1 && dir -> y == -1) {
        if (bounceQuadrant(dir, n[4], n[5], n[6], &(Vec2){-1,-1})) {
            return (Vec2) { -1, -1};
        }
        
    } else if (dir -> x == -1 && dir -> y == 1) {
        if (bounceQuadrant(dir, n[6], n[7], n[0], &(Vec2){1,-1})) {
            return (Vec2){ -1, 1};
        }
    }
    
    // that's us.
    return (Vec2) { 0, 0};
}


bool isDiagonal (Vec2* v) {
    if (v -> x * v -> x + v -> y * v -> y == 2) return true;
    return false;
}

void straightBounce (Vec2* v) {
    v -> x *= -1;
    v -> y *= -1;
}

/* o: origin */
/* u: up */
/* d: diagonal upper right */
/* r: right */
/* q: quadrant rotation indicator */
bool diagonalBounce(Vec2* o, int u, int d, int r, Vec2* q) {
    if (u == 0 && r == 0 && d == 0) {
        return false;
    } else if (u == 0 && r == 0) {
        straightBounce(o);
        return true;
    } else if (u != 0 && r != 0 && d != 0) {
        straightBounce(o);
        return true;
    } else if (u != 0) {
        o -> x = 1 * q -> x;
        o -> y = -1 * q -> y;
        return true;
    } else if (r != 0) {
        o -> x = -1 * q -> x;
        o -> y = 1 * q -> y;
        return true;
    }
    return false;
}

bool bounceQuadrant(Vec2* o, int u, int d, int r, Vec2* q) {
    if (isDiagonal(o)) {
        return diagonalBounce(o, u, d, r, q);
    } else {
        straightBounce(o);
        return true;
    }
}


