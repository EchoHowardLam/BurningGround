#include <time.h>
#include <math.h>

#include <shared_definition.h>
#include <game_object.h>
#include <local_region.h>
#include <image_load.h>
#include <map_load.h>
#include <ai.h>

#include <ui.h>
#include <magic_interface.h>
#include <event_handle.h>

extern double DIRECTION2X[4];
extern double DIRECTION2Y[4];
extern double DIAGONALX[4];
extern double DIAGONALY[4];

void clearScreen(void)
{
	// this creates less flicker than standard "clear()"
	move(0, 0);
	for (int i = 0; i < SCREEN_HEIGHT;i++)
		printw("                                                                                ");
	
}

void printInMiddle(int y, int colorPair, char *string)
{
	int length = (int) strlen(string);
	
	int x = (SCREEN_WIDTH - length)/2;
	attron(COLOR_PAIR(colorPair));
	mvprintw(y, x, "%s",string);
	attroff(COLOR_PAIR(colorPair));
}

void printInMiddleSkillDisplay(int y, int colorPair, char *string, int colorPair2, char *string2)
{
	int length = (int)strlen(string) + (int)strlen(string2);

	int x = (SCREEN_WIDTH - length) / 2;
	attron(COLOR_PAIR(colorPair));
	mvprintw(y, x, "%s", string);
	attroff(COLOR_PAIR(colorPair));
	attron(COLOR_PAIR(colorPair2));
	printw("%s", string2);
	attroff(COLOR_PAIR(colorPair2));
}

char *executablePath;

int doMenu(void)
{
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
		
		printInMiddle(7, COLOR_B_YELLOW, "Burning Ground v1.1");
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

void doCredit(void)
{

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

void error(void)
{

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

// 0 no restart, 1 restart
BOOL gameOver(void)
{
	while (1) {
		// 1. get buffered user input
		int ch = getch();
		if ((ch == 'q') || (ch == 'Q'))
			return FALSE;
		else if ((ch == 'r') || (ch == 'R'))
			return TRUE;
		// 2. render the display this turn
		clear();		// clear what's on screen last time

		printInMiddle(7, COLOR_B_RED, "Game Over");
		printInMiddle(9, COLOR_RED, "You died");

		printInMiddle(11, COLOR_WHITE, "[R] restart");

		printInMiddle(20, COLOR_B_RED, "Press [Q] to quit ... ");

		refresh();		// update the display in one go, very important

						// 3. stop running for some time to prevent using up all CPU power;
		threadSleep(10);			// want to sleep for roughly 10ms
	}
	return FALSE;
}

void winGame(void)
{
	while (1) {
		int ch = getch();
		if (ch == ' ')
			return;
		clear();
		printInMiddle(7, COLOR_B_YELLOW, "Congratulations");
		printInMiddle(9, COLOR_B_YELLOW, "You Win");
		printInMiddle(20, COLOR_B_RED, "Press space to quit ELITE Stream...");
		refresh();
		threadSleep(10);
	}
	return;
}

void bossMsg(void)
{
	while (1) {
		int ch = getch();
		if (ch == ' ')
			return;
		clear();
		printInMiddle(7, COLOR_B_RED, "Htrahdis Algebraica:");
		printInMiddle(9, COLOR_B_RED, "You Shall Not Pass!");
		printInMiddle(20, COLOR_B_RED, "Press space to continue...");
		refresh();
		threadSleep(10);
	}
	return;
}

void helpMenu(void)
{

	while (1) {
		// 1. get buffered user input
		int ch = getch();
		if (ch == ' ')
			break;
		// 2. render the display this turn
		clear();		// clear what's on screen last time

		printInMiddle(7, COLOR_WHITE, "Controls");
		printInMiddle(9, COLOR_WHITE, "WASD/Arrow keys: Movement");
		printInMiddle(10, COLOR_WHITE, "[1] ~ [3]: Select Magic");
		printInMiddle(11, COLOR_WHITE, "[Q] Health Potion");
		printInMiddle(12, COLOR_WHITE, "[E] Mana Potion");
		printInMiddle(13, COLOR_WHITE, "[R] Reset Game");
		printInMiddle(14, COLOR_WHITE, "[H] Help Menu");
		

		printInMiddle(20, COLOR_WHITE, "[Space] Resume gameplay");

		refresh();		// update the display in one go, very important

						// 3. stop running for some time to prevent using up all CPU power;
		threadSleep(10);			// want to sleep for roughly 10ms
	}
	return;
}

void chooseSkill(ArcaneType skillSet[UI_SKILL_SLOT], int oldLv)
{
	if (magicUnlockedAtLevel[oldLv - 1] == NOMAGIC) return;
	int selectedItem = 0;
	int totalItem = UI_SKILL_SLOT + 1;

	while (1) {
		// 1. get buffered user input
		int ch = getch();
		if (ch == KEY_UP || ch == 'w' || ch == 'W')
			selectedItem = (selectedItem - 1 + totalItem) % totalItem;
		else if (ch == KEY_DOWN || ch == 's' || ch == 'S')
			selectedItem = (selectedItem + 1) % totalItem;
		else if (ch == ' ') {
			break;
		}

		// 2. render the display this turn
		clear();		// clear what's on screen last time

		printInMiddle(7, COLOR_B_YELLOW, "Level Up");
		printInMiddleSkillDisplay(9, COLOR_WHITE, "You can learn ", magicNameString[magicUnlockedAtLevel[oldLv - 1]].color, magicNameString[magicUnlockedAtLevel[oldLv - 1]].string);
		printInMiddle(10, COLOR_WHITE, "Which of the following known skills would you abandon?");
		int firstOptionY = 11;
		for (int k = 0; k < UI_SKILL_SLOT; k++)
			printInMiddle(firstOptionY + k, COLOR_B_BLACK, magicNameString[skillSet[k]].string);
		printInMiddle(firstOptionY + UI_SKILL_SLOT + 1, COLOR_RED, "Abandon the new skill");

		for (int k = 0; k < UI_SKILL_SLOT; k++)
			if (selectedItem == k)
				printInMiddle(firstOptionY + k, COLOR_WHITE, magicNameString[skillSet[k]].string);
		if (selectedItem == UI_SKILL_SLOT)
			printInMiddle(firstOptionY + UI_SKILL_SLOT + 1, COLOR_B_RED, "Abandon the new skill");

		printInMiddle(20, 7, "                  Press space to select ...                  ");

		refresh();		// update the display in one go, very important

						// 3. stop running for some time to prevent using up all CPU power;
		threadSleep(10);			// want to sleep for roughly 10ms
	}
	if (selectedItem == UI_SKILL_SLOT) return;
	skillSet[selectedItem] = magicUnlockedAtLevel[oldLv - 1];
	return;
}

void drawBackground() {
	clearScreen();		// just call clear screen now...
}

extern GameObject gameObject[MAX_OBJECT];		// stores all game object!

int playerId;

// 0 fatal error 1 gameover 2 restart 3 finished level
int doGameLoop(PlayerState *playerStat, LevelName gameLevel) {
	// Extended characters table: http://melvilletheatre.com/articles/ncurses-extended-characters/index.html
	// e.g. addch(97 | A_ALTCHARSET) will print out a "brick" character
	//      addch(96 | A_ALTCHARSET) will print out a diamond
	
	// setup the level and player!
	clear();
	initializeObjects();
	if (!initializeInputEvents()) return 0;
	int playerLv = playerStat->lv;
	int lastKnownExp = playerStat->exp;
	Coordinate start, end = {
		-1,	//x
		-1	//y 
	};
	Region localMap;
	if (!loadLevel(&localMap, gameLevel, &start, &end, executablePath)) return 0;
	playerId = createHumanoid(&localMap, -1, HUMANOID_TYPE_HUMAN, start.x, start.y, playerLv);
	if (playerId == -1) return 0;
	int playerAliveFlag = 1;
	gameObject[playerId].endurance = playerStat->hp;
	gameObject[playerId].mana = playerStat->mp;
	gameObject[playerId].attri2 = playerStat->exp;
	gameObject[playerId].spawnRegionCount = &playerAliveFlag;
	int bossAliveFlag = 1;
	if (gameLevel == HELL)
	{
		bossMsg();
		if (!initializeInputEvents()) return 0;
		int bossId = createObject(&localMap, -1, PROFESSOR_HTRAHDIS, start.x + 300, start.y);
		if ( bossId == -1)
			return 0;
		gameObject[bossId].spawnRegionCount = &bossAliveFlag;
	}
	
	doInitialSpawn(&localMap);

	Coordinate scrTopLeft = {0, 0};
	BOOL keyboardPress[ACCEPTABLE_KEY_NUM] = { FALSE };
	MOutput mouseEvents;
	mouseEvents.buttonState = 0;

	BOOL restart = FALSE;
	BOOL pass = FALSE;
	BOOL playerFlying = FALSE;
	ArcaneType skillSet[UI_SKILL_SLOT];
	for (int k = 0; k < UI_SKILL_SLOT; k++)
		skillSet[k] = playerStat->skillSet[k];
	int selectedSkillIndex = 0;
	int potions[2] = { playerStat->potions[0], playerStat->potions[1] };
	BOOL debugVision = FALSE;
	int coolDown = 0;

	BOOL holdKey[2] = {FALSE, FALSE}; // corresponds to qQ, eE

	// main game loop...
	for (int round = 0; ; round++)
	{
		for (int L = 0; L < 5; L++)
		{
			// Auto-cancel flying state
			//if (checkObjectOnFeet(&localMap, playerId))
				playerFlying = FALSE;

			// 1. get buffered user input and determine player action
			if (coolDown > 0) coolDown--;
			for (int i = 0; i < ACCEPTABLE_KEY_NUM; i++)
				keyboardPress[i] = FALSE;
			getAllUserInputs(keyboardPress, &mouseEvents);
			combineWASDwasdKeys(keyboardPress);
			combineArrowKeys(keyboardPress);
			combinewasdArrowKeys(keyboardPress);
			if (keyboardPress[' '])
			{
				playerFlying = TRUE;
			}
			if (keyboardPress[KB_UP_KEY]) {
				//playerFlying = TRUE;
				if (playerFlying)
					controlObjectY(playerId, floor(gameObject[playerId].y) - 0.5, 0.1);
				else
					controlObjectY(playerId, floor(gameObject[playerId].y) - 0.5, 0.2);
			}
			else if (keyboardPress[KB_DOWN_KEY]) {
				if (playerFlying)
					controlObjectY(playerId, floor(gameObject[playerId].y) + 1.5, 0.15);
				else
					controlObjectY(playerId, floor(gameObject[playerId].y) + 1.5, 0.15);
			}
			if (keyboardPress[KB_LEFT_KEY])
			{
				if (playerFlying)
					controlObjectX(playerId, floor(gameObject[playerId].x) - 0.5, 0.2);
				else
					controlObjectX(playerId, floor(gameObject[playerId].x) - 0.5, 0.15);
			}
			else if (keyboardPress[KB_RIGHT_KEY]) {
				if (playerFlying)
					controlObjectX(playerId, floor(gameObject[playerId].x) + 1.5, 0.2);
				else
					controlObjectX(playerId, floor(gameObject[playerId].x) + 1.5, 0.15);
			}
			for (int k = 1; k <= UI_SKILL_SLOT; k++)
				if (keyboardPress['0' + k])
					selectedSkillIndex = k - 1;
			if (mouseEvents.buttonState && coolDown <= 0) {
				double destX = mouseEvents.x + scrTopLeft.x + 0.5;
				double destY = mouseEvents.y + scrTopLeft.y + 0.5;
				coolDown += castMagic(&localMap, playerId, skillSet[selectedSkillIndex], destX, destY);
			}
			if (keyboardPress['q'] || keyboardPress['Q'])
			{
				if (!holdKey[0])
				{
					holdKey[0] = TRUE;
					drinkHPPotion(playerId, &potions[0]);
				}
			}
			else holdKey[0] = FALSE;
			if (keyboardPress['e'] || keyboardPress['E'])
			{
				if (!holdKey[1])
				{
					holdKey[1] = TRUE;
					drinkMPPotion(playerId, &potions[1]);
				}
			}
			else holdKey[1] = FALSE;
			if (keyboardPress['~'])
				debugVision = !debugVision;
			if (keyboardPress['+'])
				gameObject[playerId].endurance += 1000;
			if (gameObject[playerId].endurance > gameObject[playerId].max_endurance)
				gameObject[playerId].endurance = gameObject[playerId].max_endurance;
			if (keyboardPress['-'])
				gameObject[playerId].mana += 1000;
			if (gameObject[playerId].mana > gameObject[playerId].max_mana)
				gameObject[playerId].mana = gameObject[playerId].max_mana;
			if (keyboardPress['/'])
				gameObject[playerId].attri2++;
			if (keyboardPress['r'] || keyboardPress['R'])
			{
				deleteObject(&localMap, playerId, TRUE);
				restart = TRUE;
			}
			if (keyboardPress[27])
				playerAliveFlag = 0;
			playerFlying = castFlying(playerId, playerFlying);
			
			aiRun(&localMap, playerId);
			spawnCheck(&localMap);

			// 2. update all game objects positions
			updateObjectsStatus(&localMap);
			acceObjects(&localMap);
			moveObjects(&localMap);
			rotateObjects(&localMap);

			// 3. Update player lv and check for finishing level
			if (playerAliveFlag >= 1 && (bossAliveFlag >= 1))
			{
				while (gameObject[playerId].attri2 >= EXP_NEEDED_TO_LV_UP[playerLv - 1])
				{
					gameObject[playerId].attri2 -= EXP_NEEDED_TO_LV_UP[playerLv - 1];
					if (playerLv < 30)
					{
						updateHumanoidStatistic(playerId, playerLv, playerLv + 1);
						chooseSkill(skillSet, playerLv);
						if (!initializeInputEvents()) return 0;
						playerLv++;
					}
				}
				lastKnownExp = gameObject[playerId].attri2;
				if (end.x != -1) // valid
				{
					if ((fabs(gameObject[playerId].x - end.x) <= 3.0) && (fabs(gameObject[playerId].y - end.y) <= 3.0))
					{
						pass = TRUE;
						break;
					}
				}
			}
		}
		if (bossAliveFlag < 1)
			pass = TRUE;
		if (pass)
			break;

		if (keyboardPress['h'] || keyboardPress['H'])
		{
			helpMenu();
			if (!initializeInputEvents()) return 0;
		}
		
		// 4. render the display this turn
		if (round > 50 && playerAliveFlag >= 1)
		{
			clear();
			scrTopLeft.x = floor(gameObject[playerId].x) - SCREEN_WIDTH / 2;
			scrTopLeft.y = floor(gameObject[playerId].y) - SCREEN_HEIGHT / 2;
			drawLocalRegion(&localMap, gameObject[playerId].underEffect[EFFECT_BLIND], scrTopLeft, SCREEN_WIDTH, SCREEN_HEIGHT);
			displayObjects(&localMap, playerId, scrTopLeft, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (debugVision) drawLocalRegionObjId(&localMap, gameObject[playerId].underEffect[EFFECT_BLIND], scrTopLeft, SCREEN_WIDTH, SCREEN_HEIGHT);
			displayCrossHair(mouseEvents.x, mouseEvents.y);
			drawUI(playerId, playerLv, skillSet, selectedSkillIndex, potions[0], potions[1]);
			refresh();		// update the display in one go
		}

		// 5. stop running for some time to prevent using up all CPU power;
		// if you want to compensate for computational time and sleep non-fixed amount of time,
		// you will need to get system time like clock() and calculate, but that is not necessary most of the time
		if (round > 50)
			threadSleep(20);			// want to sleep for a few ms; for Mac, probably have to include another library

		if (playerAliveFlag < 1)
			break;
	}
	cleanUpLocalRegion(&localMap);
	if (restart) // You keep nothing if you are not paying anything and retry
		return 2;
	playerStat->lv = playerLv;
	playerStat->exp = lastKnownExp;
	for (int k = 0; k < UI_SKILL_SLOT; k++)
		playerStat->skillSet[k] = skillSet[k];
	playerStat->potions[0] = potions[0];
	playerStat->potions[1] = potions[1];
	if (pass == TRUE)
	{
		playerStat->hp = gameObject[playerId].endurance;
		playerStat->mp = gameObject[playerId].mana;
		return 3;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
#if defined(_WIN32) || defined(_WIN64)
	//system("cd ../PDCursesSample/");
	//executablePath = ".\\";
	executablePath = argv[0];
#else
	executablePath = argv[0];
#endif
	if (!loadImageFiles(executablePath))
	{
		error();
		return 0;
	}

	// settings
	for (int i = 1; i <= MAX_LV; i++)
		EXP_NEEDED_TO_LV_UP[i - 1] = 3 * i * i + 3 * i + 1; // guess what geometry meaning it has
	
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
			PlayerState playerStat = {
				30,										//lv
				100000,									//hp
				100000,									//mp
				0,										//exp
				{ ARCANE_FIREBALL, ARCANE_FIRELASER, NOMAGIC },	//skillset
				{ 8, 8 }								//potions
			};
			BOOL restart;
			LevelName curGameLevel = TUTORIAL;
			do {
				restart = FALSE;
				switch (doGameLoop(&playerStat, curGameLevel))
				{
				case 0:
					error();
					return 0;
				case 1:
					restart = gameOver();
					break;
				case 2:
					restart = TRUE;
					break;
				case 3:
				{
					restart = TRUE;
					switch (curGameLevel)
					{
					case TUTORIAL:
						curGameLevel = FOREST; break;
					case FOREST:
						curGameLevel = PLATFORM; break;
					case PLATFORM:
						curGameLevel = HELL; break;
					case HELL:
						winGame(); restart = FALSE; break;
					default:
						break;
					}
					break;
				}
				default:
					break;
				}
			} while (restart);
		}
		else if (selectedMenu == 1) doCredit();
	} while (selectedMenu != 2);
	

	// Cleaning up...
	cleanUpImageFiles();
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

