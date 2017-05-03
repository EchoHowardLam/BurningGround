#include "local_region.h"

Region generateEmptyLocalRegion(int w, int h)
{
	//Region *new_region = malloc(sizeof(Region));
	Region new_region;
	new_region.width = w;
	new_region.height = h;
	new_region.appearance = malloc(h * sizeof(chtype *));
	for (int i = 0; i < h; i++)
	{
		new_region.appearance[i] = malloc(w * sizeof(chtype));
		for (int j = 0; j < w; j++)
			new_region.appearance[i][j] = ' ';
	}
	return new_region; // shallow copy is ok
}

void cleanUpLocalRegion(Region *target)
{
	int h = target->height;
	for (int i = 0; i < h; i++)
	{
		free(target->appearance[i]);
	}
	free(target->appearance);
	return;
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
					target->appearance[ry][rx] = (97 | A_ALTCHARSET);
			}
			else if ((i == 0 || i == rectH - 1) || (j == 0 || j == rectW - 1)) {
				if ((ry >= 0 && ry < target->height) && (rx >= 0 && rx < target->width))
					target->appearance[ry][rx] = (97 | A_ALTCHARSET);
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
