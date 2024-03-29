#pragma once

#include <shared_definition.h>
#include <local_region.h>
#include <game_object.h>

typedef struct {
	char *string;
	int color;
} MagicNameString;

// WARNING: the index of magicNameString MUST correspond to the ArcaneType
extern MagicNameString magicNameString[100];
extern ArcaneType magicUnlockedAtLevel[MAX_LV]; // index 0 is for level up from lv1 to lv2


// return cooldown
int castMagic(Region *environment, int casterId, ArcaneType magic, double destX, double destY);
// return state of flight: 0 not flying 1 flying
BOOL castFlying(int casterId, BOOL enableFlying);
