#pragma once

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

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
void threadSleep(unsigned ms);
