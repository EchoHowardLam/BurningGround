#include <magic_interface.h>

MagicNameString magicNameString[100] = {
	{ "-NO SKILL-", COLOR_B_BLACK },
	{ "Fire Ball", COLOR_RED },
	{ "Ice Ball", COLOR_CYAN },
	{ "Dirt Ball", COLOR_YELLOW },
	{ "Firework", COLOR_RED },
	{ "Cold Bomb", COLOR_CYAN },
	{ "Fire Rain", COLOR_RED },
	{ "Ice Rain", COLOR_CYAN },
	{ "Hail", COLOR_CYAN },
	{ "Fall Rain", COLOR_B_BLACK },
	{ "Fire Ray", COLOR_RED },
	{ "Freezing Ray", COLOR_CYAN },
};

ArcaneType magicUnlockedAtLevel[MAX_LV] = {
	ARCANE_ICEBALL, NOMAGIC, ARCANE_FIRERAIN, ARCANE_ICERAIN, NOMAGIC,
	ARCANE_ICESPIKERAIN, NOMAGIC, ARCANE_FIRELASER, ARCANE_ICELASER, NOMAGIC,
	ARCANE_MYTHRAIN, NOMAGIC, ARCANE_FIREBALL_FRAG, ARCANE_ICEBALL_FRAG, NOMAGIC,
	NOMAGIC, NOMAGIC, NOMAGIC, NOMAGIC, NOMAGIC,
	NOMAGIC, NOMAGIC, NOMAGIC, NOMAGIC, NOMAGIC,
	NOMAGIC, NOMAGIC, NOMAGIC, NOMAGIC, NOMAGIC,
};

int castMagic(Region *environment, int casterId, ArcaneType magic, double destX, double destY)
{
	if (casterId == -1) return 0;
	if (gameObject[casterId].underEffect[EFFECT_STUN] >= 0) return 0;
	int cooldown = 0;
	double ox = gameObject[casterId].x;
	double oy = gameObject[casterId].y;
	if (gameObject[casterId].type == LIFE_HUMANOID && gameObject[casterId].attri == HUMANOID_TYPE_HUMAN)
		oy--;
	switch (magic)
	{
	case ARCANE_FIREBALL:
		if (gameObject[casterId].mana >= 300)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, ox, oy, destX, destY, 0.2, 200, SPHERE_FIRE, 0, DMG_STANDARD_FIREBALL_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 300;
				cooldown = 50;
			}
		break;
	case ARCANE_ICEBALL:
		if (gameObject[casterId].mana >= 300)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, ox, oy, destX, destY, 0.2, 200, SPHERE_ICE, 0, DMG_STANDARD_ICEBALL_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 300;
				cooldown = 50;
			}
		break;
	case ARCANE_DIRTBALL:
		if (gameObject[casterId].mana >= 200)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, ox, oy, destX, destY, 0.2, 200, SPHERE_EARTH, ENCHANT_SLOW | ENCHANT_STUN | ENCHANT_ENTANGLE | ENCHANT_SILENT, DMG_STANDARD_DIRTBALL_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 200;
				cooldown = 50;
			}
		break;
	case ARCANE_FIREBALL_FRAG:
		if (gameObject[casterId].mana >= 350)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, ox, oy, destX, destY, 0.2, 200, SPHERE_FIRE, ENCHANT_SHRAPNEL, DMG_STANDARD_FIREBALL_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 350;
				cooldown = 50;
			}
		break;
	case ARCANE_ICEBALL_FRAG:
		if (gameObject[casterId].mana >= 350)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_BLOB, ox, oy, destX, destY, 0.2, 200, SPHERE_ICE, ENCHANT_SHRAPNEL, DMG_STANDARD_ICEBALL_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 350;
				cooldown = 50;
			}
		break;
	case ARCANE_FIRERAIN:
		if (gameObject[casterId].mana >= 80)
			if (createObjectMagicRain(environment, casterId, MAGIC_BLOB, destX, destY, 30, 20, 0.1, SPHERE_FIRE, ENCHANT_SHRAPNEL, DMG_STANDARD_FIRERAIN_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 80;
				cooldown = 10;
			}
		break;
	case ARCANE_ICERAIN:
		if (gameObject[casterId].mana >= 80)
			if (createObjectMagicRain(environment, casterId, MAGIC_BLOB, destX, destY, 30, 20, 0.1, SPHERE_ICE, ENCHANT_SHRAPNEL, DMG_STANDARD_ICERAIN_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 80;
				cooldown = 10;
			}
		break;
	case ARCANE_MYTHRAIN:
		if (gameObject[casterId].mana >= 10)
			if (createObjectMagicRain(environment, casterId, MAGIC_BLOB, destX, destY, 100, 20, 0.1, SPHERE_MYTH, ENCHANT_ENTANGLE | ENCHANT_SILENT, 0) != -1)
			{
				gameObject[casterId].mana -= 10;
				cooldown = 5;
			}
		break;
	case ARCANE_ICESPIKERAIN:
		if (gameObject[casterId].mana >= 300)
			if (createObjectMagicRain(environment, casterId, MAGIC_SPIKE, destX, destY, 30, 20, 0.0, SPHERE_ICE, 0, DMG_STANDARD_ICESPIKE_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 300;
				cooldown = 10;
			}
		break;
	case ARCANE_FIRELASER:
		if (gameObject[casterId].mana >= 30)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_LASER, ox, oy, destX, destY, 0.0, -1, SPHERE_FIRE, 0, DMG_STANDARD_FIRELASER_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 30;
				cooldown = 1;
			}
		break;
	case ARCANE_ICELASER:
		if (gameObject[casterId].mana >= 30)
			if (createObjectMagicProjectile(environment, casterId, MAGIC_LASER, ox, oy, destX, destY, 0.0, -1, SPHERE_ICE, 0, DMG_STANDARD_ICELASER_DAMAGE) != -1)
			{
				gameObject[casterId].mana -= 30;
				cooldown = 1;
			}
		break;
	default:
		break;
	}
	return cooldown;
}

BOOL castFlying(int casterId, BOOL enableFlying)
{
	if (casterId == -1) return FALSE;
	if (!enableFlying)
	{
		setObjectFlyingState(casterId, FALSE, TRUE);
		return FALSE;
	}
	if (gameObject[casterId].underEffect[EFFECT_STUN] >= 0) return (!gameObject[casterId].underGravity);
	BOOL flying = FALSE;
	if (gameObject[casterId].mana >= 2)
	{
		flying = (setObjectFlyingState(casterId, TRUE, TRUE) == 2);
		if (flying)
			gameObject[casterId].mana -= 2;
	}
	return flying;
}
