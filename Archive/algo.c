#include <stdbool.h>
#include <stdio.h>

#define N 9

bool check_valid(int grid[N][N], int row, int col, int num) {
    // Überprüfen Sie, ob die angegebene Zahl bereits in derselben Zeile oder Spalte vorhanden ist
    for (int i = 0; i < N; i++) {
        if (grid[row][i] == num || grid[i][col] == num)
            return false;
    }

    // Überprüfen Sie, ob die angegebene Zahl bereits in derselben 3x3-Box vorhanden ist
    int box_x = row / 3 * 3;
    int box_y = col / 3 * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[box_x + i][box_y + j] == num)
                return false;
        }
    }

    return true;
}

bool generate_sudoku(int grid[N][N], int row, int col) {
    if (row == N) {
        // Das Raster ist vollständig ausgefüllt, also speichern wir es als Lösung
        return true;
    }

    if (col == N) {
        // Wir haben die letzte Spalte erreicht, also gehen wir zur nächsten Zeile
        return generate_sudoku(grid, row + 1, 0);
    }

    if (grid[row][col] != 0) {
        // Das Feld ist bereits gefüllt, also gehen wir zur nächsten Spalte
        return generate_sudoku(grid, row, col + 1);
    }

    for (int num = 1; num <= 9; num++) {
        if (check_valid(grid, row, col, num)) {
            grid[row][col] = num;

            if (generate_sudoku(grid, row, col + 1))
                return true;

            grid[row][col] = 0;
        }
    }

    return false;
}

void print_grid(int grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int grid[N][N] = {0};
    if (generate_sudoku(grid, 0, 0)) {
        print_grid(grid);
    } else {
        printf("Keine Lösung gefunden\n");
    }

    return 0;
}