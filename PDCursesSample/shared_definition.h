#pragma once

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define M_PI 3.14159265358979323846

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

#define HUMANOID_TYPE_MASK 6
#define HUMANOID_TYPE_HUMAN 0
#define HUMANOID_TYPE_WIZARD 2
#define HUMANOID_TYPE_CORRUPTED_WINGMAN 4
#define HUMANOID_TYPE_WINGMAN 6

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
#define ENCHANT_EFFECT_MASK (256 - 1)
#define ENCHANT_COLD_SLOW 1
#define ENCHANT_SLOW 2
#define ENCHANT_BLIND 4
#define ENCHANT_STUN 8
#define ENCHANT_ENTANGLE 16
#define ENCHANT_CONFUSE 32
#define ENCHANT_CLOAK 64
#define ENCHANT_SILENT 128
#define ENCHANT_SHRAPNEL 1024

// increment as array index
#define TOTAL_EFFECT_COUNT 7
#define EFFECT_COLD_SLOW 0
#define EFFECT_SLOW 1
// blind is only useful for player
#define EFFECT_BLIND 2
#define EFFECT_STUN 3
#define EFFECT_ENTANGLE 4
#define EFFECT_CONFUSE 5
#define EFFECT_INVISIBLE 6


#define DMG_STANDARD_MOSQUITO_DAMAGE 2
#define DMG_STANDARD_BEE_DAMAGE 5
#define DMG_STANDARD_SLUDGE_MELEE_DAMAGE 10

#define DMG_STANDARD_FIREBALL_DAMAGE 500
#define DMG_STANDARD_ICEBALL_DAMAGE 450
#define DMG_STANDARD_DIRTBALL_DAMAGE 100
#define DMG_STANDARD_FIRERAIN_DAMAGE 100
#define DMG_STANDARD_ICERAIN_DAMAGE 90
#define DMG_STANDARD_FIRELASER_DAMAGE 20
#define DMG_STANDARD_ICELASER_DAMAGE 18

#define DMG_STANDARD_BOMB_DAMAGE 500

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
			   LIFE_MUSHROOM, LIFE_RABBIT, LIFE_SLUDGE,
			   LIFE_GRASS, LIFE_BEE, SPAWN_BEE_HIVE,
			   LIFE_BAT, LIFE_BIRD,
			   SPAWN_DURIAN_TREE, LIFE_DURIAN,
			   MAGIC_BLOB, MAGIC_SPIKE, MAGIC_LASER, MAGIC_FLAME, MAGIC_FRAGMENT,
			   SOLID_DURIAN,
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
double getRandomOfRange(int base);
