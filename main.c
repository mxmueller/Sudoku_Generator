#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ROW_MAX 9
#define COL_MAX 9
#define BOX_MAX 3
#define NUM_OF_FIELDS_MIN 17
#define NUM_OF_FIELDS_MAX 32
#define AN_ARRAY_SIZE 9 // AN = Available Numbers

void display_board(int board[ROW_MAX][COL_MAX]) {
    for (int row_count = 0; row_count < ROW_MAX; row_count++) {
        for (int col_count = 0; col_count < COL_MAX; col_count++) {
            printf("%d ", board[row_count][col_count]);
        }
        printf("\n");
    }
    printf("################\n");
}

int validate_given_option(int board[ROW_MAX][COL_MAX], int row_instance, int col_instance, int given_option) {
    int bow_row_coordinate,
        box_col_coordinate;
    bow_row_coordinate = row_instance / 3 * 3;
    box_col_coordinate = col_instance / 3 * 3;

    for (int i = 0; i < 9; i++) {
        if (board[row_instance][i] == given_option) // option in row
           return 0;

        if (board[i][col_instance] == given_option) // option in col
            return 0;
    }

    for (int box_row_count = 0; box_row_count < BOX_MAX; ++box_row_count) { // check 3x3 box
        for (int box_col_count = 0; box_col_count < BOX_MAX; ++box_col_count) {
            if(board[bow_row_coordinate + box_row_count][box_col_coordinate + box_col_count] == given_option)
                return 0;
        }
    }

    return 1; // valid :)
}

int build_sudoku(int board[ROW_MAX][COL_MAX], int row_instance, int col_instance, int available_numbers[AN_ARRAY_SIZE]) {
    if (row_instance == ROW_MAX) // finished build (end of matrix)
        return 1;

    if (col_instance == COL_MAX) // finished col, move on to next row
        return build_sudoku(board, row_instance + 1, 0, available_numbers);

    if (board[row_instance][col_instance] != 0) // location is already written
        return build_sudoku(board, row_instance, col_instance + 1, available_numbers);

    for (int option = 0; option <= 9; ++option) {
        if (validate_given_option(board, row_instance, col_instance, available_numbers[option])) {
            board[row_instance][col_instance] = available_numbers[option];
            if (build_sudoku(board, row_instance, col_instance + 1, available_numbers)) {
                return 1;
            }
            board[row_instance][col_instance] = 0;
        }
    }
    return 0;
}

int blackening_sudoku(int board[ROW_MAX][COL_MAX], int black_outs_left) {
    if (black_outs_left != 0) {
        int random_row, random_col;
            random_row = rand() % 9;
            random_col = rand() % 9;
        if(board[random_row][random_col] != 0 ) {
            board[random_row][random_col] = 0;  // blacked out fields are replaced with 0
            blackening_sudoku(board, black_outs_left - 1);
        } else {
            blackening_sudoku(board, black_outs_left); // retry
        }
    } else {
        return 1;
    }
}

void sudoku() {
    int board[ROW_MAX][COL_MAX] = {0},
    available_numbers[AN_ARRAY_SIZE] = {1,2,3,4,5,6,7,8,9};

    srand(time(NULL));
    for (int i = AN_ARRAY_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1), temp = available_numbers[i];
        available_numbers[i] = available_numbers[j];
        available_numbers[j] = temp;
    } // Shuffle to get random sudoku each time

    if(build_sudoku(board, 0, 0, available_numbers)) { // ! PAP
        display_board(board);
        if(blackening_sudoku(board, ((ROW_MAX * COL_MAX) - NUM_OF_FIELDS_MIN)))
            display_board(board);
    } else {
        // error handling
    }
}

int main() {
    sudoku();
    return 0;
}
