#include <stdbool.h>
#include <stdint.h>
typedef uint8_t u8;

u8 find_zeros(u8 *grid, u8 *zeros) {
    u8 index = 0;
    for (u8 i = 0; i < 81; i++) {
        if (grid[i] == 0) {
            zeros[index] = i;
            index++;
        }
    }
    return index;
}


bool is_valid(u8 *grid, u8 num, u8 index) {
    if (!(1 <= num && num < 10)) {
        return false;
    }
    u8 column = index % 9;
    u8 row = index / 9;
    u8 square_col = column / 3;
    u8 square_row = index / 27;
    u8 tmp = grid[index];
    grid[index] = 0;
    // check row
    for (u8 i = row * 9; i < (row + 1) * 9; i++) {
        if (grid[i] == num) {
            grid[index] = tmp;
            return false;
        }
    }
    // check column
    for (u8 i = column; i < 81; i += 9) {
        if (grid[i] == num) {
            grid[index] = tmp;
            return false;
        }
    }
    // check square
    for (
        u8 i = square_row * 27 + square_col * 3;
        i < (square_row + 1) * 27 + square_col * 3;
        i += 9) {
        for (u8 j = 0; j < 3; j++) {
            if (grid[i + j] == num) {
                grid[index] = tmp;
                return false;
            }
        }
    }
    grid[index] = tmp;
    return true;
}

bool solve(u8 *grid) {
    u8 zeros[82] = {0};
    u8 length = find_zeros(grid, zeros);
    for (u8 i = 0; i < 81; i++) {
        if (grid[i] == 0) {
            continue;
        }
        else if (!is_valid(grid, grid[i], i)) {
            return false;
        }
        else {
        }
    }
    u8 grid_index = 0;
    u8 zeros_index = 0;
    for (;;) {
        u8 num = 0;
        if (!(0 <= zeros_index && zeros_index < length)) {
            break;
        } else {
            grid_index = zeros[zeros_index];
        }
        if (!(0 <= grid_index && grid_index < 81)) {
            return false;
        } else {
            num = grid[grid_index] + 1;
        }
        for (;;) {
            if (num == 10) {
                grid[grid_index] = 0;
                if (zeros_index == 0) {
                    return false;
                } else {
                    zeros_index--;
                    break;
                }
            }
            if (is_valid(grid, num, grid_index)) {
                grid[grid_index] = num;
                zeros_index++;
                break;
            }
            num++;
        }
    }
    for (u8 x = 0; x < 81; x++) {
        if (!is_valid(grid, grid[x], x)) {
            return false;
        }
    }
    return true;
}
