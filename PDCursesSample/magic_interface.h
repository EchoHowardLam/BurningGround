#pragma once

#include "shared_definition.h"
#include "local_region.h"
#include "gameobj.h"

typedef enum {
	NOMAGIC = 0,
	ARCANE_FIREBALL, ARCANE_ICEBALL, ARCANE_DIRTBALL,
	ARCANE_FIRERAIN, ARCANE_ICERAIN,
	ARCANE_FIRELASER, ARCANE_ICELASER,
} ArcaneType;

// return cooldown
int castMagic(Region *environment, int casterId, ArcaneType magic, double destX, double destY);
// return state of flight: 0 not flying 1 flying
BOOL castFlying(int casterId, BOOL enableFlying);
