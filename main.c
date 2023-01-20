#include <stdio.h>

#define ROWS 3
#define COLS 9

char field() {
    // IDEA: the randomizer generates the sudoku in a 17 field array
    // each time the field func gets called the next value gets printed
    //

    return '*';
}

char cell(int mode) {
    for (int block = 0; block < 5; ++block) {
        if (mode == 0) {
            if(block == 0 || block == 4) {
                printf("|");
            } else {
                printf("%c", field());
            }
        } else if (mode == 1 || mode == 2) {
            if(block == 4) {
                printf("|");
            } else if (block != 0){
                printf("%c", field());
            }
        }
    }
}

char row() {
    for (int r = 0; r < 3; ++r) {
        switch (r) {
            case 0:
                cell(0);
                break;// left
            case 1:
                cell(1);
                break;// middle
            case 2:
                cell(2);
                break;// right
        }
    }
}

char display() {
    char horizontal_limitation[] = "--- --- ---";
    for (int i = 0; i < 13; ++i) {
        if(i == 0 || i == 4 || i == 8 || i == 12) {
            puts(horizontal_limitation);
        } else {
            row();
            printf("\n");
        }
    }
}

int main() {
    display();
    return 0;
}
