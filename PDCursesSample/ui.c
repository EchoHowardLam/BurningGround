#include "ui.h"

void drawUI(int observerId)
{
	if (observerId == -1) return;
	if (move(UI_HP_POS_Y, UI_HP_POS_X) != ERR)
	{
		int hpBarColorLen = UI_HP_BAR_LEN * gameObject[observerId].endurance / gameObject[observerId].max_endurance;
		attron(COLOR_PAIR(COLOR_B_RED));
		printw("%s", "HP: ");
		int i;
		for (i = 0; i < hpBarColorLen; i++)
			addch('+');
		attroff(COLOR_PAIR(COLOR_B_RED));
		attron(COLOR_PAIR(COLOR_RED));
		for (; i < UI_HP_BAR_LEN; i++)
			addch('+');
		attroff(COLOR_PAIR(COLOR_RED));
	}
	if (move(UI_MP_POS_Y, UI_MP_POS_X) != ERR)
	{
		int mpBarColorLen = UI_MP_BAR_LEN * gameObject[observerId].mana / gameObject[observerId].max_mana;
		attron(COLOR_PAIR(COLOR_B_BLUE));
		printw("%s", "MP: ");
		int i;
		for (i = 0; i < mpBarColorLen; i++)
			addch('+');
		attroff(COLOR_PAIR(COLOR_B_BLUE));
		attron(COLOR_PAIR(COLOR_BLUE));
		for (; i < UI_HP_BAR_LEN; i++)
			addch('+');
		attroff(COLOR_PAIR(COLOR_BLUE));
	}
	return;
}
