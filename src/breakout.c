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
    game_speed = 220;
    
    int i;
    
    // initialize board empty
    for (i = 0; i < BOARD_SIZE ; i++){
        board[i] = EMPTY;
    }
    
    // set plate
    for (i = 1; i < WIDTH-2; i++) {
        set_field(board, i, 0, DEADPOOL);
    }
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
        set_field(board, middle+1, 0, DEADPOOL);
    }
}

void move_plate_right(BOARD board) {
    FIELD middle = get_plate(board);
    if (middle < WIDTH-3) {
        set_plate(board, middle+1);
        set_field(board, middle-1, 0, DEADPOOL);
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
bool hasCollided = false;
void simulateBall(BOARD board) {
    board[idx] = EMPTY;
    pos.x = get_x(idx);
    pos.y = get_y(idx);
    Vec2 cols[3];
    get_new_direction(board, &dir, pos, cols);

    for (int i = 0; i < 3; i++) {
        cols[i].x = pos.x + cols[i].x;
        cols[i].y = pos.y + cols[i].y;

        // reset collision highlight
        hasCollided = false;

        if (cols[i].x != pos.x || cols[i].y != pos.y) {
            if (is_vec_on_board(cols[i])) { // COLLISION!
                resolveCollison(board, cols[i]);
                hasCollided = true;
            }
        }
    }
    
    
    idx = get_next_index(idx, dir);
    board[idx] = BALL;
    
}
bool looseGame = false;
bool winGame = false;
int bricksResolvedCount = 0;
void resolveCollison(BOARD board, const Vec2 col) {
    PRINTF("COL x:%d y:%d\n", col.x, col.y);
    int idx = get_index_vec(col);
    if (board[idx] == PLATE || board[idx] == CONTROL) {
        return;
    } else if (is_brick(board, idx) || is_collider(board, idx)) {
        FIELD brickNum = board[idx];
        int y = get_y(idx);
        int idx = get_index(0, y);
        int endIdx = get_index(WIDTH-1, y);
        while (idx != endIdx) {
            if (board[idx] == brickNum) {
                board[idx] = EMPTY;
            }
            idx = get_next_index(idx, (Vec2){ 1,0 });
        }
        bricksResolvedCount += 1;
    } else if (board[idx] == DEADPOOL) {
        looseGame = true;
    }
}

int game_state_needs_update() {
    if (looseGame || winGame) {
        return 0;
    }
    if (frame_count == game_speed) {
        frame_count = 0;
        return 1;
    }
    frame_count++;
    return 0;
}


bool has_won(const BOARD board) {
    int i;
    for (i = 0; i < BOARD_SIZE; i++) {
        if (board[i] >= BRICK_MIN && board[i] < BRICK_MAX) {
            return false;
        }
    }
    
    return true;
}

// update game state
void update_game_state() {
    simulateBall(board);
    winGame = has_won(board);
}


void draw_score(const BOARD board) {
    int i;
    int y = 1;
    int idx = get_index(1, y);
    for (i = 0; i < bricksResolvedCount; i++) {
        if (get_y(idx) == HEIGHT-1) {
            y += 1;
            idx = get_index(1, y);
        }
        display_plot_led(idx, COLOR_BLUE);
        idx = get_next_index(idx, (Vec2){ 0, 1 });
    }
    
}

void draw(const BOARD board) {
   
    if (winGame) {
        display_fill_all(COLOR_GREEN);
        draw_score(board);
        return;
    }
    if (looseGame) {
           display_fill_all(COLOR_RED);
           draw_score(board);
           return;
       }
    int i = 0;
    // DRAW SCANLINE
    for (i = 0; i < BOARD_SIZE ; i++){
        if (board[i] == EMPTY || board[i] == DEADPOOL) {
            display_plot_led(i, COLOR_BLACK);
        } else if (board[i] == BALL) {
            if (hasCollided) {
                display_plot_led(i, COLOR_BALL_HIGHLIGHT);
            } else {
                display_plot_led(i, COLOR_BALL);
            }
        } else if (board[i] == PLATE) {
            display_plot_led(i, COLOR_PLATE);
        } else if (board[i] == CONTROL) {
            display_plot_led(i, COLOR_CONTROL);
        } else if (board[i] == COLLISION) {
            display_plot_led(i, COLOR_COLLISION);
        } else if (is_brick(board, i) || is_collider(board, i)) {
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

void get_new_direction(BOARD board, Vec2 *direction, Vec2 current_position, Vec2 cols[3]) {
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
    
    bounce(direction, neighbors, cols);
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
void bounce(Vec2* dir, int n[8], Vec2 cols[3]) {
    if (dir -> x == 1 && dir -> y == 1) {
        if (bounceQuadrant(dir, n[0], n[1], n[2], &(Vec2){1,1})) {
            cols[0] = (Vec2){ 1,1 };
        }
    } else if (dir -> x == 1 && dir -> y == -1) {
        if (bounceQuadrant(dir, n[2], n[3], n[4], &(Vec2){-1,1})) {
            cols[0] = (Vec2){0,1};
            cols[1] = (Vec2){1,-1};
            cols[2] = (Vec2){1,0};
        }
        
    } else if (dir -> x == -1 && dir -> y == -1) {
        if (bounceQuadrant(dir, n[4], n[5], n[6], &(Vec2){-1,-1})) {
            cols[0] = (Vec2){0,1};
            cols[1] = (Vec2){1,-1};
        }
        
    } else if (dir -> x == -1 && dir -> y == 1) {
        if (bounceQuadrant(dir, n[6], n[7], n[0], &(Vec2){1,-1})) {
            cols[0] = (Vec2){1,-1};
            cols[1] = (Vec2){1,0};
        }
    }
    
    // that's us.
    return;
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


void toggle_collider(BOARD board, const int index) {
    if (is_brick(board, index) || is_plate(board, index) || is_ball(board, index) || is_wall(board, index)) {
        // do not edit;
        return;
    }
    
    if (board[index] == COLLIDER) {
        board[index] = EMPTY;
    } else {
        board[index] = COLLIDER;
    }
}

bool is_brick(const BOARD board, const int index) {
    return board[index] >= BRICK_MIN && board[index] < BRICK_MAX;
}

bool is_plate(const BOARD board, const int index) {
    return board[index] == PLATE;
}

bool is_ball(const BOARD board, const int index) {
    return board[index] == BALL;
}

bool is_wall(const BOARD board, const int index) {
    return board[index] == WALL;
}

bool is_collider(const BOARD board, const int index) {
    return board[index] == COLLIDER;
}
