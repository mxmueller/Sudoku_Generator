/*
    Author: 1081078
    Baden-Wuerttemberg Cooperative State University
    Projekt: messages.c
    Datum: 01.02.2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define ROW_MAX 9
#define COL_MAX 9
#define BOX_MAX 3
#define NUM_OF_FIELDS_MIN 17
#define AN_SIZE 9

#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int *input_interaction() {
    char input_char;
    int input_int, entry_attempt, num_scanner;
    static int collection[4];
    collection[3] = 0; // selected mode in collection

    printf("MODE: \n #  to undo\n <  to turn on rules\n >  to turn off rules\n"
           " +  to show solution\n -  to hide solution\n i  for insert mode\n"
           "ESC to exit:\n");

    num_scanner = scanf(" %c", &input_char);

    if (input_char == '#') {
        collection[3] = 1;
        return collection;
    } else if (input_char == '<') {
        collection[3] = 2;
        return collection;
    } else if (input_char == '>') {
        collection[3] = 3;
        return collection;
    } else if (input_char == '+') {
        collection[3] = 4;
        return collection;
    } else if (input_char == '-') {
        collection[3] = 5;
        return collection;
    } else if (input_char == 'i') {
        printf("Enter a character between 'a' and 'i' and an integer between 1 and 9: ");
        num_scanner = scanf(" %c%d", &input_char, &input_int);

        if (num_scanner != 2 || !isalpha(input_char) || input_char < 'a' || input_char > 'i'
            || input_int < 1 || input_int > 9) {
            printf("Invalid input!\n");
            return NULL;
        }

        printf("Enter your try between 1 and 9: ");
        scanf("%d", &entry_attempt);

        if (entry_attempt < 1 || entry_attempt > 9) {
            printf("Invalid input!\n");
            return NULL;
        }

        collection[0] = ((tolower(input_char) - 'a') + 1) - 1;
        collection[1] = input_int - 1;
        collection[2] = entry_attempt;
        return collection;
    } else if (input_char == 27) { // ESC
        collection[3] = 6;
        return collection;
    }

    printf("Invalid input!\n");
    return NULL;
}

void change_output_to_yellow() {
        printf("\033[0;31m");
}

void reset_output () {
    printf("\033[0m");
}

// void display_board(int board[ROW_MAX][COL_MAX], struct last_moves_archive *archive) {
void display_board(int board[ROW_MAX][COL_MAX]) {
    puts("    a b c   d e f   g h i"); // header
    int show_line = 1;
    for (int row_count = 0; row_count < ROW_MAX; row_count++) {
        if (row_count == 0 || row_count == 3 || row_count == 6)
            printf("    - - -   - - -   - - -\n");
        for (int col_count = 0; col_count < COL_MAX; col_count++) {
            if (col_count == 0) {
                printf("%d", show_line++);
                printf(" | %d", board[row_count][col_count]);
            }
            if (col_count == 2 || col_count == 5 || col_count == 8)
                printf(" %d |", board[row_count][col_count]);
            if (col_count == 1 || col_count == 3 || col_count == 4 || col_count == 6 || col_count == 7)
                printf(" %d", board[row_count][col_count]);
        }
        printf("\n");
    }
    printf("    - - -   - - -   - - -\n");
    printf("\n\n");
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
            if (board[bow_row_coordinate + box_row_count][box_col_coordinate + box_col_count] == given_option)
                return 0;
        }
    }
    return 1; // valid :)
}

int build_sudoku(int board[ROW_MAX][COL_MAX], int row_instance, int col_instance, int available_numbers[AN_SIZE]) {
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
        if (board[random_row][random_col] != 0) {
            board[random_row][random_col] = 0;  // blacked out fields are replaced with 0
            blackening_sudoku(board, black_outs_left - 1);
        } else {
            blackening_sudoku(board, black_outs_left); // retry
        }
    } else {
        return 1;
    }
}

struct last_moves_coordinates {
    int last_row;
    int last_col;
};

struct last_moves_archive {
    int size;
    int capacity;
    struct last_moves_coordinates *data;
};

struct last_moves_archive *create_last_moves_archive(int capacity) {
    struct last_moves_archive *arr = (struct last_moves_archive *) malloc(sizeof(struct last_moves_archive));
    arr->capacity = capacity;
    arr->size = 0;
    arr->data = (struct last_moves_coordinates *) malloc(arr->capacity * sizeof(struct last_moves_coordinates));
    return arr;
}

void add_coordinates_to_archive(struct last_moves_archive *arr, int last_row, int last_col) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = (struct last_moves_coordinates *) realloc(arr->data, arr->capacity * sizeof(struct last_moves_coordinates));
    }
    arr->data[arr->size].last_row = last_row;
    arr->data[arr->size].last_col = last_col;
    arr->size++;
}

void transmit_last_move_and_delete_it(struct last_moves_archive *arr, int *last_row, int *last_col) {
    if (arr->size == 0) {
        *last_row = -1;
        *last_col = -1;
        return;
    }
    arr->size--;
    *last_row = arr->data[arr->size].last_row;
    *last_col = arr->data[arr->size].last_col;
}

struct last_moves_coordinates get_last_element(struct last_moves_archive *arr) {
    return arr->data[arr->size - 1];
}

void delete_last_moves_archive(struct last_moves_archive *arr) {
    free(arr->data);
    free(arr);
}

// ! PAP
void play(int* inputs, int board[ROW_MAX][COL_MAX], struct last_moves_archive *archive, int game_move) {
    int row_selected_by_the_player = inputs[1],
    col_selected_by_the_player = inputs[0],
    attempt_selected_by_the_player = inputs[2],
    mode = inputs[3];

    printf("%d", game_move);

    if (mode != 0) {
        switch (mode) {
            case 1: // undo
                if(game_move != 0) {
                    int undo_row, undo_col;
                    transmit_last_move_and_delete_it(archive, &undo_row, &undo_col);
                    board[undo_row][undo_col] = 0; // undo last move
                    display_board(board);
                    play(input_interaction(), board, archive, game_move-1);
                }
                break;
            case 2: // turn rules on
                printf("<");
                break;
            case 3: // turn rules off
                printf(">");
                break;
            case 4: // show solution
                printf("+");
                break;
            case 5: // hide solution
                printf("-");
                break;
            case 6:
                delete_last_moves_archive(archive);
                break;
        }
    }

    if(validate_given_option(board, row_selected_by_the_player, col_selected_by_the_player, attempt_selected_by_the_player)) {
        puts("erlaubt!");
        board[inputs[1]][inputs[0]] = inputs[2];
        add_coordinates_to_archive(archive, row_selected_by_the_player, col_selected_by_the_player);
        game_move++;
    } else {
        puts("Nicht erlaubt!");
    }

    display_board(board);
    play(input_interaction(), board, archive, game_move);
}

void sudoku() {
    int board[ROW_MAX][COL_MAX] = {0};
    int available_numbers[AN_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct last_moves_archive *archive = create_last_moves_archive(5);

    srand(time(NULL));

    for (int i = AN_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1), temp = available_numbers[i];
        available_numbers[i] = available_numbers[j];
        available_numbers[j] = temp;
    } // Shuffle to get random sudoku each time

    if (build_sudoku(board, 0, 0, available_numbers)) { // ! PAP
        display_board(board);
        if (blackening_sudoku(board, ((ROW_MAX * COL_MAX) - NUM_OF_FIELDS_MIN))) {
            display_board(board);
            play(input_interaction(), board, archive, 0);
        }
    } else {
        // error handling
    }
}

int main() {
    // sudoku();
    printf(ANSI_COLOR_CYAN    "This text is CYAN!"    ANSI_COLOR_RESET "\n");

    return 0;
}
