#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#include <math.h>

#include "shared_definition.h"
#include "local_region.h"
#include "image_load.h"

#define MAX_OBJECT 500

#define DESTROY_CRITERIA_HIT 1
#define DESTROY_CRITERIA_STOP 2

#define TURNING_UNSETTLED 2

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
	int facingDir;				// 0 left 1 right
	CharacterImage* sprite;
	int master;					// id of the object that spawns it, -1 for none
} GameObject;

extern GameObject gameObject[MAX_OBJECT];		// stores all game object!

void initializeObjects(void);
int createObject(Region *environment, int master, ObjectType type, double startX, double startY);
int createObjectProjectileDir(Region *environment, int master, ObjectType type, double startX, double startY, double dirX, double dirY, double speed, int lifespan, int destroyCriteria, BOOL underGravity);
int createObjectProjectileDest(Region *environment, int master, ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int destroyCriteria, BOOL underGravity);
int defaultObjectsInit(Region *environment, int objId);
void deleteObject(Region *environment, int id, BOOL silentDelete);
void displayObjects(Coordinate scrTopLeftPos, int scrW, int scrH); // please call updateObjectsStatus before calling this function
void displayCrossHair(int X, int Y);
void pushObjectDir(int id, double dirX, double dirY, double speed);
void acceObjects(Region *environment); // please call updateObjectsStatus before calling this function
void controlObjectX(int id, double destX, double speed);
void controlObjectY(int id, double destY, double speed);
void moveObjects(Region *environment); // please call updateObjectsStatus before calling this function
void rotateObjects(Region *environment);
void updateObjectsStatus(Region *environment);
BOOL checkObjectCollision(Region *environment, int objId, double x, double y); // please call updateObjectsStatus before calling this function, 0 miss 1 hit
BOOL checkObjectOnFeet(Region *environment, int objId);
BOOL removeEnvironmentBlock(Region *environment, double x, double y);
BOOL registerEnvironmentObject(Region *environment, int objId);
BOOL removeEnvironmentObject(Region *environment, int objId, double oldX, double oldY, CharacterImage *oldImage);

void doInitialSpawn(Region *target);
void spawnCheck(Region *target);
