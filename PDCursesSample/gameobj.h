#pragma once

#include <math.h>

#include "shared_definition.h"
#include "local_region.h"

#define MAX_OBJECT 100

#define DESTROY_CRITERIA_HIT 1
#define DESTROY_CRITERIA_STOP 2

typedef struct {
	double x, y;				// current position
	double dispX, dispY;		// displacement needed to move before fixing position
	Vector speed;
	ObjectType type;
	int turnsAlive;				// how long this object has lived
	int lifespan;				// how long this object can live
	int destroyCriteria;		// what criteria trigger destruction, masked by DESTROY_CRITERIA_...
	BOOL holdPos;				// whether this object will hold position after dispX/Y is exhausted
	BOOL underGravity;			// whether this object is affected by gravity
} GameObject;

extern GameObject gameObject[MAX_OBJECT];		// stores all game object!

int createObject(ObjectType type, double startX, double startY);
int createObjectProjectileDest(ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int destroyCriteria, BOOL holdPos, BOOL underGravity);
void displayObjects(Coordinate scrTopLeftPos, int scrW, int scrH);
void pushObjectDir(int id, double dirX, double dirY, double speed);
void pushObjectDest(int id, double destX, double destY, double speed, BOOL holdPos);
void moveObjects(Region *environment);

