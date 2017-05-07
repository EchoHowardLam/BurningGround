#pragma once

#define UI_HP_POS_X 1
#define UI_HP_POS_Y 24
#define UI_MP_POS_X 44
#define UI_MP_POS_Y 24
#define UI_LV_POS_X 37
#define UI_HP_BAR_LEN 33
#define UI_MP_BAR_LEN 33
#define UI_LV_BAR_LEN 6
#define UI_SKILL_POS_X 1
#define UI_SKILL_POS_Y 23
#define UI_SKILL_SLOT 3

#include <string.h>

#include "shared_definition.h"
#include "gameobj.h"
#include "magic_interface.h"

void drawUI(int observerId, int lv, int *skillSet, int selectedSkillIndex, int hpPotionNum, int mpPotionNum);
void drawUIHPBar(int coloredBarLen);
void drawUIMPBar(int coloredBarLen);
void drawUILv(int lv);
void drawUISkillBar(int *skillSet, int selectedSkillIndex);
void drawUIPotions(int hpPotionNum, int mpPotionNum);
void drawUIMenuItem();

void drinkHPPotion(int userId, int *hpPotionNum);
void drinkMPPotion(int userId, int *mpPotionNum);
