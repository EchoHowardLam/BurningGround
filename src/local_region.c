#include <local_region.h>

Region generateEmptyLocalRegion(int w, int h, int spawns)
{
	//Region *new_region = malloc(sizeof(Region));
	Region new_region;
	new_region.width = w;
	new_region.height = h;
	new_region.numSpawns = spawns;
	new_region.appearance = (chtype **)malloc(h * sizeof(chtype *));
	new_region.blocked = (BOOL **)malloc(h * sizeof(BOOL *));
	new_region.objId = (int **)malloc(h * sizeof(int *));
	new_region.color = (int **)malloc(h * sizeof(int *));
	new_region.spawns = (SpawnRegion **)malloc(spawns * sizeof(SpawnRegion *));
	for (int i = 0; i < h; i++)
	{
		new_region.appearance[i] = (chtype *)malloc(w * sizeof(chtype));
		new_region.blocked[i] = (BOOL *)malloc(w * sizeof(BOOL));
		new_region.objId[i] = (int *)malloc(w * sizeof(int));
		new_region.color[i] = (int *)malloc(w * sizeof(int));
		for (int j = 0; j < w; j++) {
			new_region.appearance[i][j] = ' ';
			new_region.blocked[i][j] = 0;
			new_region.objId[i][j] = -1;
			new_region.color[i][j] = 0;
		}
	}
	return new_region; // shallow copy is ok
}

void cleanUpLocalRegion(Region *target)
{
	int h = target->height;
	for (int i = 0; i < h; i++)
	{
		free(target->appearance[i]);
		free(target->blocked[i]);
		free(target->objId[i]);
		free(target->color[i]);
	}
	for (int i=0; i<target->numSpawns; i++) {
		free(target->spawns[i]);
	}
	free(target->spawns);
	free(target->appearance);
	free(target->blocked);
	free(target->objId);
	free(target->color);
	return;
}

void localRegionAddRectWithChar(Region *target, int x, int y, int rectW, int rectH, BOOL fill, BOOL block, chtype display, int color)
{
	if (target == NULL) return;
	for (int i = 0, ry = y; i < rectH; i++, ry++)
	{
		for (int j = 0, rx = x; j < rectW; j++, rx++)
		{
			if (rx >= 0 && ry >= 0 && rx < target->width && ry < target->height) {
				if (fill)
				{
					if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width)) {
						target->appearance[ry][rx] = display;
						target->blocked[ry][rx] = block;
						target->color[ry][rx] = color;
					}
				}
				else if ((i == 0 || i == rectH - 1) || (j == 0 || j == rectW - 1)) {
					if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width)) {
						target->appearance[ry][rx] = display;
						target->blocked[ry][rx] = block;
						target->color[ry][rx] = color;
					}
				}
			}
		}
	}
	return;
}

void localRegionAddRect(Region *target, int x, int y, int rectW, int rectH, BOOL fill, BOOL block, int color)
{
	localRegionAddRectWithChar(target, x, y, rectW, rectH, fill, block, (97 | A_ALTCHARSET), COLOR_PAIR(COLOR_WHITE));
	return;
}

void localRegionDelRect(Region *target, int x, int y, int rectW, int rectH, BOOL fill)
{
	localRegionAddRectWithChar(target, x, y, rectW, rectH, fill, 0, ' ', COLOR_PAIR(COLOR_WHITE));
	return;
}

double euclidean(int x, int y) {
	return sqrt(x*x+y*y);
}

void localRegionAddCircleWithChar(Region *target, int cx, int cy, int radius, int height, int fill, BOOL block, chtype display, int color) {
	for (int i=0, ry=cy-(height-radius); i <= height; i++, ry++) {
		for (int j=0, rx=cx-radius; j <= 2*radius; j++, rx++) {
			if (rx >= 0 && ry >= 0 && rx < target->width && ry < target->height) {
				if (fill) {
					if (euclidean(cx-rx, cy-ry) <= radius) {
						target->appearance[ry][rx] = display;
						target->blocked[ry][rx] = block;
						target->color[ry][rx] = color;
					}
				} else {
					if (euclidean(cx-rx, cy-ry) <= radius && euclidean(cx-rx, cy-ry) >= radius-1) {
						target->appearance[ry][rx] = display;
						target->blocked[ry][rx] = block;
						target->color[ry][rx] = color;
					}
				}
			}
		}
	}
}

void localRegionDelCircle(Region *target, int cx, int cy, int radius, int height, int fill) {
	localRegionAddCircleWithChar(target, cx, cy, radius, height, fill, 0, ' ', COLOR_PAIR(COLOR_WHITE));
	return;
}

void localRegionAddUTriWithChar(Region *target, int cx, int cy, int width, int height, int fill, BOOL block, chtype display, int color) {
	double left = cx-(width/2.0);
	double right = cx+(width/2.0);
	double slope = (width/2.0)/height;
	for (int i=0, ry=cy; i < height; i++, ry++) {
		for (int j=0, rx=(int)floor(left); j < width; j++, rx++) {
			if (rx >= 0 && ry >= 0 && rx < target->width && ry < target->height) {
				if (fill) {
					if (rx >= left+(slope*(height-i)) && rx <= right-(slope*(height-i))) {
						target->appearance[ry][rx] = display;
						target->blocked[ry][rx] = block;
						target->color[ry][rx] = color;
					}
				}
			}
		}
	}
}

void localRegionDelUTri(Region *target, int cx, int cy, int radius, int height, int fill) {
	localRegionAddUTriWithChar(target, cx, cy, radius, height, fill, 0, ' ', COLOR_PAIR(COLOR_WHITE));
	return;
}

void drawLocalRegion(Region *target, int blind, Coordinate scrTopLeftPos, int scrW, int scrH)
{
	if (target == NULL) return;
	//if (blind >= 0) return;
	scrW = SCREEN_WIDTH;
	scrH = SCREEN_HEIGHT;
	int x = (int)floor(scrTopLeftPos.x);
	int y = (int)floor(scrTopLeftPos.y);
	if (target == NULL) return;
	for (int i = 0, ry = y; i < scrH; i++, ry++)
	{
		if (move(i, 0) == ERR) return;
		for (int j = 0, rx = x; j < scrW; j++, rx++)
		{
			if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width))
			{
				attron(target->color[ry][rx]);
				if (blind >= 0 && (target->appearance[ry][rx] != ' '))
					addch('.');
				else
					addch(target->appearance[ry][rx]);
				attroff(target->color[ry][rx]);
			}
			else
			{
				if (ry >= target->height)
				{
					attron(COLOR_PAIR(COLOR_B_RED));
					addch('~');
					attroff(COLOR_PAIR(COLOR_B_RED));
				}
				else {
					attron(COLOR_PAIR(COLOR_B_BLACK));
					addch('-');
					attroff(COLOR_PAIR(COLOR_B_BLACK));
				}
			}
				
		}
	}
	return;
}

void drawLocalRegionBlocked(Region *target, int blind, Coordinate scrTopLeftPos, int scrW, int scrH)
{
	if (target == NULL) return;
	scrW = SCREEN_WIDTH;
	scrH = SCREEN_HEIGHT;
	int x = (int)floor(scrTopLeftPos.x);
	int y = (int)floor(scrTopLeftPos.y);
	if (target == NULL) return;
	for (int i = 0, ry = y; i < scrH; i++, ry++)
	{
		for (int j = 0, rx = x; j < scrW; j++, rx++)
		{
			if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width))
			{
				if (move(i, j) == ERR) return;
				bool showLand = FALSE;
				/*if (blind >= 0)
					showLand = TRUE;*/
				if (target->blocked[ry][rx] && (showLand || target->objId[ry][rx] != -1))
				{
					attron(COLOR_PAIR(COLOR_WHITE));
					addch(rand() % 95 + 32);
					attroff(COLOR_PAIR(COLOR_WHITE));
				}
			}
		}
	}
	return;
}

void drawLocalRegionObjId(Region *target, int blind, Coordinate scrTopLeftPos, int scrW, int scrH)
{
	if (target == NULL) return;
	scrW = SCREEN_WIDTH;
	scrH = SCREEN_HEIGHT;
	int x = (int)floor(scrTopLeftPos.x);
	int y = (int)floor(scrTopLeftPos.y);
	if (target == NULL) return;
	for (int i = 0, ry = y; i < scrH; i++, ry++)
	{
		for (int j = 0, rx = x; j < scrW; j++, rx++)
		{
			if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width))
			{
				if (move(i, j) == ERR) return;
				if (target->objId[ry][rx] != -1)
				{
					attron(COLOR_PAIR(COLOR_WHITE));
					addch(target->objId[ry][rx] % 10 + '0');
					attroff(COLOR_PAIR(COLOR_WHITE));
				}
			}
		}
	}
	return;
}
