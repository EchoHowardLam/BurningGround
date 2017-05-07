#include "magic_interface.h"

void castMagic(Region *environment, int casterId, ArcaneType magic, double destX, double destY)
{
	if (casterId == -1) return;
	if (gameObject[casterId].underEffect[EFFECT_STUN] >= 0) return;
	switch (magic)
	{
	case ARCANE_FIREBALL:
		if (gameObject[casterId].mana >= 300)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, gameObject[casterId].x, gameObject[casterId].y, destX, destY, 0.2, -1, SPHERE_FIRE, 0, DMG_STANDARD_FIREBALL_DAMAGE) != -1)
				gameObject[casterId].mana -= 300;
		break;
	case ARCANE_ICEBALL:
		if (gameObject[casterId].mana >= 300)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, gameObject[casterId].x, gameObject[casterId].y, destX, destY, 0.2, -1, SPHERE_ICE, 0, DMG_STANDARD_ICEBALL_DAMAGE) != -1)
				gameObject[casterId].mana -= 300;
		break;
	case ARCANE_FIRERAIN:
		if (gameObject[casterId].mana >= 100)
			if (createObjectMagicRain(environment, -1, MAGIC_BLOB, destX, destY, 30, 20, 0.1, SPHERE_FIRE, ENCHANT_SHRAPNEL, DMG_STANDARD_FIRERAIN_DAMAGE) != -1)
				gameObject[casterId].mana -= 100;
		break;
	case ARCANE_ICERAIN:
		if (gameObject[casterId].mana >= 100)
			if (createObjectMagicRain(environment, -1, MAGIC_BLOB, destX, destY, 30, 20, 0.1, SPHERE_ICE, ENCHANT_SHRAPNEL, DMG_STANDARD_ICERAIN_DAMAGE) != -1)
				gameObject[casterId].mana -= 100;
		break;
	case ARCANE_FIRELASER:
		if (gameObject[casterId].mana >= 30)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_LASER, gameObject[casterId].x, gameObject[casterId].y, destX, destY, 0.0, -1, SPHERE_FIRE, 0, DMG_STANDARD_FIRELASER_DAMAGE) != -1)
				gameObject[casterId].mana -= 30;
		break;
	case ARCANE_ICELASER:
		if (gameObject[casterId].mana >= 30)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_LASER, gameObject[casterId].x, gameObject[casterId].y, destX, destY, 0.0, -1, SPHERE_ICE, 0, DMG_STANDARD_ICELASER_DAMAGE) != -1)
				gameObject[casterId].mana -= 30;
		break;
	default:
		break;
	}
	return;
}
//createObjectMagicProjectile(environment, gameObject[id].master, MAGIC_SPIKE, gameObject[id].x, gameObject[id].y, 0.0, 0.0, 0.0, 1000, SPHERE_ICE, 0);
