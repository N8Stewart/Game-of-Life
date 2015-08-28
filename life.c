//The Game of Life (http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
//Originally Implemented by: Eric Bauer
//Modified and re-implemented by: Nate Stewart (N8Stewart)
//Last Updated: 08/19/15

#include "life.h"

/*
 * Main controller method. Sets up the game, facilitates user input, and controls the general flow of the program.
 */
int main() {
	int cellsAlive = 0;
	char cont = ' ';
    int i = 0, m = 0, n = 0;
    unsigned int matches = 0;
    int totalGen = 0;
    int menuOption = 0;
    int iter = 0;
    
    // gather space on the heap for the two matrices
    int *oldGen = (int *)malloc(sizeof(int) * ROW_SIZE * COL_SIZE);
    int *newGen = (int *)malloc(sizeof(int) * ROW_SIZE * COL_SIZE);
	
    //create random seed
	srand(time(NULL));
	
    //initialize oldGen to all 0's
	for (m = 0; m < ROW_SIZE; m++) {
		for (n = 0; n < COL_SIZE; n++) {
			*(oldGen + (m * COL_SIZE) + n) = 0;
		}
	}
	
    //initialize newGen to all 0's
	updateGen(oldGen, newGen, ROW_SIZE, COL_SIZE);
    displayMenu();
    while ((menuOption = getUserInput()) < 1 || menuOption > 3) {
        printf("Invalid entry.\n\n");
        displayMenu();
    }
    
    // Switch on the menu option to figure out what to do
    switch(menuOption) {
        case 1:
            
            break;
        case 2:
            
            break;
        case 3:
            
            break;
    }
	
	//prompt for number of cells to randomly seed
	do {
        
		printf("\nHow many alive cells should the seed generate? (maximum of %d)", ROW_SIZE * COL_SIZE);
		matches = scanf("%d", &cellsAlive);
        if (matches != 1) {
            printf("Invalid input.\n");
            break;
        }
        
	} while (!randSeed(cellsAlive, oldGen, ROW_SIZE, COL_SIZE));
			
	
	printf("The current random seed is:\n");
	printLife(oldGen, ROW_SIZE, COL_SIZE);

	while (cont != 'n') {
		
        printf("\nHow many generations do you simulate (positive integer or -1 to quit)?");
		matches = scanf("%d", &iter);
        
		if (matches != 1) {
            printf("Invalid input.\n");
            break;
        } else if (iter == -1) {
			break;
		}

		for (i = 0; i <= (iter + totalGen); i++) {
            printf("\nGeneration %d:\n",i);
			printLife(oldGen, ROW_SIZE, COL_SIZE);
			getNewGen(oldGen, newGen, ROW_SIZE, COL_SIZE);
			updateGen(oldGen, newGen, ROW_SIZE, COL_SIZE);
		}
		totalGen += iter;

	}
    
    free(oldGen);
    free(newGen);

	return 0;
}

/*
 * Setup the game using various user inputs 
 */
void initGame() {
    
}

/*
 * Output the menu to the main menu to the console.
 */
void displayMenu() {
    printf("Welcome to Conway's Game of Life.\n");
    printf("1. Description of Game\n");
    printf("2. Begin game\n");
    printf("3. Quit\n");
}

/*
 * Output the a description of the game and the url where more information can be found
 */
void outputDescription() {
    printf("\nConway's Game of Life is a mathematical implementation of cellular automation.\n");
    printf("Essentially, an initial seed of 'living cells' is created and then every generation, some cells die, others live, and others are born. This occurs until the only living cells form known patterns that never die.\n");
    printf("For more information, see the wikipedia page: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life.\n");
}

/*
 * Grab user input from the console. 
 * Return the integer entered by the user if the input is an integer followed by '\n'. Otherwise return -1.
 */
int getUserInput() {
    int userInput;
    char term;
    if(scanf("%d%c", &userInput, &term) != 2 || term != '\n') {
        // Clear stdin
        while (getchar()!='\n');        
        userInput = -1;
    }
    
    return userInput;
}

/* 
 * Generate new colonies into newGen array 
 */
void getNewGen(int *oldGen, int *newGen, int rows, int columns) {
    int m = 0, n = 0;
    
	for (; m < ROW_SIZE; m++) {
		for (n = 0; n < COL_SIZE; n++){
			*(newGen + (m * columns) + n) = getNewCell(oldGen, rows, columns, m, n);
		}
	}
}

/*
 * Get the current cell's life status based on the game's rules found on the wiki page.
 */
bool getNewCell(int *matrix, int rows, int columns, int row, int col) {
	//	0 = dead, 1 = alive, 0 = invalid array bound
	bool life = *(matrix + (row * columns) + col);
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
	if ((row - 1) < 0) {
		neighbors[1] = 0;
	} else {
		neighbors[1] = *(matrix + ((row - 1) * columns) + (col + 0));
	}
	if ((row + 1) >= ROW_SIZE) {
		neighbors[6] = 0;
	} else {
		neighbors[6] = *(matrix + ((row + 1) * columns) + (col + 0));
	}
	//horizontal
	if ((col - 1) < 0) {
		neighbors[3] = 0;
	} else {
		neighbors[3] = *(matrix + ((row + 0) * columns) + (col - 1));
	}
	if ((col + 1) >= COL_SIZE) {
		neighbors[4] = 0;
	} else {
		neighbors[4] = *(matrix + ((row + 0) * columns) + (col + 1));
	}
	//---------CORNER neighbors
	if (((row - 1) < 0) || ((col - 1 < 0))) {
		neighbors[0] = 0;
	} else {
		neighbors[0] = *(matrix + ((row - 1) * columns) + (col - 1));
	}
	if (((row - 1) < 0) || ((col + 1) >= COL_SIZE)) {
		neighbors[2] = 0;
	} else {
		neighbors[2] = *(matrix + ((row - 1) * columns) + (col + 1));
	}
	if (((row + 1) >= ROW_SIZE) || ((col - 1) < 0)) {
		neighbors[5] = 0;
	} else {
		neighbors[5] = *(matrix + ((row + 1) * columns) + (col - 1));
	}
	if (((row + 1) >= ROW_SIZE) || ((col + 1) >= COL_SIZE)) {
		neighbors[7] = 0;
	} else {
		neighbors[7] = *(matrix + ((row + 1) * columns) + (col + 1));
	}

	//II.) Determine whether current cell lives or dies according to rules
	if (life == 1) {
	//1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
		if (arraySum(neighbors, 8) < 2)
			life = 0;
	//2. Any live cell with two or three live neighbours lives on to the next generation.
		if (arraySum(neighbors, 8) == 2 || arraySum(neighbors, 8) == 3)
			life = 1;
	//3. Any live cell with more than three live neighbours dies, as if by overcrowding.
		if (arraySum(neighbors, 8) > 3)
			life = 0;
	} else if (life == 0) {
	//4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
		if (arraySum(neighbors, 8) == 3)
			life = 1;
	}
	
	return life;
}

int arraySum(int array[], int size) {
    /* get sum of all elements of inputted aray */
	int sum = 0;
    int i = 0;
	for (; i < size; i++) {
		sum += array[i];
	}
	return sum;
}

/*
 * Print the current generation of cells to the console.
 */
void printLife(int *matrix, int rows, int columns) {
	char live = 'X';
	char dead = '.';
    int m = 0, n = 0;
    
	for (; m < rows; m++) {
		for (n = 0; n < columns; n++) {
			if (*(matrix + (columns * m) + n) == 1) {
				printf("%c ", live);
			} else {
				printf("%c ", dead);
			}
		}
		printf("\n");
	}	
	printf("\n");
}

/*
 * Once the new generation has been determined, set the old generation to be equal to the new generation.
 */
void updateGen(int *oldGen, int *newGen, int rows, int columns) {
    int m = 0, n = 0, offset = 0;
	for (; m < rows; m++) {
		for (n = 0; n < columns; n++) {
            offset = (columns * m) + n;
			*(oldGen + offset) = *(newGen + offset);
		}
	}
}

/*
 * Generates a random starting habitat for the game's seed. Note: Seeding can reoccur in a cell at random.
 */
bool randSeed(int rands, int *matrix, int rows, int columns) {
	bool goodGen = 0;
	int maxRands = rows * columns;
	int m = 0, n = 0, i = 0;
	if (rands > maxRands) {
		printf("\nERROR: invalid cell number.\n");
	} else {
		for(i = rands; i > 0; i--) {
			m = rand() % rows;
			n = rand() % columns;
            *(matrix + (columns * m) + n) = 1;
		}
		goodGen = 1;
	}
	return goodGen;
}
