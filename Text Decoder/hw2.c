#include<stdlib.h>
#include<stdio.h>
#include<time.h>

/* grid dimensions */
#define ROWS 15
#define COLS 15

/* probability that a bush will catch fire if its neighbor is on fire */
#define BUSH_FIRE_PROBABILITY 0.75
/* probability that a tree will catch fire if its neighbor is on fire */
#define TREE_FIRE_PROBABILITY 0.50

/* time units it takes for a tree or a bush to burn completely */
#define TREE_BURN_TIME 5
#define BUSH_BURN_TIME 2


// ================================ Constants ================================

/* Set IS_RANDOM to 1 to get a different grid every time the game starts.
 * Set it to 0 to get the same grid every time */
const int IS_RANDOM = 1; 

/* The possible values of a cell in the grid */
#define BLANK -1
#define FIRE 0  
#define BUSH 1 
#define TREE 2 
#define ROCK 3
#define BURNT 4


#define high 100
#define low 1


/* The terrain grid. This is what gets drawn on the graphics window */
int grid[ROWS][COLS];

/* Helper table. For each cell, it keeps track of how long it has until it 
 * burns down completely. Cells that do not burn (e.g. rocks) have a value of -1 */
int burn_time_left[ROWS][COLS];

/* Helper table. For each cell, it keeps track of whether it should be marked
 * as BURNT at the end of the simulation step. All cells are initialized to BLANK. */
int marked_for_burning_out[ROWS][COLS];

/* AN XREIASTEITE KI ALLO BOITHITIKO PINAKA, DILWSTE TON EDW */

//EUSTATHIOS TSITSOPOULOS, XRISTOS KATSAROS
int tablehelp[ROWS][COLS];		

/*
 * step()
 * Executes one step (one unit of time) of the fire simulation.
 */

int probability () { 													//I sinartisi pou paragei pseudotixeous ari8mous.
	int pi8anothta;
	pi8anothta = low + rand() % (high - low + 1);
	return (pi8anothta);
}

void step() {
	
	int i, j;															//Dilwsi metablitwn.
	double random;
	
	srand((int)time(NULL));
	
	for (i=0; i<ROWS; i++) {
		for (j=0; j<COLS; j++){											//Arxikopoioume enan pinaka-xronometro.
			if (grid[i][j]==TREE) {
				burn_time_left[i][j]=TREE_BURN_TIME;
			}
			else if (grid[i][j]==BUSH) {
				burn_time_left[i][j]=BUSH_BURN_TIME;
			}
		}
	}
	
	for (i=0; i<ROWS; i++) {											//O pinakas-xronometro ksekinaei tin antistrofi metrisi otan ena stoixeio pairnei fwtia,kai stamataei otan midenistei to keli.
		for (j=0; j<COLS; j++){
			if (grid[i][j]==FIRE) {
				burn_time_left[i][j]=burn_time_left[i][j]-1;
				if (burn_time_left[i][j]==0) {
					grid[i][j]=BURNT;									// Otan midenistei to "xronometro" to stoixeio exei pleon kaei.
				}
			}
		}
	}

	
	for (i=0; i<ROWS; i++){												//Arxikopoioume enan boi8itiko pinaka me kena.
		for (j=0; j<COLS; j++){
			tablehelp[i][j]=BLANK;
		}
	}
	for (i=0; i<ROWS; i++) {											//Elegxoume poia stoixeia tou pinaka exoun parei fwtia.
		
		for (j=0; j<COLS; j++) {
			
			if (grid[i][j] == FIRE) {
				if (i==0 && j==0){										//Elegxoume ta diafora pi8ana simeia pou 8a eksaplw8ei i fwtia analoga me ti 8esi tou keliou pou einai se fwtia.
					if (grid[i][j+1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					else if (grid[i][j+1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					if (grid[i+1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					else if (grid[i+1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
				}
				else if (i==0 && j==COLS-1) {
					if (grid[i][j-1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					else if (grid[i][j-1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					if (grid[i+1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					else if (grid[i+1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
				}
				else if (j==0 && i == ROWS-1) {
					if (grid[i][j+1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					else if (grid[i][j+1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}

					if (grid[i-1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
					else if (grid[i-1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
				}
				else if (i==ROWS-1 && j==COLS-1) {
					if (grid[i-1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
					else if (grid[i-1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
					if (grid[i][j-1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					else if (grid[i][j-1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
				}
				else if (i==0) {
					if (grid[i][j+1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					else if (grid[i][j+1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					if (grid[i+1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					else if (grid[i+1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					if (grid[i][j-1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					else if (grid[i][j-1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
				}
				else if (i==ROWS-1) {
					if (grid[i][j+1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					else if (grid[i][j+1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					if (grid[i][j-1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					else if (grid[i][j-1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					if (grid[i-1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
					else if (grid[i-1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
				}
				else if (j==0) {
					if (grid[i][j+1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					else if (grid[i][j+1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					if (grid[i+1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					else if (grid[i+1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					if (grid[i-1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
					else if (grid[i-1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
				}
				else if (j==COLS-1) {
					if (grid[i+1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					else if (grid[i+1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					if (grid[i][j-1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					else if (grid[i][j-1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					if (grid[i-1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
					else if (grid[i-1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
				}
				else{
					if (grid[i][j+1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					else if (grid[i][j+1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j+1]=FIRE;
						}
					}
					if (grid[i+1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					else if (grid[i+1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i+1][j]=FIRE;
						}
					}
					if (grid[i][j-1]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					else if (grid[i][j-1]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i][j-1]=FIRE;
						}
					}
					if (grid[i-1][j]==BUSH) {
						random=probability()/100.0;
						if (random<BUSH_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
					else if (grid[i-1][j]==TREE) {
						random=probability()/100.0;
						if (random<TREE_FIRE_PROBABILITY){
							tablehelp[i-1][j]=FIRE;
						}
					}
				}
			}
		}
	}
	
	
	for (i=0; i<ROWS; i++) {											//Pername ta stoixeia pou exoun parei fwtia apo ton boi8itko ston pinaka grid.
		for (j=0; j<COLS; j++){
			if (tablehelp[i][j]!=BLANK) {
				grid[i][j]=tablehelp[i][j];
			}
		}
	}
}
