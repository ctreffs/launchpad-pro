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
    if (!is_index_out_of_bounds(index)) {
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
Vec2 pos = { 0, 0};
void advanceRunningLight(BOARD board) {
    board[idx] = EMPTY;
    pos.x = get_x(idx);
    pos.y = get_y(idx);
    get_new_direction(board, &dir, pos);
    idx = get_next_index(idx, dir);
    PRINTF("dir x:%d y:%d\n", dir.x, dir.y);
    if (is_index_out_of_bounds(idx)) {
        idx = 0;
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
// MARK: - collision
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
void diagonalBounce(Vec2* o, Vec2* u, Vec2* d, Vec2* r, Vec2* q) {
    if (u == NULL && r == NULL)
        straightBounce(o);
    if (u != NULL && r != NULL && d != NULL)
        straightBounce(o);
    if (u != NULL) {
        o -> x = 1 * q -> x;
        o -> y = -1 * q -> y;
    }
    if (r != NULL) {
        o -> x = -1 * q -> x;
        o -> y = 1 * q -> y;
    }
}

void bounceQuadrant(Vec2* o, Vec2* u, Vec2* d, Vec2* r, Vec2* q) {
    if (!isDiagonal(o))
        straightBounce(o);
    if (isDiagonal(o))
        diagonalBounce(o, u, d, r, q);
}


/// Callculate new direction.
/// @param dir The direction of travel.
/// @param n The neighbors.
void bounce(Vec2* dir, Vec2 n[]) {
    
    Vec2 vec_alpha = { 1, 1 };
    Vec2 vec_beta = { -1, 1 };
    Vec2 vec_gamma = { -1, -1 };
    Vec2 vec_delta = { 1, -1 };
    
    if (dir -> x == vec_alpha.x && dir -> y == vec_alpha.x)
        bounceQuadrant(dir, (n + 0), (n + 1), (n + 2), &vec_alpha);
    if (dir -> x == vec_beta.x && dir -> y == vec_beta.x)
        bounceQuadrant(dir, (n + 2), (n + 3), (n + 4), &vec_beta);
    if (dir -> x == vec_gamma.x && dir -> y == vec_gamma.x)
        bounceQuadrant(dir, (n + 4), (n + 5), (n + 6), &vec_gamma);
    if (dir -> x == vec_delta.x && dir -> y == vec_delta.x)
        bounceQuadrant(dir, (n + 6), (n + 7), (n + 0), &vec_delta);
}

void get_neighbor(BOARD board, Vec2* neighbors[], const int index, Vec2 pos) {
    
    if (is_vec_on_board(pos)) {
        // empty yes/no
        if (is_empty_field_vec(board, pos)) {
            neighbors[index] = NULL;
        } else {
            PRINTF("MPTY: index: %d x:%d y:%d\n", index, pos.x, pos.y);
            neighbors[index] = &pos;
        }
    } else {
        PRINTF("OOBS: index: %d x:%d y:%d\n", index, pos.x, pos.y);
        neighbors[index] = &pos;
    }
}


void get_new_direction(BOARD board, Vec2 *direction, Vec2 current_position) {
    int x = current_position.x;
    int y = current_position.y;
    
    Vec2* neighbors[8];
    PRINTF("curpos x:%d y:%d\n", current_position.x, current_position.y);
    get_neighbor(board, neighbors, 0, (Vec2){ x,   y+1 });
    get_neighbor(board, neighbors, 1, (Vec2){ x+1, y+1 });
    get_neighbor(board, neighbors, 2, (Vec2){ x+1, y });
    get_neighbor(board, neighbors, 3, (Vec2){ x+1, y-1 });
    get_neighbor(board, neighbors, 4, (Vec2){ x,   y-1 });
    get_neighbor(board, neighbors, 5, (Vec2){ x-1, y-1 });
    get_neighbor(board, neighbors, 6, (Vec2){ x-1, y });
    get_neighbor(board, neighbors, 7, (Vec2){ x-1, y+1 });
    
    bounce(direction, *neighbors);
}

