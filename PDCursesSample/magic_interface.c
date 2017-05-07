#include "magic_interface.h"

int castMagic(Region *environment, int casterId, ArcaneType magic, double destX, double destY)
{
	if (casterId == -1) return 0;
	if (gameObject[casterId].underEffect[EFFECT_STUN] >= 0) return 0;
	int cooldown = 0;
	switch (magic)
	{
	case ARCANE_FIREBALL:
		if (gameObject[casterId].mana >= 300)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, gameObject[casterId].x, gameObject[casterId].y, destX, destY, 0.2, -1, SPHERE_FIRE, 0, DMG_STANDARD_FIREBALL_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 300;
				cooldown = 50;
			}
		break;
	case ARCANE_ICEBALL:
		if (gameObject[casterId].mana >= 300)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, gameObject[casterId].x, gameObject[casterId].y, destX, destY, 0.2, -1, SPHERE_ICE, 0, DMG_STANDARD_ICEBALL_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 300;
				cooldown = 50;
			}
		break;
	case ARCANE_DIRTBALL:
		if (gameObject[casterId].mana >= 200)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, gameObject[casterId].x, gameObject[casterId].y, destX, destY, 0.2, -1, SPHERE_EARTH, ENCHANT_SLOW | ENCHANT_STUN | ENCHANT_ENTANGLE | ENCHANT_SILENT, DMG_STANDARD_DIRTBALL_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 200;
				cooldown = 50;
			}
		break;
	case ARCANE_FIRERAIN:
		if (gameObject[casterId].mana >= 100)
			if (createObjectMagicRain(environment, -1, MAGIC_BLOB, destX, destY, 30, 20, 0.1, SPHERE_FIRE, ENCHANT_SHRAPNEL, DMG_STANDARD_FIRERAIN_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 100;
				cooldown = 10;
			}
		break;
	case ARCANE_ICERAIN:
		if (gameObject[casterId].mana >= 100)
			if (createObjectMagicRain(environment, -1, MAGIC_BLOB, destX, destY, 30, 20, 0.1, SPHERE_ICE, ENCHANT_SHRAPNEL, DMG_STANDARD_ICERAIN_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 100;
				cooldown = 10;
			}
		break;
	case ARCANE_FIRELASER:
		if (gameObject[casterId].mana >= 50)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_LASER, gameObject[casterId].x, gameObject[casterId].y, destX, destY, 0.0, -1, SPHERE_FIRE, 0, DMG_STANDARD_FIRELASER_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 50;
				cooldown = 1;
			}
		break;
	case ARCANE_ICELASER:
		if (gameObject[casterId].mana >= 50)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_LASER, gameObject[casterId].x, gameObject[casterId].y, destX, destY, 0.0, -1, SPHERE_ICE, 0, DMG_STANDARD_ICELASER_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 50;
				cooldown = 1;
			}
		break;
	default:
		break;
	}
	return cooldown;
}
//createObjectMagicProjectile(environment, gameObject[id].master, MAGIC_SPIKE, gameObject[id].x, gameObject[id].y, 0.0, 0.0, 0.0, 1000, SPHERE_ICE, 0);
