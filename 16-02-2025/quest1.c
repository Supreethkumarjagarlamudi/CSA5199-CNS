#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define STATE_ROWS 5
#define STATE_COLS 5
#define LANE_SIZE  64 
#define STATE_SIZE (STATE_ROWS * STATE_COLS) 
#define BLOCK_SIZE 1024  
#define CAPACITY_SIZE (1600 - BLOCK_SIZE)  
#define CAPACITY_LANES (CAPACITY_SIZE / LANE_SIZE)

void initialize_state(uint64_t state[STATE_ROWS][STATE_COLS]) {
    for (int i = 0; i < STATE_ROWS; i++) {
        for (int j = 0; j < STATE_COLS; j++) {
            state[i][j] = 0;  
        }
    }
}

void absorb_block(uint64_t state[STATE_ROWS][STATE_COLS], uint64_t block[STATE_ROWS][STATE_COLS]) {
    for (int i = 0; i < STATE_ROWS; i++) {
        for (int j = 0; j < STATE_COLS; j++) {
            state[i][j] ^= block[i][j];  
        }
    }
}

int check_capacity_filled(uint64_t state[STATE_ROWS][STATE_COLS]) {
    int zero_count = 0;
    for (int i = 0; i < CAPACITY_LANES; i++) {
        int row = (STATE_SIZE - CAPACITY_LANES + i) / STATE_COLS;
        int col = (STATE_SIZE - CAPACITY_LANES + i) % STATE_COLS;
        if (state[row][col] == 0) {
            zero_count++;
        }
    }
    return zero_count == 0;
}

void generate_random_block(uint64_t block[STATE_ROWS][STATE_COLS]) {
    for (int i = 0; i < STATE_ROWS; i++) {
        for (int j = 0; j < STATE_COLS; j++) {
            block[i][j] = ((uint64_t)rand() << 32) | rand();  
        }
    }
}

int main() {
    srand(time(NULL));  

    uint64_t state[STATE_ROWS][STATE_COLS];
    uint64_t block[STATE_ROWS][STATE_COLS];

    initialize_state(state);
    
    int rounds = 0;
    while (!check_capacity_filled(state)) {
        generate_random_block(block);
        absorb_block(state, block);
        rounds++;
    }

    printf("All capacity lanes have at least one nonzero bit after %d blocks absorbed.\n", rounds);
    return 0;
}