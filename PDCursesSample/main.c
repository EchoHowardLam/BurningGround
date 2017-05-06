#include <time.h>
#include <math.h>

#include "shared_definition.h"
#include "gameobj.h"
#include "local_region.h"
#include "image_load.h"
#include "map_load.h"
#include "ai.h"

#include "event_handle.h"

extern double DIRECTION2X[4];
extern double DIRECTION2Y[4];
extern double DIAGONALX[4];
extern double DIAGONALY[4];

void clearScreen(void) {
	// this creates less flicker than standard "clear()"
	move(0, 0);
	for (int i = 0; i < SCREEN_HEIGHT;i++)
		printw("                                                                                ");
	
}

void printInMiddle(int y, int colorPair, char *string) {
	int length = (int) strlen(string);
	
	int x = (SCREEN_WIDTH - length)/2;
	attron(COLOR_PAIR(colorPair));
	mvprintw(y, x, "%s",string);
	attroff(COLOR_PAIR(colorPair));
}

char *executablePath;

int doMenu(void) {
	int selectedItem = 0;
	int totalItem = 3;

	while (1) {
		// 1. get buffered user input
		int ch = getch();
		if (ch == KEY_UP || ch == 'w' || ch == 'W')
			selectedItem = (selectedItem - 1 + totalItem) % totalItem;
		else if (ch == KEY_DOWN || ch == 's' || ch == 'S')
			selectedItem = (selectedItem + 1) % totalItem;
		else if (ch == ' ') {
			return selectedItem;
		}

		// 2. render the display this turn
		clear();		// clear what's on screen last time
		
		printInMiddle(7, COLOR_B_YELLOW, "Burning Ground");
		//printInMiddle(8, 0, "");
		printInMiddle(9, COLOR_B_BLACK, "  Start Game  ");
		printInMiddle(10, COLOR_B_BLACK, "  Credit  ");
		printInMiddle(11, COLOR_B_BLACK, "  Exit  ");
		
		switch (selectedItem % totalItem)
		{
		case 0:
			printInMiddle(9, COLOR_WHITE, "> Start Game <");
			break;
		case 1:
			printInMiddle(10, COLOR_WHITE, "> Credit <");
			break;
		case 2:
			printInMiddle(11, COLOR_WHITE, "> Exit <");
			break;
		}
		
		printInMiddle(20, 7, "                  Press space to select ...                  ");
		
		refresh();		// update the display in one go, very important

		// 3. stop running for some time to prevent using up all CPU power;
		threadSleep(10);			// want to sleep for roughly 10ms
	}
	return 1;
}

void doCredit(void) {

	while (1) {
		// 1. get buffered user input
		int ch = getch();
		if (ch == ' ')
			break;
		// 2. render the display this turn
		clear();		// clear what's on screen last time

		printInMiddle(7, COLOR_WHITE, "Credit");
		printInMiddle(9, COLOR_WHITE, " Echo ");
		printInMiddle(10, COLOR_WHITE, " Sherlock ");

		printInMiddle(20, COLOR_WHITE, "                  Press space to exit ...                  ");

		refresh();		// update the display in one go, very important

						// 3. stop running for some time to prevent using up all CPU power;
		threadSleep(10);			// want to sleep for roughly 10ms
	}
	return;
}

void gameOver(void) {

	while (1) {
		// 1. get buffered user input
		int ch = getch();
		if (ch == ' ')
			break;
		// 2. render the display this turn
		clear();		// clear what's on screen last time

		printInMiddle(7, COLOR_B_RED, "Game Over");
		printInMiddle(9, COLOR_RED, "You died");
		printInMiddle(12, COLOR_WHITE, "Hint");
		printInMiddle(13, COLOR_WHITE, "[Mouse 1] ice blob");
		printInMiddle(14, COLOR_WHITE, "[Space] freeze ray");
		printInMiddle(15, COLOR_WHITE, "[e] impact bomb");
		printInMiddle(16, COLOR_WHITE, "[g] grenade");
		printInMiddle(17, COLOR_WHITE, "[r] restart");
		printInMiddle(18, COLOR_WHITE, "[~] debug vision");

		printInMiddle(20, COLOR_WHITE, "Press space to return to menu ... ");

		refresh();		// update the display in one go, very important

						// 3. stop running for some time to prevent using up all CPU power;
		threadSleep(10);			// want to sleep for roughly 10ms
	}
	return;
}

void error(void) {

	while (1) {
		// 1. get buffered user input
		int ch = getch();
		if (ch == ' ')
			break;
		// 2. render the display this turn
		clear();		// clear what's on screen last time

		printInMiddle(7, 0, "We encounter a fatal error");

		printInMiddle(20, 0, "Press space to command suicide... ");

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

// 0 fatal error 1 gameover 2 restart
int doGameLoop() {
	// Extended characters table: http://melvilletheatre.com/articles/ncurses-extended-characters/index.html
	// e.g. addch(97 | A_ALTCHARSET) will print out a "brick" character
	//      addch(96 | A_ALTCHARSET) will print out a diamond
	
	// setup the level and player!
	clear();
	initializeObjects();
	if (!initializeInputEvents()) return 0;
	Coordinate start, end;
	Region localMap = loadLevel(TUTORIAL, &start, &end, executablePath);
	playerId = createObject(&localMap, -1, LIFE_HUMANOID, start.x, start.y);
	if (playerId == -1) return 0;
	if (createObject(&localMap, -1, LIFE_HUMANOID, start.x + 10, start.y) == -1) return 0;
	
	doInitialSpawn(&localMap);

	Coordinate scrTopLeft = {0, 0};
	BOOL keyboardPress[ACCEPTABLE_KEY_NUM] = { FALSE };
	MOutput mouseEvents;
	mouseEvents.buttonState = 0;

	BOOL restart = FALSE;
	BOOL debugVision = FALSE;
	int coolDown = 0;
	int coolDown2 = 0;

	// main game loop...
	while (1) {
		// 1. draw background...
		//drawBackground();
		clear();

		for (int L = 0; L < 5; L++)
		{
			// 2. get buffered user input and determine player action
			if (coolDown > 0) coolDown--;
			if (coolDown2 > 0) coolDown2--;
			for (int i = 0; i < ACCEPTABLE_KEY_NUM; i++)
				keyboardPress[i] = FALSE;
			getAllUserInputs(keyboardPress, &mouseEvents);
			combineWASDwasdKeys(keyboardPress);
			combineArrowKeys(keyboardPress);
			combinewasdArrowKeys(keyboardPress);
			if (keyboardPress[KB_UP_KEY]) { controlObjectY(playerId, floor(gameObject[playerId].y) - 0.5, 0.15); playerFacing = UP; }
			else if (keyboardPress[KB_DOWN_KEY]) { controlObjectY(playerId, floor(gameObject[playerId].y) + 1.5, 0.15); playerFacing = DOWN; }
			if (keyboardPress[KB_LEFT_KEY]) { controlObjectX(playerId, floor(gameObject[playerId].x) - 0.5, 0.15); playerFacing = WEST; }
			else if (keyboardPress[KB_RIGHT_KEY]) { controlObjectX(playerId, floor(gameObject[playerId].x) + 1.5, 0.15); playerFacing = EAST; }
			if (mouseEvents.buttonState && coolDown <= 0) {
				coolDown += 50;
				double destX = mouseEvents.x + scrTopLeft.x + 0.5;
				double destY = mouseEvents.y + scrTopLeft.y + 0.5;
				createObjectMagicProjectile(&localMap, playerId, MAGIC_BLOB, gameObject[playerId].x, gameObject[playerId].y, destX, destY, 0.2, -1, SPHERE_ICE, 0);
					//createObjectProjectileDest(&localMap, playerId, BULLET, gameObject[playerId].x, gameObject[playerId].y, destX, destY, 0.2, -1, DESTROY_CRITERIA_HIT, FALSE);
			}
			if (keyboardPress[' '])
			{
				double destX = mouseEvents.x + scrTopLeft.x + 0.5;
				double destY = mouseEvents.y + scrTopLeft.y + 0.5;
				createObjectMagicProjectile(&localMap, playerId, MAGIC_LASER, gameObject[playerId].x, gameObject[playerId].y, destX, destY, 0.0, -1, SPHERE_ICE, 0);
			}
			if (keyboardPress['e'] && coolDown2 <= 0)
			{
				coolDown2 += 300;
				double destX = mouseEvents.x + scrTopLeft.x + 0.5;
				double destY = mouseEvents.y + scrTopLeft.y + 0.5;
				createObjectProjectileDest(&localMap, playerId, BOMB, gameObject[playerId].x, gameObject[playerId].y, destX, destY, 0.3, 500, DESTROY_CRITERIA_STOP, TRUE);
			}
			else if (keyboardPress['g'] && coolDown2 <= 0) {
				coolDown2 += 200;
				double destX = mouseEvents.x + scrTopLeft.x + 0.5;
				double destY = mouseEvents.y + scrTopLeft.y + 0.5;
				createObjectProjectileDest(&localMap, playerId, BOMB, gameObject[playerId].x, gameObject[playerId].y, destX, destY, 0.3, 200, 0, TRUE);
			}
			else if (keyboardPress['l'] && coolDown2 <= 0) {
				coolDown2 += 10;
				double destX = mouseEvents.x + scrTopLeft.x + 0.5;
				double destY = mouseEvents.y + scrTopLeft.y + 0.5;
				createObjectMagicRain(&localMap, -1, MAGIC_BLOB, destX, destY, 20, 20, 0.1, SPHERE_MYTH, ENCHANT_STUN | ENCHANT_ENTANGLE);
				//createObjectMist(&localMap, playerId, MIST, gameObject[playerId].x, gameObject[playerId].y, 1000, SPHERE_ICE, 0);
			}
			if (keyboardPress['~'])
				debugVision = !debugVision;
			if (keyboardPress['r'])
			{
				deleteObject(&localMap, playerId, TRUE);
				restart = TRUE;
			}
			
			aiRun(&localMap, playerId);
			spawnCheck(&localMap);

			// 3. update all game objects positions
			updateObjectsStatus(&localMap);
			acceObjects(&localMap);
			moveObjects(&localMap);
			rotateObjects(&localMap);
		}
		
		// 4. render the display this turn
		if (playerId != -1 && gameObject[playerId].type == LIFE_HUMANOID)
		{
			scrTopLeft.x = floor(gameObject[playerId].x) - SCREEN_WIDTH / 2;
			scrTopLeft.y = floor(gameObject[playerId].y) - SCREEN_HEIGHT / 2;
			drawLocalRegion(&localMap, gameObject[playerId].underEffect[EFFECT_BLIND], scrTopLeft, SCREEN_WIDTH, SCREEN_HEIGHT);
			displayObjects(&localMap, playerId, scrTopLeft, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (debugVision) drawLocalRegionBlocked(&localMap, gameObject[playerId].underEffect[EFFECT_BLIND], scrTopLeft, SCREEN_WIDTH, SCREEN_HEIGHT);
			displayCrossHair(mouseEvents.x, mouseEvents.y);
			refresh();		// update the display in one go
		}

		// 5. stop running for some time to prevent using up all CPU power;
		// if you want to compensate for computational time and sleep non-fixed amount of time,
		// you will need to get system time like clock() and calculate, but that is not necessary most of the time
		threadSleep(20);			// want to sleep for a few ms; for Mac, probably have to include another library

		if (gameObject[playerId].type != LIFE_HUMANOID)
			break;
	}
	cleanUpLocalRegion(&localMap);
	if (restart) return 2;
	return 1;
}

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
#if defined(_WIN32) || defined(_WIN64)
	system("cd ../PDCursesSample/");
	executablePath = ".\\";
#else
	executablePath = argv[0];
#endif
	if (!loadImageFiles(executablePath))
	{
		error();
		return 0;
	}
	
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
	setUpColors();

	// Game logic!
	int selectedMenu;
	do {
		selectedMenu = doMenu();
		if (selectedMenu == 0)
		{
			BOOL restart;
			do {
				restart = FALSE;
				switch (doGameLoop())
				{
				case 0:
					error();
					return 0;
				case 1:
					gameOver();
					break;
				case 2:
					restart = TRUE;
					break;
				default:
					break;
				}
			} while (restart);
		}
		else if (selectedMenu == 1) doCredit();
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

