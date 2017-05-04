#pragma once

#include <math.h>

#include "shared_definition.h"

typedef struct region {
	chtype **appearance;
	BOOL **blocked;
	int **objId;
	int width;
	int height;
} Region;

Region generateEmptyLocalRegion(int w, int h);
void cleanUpLocalRegion(Region *target);


void localRegionAddRectWithChar(Region *target, int x, int y, int rectW, int rectH, int fill, BOOL block, chtype display); // fill is bool
void localRegionAddRect(Region *target, int x, int y, int rectW, int rectH, int fill, BOOL block); // fill is bool
void localRegionDelRect(Region *target, int x, int y, int rectW, int rectH, int fill); // fill is bool

void localRegionAddCircleWithChar(Region *target, int cx, int cy, int radius, int height, int fill, BOOL block, chtype display);
void localRegionDelCircle(Region *target, int cx, int cy, int radius, int height, int fill);


void drawLocalRegion(Region *target, Coordinate scrTopLeftPos, int scrW, int scrH); // x, y refer to the global coordinates of screen's top left corner
void drawLocalRegionBlocked(Region *target, Coordinate scrTopLeftPos, int scrW, int scrH); // x, y refer to the global coordinates of screen's top left corner
