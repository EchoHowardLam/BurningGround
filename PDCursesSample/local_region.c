#include "local_region.h"

Region generateEmptyLocalRegion(int w, int h)
{
	//Region *new_region = malloc(sizeof(Region));
	Region new_region;
	new_region.width = w;
	new_region.height = h;
	new_region.appearance = malloc(h * sizeof(chtype *));
	new_region.blocked = malloc(h * sizeof(BOOL *));
	new_region.objId = malloc(h * sizeof(int *));
	for (int i = 0; i < h; i++)
	{
		new_region.appearance[i] = malloc(w * sizeof(chtype));
		new_region.blocked[i] = malloc(w * sizeof(BOOL));
		new_region.objId[i] = malloc(w * sizeof(int));
		for (int j = 0; j < w; j++) {
			new_region.appearance[i][j] = ' ';
			new_region.blocked[i][j] = 0;
			new_region.objId[i][j] = -1;
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
	}
	free(target->appearance);
	free(target->blocked);
	free(target->objId);
	return;
}

void localRegionAddRectWithChar(Region *target, int x, int y, int rectW, int rectH, BOOL fill, BOOL block, chtype display)
{
	if (target == NULL) return;
	for (int i = 0, ry = y; i < rectH; i++, ry++)
	{
		for (int j = 0, rx = x; j < rectW; j++, rx++)
		{
			if (fill)
			{
				if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width)) {
					target->appearance[ry][rx] = display;
					target->blocked[ry][rx] = block;
				}
			}
			else if ((i == 0 || i == rectH - 1) || (j == 0 || j == rectW - 1)) {
				if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width)) {
					target->appearance[ry][rx] = display;
					target->blocked[ry][rx] = block;
				}
			}
		}
	}
	return;
}

void localRegionAddRect(Region *target, int x, int y, int rectW, int rectH, BOOL fill, BOOL block)
{
	localRegionAddRectWithChar(target, x, y, rectW, rectH, fill, block, (97 | A_ALTCHARSET));
	return;
}

void localRegionDelRect(Region *target, int x, int y, int rectW, int rectH, BOOL fill)
{
	localRegionAddRectWithChar(target, x, y, rectW, rectH, fill, 0, ' ');
	return;
}

double euclidean(int x, int y) {
	return sqrt(x*x+y*y);
}

void localRegionAddCircleWithChar(Region *target, int cx, int cy, int radius, int height, int fill, BOOL block, chtype display) {
	for (int i=0, ry=cy-(height-radius); i <= height; i++, ry++) {
		for (int j=0, rx=cx-radius; j <= 2*radius; j++, rx++) {
			if (fill) {
				if (euclidean(cx-rx, cy-ry) <= radius) {
					target->appearance[ry][rx] = display;
					target->blocked[ry][rx] = block;
				}
			} else {
				if (euclidean(cx-rx, cy-ry) <= radius && euclidean(cx-rx, cy-ry) >= radius-1) {
					target->appearance[ry][rx] = display;
					target->blocked[ry][rx] = block;
				}
			}
		}
	}
}

void localRegionDelCircle(Region *target, int cx, int cy, int radius, int height, int fill) {
	localRegionAddCircleWithChar(target, cx, cy, radius, height, fill, 0, ' ');
	return;
}

void drawLocalRegion(Region *target, Coordinate scrTopLeftPos, int scrW, int scrH)
{
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
				attron(COLOR_PAIR(COLOR_WHITE));
				addch(target->appearance[ry][rx]);
				attroff(COLOR_PAIR(COLOR_WHITE));
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

void drawLocalRegionBlocked(Region *target, Coordinate scrTopLeftPos, int scrW, int scrH)
{
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
				if (target->blocked[ry][rx] && target->objId[ry][rx] != -1)
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
