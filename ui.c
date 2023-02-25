#include "ui.h"

void drawUI(int observerId, int lv, ArcaneType *skillSet, int selectedSkillIndex, int hpPotionNum, int mpPotionNum)
{
	if (observerId == -1) return;
	for (int k = UI_SKILL_POS_Y; k <= UI_EXP_POS_Y; k++)
		if (move(k, 1) == ERR) return;
		else
			printw("                                                                              ");
	drawUIHPBar(UI_HP_BAR_LEN * gameObject[observerId].endurance / gameObject[observerId].max_endurance);
	drawUIMPBar(UI_MP_BAR_LEN * gameObject[observerId].mana / gameObject[observerId].max_mana);
	if (gameObject[observerId].type == LIFE_HUMANOID)
		drawUIEXPBar(UI_EXP_BAR_LEN * gameObject[observerId].attri2 / EXP_NEEDED_TO_LV_UP[lv - 1]);
	drawUILv(lv);
	drawUISkillBar(skillSet, selectedSkillIndex);
	drawUIPotions(hpPotionNum, mpPotionNum);
	drawUIMenuItem();
	return;
}

void drawUIHPBar(int coloredBarLen)
{
	if (move(UI_HP_POS_Y, UI_HP_POS_X) == ERR) return;
	char dispChar = '+';
	if (coloredBarLen > UI_HP_BAR_LEN)
	{
		coloredBarLen = UI_HP_BAR_LEN;
		dispChar = '!';
	}
	attron(COLOR_PAIR(COLOR_RED));
	addch('|');
	int i;
	int len = UI_HP_BAR_LEN - coloredBarLen;
	for (i = 0; i < len; i++)
		addch('_');
	attroff(COLOR_PAIR(COLOR_RED));
	attron(COLOR_PAIR(COLOR_B_RED));
	for (; i < UI_HP_BAR_LEN; i++)
		addch(dispChar);
	attroff(COLOR_PAIR(COLOR_B_RED));
	attron(COLOR_PAIR(COLOR_RED));
	addch('|');
	attroff(COLOR_PAIR(COLOR_RED));
	return;
}
void drawUIMPBar(int coloredBarLen)
{
	if (move(UI_MP_POS_Y, UI_MP_POS_X) == ERR) return;
	char dispChar = '+';
	if (coloredBarLen > UI_MP_BAR_LEN)
	{
		coloredBarLen = UI_MP_BAR_LEN;
		dispChar = '!';
	}
	attron(COLOR_PAIR(COLOR_BLUE));
	addch('|');
	attroff(COLOR_PAIR(COLOR_BLUE));
	attron(COLOR_PAIR(COLOR_B_BLUE));
	int i;
	for (i = 0; i < coloredBarLen; i++)
		addch(dispChar);
	attroff(COLOR_PAIR(COLOR_B_BLUE));
	attron(COLOR_PAIR(COLOR_BLUE));
	for (; i < UI_HP_BAR_LEN; i++)
		addch('_');
	addch('|');
	attroff(COLOR_PAIR(COLOR_BLUE));
	return;
}
void drawUIEXPBar(int coloredBarLen)
{
	if (move(UI_EXP_POS_Y, UI_EXP_POS_X + (UI_EXP_BAR_LEN - coloredBarLen) / 2) == ERR) return;
	char dispChar = '=';
	if (coloredBarLen > UI_EXP_BAR_LEN)
	{
		coloredBarLen = UI_EXP_BAR_LEN;
		dispChar = '!';
	}
	attron(COLOR_PAIR(COLOR_WHITE));
	for (int i = 0; i < coloredBarLen; i++)
		addch(dispChar);
	attroff(COLOR_PAIR(COLOR_WHITE));
	return;
}

void drawUILv(int lv)
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

void drawUISkillBar(ArcaneType *skillSet, int selectedSkillIndex)
{
	if (skillSet == NULL) return;
	for (int i = 0; i < UI_SKILL_SLOT; i++)
	{
		if (move(UI_SKILL_POS_Y, UI_SKILL_POS_X + i * 18) == ERR) continue;
		int fcolor = magicNameString[skillSet[i]].color;
		if (i == selectedSkillIndex)
		{
			if (fcolor < COLOR_WHITE)
				fcolor += 8;
			else if (fcolor == COLOR_B_BLACK)
				fcolor = COLOR_WHITE;
		}
		attron(COLOR_PAIR(fcolor));
		printw("[%d] %s", i + 1, magicNameString[skillSet[i]].string);
		attroff(COLOR_PAIR(fcolor));
	}
	return;
}

void drawUIPotions(int hpPotionNum, int mpPotionNum)
{
	if (hpPotionNum > 8) hpPotionNum = 8;
	if (mpPotionNum > 8) mpPotionNum = 8;
	if (move(UI_SKILL_POS_Y, UI_SKILL_POS_X + 3 * 18) == ERR) return;
	attron(COLOR_PAIR(COLOR_B_RED));
	for (int i = 0; i < hpPotionNum; i++)
	{
		addch(240 | A_ALTCHARSET);
	}
	attroff(COLOR_PAIR(COLOR_B_RED));
	attron(COLOR_PAIR(COLOR_B_BLUE));
	for (int i = 0; i < mpPotionNum; i++)
	{
		addch(240 | A_ALTCHARSET);
	}
	attroff(COLOR_PAIR(COLOR_B_BLUE));
	return;
}

void drawUIMenuItem()
{
	if (move(UI_SKILL_POS_Y, 73) == ERR) return;
	attron(COLOR_PAIR(COLOR_WHITE));
	addch('[');
	attroff(COLOR_PAIR(COLOR_WHITE));
	attron(COLOR_PAIR(COLOR_B_YELLOW));
	printw("H");
	attroff(COLOR_PAIR(COLOR_B_YELLOW));
	attron(COLOR_PAIR(COLOR_WHITE));
	printw("]elp");
	attroff(COLOR_PAIR(COLOR_WHITE));
	return;
}

void drinkHPPotion(int userId, int *hpPotionNum)
{
	if (userId == -1) return;
	if ((*hpPotionNum) > 0)
	{
		if (gameObject[userId].endurance < gameObject[userId].max_endurance / 2)
			gameObject[userId].endurance += gameObject[userId].max_endurance / 2;
		else
			gameObject[userId].endurance = gameObject[userId].max_endurance;
		(*hpPotionNum)--;
	}
	return;
}

void drinkMPPotion(int userId, int *mpPotionNum)
{
	if (userId == -1) return;
	if ((*mpPotionNum) > 0)
	{
		if (gameObject[userId].mana < gameObject[userId].max_mana / 2)
			gameObject[userId].mana += gameObject[userId].max_mana / 2;
		else
			gameObject[userId].mana = gameObject[userId].max_mana;
		(*mpPotionNum)--;
	}
	return;
}
