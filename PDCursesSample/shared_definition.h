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

// increment as bit mask
#define SPHERE_FIRE 1
#define SPHERE_WATER 2
#define SPHERE_EARTH 4
#define SPHERE_WIND 8
#define SPHERE_ICE 16
#define SPHERE_MYTH 32

// outside ENCHANT_EFFECT_MASK, it is the enchantments that only affect the object itself
#define ENCHANT_EFFECT_MASK (64 - 1)
#define ENCHANT_COLD_SLOW 1
#define ENCHANT_BLIND 2
#define ENCHANT_STUN 4
#define ENCHANT_ENTANGLE 8
#define ENCHANT_CONFUSE 16
#define ENCHANT_CLOAK 32

// increment as array index
#define TOTAL_EFFECT_COUNT 6
#define EFFECT_COLD_SLOW 0
// only useful for player
#define EFFECT_BLIND 1
#define EFFECT_STUN 2
#define EFFECT_ENTANGLE 3
#define EFFECT_CONFUSE 4
#define EFFECT_INVISIBLE 5

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
			   LIFE_HUMANOID, LIFE_EYEBALL, LIFE_MOSQUITOES,
			   LIFE_MUSHROOM,
			   MAGIC_BLOB, MAGIC_SPIKE, MAGIC_LASER,
			   MIST, CORRUPTING_MIST,
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
