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
    int totalGen = 0;
    int currGen = 0;
    int menuOption = 0;
    int iter = 0;
    int offset = 0;
    bool isStable = false;
	int stableIteration = -1;
    
    // gather space on the heap for the two matrices and the history
    int *newGen = malloc(sizeof(int) * ROW_SIZE * COL_SIZE);
    int **history = malloc(sizeof(int *) * HISTORY_SIZE);
    for (i = 0; i < HISTORY_SIZE; i++) {
        *(history + i) = malloc(sizeof(int) * ROW_SIZE * COL_SIZE);
    }
    
    while ((menuOption = getMenuOption()) != 3) {
        
        if (menuOption == 1) {
            outputDescription();
            continue;
        }
    
        //create random seed
        srand(time(NULL));
	
        //initialize oldGen and newGen to all 0's
        for (m = 0; m < ROW_SIZE; m++) {
            for (n = 0; n < COL_SIZE; n++) {
                offset = (m * COL_SIZE) + n;
                *(*history + offset) = 0;
                *(newGen + offset) = 0;
            }
        }

        // Get the number of random seeds
        printf("\nHow many alive cells should the seed generate? (maximum of %d)", ROW_SIZE * COL_SIZE);
        while ((cellsAlive = getUserInput()) < 0 || cellsAlive > (ROW_SIZE * COL_SIZE)) {
            printf("Invalid entry.\n\n");
            printf("\nHow many alive cells should the seed generate? (maximum of %d)", ROW_SIZE * COL_SIZE);
        }
    
        // Seed the matrix and output it to the screen
        randSeed(cellsAlive, *history, ROW_SIZE, COL_SIZE);
        printf("The current random seed is:\n");
        printLife(*history, ROW_SIZE, COL_SIZE);

        // Reset the generation count and begin a new game
        currGen = 0;
        totalGen = 0;
        isStable = false;
        while (cont != 'n') {

            // Get the number of random seeds
            printf("\nEnter number of generations to simulate (positive integer, 0 to simulate until stable, or -1 to return to the menu)?");
            while ((iter = getUserInput()) < -1) {
                printf("Invalid entry.\n\n");
                printf("\nEnter number of generations to simulate (positive integer, 0 to simulate until stable, or -1 to return to the menu)?");
            }

            if (iter == -1) {
                break;
            } else if (iter == 0) { // simulate until stabilization occurs or MAX_GENERATIONS generations have passed.
                while(!isStable && currGen <  MAX_GENERATIONS) {
                    printf("\nGeneration %d:\n", currGen);
                    printLife(*history, ROW_SIZE, COL_SIZE);
                    getNewGen(*history, newGen, ROW_SIZE, COL_SIZE);
                    isStable = analyzeHistory(newGen, history, HISTORY_SIZE, ROW_SIZE, COL_SIZE);
                    swapGen(&newGen, &history, HISTORY_SIZE);
                    
                    currGen++;
                }
                if (isStable) {
					if (stableIteration == -1) {
						stableIteration = currGen;
					}
                    printf("Game stabilized after %d moves.  No further generations will unstabilize it.\n", stableIteration);
				} else 
                    printf("Unable to stabilize current game. Most likely a complex oscillation is currently alive.\n");
                totalGen = currGen - 1;
            } else {
                for (; currGen <= (iter + totalGen); currGen++) {
                
                    printf("\nGeneration %d:\n", currGen);
                    printLife(*history, ROW_SIZE, COL_SIZE);
                    getNewGen(*history, newGen, ROW_SIZE, COL_SIZE);

                    //Analyze stability
                    if (!isStable) {
                        isStable = analyzeHistory(newGen, history, HISTORY_SIZE, ROW_SIZE, COL_SIZE);
					} else {
						if (stableIteration == -1) {
							stableIteration = currGen;
						}
                    	printf("Game stabilized after %d moves.  No further generations will unstabilize it.\n", stableIteration);
					}
                    swapGen(&newGen, &history, HISTORY_SIZE);
                }
                totalGen += iter;
            }
        }
    }
    
    // Free the 2 matrix generations and the history
    free(newGen);
    for (i = 0; i < HISTORY_SIZE; i++) {
        free(*(history + i));
    }
    free(history);

	return 0;
}

/*
 * If (history + 0) = newGen or if (history + HISTORY_SIZE - 1) == newGen, we can be certain the game has stabilized.
 * otherwise, we can be about 75% sure the game has not yet stabilized.
 */
bool analyzeHistory(int *newGen, int **history, int historySize, int rows, int columns) {
    int m, n, offset;
    bool isStable1 = true, isStable6 = true;
    for (m = 0; m < rows; m++) {
        for (n = 0; n < columns; n++) {
            // Test if the generations are stable
            offset = (m * columns) + n;
            if (*(*history + offset) != *(newGen + offset))
                isStable1 = false;
            if (*(*(history + historySize - 1) + offset) != *(newGen + offset))
                isStable6 = false;
            
            if (!isStable1 && !isStable6) // both generations tested are not stable
                break;
        }
        if (!isStable1 && !isStable6) // both generations tested are not stable
            break;
    }
    
    // The game is stable if either the newest generation is stable or the oldest generation is stable
    return isStable1 || isStable6;
    
}

/*
 * Output the menu and ask for user input until a valid input is provided
 */
int getMenuOption() {
    
    int menuOption = 0;
    
    displayMenu();
    while ((menuOption = getUserInput()) < 1 || menuOption > 3) {
        printf("Invalid entry.\n\n");
        displayMenu();
    }
    
    return menuOption;
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
    printf("For more information, see the wikipedia page: https://en.wikipedia.org/wiki/Conway%%27s_Game_of_Life.\n\n");
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
        userInput = -2;
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
	bool currGenAlive = *(matrix + (row * columns) + col);
	//I.)get current neighbors' life statuses
	/* Neighbor ordering (X = current cell)
				0,1,2
				3,X,4
				5,6,7
	*/
	int neighbors[8];
    bool nextGenAlive;
    int numNeighborsAlive;
	/*If a index manipulation results in an out of bounds, that undefined cell = 0*/
	//--------ORTHAGONAL neighbors
	//veritcal
	if ((row - 1) < 0) { // we are on the top row, so wrap around to the bottom row
        neighbors[1] = *(matrix + ((row - 1 + rows) * columns) + (col + 0));
	} else {
		neighbors[1] = *(matrix + ((row - 1) * columns) + (col + 0));
	}
	if ((row + 1) >= rows) { // We are on the bottom row, so wrap around to the top row
		neighbors[6] = *(matrix + ((0) * columns) + (col + 0));
	} else {
		neighbors[6] = *(matrix + ((row + 1) * columns) + (col + 0));
	}
	//horizontal
	if ((col - 1) < 0) { // we are on the left most column
		neighbors[3] = *(matrix + ((row + 0) * columns) + (col - 1 + columns));
	} else {
		neighbors[3] = *(matrix + ((row + 0) * columns) + (col - 1));
	}
	if ((col + 1) >= columns) { // we are on the right most column
        neighbors[4] = *(matrix + ((row + 0) * columns) + (0));
	} else {
		neighbors[4] = *(matrix + ((row + 0) * columns) + (col + 1));
	}
	//---------CORNER neighbors
    if ((row - 1) < 0) {
        if ((col - 1) < 0) { // top left corner
            neighbors[0] = *(matrix + ((row - 1 + rows) * columns) + (col - 1 + columns));
        } else { // top row but not far left column
            neighbors[0] = *(matrix + ((row - 1 + rows) * columns) + (col - 1));
        }
    } else if ((col - 1) < 0) { // left column but not top row
        neighbors[0] = *(matrix + ((row - 1) * columns) + (col - 1 + columns));
    } else { // neither the top row or the far left column
        neighbors[0] = *(matrix + ((row - 1) * columns) + (col - 1));
    }
    
    if ((row - 1) < 0) {
        if ((col + 1) >= columns) { // top right corner
            neighbors[2] = *(matrix + ((row - 1 + rows) * columns) + (0));
        } else { // top row but not far right column
            neighbors[2] = *(matrix + ((row - 1 + rows) * columns) + (col + 1));
        }
    } else if ((col + 1) >= columns) { // right column but not top row
        neighbors[2] = *(matrix + ((row - 1) * columns) + (0));
    } else { // neither the top row or the far right column
        neighbors[2] = *(matrix + ((row - 1) * columns) + (col + 1));
    }
    
    if ((row + 1) >= rows) {
        if ((col - 1) < 0) { // bottom left corner
            neighbors[5] = *(matrix + ((0) * columns) + (col - 1 + columns));
        } else { // bottom row but not far left column
            neighbors[5] = *(matrix + ((0) * columns) + (col - 1));
        }
    } else if ((col - 1) < 0) { // far left column but not bottom row
        neighbors[5] = *(matrix + ((row + 1) * columns) + (col - 1 + columns));
    } else { // neither the bottom row or the left column
        neighbors[5] = *(matrix + ((row + 1) * columns) + (col - 1));
    }
    
    if ((row + 1) >= rows) { 
        if ((col + 1) >= columns) { // bottom right corner
            neighbors[7] = *(matrix + ((0) * columns) + (0));
        } else { // bottom row but not right most column
            neighbors[7] = *(matrix + ((0) * columns) + (col + 1));
        }
    } else if ((col + 1) >= columns) { // right most column but not bottom row
        neighbors[7] = *(matrix + ((row + 1) * columns) + (0));
    } else { // neither the bottom row or the right column
        neighbors[7] = *(matrix + ((row + 1) * columns) + (col + 1));
    }

	//II.) Determine whether current cell lives or dies according to rules
    numNeighborsAlive = arraySum(neighbors);
    nextGenAlive = false;
	if (currGenAlive == 1) {
	//1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
		if (numNeighborsAlive < 2)
			nextGenAlive = false;
	//2. Any live cell with two or three live neighbours lives on to the next generation.
		if (numNeighborsAlive == 2 || numNeighborsAlive == 3)
			nextGenAlive = true;
	//3. Any live cell with more than three live neighbours dies, as if by overcrowding.
		if (numNeighborsAlive > 3)
			nextGenAlive = false;
	} else {
	//4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
		if (numNeighborsAlive == 3)
			nextGenAlive = true;
	}
	
	return nextGenAlive;
}

/*
 * @returns the number of alive neighbors
 */ 
int arraySum(int *array) {
	int sum = 0;
    int i = 0;
    int size = 8;
	for (; i < size; i++) {
		sum += *(array + i);
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
 * Swap the history so (history + 0) = newGen and newGen = (history + historySize - 1)
 */
void swapGen(int **newGen, int ***history, int historySize) {
    
    int i = historySize - 1;
    
    // Store last spot in history into temp. This will become newGen
    int *temp = *(*history + i);
    for (; i > 0; i--) {
        *(*history + i) = *(*history + i - 1);
    }
    *(*history) = *newGen;
    *newGen = temp;
    
}

/*
 * Generates a random starting habitat for the game's seed. Note: Seeding can reoccur in a cell at random.
 */
void randSeed(int rands, int *matrix, int rows, int columns) {
	int m = 0, n = 0, i = 0;
	for (i = rands; i > 0; i--) {
		m = rand() % rows;
        n = rand() % columns;
        *(matrix + (columns * m) + n) = 1;
    }
}
