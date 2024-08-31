#include "sudoku.h"
#include <stdio.h>
#include <assert.h>
#include "str.h"


int main(void) {
    FILE* tests = fopen("sudoku.txt", "rb");
    assert(tests);
    char testcase_str[4096];
    while (fgets(testcase_str, 4096, tests) != 0) {
        printf("%s\n", testcase_str);
        u8 grid[81] = {0};
        for (int i = 0; i < 81; i++) {
            if ('0' <= testcase_str[i] && testcase_str[i] <= '9') {
                grid[i] = testcase_str[i] - '0';
            }
        }
        printf("Testing:\n");
        for (int i = 0; i < 81; i++) {
            if (i % 9 == 0) {
                printf("\n");
                if ((i / 9) % 3 == 0) {
                    printf("---------------------\n");
                }
            }
            else if (i % 3 == 0) {
                printf(" | ");
            }
            else {
                printf(" ");
            }
            printf("%d", grid[i]);
        }
        printf("\n\n");
        assert(solve(grid) == true);
        for (int i = 0; i < 81; i++) {
            if (i % 9 == 0) {
                printf("\n");
                if ((i / 9) % 3 == 0) {
                    printf("---------------------\n");
                }
            }
            else if (i % 3 == 0) {
                printf(" | ");
            }
            else {
                printf(" ");
            }
            printf("%d", grid[i]);
        }
        printf("\n\n");
    }
}
