/* 
 * 	Setup for hw2 - fire simulation.
 * 	-- Initializes the window 
 * 	-- Initializes the tables.
 *	-- Students are not expected to add code here. 
*/


// ============================== Header files ================================
#include "introGlutLib.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

// ============================ special #include ==============================
/* Include student code here. This is done for two reasons:
 * - all student written code is in a separate file
 * - we want to facilitate compilation since we haven't yet talked about 
 *   header files 
 */
#include"hw2.c"

// =========================== #define directives =============================
#define TRUE 1
#define FALSE 0

/* Button dimensions in pixels */
#define BUTTON_HEIGHT 35 
#define BUTTON_LENGTH 70

/* Position of QUIT button relative to the window */
#define QUIT_BUTTON_X 0	 
#define QUIT_BUTTON_Y (NU_SCREENHEIGHT-BUTTON_HEIGHT)

/* Position of message area, relative to the quit button */
#define MESSAGE_AREA_X (QUIT_BUTTON_X + BUTTON_LENGTH * 2)
#define MESSAGE_HEIGHT 15
#define MESSAGE_AREA_Y (NU_SCREENHEIGHT-MESSAGE_HEIGHT)

/* Position of terrain grid relative to the window and button area */
#define GRID_UPPER_LEFT_X 0
#define GRID_UPPER_LEFT_Y (NU_SCREENHEIGHT-BUTTON_HEIGHT)
#define GRID_HEIGHT (NU_SCREENHEIGHT-BUTTON_HEIGHT)
#define GRID_WIDTH NU_SCREENWIDTH

/* Dimensions of individual grid cells relative to the size and dimensions of the grid */
#define SQ_WIDTH (GRID_WIDTH / COLS)
#define SQ_HEIGHT (GRID_HEIGHT / ROWS)

/* Probabilities for various types of terrain. Must add up to 1 */
#define IS_TREE_PROBABILITY 0.55
#define IS_BUSH_PROBABILITY 0.40
#define IS_ROCK_PROBABILITY 0.05

// ================================ Globals ==================================
static int fireStarted = FALSE; /* indicates whether the spark has been set */

// ========================== Function prototypes ============================
void myDisplay();			
void DrawQuitButton() ;
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char key, int x, int y);
void InitializeObjects();
void DrawTerrain();
int isFlammable(int row, int col);


// ================================ Main ======================================				
int main() {

	InitializeObjects();	
	InitGraphics();		
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glutMainLoop();			

	return 0;
}

// ============================Helper functions ===============================				
 

/*
 * isFlammable(row, col)
 * Checks whether the terrain grid contains something flammable in a given cell
 *
 * Parameters: row and column indices of a two-dimensional table representing 
 * 				the terrain grid
 * Returns: 1 if the cell at indices [row][col] contains something flammable
 * 			(currently, a tree or a bush),
 * 			0 otherwise
 */
int isFlammable(int row, int col) {
	return grid[row][col] == BUSH || grid[row][col] == TREE;
}

/* InitializeObjects()
 * Initializes all tables and the seed for the randomizer.
 *
 * Parameters: None
 * Returns: Nothing 
 * */
void InitializeObjects() {

	int i, j, prob;
	if (IS_RANDOM) { 
		srand( (int) time(NULL) );
	}	
	else { /* Set IS_RANDOM to 0 for constant seed (for debugging)*/ 
		srand(0);
	}
	
	for (i=0; i<ROWS; i++) {
		for (j=0; j<COLS; j++) {
			marked_for_burning_out[i][j] = BLANK; /* generic initialization */
			burn_time_left[i][j]=0;
			prob = rand()%100;
			if (prob <  IS_BUSH_PROBABILITY*100) {
				grid[i][j] = BUSH;
				burn_time_left[i][j]=BUSH_BURN_TIME;
			}
			else if (prob < (IS_BUSH_PROBABILITY+IS_TREE_PROBABILITY)*100) {
				grid[i][j] = TREE;
				burn_time_left[i][j]=TREE_BURN_TIME;
			}
			else {
				grid[i][j] = ROCK;
				burn_time_left[i][j]=-1; /* rocks don't burn */
			}
		}	
	}
}
		
		

/**********************************************************************
 myMouse(button, state, x, y)

 GLUT CALLBACK: Don't call this function in your program--GLUT does it.

 button: Which button was clicked. 
			Possible values are GLUT_LEFT for the left button and GLUT_RIGHT 
			for the right one
 state: Is the button clicked (GLUT_DOWN) or not (GLUT_UP)?
 x, y: The coordinates of the place where you clicked (in pixels)
***********************************************************************/
			
void myMouse(int button, int state, int x, int y) {
	
	/* Even though the normal output window has the origin point 
	 * in the lower left corner, the mouse handling function assumes that
	 * it's the upper left corner. So we change y to keep things consistent .
	 */
	int new_y = NU_SCREENHEIGHT-y;
	int row, col;

	if (state == GLUT_DOWN) {
		
		/* A button is being pressed. Set the correct motion function */
		if (button==GLUT_LEFT) {
			
			/* check if the quit button was pressed, and if so, exit. */
			if (x <= QUIT_BUTTON_X+BUTTON_LENGTH  && x>=QUIT_BUTTON_X 
				&& new_y <= QUIT_BUTTON_Y+BUTTON_HEIGHT && new_y>=QUIT_BUTTON_Y ) { 
#ifdef __linux__
					glutLeaveMainLoop();
#else
					exit(0);
#endif
			}
			
			/* check if we are inside the terrain */
			if (new_y < GRID_UPPER_LEFT_Y) {
				/* figure out the exact cell */
				row = ROWS - (new_y / SQ_HEIGHT) - 1;
				col = x / SQ_WIDTH;
				/* if the fire has already been set, do one step of the simulation */
				if (fireStarted) {
					step();
					return;
				}
				/* otherwise, if the area is flammable, start the fire */
				if (isFlammable(row, col)) {
					grid[row][col] = FIRE;
					fireStarted = TRUE;
				} 
			}
		}
	}
} 


/**********************************************************************
 myKeyboard(key, x, y)

 GLUT CALLBACK: Don't call this function in your program--GLUT does it.
***********************************************************************/

void myKeyboard(unsigned char key, int x, int y) {
	switch(key)  {
		case 27:	/* User pressed the Esc key  */
		case 'Q':	/* User pressed the Q key */
		case 'q':  
#ifdef __linux__
				glutLeaveMainLoop();
#else
				exit(0);
#endif
		default: /* For any other key, execute one step of the simulation */
				step();
	}
}



/***************************************************************
 myDisplay()

 GLUT CALLBACK: Don't call this function in your program--GLUT does it.
***************************************************************/

void myDisplay(void) {

	int i, j;
	int ll_x, ll_y, ur_x, ur_y; /* lower left and upper right coords */ 

	ClearWindow();
	SetBackgndColor(1, 1, 1);
	SetPenColor(0.2, 0.0, 0.2);
	DrawFillBox(0, NU_SCREENHEIGHT, NU_SCREENWIDTH, NU_SCREENHEIGHT);

	for (i=0; i<ROWS; i++) {
		for (j=0; j<COLS; j++) {
			ll_x = SQ_WIDTH * j;
			ll_y = SQ_HEIGHT * (ROWS - (i+1));
			ur_x = ll_x + SQ_WIDTH;
			ur_y = ll_y + SQ_HEIGHT;
			switch(grid[i][j]) {
				case FIRE: SetPenColor(1, 0, 0); /* red */
							break;
				case TREE: SetPenColor(0.0,0.25,0.0); /* dark green */
							break;
				case BUSH: SetPenColor(0.9, 1, 0.6); /* yellow-greenish */
							break;;
				case ROCK: SetPenColor(0.5, 0.5, 0.5); /* grey */
							break;	
				case BURNT: SetPenColor(0, 0, 0); /* black */
							break;
				default: SetPenColor(1, 1, 1); /* this shouldn't really happen */
							break;
			}
			DrawFillBox(ll_x, ll_y, ur_x, ur_y);
		}
	}

	DrawQuitButton();
	if (!fireStarted) {
		SetPenColor(0, 0, 0);
		DrawText2D(helv12, MESSAGE_AREA_X, MESSAGE_AREA_Y, \
			"CLICK ON ANY TREE (GREEN) OR BUSH (YELLOW) TO START FIRE");
	}
	else {
		SetPenColor(0, 0, 0);
		DrawText2D(helv12, MESSAGE_AREA_X, MESSAGE_AREA_Y, \
			"CLICK ANYWHERE ON THE GRID OR HIT ANY KEY (EXCEPT Q WHICH QUITS) TO RUN THE SIMULATION");
	}
	
}

/***************************************************************
    DrawQuitButton()
	This function draws a quit button at a location specified
	through #defined literals. 
***************************************************************/
void DrawQuitButton() {

	SetPenColor(1.0,0.0,0.0);		
	DrawFillBox(QUIT_BUTTON_X,QUIT_BUTTON_Y,QUIT_BUTTON_X+BUTTON_LENGTH,QUIT_BUTTON_Y+BUTTON_HEIGHT);
	SetPenColor(1.0,1.0,1.0);
	DrawText2D(helv18, QUIT_BUTTON_X+10,QUIT_BUTTON_Y+10,"QUIT");
}

		
