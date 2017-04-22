#pragma once

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#if defined(_WIN32) || defined(_WIN64)
#include "curses.h"
#include <Windows.h>
#else
#include "ncurses.h"		// please download yourself on Mac...
#include <unistd.h>
#endif

struct vector {
	double x;
	double y;
};

typedef struct vector Vector;
typedef struct vector Coordinate;
typedef struct vector Dimension;
typedef int BOOL;

typedef enum { NOTHING = 0, PLAYER, BULLET, BOMB, FRAGMENT } ObjectType;
typedef enum { UP, EAST, DOWN, WEST } Direction;
extern double DIRECTION2X[4];
extern double DIRECTION2Y[4];
extern double DIAGONALX[4];
extern double DIAGONALY[4];

void setupDisplay();
void cpuSleep(unsigned ms);
