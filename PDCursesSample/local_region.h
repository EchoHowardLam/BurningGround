#pragma once

#include <math.h>

#include "shared_definition.h"

typedef struct region {
	chtype **appearance;
	int width;
	int height;
} Region;

Region generateEmptyLocalRegion(int w, int h);
void cleanUpLocalRegion(Region *target);
void localRegionAddRect(Region *target, int x, int y, int rectW, int rectH, int fill); // fill is bool
void drawLocalRegion(Region *target, Coordinate scrTopLeftPos, int scrW, int scrH); // x, y refer to the global coordinates of screen's top left corner
