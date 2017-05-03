#include "local_region.h"

Region generateEmptyLocalRegion(int w, int h)
{
	//Region *new_region = malloc(sizeof(Region));
	Region new_region;
	new_region.width = w;
	new_region.height = h;
	new_region.appearance = malloc(h * sizeof(char *));
	for (int i = 0; i < h; i++)
	{
		new_region.appearance[i] = malloc(w * sizeof(char));
		for (int j = 0; j < w; j++)
			new_region.appearance[i][j] = ' ';
	}
	return new_region; // shallow copy is ok
}

void localRegionAddRect(Region *target, int x, int y, int rectW, int rectH, int fill)
{
	if (target == NULL) return;
	for (int i = 0, ry = y; i < rectH; i++, ry++)
	{
		for (int j = 0, rx = x; j < rectW; j++, rx++)
		{
			if (fill)
			{
				if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width))
					target->appearance[ry][rx] = '+';
			}
			else if ((i == 0 || i == rectH - 1) || (j == 0 || j == rectW - 1)) {
				if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width))
					target->appearance[ry][rx] = '+';
			}
		}
	}
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
				addch(target->appearance[ry][rx]);
			else
			{
				if (ry >= target->height)
				{
					attron(COLOR_PAIR(4));
					addch('~');
					attroff(COLOR_PAIR(4));
				}
				else {
					attron(COLOR_PAIR(4));
					addch('-');
					attroff(COLOR_PAIR(4));
				}
			}
				
		}
	}
	return;
}
