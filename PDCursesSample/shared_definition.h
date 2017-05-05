#pragma once

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include "curses.h"
#include <Windows.h>
#define COLOR_B_BLUE 9
#define COLOR_B_CYAN 11
#define COLOR_B_RED 12
#define COLOR_B_YELLOW 14
#else
#include "ncurses.h"
#include <unistd.h>
#define COLOR_B_RED 9
#define COLOR_B_YELLOW 11
#define COLOR_B_BLUE 12
#define COLOR_B_CYAN 14
#endif

#define COLOR_B_BLACK 8
#define COLOR_B_GREEN 10
#define COLOR_B_MAGENTA 13
#define COLOR_B_WHITE 15

#define BG_MOUSE_BUTTON1_PRESSED 1

#define GRAVITATIONAL_ACC 0.002
#define FRICTION_TRIGGER 0.0001

struct vector {
	double x;
	double y;
};

struct mouseOutput {
	int x, y;
	int buttonState;
};

typedef struct vector Vector;
typedef struct vector Coordinate;
typedef struct vector Dimension;
typedef struct mouseOutput MOutput;
typedef int BOOL;

typedef enum { NOTHING = 0,
			   DEMO_LIFE_CAN_FLY, DEMO_LIFE_CANNOT_FLY,
			   DEMO_OBJ_USING_IMG_LOADER,
			   LIFE_HUMANOID, LIFE_MUSHROOM, LIFE_EYEBALL, LIFE_MOSQUITOES,
			   BULLET, BOMB, FRAGMENT } ObjectType;
typedef enum { UP, EAST, DOWN, WEST } Direction;
extern double DIRECTION2X[4];
extern double DIRECTION2Y[4];
extern double DIAGONALX[4];
extern double DIAGONALY[4];

void setupDisplay();
void threadSleep(unsigned ms);


int lastIndexOf(char *str, char needle);
int indexNotOf(char *str, char needle);
int lastIndexNotOf(char *str, char needle);
