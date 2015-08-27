#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define ROW_SIZE 20
#define COL_SIZE 35

typedef enum { false, true } bool;

bool getNewCell(int *matrix, int rows, int columns, int row, int col);
void getNewGen(int *oldGen, int *newGen, int rows, int columns);
int arraySum(int arry[], int size);
bool randSeed(int rands, int *matrix, int rows, int columns);
void updateGen(int *oldGen, int *newGen, int rows, int columns);
void printLife(int *matrix, int rows, int columns);
void displayMenu();
int getUserInput();