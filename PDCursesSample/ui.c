#include "ui.h"

void drawUI(int observerId, int lv)
{
	if (observerId == -1) return;
	drawHPBar(UI_HP_BAR_LEN * gameObject[observerId].endurance / gameObject[observerId].max_endurance);
	drawMPBar(UI_MP_BAR_LEN * gameObject[observerId].mana / gameObject[observerId].max_mana);
	drawLv(lv);
	return;
}

void drawHPBar(int coloredBarLen)
{
	if (move(UI_HP_POS_Y, UI_HP_POS_X) == ERR) return;
	attron(COLOR_PAIR(COLOR_RED));
	addch('|');
	int i;
	int len = UI_HP_BAR_LEN - coloredBarLen;
	for (i = 0; i < len; i++)
		addch('_');
	attroff(COLOR_PAIR(COLOR_RED));
	attron(COLOR_PAIR(COLOR_B_RED));
	for (; i < UI_HP_BAR_LEN; i++)
		addch('+');
	attroff(COLOR_PAIR(COLOR_B_RED));
	attron(COLOR_PAIR(COLOR_RED));
	addch('|');
	attroff(COLOR_PAIR(COLOR_RED));
	return;
}
void drawMPBar(int coloredBarLen)
{
	if (move(UI_MP_POS_Y, UI_MP_POS_X) == ERR) return;
	attron(COLOR_PAIR(COLOR_BLUE));
	addch('|');
	attroff(COLOR_PAIR(COLOR_BLUE));
	attron(COLOR_PAIR(COLOR_B_BLUE));
	int i;
	for (i = 0; i < coloredBarLen; i++)
		addch('+');
	attroff(COLOR_PAIR(COLOR_B_BLUE));
	attron(COLOR_PAIR(COLOR_BLUE));
	for (; i < UI_HP_BAR_LEN; i++)
		addch('_');
	addch('|');
	attroff(COLOR_PAIR(COLOR_BLUE));
	return;
}

void drawLv(int lv)
{
	int lvNumLen;
	if (lv > 99) lvNumLen = 3;
	else if (lv > 9) lvNumLen = 2;
	else if (lv > -1) lvNumLen = 1;
	else
		return;
	if (move(UI_HP_POS_Y, UI_LV_POS_X + (UI_LV_BAR_LEN - (3 + lvNumLen)) / 2) == ERR) return;
	attron(COLOR_PAIR(COLOR_B_YELLOW));
	printw("LV-%d", lv);
	attroff(COLOR_PAIR(COLOR_B_YELLOW));
	return;
}
