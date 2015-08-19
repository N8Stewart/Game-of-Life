//The Game of Life (http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
//Originally Implemented by: Eric Bauer
//Modified and re-implemented by: Nate Stewart (N8Stewart)
//Last Updated: 08/19/15

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

//old generation (aka current generation
int oldGen[ROW_SIZE][COL_SIZE];
//future generation
int newGen[ROW_SIZE][COL_SIZE];

//main loop
int main()
{
	int cellsAlive = 0;
	char cont = ' ';
    int j = 0, i = 0;
    unsigned int matches = 0;
	//create random seed
	srand(time(NULL));
	//initialize oldGen to all 0's
	for(j = 0; j<ROW_SIZE; j++){
		for (i = 0; i<COL_SIZE; i++){
			oldGen[j][i] = 0;
		}
	}
	int totalGen = 0;
	//initialize newGen to all 0's
	updateGen();
	printf("==(1010100)===Welcome to the Game of Life===(0110010)==\n");
	
	//prompt for number of cells to randomly seed
	do{
		printf("\nHow many alive cells should the seed generate? (maximum of %d) ",ROW_SIZE*COL_SIZE);
		matches = scanf("%d", &cellsAlive);
        if(matches != 1) {
            printf("Invalid input.\n");
            break;
        }
	}while(!randSeed(cellsAlive));
			
	int iter = 0;
	printf("The current random seed is:\n");
	printLife();

	while(cont != 'n'){
		printf("\nHow many generations do you simulate (positive integer or -1 to quit)? ");
		matches = scanf("%d",&iter);
		if(matches != 1) {
            printf("Invalid input.\n");
            break;
        } else if(iter == -1){
			break;
		}

		for(i = 0; i<=iter+totalGen; i++){
			printf("\nGeneration %d:\n",i);
			printLife();
			getNewGen();
			updateGen();
		}
		totalGen += iter;

		/*printf("\nWould you like to simulate more(y/n)? ");
		scanf("%c",&cont);*/
		
	}

	return 0;
}

void getNewGen(){
    int j = 0, i = 0;
/*Generate new colonies into newGen array*/
	for(; j<ROW_SIZE; j++){
		for (i = 0; i<COL_SIZE; i++){
			newGen[j][i] = getNewCell(j,i);
		}
	}
}

bool getNewCell(int row, int col){
/*get current cell's life status*/
	//	0 = dead, 1 = alive, 0 = invalid array bound
	bool life = oldGen[row][col];
	//I.)get current neighbors' life statuses
	/* Neighbor ordering (X = current cell)
				0,1,2
				3,X,4
				5,6,7
	*/
	int neighbors[8];
	/*If a index manipulation results in an out of bounds, that undefined cell = 0*/
	//--------ORTHAGONAL neighbors
	//veritcal
	if(row-1<0){
		neighbors[1] = 0;
	} else {
		neighbors[1] = oldGen[row-1][col];
	}
	if(row+1>=ROW_SIZE){
		neighbors[6] = 0;
	} else {
		neighbors[6] = oldGen[row+1][col];
	}
	//horizontal
	if(col-1<0){
		neighbors[3] = 0;
	} else {
		neighbors[3] = oldGen[row][col-1];
	}
	if(col+1>=COL_SIZE){
		neighbors[4] = 0;
	} else {
		neighbors[4] = oldGen[row][col+1];
	}
	//---------CORNER neighbors
	if((row-1<0) || (col-1<0)){
		neighbors[0] = 0;
	} else {
		neighbors[0] = oldGen[row-1][col-1];
	}
	if((row-1<0) || (col+1>=COL_SIZE)){
		neighbors[2] = 0;
	} else {
		neighbors[2] = oldGen[row-1][col+1];
	}
	if((row+1>=ROW_SIZE) || (col-1<0)){
		neighbors[5] = 0;
	} else {
		neighbors[5] = oldGen[row+1][col-1];
	}
	if((row+1>=ROW_SIZE) || (col+1>=COL_SIZE)){
		neighbors[7] = 0;
	} else {
		neighbors[7] = oldGen[row+1][col+1];
	}

	//II.) Determine whether current cell lives or dies according to rules
	if(life == 1){
	//1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
		if(arraySum(neighbors,8)<2)
			life = 0;
	//2. Any live cell with two or three live neighbours lives on to the next generation.
		if(arraySum(neighbors,8)==2 || arraySum(neighbors,8)==3)
			life = 1;
	//3. Any live cell with more than three live neighbours dies, as if by overcrowding.
		if(arraySum(neighbors,8)>3)
			life = 0;
	} else if(life ==0) {
	//4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
		if(arraySum(neighbors,8)==3)
			life = 1;
	}
	
	return life;
}

int arraySum(int arry[], int size){
/*gets sum of all elements of inputted aray*/
	int sum = 0;
    int i = 0;
	for(; i<size; i++){
		sum += arry[i];
	}
	return sum;
}

void printLife(){
/*prints out the current generation of cells*/
	char live = 'X';
	char dead = '.';
    int j = 0, i = 0;
    
	for(; j<ROW_SIZE; j++){
		for (i = 0; i<COL_SIZE; i++){
			if(oldGen[j][i]==1){
				printf("%c ",live);
			} else {
				printf("%c ",dead);
			}
		}
		printf("\n");
	}	
	printf("\n");
}

void updateGen(){
/*inchworms the oldGen to be the newGen once newGen is determined*/
    int j = 0, i = 0;
	for(; j<ROW_SIZE; j++){
		for (i = 0; i<COL_SIZE; i++){
			oldGen[j][i] = newGen[j][i];
		}
	}
}

bool randSeed(int rands){
/*generates a random starting habitat for the game's seed. 
	Note: seeding in a cell can reoccur at random*/
	bool goodGen = 0;
	int maxRands = ROW_SIZE*COL_SIZE;
	int m = 0, n = 0, i = 0;
	if(rands>maxRands){
		printf("\nERROR: invalid cell number.\n");
	} else {
		for(i = rands; i>0; i--){
			m = rand()%ROW_SIZE;
			n = rand()%COL_SIZE;
			oldGen[m][n] = 1;
		}
		goodGen = 1;
	}
	return goodGen;
}