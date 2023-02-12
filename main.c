#include <stdio.h>

int main() {
    double weight, height;
    double bmi = 0;

    printf("Enter weight (in kg):");
    scanf("%lf", &weight);
    printf("Enter height (in m) (Format 1.87):");
    scanf("%lf", &height);

    height = height * 2;
    bmi = (float)weight/(float)height;
    printf("BMI: %lf", bmi);

    if (bmi == 0) {
        printf("\nSomthing went wrong!");
        return 0;
    }

    if (bmi < 19)
        printf("\nUntergewicht");

    if (bmi < 25 && bmi >= 19)
        printf("\nNormalgewicht");

    if (bmi < 30 && bmi >= 25)
        printf("\nleichts Übergewicht");

    if (bmi >= 30)
        printf("\nMehr Bewegung");

    return 0;
}
