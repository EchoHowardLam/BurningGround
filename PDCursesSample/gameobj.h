#pragma once

#include <windows.h>
#include <stdlib.h>
#include <math.h>

#include "shared_definition.h"
#include "local_region.h"

#define MAX_OBJECT 500

#define DESTROY_CRITERIA_HIT 1
#define DESTROY_CRITERIA_STOP 2

typedef struct {
	double x, y;				// current position
	double dispX, dispY;		// displacement needed to move before fixing position
	Vector vel;
	Vector motiveVel;
	ObjectType type;
	int turnsAlive;				// how long this object has lived
	int lifespan;				// how long this object can live
	int destroyCriteria;		// what criteria trigger destruction, masked by DESTROY_CRITERIA_...
	//BOOL holdPos;				// whether this object will hold position after dispX/Y is exhausted
	BOOL underMove;				// whether this object will continue moving by motiveSpeed
	BOOL underGravity;			// whether this object is affected by gravity
} GameObject;

extern GameObject gameObject[MAX_OBJECT];		// stores all game object!

void initializeObjects(void);
int createObject(ObjectType type, double startX, double startY);
int createObjectProjectileDir(ObjectType type, double startX, double startY, double dirX, double dirY, double speed, int lifespan, int destroyCriteria, BOOL underGravity);
int createObjectProjectileDest(ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int destroyCriteria, BOOL underGravity);
void displayObjects(Coordinate scrTopLeftPos, int scrW, int scrH);
void pushObjectDir(int id, double dirX, double dirY, double speed);
void acceObjects(Region *environment);
void controlObjectX(int id, double destX, double speed);
void controlObjectY(int id, double destY, double speed);
void moveObjects(Region *environment);
BOOL checkObjectOnFeet(Region *environment, int objId);
BOOL removeEnvironmentBlock(Region *environment, double x, double y);
