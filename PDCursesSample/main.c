#include <stdlib.h> 
#include <time.h>
#include <math.h>

#include "shared_definition.h"
#include "gameobj.h"
#include "local_region.h"
#include "image_load.h"

extern double DIRECTION2X[4];
extern double DIRECTION2Y[4];
extern double DIAGONALX[4];
extern double DIAGONALY[4];

void clearScreen() {
	// this creates less flicker than standard "clear()"
	move(0, 0);
	for (int i = 0; i < SCREEN_HEIGHT;i++)
		printw("                                                                                ");
	
}

void printInMiddle(int y, int colorPair, char *string) {
	int length = strlen(string);
	
	int x = (SCREEN_WIDTH - length)/2;
	attron(COLOR_PAIR(colorPair));
	mvprintw(y, x, "%s",string);
	attroff(COLOR_PAIR(colorPair));
}

//char *executablePath;

int doMenu() {
	int selectedItem = 0;
	int totalItem = 3;
	//loadImageFiles(executablePath);

	while (1) {
		// 1. get buffered user input
		int ch = getch();
		if (ch == KEY_UP)
			selectedItem = (selectedItem - 1 + totalItem) % totalItem;
		else if (ch == KEY_DOWN)
			selectedItem = (selectedItem + 1) % totalItem;
		else if (ch == ' ') {
			return selectedItem;
		}

		// 2. render the display this turn
		clear();		// clear what's on screen last time
		
		printInMiddle(7, 0, "Burning Ground");
		//printInMiddle(8, 0, "");
		printInMiddle(9, 0, " Start Game ");
		printInMiddle(10, 0, " Credit ");
		printInMiddle(11, 0, " Exit ");
		
		switch (selectedItem % totalItem)
		{
		case 0:
			printInMiddle(9, 1, " Start Game ");
			break;
		case 1:
			printInMiddle(10, 1, " Credit ");
			break;
		case 2:
			printInMiddle(11, 1, " Exit ");
			break;
		}
		
		printInMiddle(20, 0, "Press space to select ... ");
		
		refresh();		// update the display in one go, very important

		// 3. stop running for some time to prevent using up all CPU power;
		threadSleep(10);			// want to sleep for roughly 10ms
	}
}

void doCredit() {

	while (1) {
		// 1. get buffered user input
		int ch = getch();
		if (ch == ' ')
			break;
		// 2. render the display this turn
		clear();		// clear what's on screen last time

		printInMiddle(7, 0, "Credit");
		printInMiddle(9, 0, " Echo ");
		printInMiddle(10, 0, " Sherlock ");

		printInMiddle(20, 0, "Press space to exit ... ");

		refresh();		// update the display in one go, very important

						// 3. stop running for some time to prevent using up all CPU power;
		threadSleep(10);			// want to sleep for roughly 10ms
	}
	return;
}

void drawBackground() {
	clearScreen();		// just call clear screen now...
}

extern GameObject gameObject[MAX_OBJECT];		// stores all game object!

int playerId;
Direction playerFacing;

int doGameLoop() {
	// Extended characters table: http://melvilletheatre.com/articles/ncurses-extended-characters/index.html
	// e.g. addch(97 | A_ALTCHARSET) will print out a "brick" character
	//      addch(96 | A_ALTCHARSET) will print out a diamond
	
	// setup the level and player!
	clear();
	playerId = createObject(PLAYER, 5, 5);

	Region localMap = generateEmptyLocalRegion(1000, 50);
	localRegionAddRect(&localMap, 0, 0, 1000, 50, 0);
	localRegionAddRect(&localMap, 0, 18, 35, 15, 1);
	localRegionAddRect(&localMap, 36, 18, 5, 1, 1);
	localRegionAddRect(&localMap, 60, 15, 10, 5, 1);
	localRegionAddRect(&localMap, 80, 20, 15, 5, 1);
	localRegionAddRect(&localMap, 200, 17, 30, 25, 1);
	for (int i = 0; i < 100; i++)
		localRegionAddRect(&localMap, 50 + rand() % 900, 10 + rand() % 30, 5 + rand() % 10, 5 + rand() % 10, 1);

	Coordinate scrTopLeft = {0, 0};
	
	// main game loop...
	while (1) {
		// 1. draw background...
		//drawBackground();
		clear();
		
		// 2. get buffered user input and determine player action
		int ch = getch();
		BOOL fired = FALSE;
		while (ch != ERR)
		{
			if (ch == KEY_UP) { controlObjectY(playerId, floor(gameObject[playerId].y) - 0.5, 0.6); playerFacing = UP; }
			else if (ch == KEY_DOWN) { controlObjectY(playerId, floor(gameObject[playerId].y) + 1.5, 0.5); playerFacing = DOWN; }
			else if (ch == KEY_LEFT) { controlObjectX(playerId, floor(gameObject[playerId].x) - 0.5, 0.5); playerFacing = WEST; }
			else if (ch == KEY_RIGHT) { controlObjectX(playerId, floor(gameObject[playerId].x) + 1.5, 0.5); playerFacing = EAST; }
			//if (ch == KEY_UP) { pushObjectDir(playerId, 0.0, -1.0, 0.5); playerFacing = NORTH; }
			//else if (ch == KEY_DOWN) { pushObjectDir(playerId, 0.0, 1.0, 0.2); playerFacing = SOUTH; }
			//else if (ch == KEY_LEFT) { pushObjectDir(playerId, -1.0, 0.0, 0.2); playerFacing = WEST; }
			//else if (ch == KEY_RIGHT) { pushObjectDir(playerId, 1, 0.0, 0.2); playerFacing = EAST; }
			else if (fired) {} // avoid multiplt fire in one go
			else if (ch == ' ') {
				// shoot!
				fired = TRUE;
				double destX = gameObject[playerId].x + DIRECTION2X[playerFacing];
				double destY = gameObject[playerId].y + DIRECTION2Y[playerFacing];
				int bulletId = createObjectProjectileDest(BULLET, gameObject[playerId].x, gameObject[playerId].y, destX, destY, 0.8, -1, DESTROY_CRITERIA_HIT, FALSE);
			}
			else if (ch == 'b') {
				fired = TRUE;
				double destX = gameObject[playerId].x + DIRECTION2X[playerFacing];
				double destY = gameObject[playerId].y + DIRECTION2Y[playerFacing] - 0.4;
				int bombId = createObjectProjectileDest(BOMB, gameObject[playerId].x, gameObject[playerId].y, destX, destY, 0.8, 20, 0, TRUE);
			}
			ch = getch();
		}

		// 3. update all game objects positions
		acceObjects(&localMap);
		moveObjects(&localMap);
		scrTopLeft.x = floor(gameObject[playerId].x) - SCREEN_WIDTH / 2;
		scrTopLeft.y = floor(gameObject[playerId].y) - SCREEN_HEIGHT / 2;
		drawLocalRegion(&localMap, scrTopLeft, SCREEN_WIDTH, SCREEN_HEIGHT);
		
		// 4. render the display this turn
		displayObjects(scrTopLeft, SCREEN_WIDTH, SCREEN_HEIGHT);
		refresh();		// update the display in one go

		// 5. stop running for some time to prevent using up all CPU power;
		// if you want to compensate for computational time and sleep non-fixed amount of time,
		// you will need to get system time like clock() and calculate, but that is not necessary most of the time
		threadSleep(20);			// want to sleep for a few ms; for Mac, probably have to include another library
	}
}

int main()
//int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
	//loadImageFiles("");
	//executablePath = argv[0];
	//loadImageFiles();
	
	// NOTE: Official HOWTO for Curses library: http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
	// NOTE: How to setup PDCurses: https://jdonaldmccarthy.wordpress.com/2014/09/05/how-to-set-up-pdcurses-in-visual-studio-2013-c/

	// Setting up the environment; you don't need to understand these things unless you want something entirely different
	setupDisplay();
	initscr();				// start the curses mode	
	raw();					// disable line buffering 
	keypad(stdscr, TRUE);	// to detect UP, DOWN etc	
	timeout(0);				// make getch() non-blocking, i.e. won't stop the program when we ask for user input
	noecho();				// don't print user input on screen while we do getch 
	curs_set(0);						// don't display cursor
	
	// Set up colors...colors are always in pairs in a terminal!
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);		// 1: inverse
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);	// 2: highlight

	// Game logic!
	int selectedMenu;
	do {
		selectedMenu = doMenu();
		if (selectedMenu == 0) doGameLoop();
		else if(selectedMenu == 1) doCredit();
	} while (selectedMenu != 2);
	

	// Cleaning up...
	endwin();			// end the curses mode
	return 0;

	/*Beep(523, 500);  // 523 hertz (C5) for 500 milliseconds
	Beep(587, 500);
	Beep(659, 500);
	Beep(698, 500);
	Beep(784, 500);
	Sleep(500);    // 500 ms delay
	puts("\n\\a makes a beep on the internal speaker too ...");
	Sleep(500);
	puts("\a");
	Sleep(500);
	puts("\a");*/
}

