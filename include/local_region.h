#pragma once

#include <math.h>

#include <shared_definition.h>

typedef struct {
	int x;
	int y;
	int width;
	int height;
	double chance;
	ObjectType mob;
	int initial;
	int max;
	int currMobSpawned;
} SpawnRegion;

typedef struct region {
	chtype **appearance;
	BOOL **blocked;
	int **objId;
	int **color;
	int numSpawns;
	SpawnRegion **spawns;
	int width;
	int height;
} Region;

Region generateEmptyLocalRegion(int w, int h, int spawns);
void cleanUpLocalRegion(Region *target);


void localRegionAddRectWithChar(Region *target, int x, int y, int rectW, int rectH, int fill, BOOL block, chtype display, int color); // fill is bool
void localRegionAddRect(Region *target, int x, int y, int rectW, int rectH, int fill, BOOL block, int color); // fill is bool
void localRegionDelRect(Region *target, int x, int y, int rectW, int rectH, int fill); // fill is bool

void localRegionAddCircleWithChar(Region *target, int cx, int cy, int radius, int height, int fill, BOOL block, chtype display, int color);
void localRegionDelCircle(Region *target, int cx, int cy, int radius, int height, int fill);

void localRegionAddUTriWithChar(Region *target, int cx, int cy, int width, int height, int fill, BOOL block, chtype display, int color);
void localRegionDelUTri(Region *target, int cx, int cy, int width, int height, int fill);


void drawLocalRegion(Region *target, int blind, Coordinate scrTopLeftPos, int scrW, int scrH); // x, y refer to the global coordinates of screen's top left corner
void drawLocalRegionBlocked(Region *target, int blind, Coordinate scrTopLeftPos, int scrW, int scrH); // x, y refer to the global coordinates of screen's top left corner
void drawLocalRegionObjId(Region *target, int blind, Coordinate scrTopLeftPos, int scrW, int scrH); // x, y refer to the global coordinates of screen's top left corner
