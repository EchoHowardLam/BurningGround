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
	int max_endurance;
	int max_mana;
	int endurance;				// commonly known as HP
	int mana;					// commonly known as MP, for magics, it is their damage
	double x, y;				// current position
	double dispX, dispY;		// For normal objects, displacement needed to move before fixing position
								// For magics, dispX/Y are used as direction guidance
	Vector vel;
	Vector motiveVel;
	ObjectType type;
	BOOL isBackground;			// whether this object will influence others directly, not related to whether it is affected by others
								// turn this on for plants
	//BOOL isIntangibleToAttacks;	// whether this object is immune to attacks
								// turn this on for trees
	int magicConductivity;		// damages and effects received from magic is scaled by magicConductivity / 100
	int turnsAlive;				// how long this object has lived
	int lifespan;				// how long this object can live
	int destroyCriteria;		// what criteria trigger destruction, masked by DESTROY_CRITERIA_...
	//BOOL holdPos;				// whether this object will hold position after dispX/Y is exhausted
	BOOL underMove;				// whether this object will continue moving by motiveSpeed
	BOOL submergeGround;		// whether this object can submerge underground
	BOOL underGravity;			// whether this object is affected by gravity
	BOOL fixedFlight;			// whether this object will stabilize in air when it is not underGravity
	int facingDir;				// 0 left 1 right
	CharacterImage* sprite;
	int master;					// id of the object that spawns it, -1 for none

	// usage depends on object type, basically private data
	// For magic, attri = SPHERE, attri2 = ENCHANT
	// For humanoid, attri = type of character, attri2 = exp
	// For LIFE_RABBIT, attri = hiding state(0 not hiding, 1 hiding), attri2 = which x direction to wander(-1, 0, 1)
	// For LIFE_SLIME, LIFE_SLUDGE, attri2 = which x direction to wander(-1, 0, 1)
	int attri;
	int attri2;

	// effects affecting the object, time remaining for the effect to disappear, lowest value = -1
	// check for effect: (underEffect[EFFECT_ID] >= 0) --> under influence
	int underEffect[TOTAL_EFFECT_COUNT];
	
	int *spawnRegionCount;
} GameObject;

extern GameObject gameObject[MAX_OBJECT];		// stores all game object!

void initializeObjects(void);
void updateHumanoidStatistic(int id, int oldLv, int newLv);
int createHumanoid(Region *environment, int master, int humanoidType, double startX, double startY, int lv);
int createObject(Region *environment, int master, ObjectType type, double startX, double startY);
int createObjectProjectileDir(Region *environment, int master, ObjectType type, double startX, double startY, double dirX, double dirY, double speed, int lifespan, int destroyCriteria, BOOL underGravity);
int createObjectProjectileDest(Region *environment, int master, ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int destroyCriteria, BOOL underGravity);
int createObjectMagicProjectileDir(Region *environment, int master, ObjectType type, double startX, double startY, double dirX, double dirY, double speed, int lifespan, int sphere, int enchant, int damage);
int createObjectMagicProjectile(Region *environment, int master, ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int sphere, int enchant, int damage);
int createObjectMagicRain(Region *environment, int master, ObjectType type, double CX, double CY, int W, int H, double speed, int sphere, int enchant, int damage);
int createObjectMist(Region *environment, int master, ObjectType type, double startX, double startY, int lifespan, int sphere, int enchant);
int defaultObjectsInit(Region *environment, int objId);
void deleteObject(Region *environment, int id, BOOL silentDelete);
void displayObjects(Region *environment, int observerId, Coordinate scrTopLeftPos, int scrW, int scrH); // please call updateObjectsStatus before calling this function
void displayCrossHair(int X, int Y);
void pushObjectDir(int id, double dirX, double dirY, double speed);
void acceObjects(Region *environment); // please call updateObjectsStatus before calling this function
void controlObjectX(int id, double destX, double speed);
void controlObjectY(int id, double destY, double speed);
void moveObjects(Region *environment); // please call updateObjectsStatus before calling this function
void rotateObjects(Region *environment);
void updateObjectsStatus(Region *environment);
BOOL triggerObjectHitEvent(Region *environment, int objId, double newX, double newY);
BOOL interactObject(int sourceId, int targetId, BOOL physicalTouch, int damage, int sphere, int effect);
BOOL checkObjectCollision(Region *environment, int objId, double x, double y); // please call updateObjectsStatus before calling this function, 0 miss 1 hit
BOOL checkObjectOnFeet(Region *environment, int objId);
BOOL checkObjectSubmergedInGround(Region *environment, int objId);
BOOL removeEnvironmentBlock(Region *environment, double x, double y);
BOOL registerEnvironmentObject(Region *environment, int objId);
BOOL removeEnvironmentObject(Region *environment, int objId, double oldX, double oldY, CharacterImage *oldImage);

int setObjectFlyingState(int objId, BOOL enableFlying, BOOL stableFlight);

void doInitialSpawn(Region *target);
void spawnCheck(Region *target);
