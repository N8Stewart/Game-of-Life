#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define ROW_SIZE 20
#define COL_SIZE 35

typedef enum { false, true } bool;

bool getNewCell(int row, int col);
void getNewGen();
int arraySum(int arry[], int size);
bool randSeed(int rands);
void updateGen();
void printLife();
void displayMenu();
int getUserInput();