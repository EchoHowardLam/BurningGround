#pragma once

#define UI_HP_POS_X 1
#define UI_HP_POS_Y 22
#define UI_MP_POS_X 44
#define UI_MP_POS_Y 22
#define UI_LV_POS_X 37
#define UI_HP_BAR_LEN 33
#define UI_MP_BAR_LEN 33
#define UI_LV_BAR_LEN 6

#include "shared_definition.h"
#include "gameobj.h"

void drawUI(int observerId, int lv);
void drawHPBar(int coloredBarLen);
void drawMPBar(int coloredBarLen);
void drawLv(int lv);
