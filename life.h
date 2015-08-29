#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define ROW_SIZE 20
#define COL_SIZE 35
#define HISTORY_SIZE 6

typedef enum { false, true } bool;

bool getNewCell(int *matrix, int rows, int columns, int row, int col);
void getNewGen(int *oldGen, int *newGen, int rows, int columns);
int arraySum(int *array);
void randSeed(int rands, int *matrix, int rows, int columns);
void swapGen(int **newGen, int ***history, int historySize);
void printLife(int *matrix, int rows, int columns);
void displayMenu();
int getUserInput();
int getMenuOption();
void outputDescription();
bool analyzeHistory(int *newGen, int **history, int historySize, int rows, int columns);