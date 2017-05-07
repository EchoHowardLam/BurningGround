#include "gameobj.h"

GameObject gameObject[MAX_OBJECT];

void initializeObjects(void)
{
	for (int i = 0; i < MAX_OBJECT; i++)
		gameObject[i].type = NOTHING;
	return;
}

void updateHumanoidStatistic(int id, int oldLv, int newLv)
{
	if (id == -1) return;
	int old_multiplier = (int)floor(pow(1.1, oldLv - 1));
	int new_multiplier = (int)floor(pow(1.1, newLv - 1));
	gameObject[id].max_endurance = 1000 * new_multiplier;
	gameObject[id].max_mana = 1000 * new_multiplier;
	gameObject[id].endurance += (gameObject[id].max_endurance - 1000 * old_multiplier);
	gameObject[id].mana += (gameObject[id].max_mana - 1000 * old_multiplier);
	return;
}

int createHumanoid(Region *environment, int master, int humanoidType, double startX, double startY, int lv)
{
	int ret = createObject(environment, master, LIFE_HUMANOID, startX, startY);
	if (ret != -1)
	{
		int multiplier = (int)floor(pow(1.1, lv - 1));
		gameObject[ret].attri = humanoidType;
		switch (humanoidType)
		{
		case HUMANOID_TYPE_HUMAN:
			gameObject[ret].max_endurance = 1000 * multiplier;
			gameObject[ret].max_mana = 1000 * multiplier;
			gameObject[ret].endurance = 1000 * multiplier;
			gameObject[ret].mana = 1000 * multiplier;
			break;
		case HUMANOID_TYPE_WIZARD:
			gameObject[ret].max_endurance = 20000;
			gameObject[ret].max_mana = 200000;
			gameObject[ret].endurance = 20000;
			gameObject[ret].mana = 200000;
			break;
		case HUMANOID_TYPE_WINGMAN:
		case HUMANOID_TYPE_CORRUPTED_WINGMAN:
			gameObject[ret].max_endurance = 40000;
			gameObject[ret].max_mana = 20000;
			gameObject[ret].endurance = 40000;
			gameObject[ret].mana = 20000;
			break;
		}
	}
	return ret;
}

int createObject(Region *environment, int master, ObjectType type, double startX, double startY)
{
	for (int i = MAX_OBJECT - 1; i >= 0; i--)
	{
		if (gameObject[i].type != NOTHING)
			continue;		// if not empty, try next

		gameObject[i].max_endurance = 100;
		gameObject[i].max_mana = 0;
		gameObject[i].endurance = 100;
		gameObject[i].mana = 0;
		gameObject[i].type = type;
		gameObject[i].isBackground = FALSE;
		//gameObject[i].isIntangible = FALSE;
		gameObject[i].magicConductivity = 100;
		gameObject[i].x = startX;
		gameObject[i].y = startY;
		gameObject[i].dispX = 0.0;
		gameObject[i].dispY = 0.0;
		gameObject[i].vel.x = 0.0;
		gameObject[i].vel.y = 0.0;
		gameObject[i].motiveVel.x = 0.0;
		gameObject[i].motiveVel.y = 0.0;
		gameObject[i].turnsAlive = 0;
		gameObject[i].lifespan = -1;
		gameObject[i].destroyCriteria = 0;
		gameObject[i].underMove = FALSE;
		switch (type)
		{
		case DEMO_LIFE_CAN_FLY:
			gameObject[i].underGravity = FALSE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case LIFE_HUMANOID:
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case LIFE_EYEBALL:
			gameObject[i].endurance = 100;
			gameObject[i].underGravity = FALSE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case LIFE_MOSQUITOES:
			gameObject[i].endurance = 1;
			gameObject[i].underGravity = FALSE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case LIFE_RABBIT:
			gameObject[i].endurance = 200;
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case LIFE_SLUDGE:
		case LIFE_SLIME:
			gameObject[i].magicConductivity = 0;
			gameObject[i].endurance = 100;
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case LIFE_GRASS:
			gameObject[i].endurance = 1;
			gameObject[i].isBackground = TRUE;
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case LIFE_MUSHROOM:
			gameObject[i].endurance = 1;
			gameObject[i].destroyCriteria = DESTROY_CRITERIA_HIT;
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case SPAWN_BEE_HIVE:
			gameObject[i].endurance = 100;
			gameObject[i].underGravity = FALSE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case LIFE_BEE:
			gameObject[i].endurance = 20;
			gameObject[i].underGravity = FALSE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case SPAWN_DURIAN_TREE:
			gameObject[i].isBackground = TRUE;
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case LIFE_DURIAN:
			gameObject[i].endurance = 10;
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = TRUE;
			break;
		case DEMO_LIFE_CANNOT_FLY:
		default:
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = TRUE; // can still immediately fly if underGravity = FALSE
			break;
		}
		gameObject[i].facingDir = 1;
		gameObject[i].master = master;

		gameObject[i].attri = 0;
		gameObject[i].attri2 = 0;

		gameObject[i].spawnRegionCount = NULL;
		return defaultObjectsInit(environment, i);
	}
	return -1;
}

int createObjectProjectileDir(Region *environment, int master, ObjectType type, double startX, double startY, double dirX, double dirY, double speed, int lifespan, int destroyCriteria, BOOL underGravity)
{
	for (int i = MAX_OBJECT - 1; i >= 0; i--)
	{
		if (gameObject[i].type != NOTHING)
			continue;		// if not empty, try next

		gameObject[i].max_endurance = 1;
		gameObject[i].max_mana = 0;
		gameObject[i].endurance = 1;
		gameObject[i].mana = 0;
		gameObject[i].type = type;
		gameObject[i].isBackground = FALSE;
		//gameObject[i].isIntangible = FALSE;
		gameObject[i].magicConductivity = 100;
		gameObject[i].x = startX;
		gameObject[i].y = startY;
		gameObject[i].dispX = 0.0;
		gameObject[i].dispY = 0.0;
		double disp = sqrt(dirX * dirX + dirY * dirY);
		if (fabs(disp) < 0.01)
		{
			gameObject[i].vel.x = 0.0;
			gameObject[i].vel.y = 0.0;
		}
		else {
			gameObject[i].vel.x = dirX / disp * speed;
			gameObject[i].vel.y = dirY / disp * speed;
		}
		gameObject[i].motiveVel.x = 0.0;
		gameObject[i].motiveVel.y = 0.0;
		gameObject[i].turnsAlive = 0;
		gameObject[i].lifespan = lifespan;
		gameObject[i].destroyCriteria = destroyCriteria;
		gameObject[i].underMove = FALSE;
		gameObject[i].underGravity = underGravity;
		gameObject[i].fixedFlight = FALSE;
		gameObject[i].facingDir = 1;
		gameObject[i].master = master;

		gameObject[i].attri = 0;
		gameObject[i].attri2 = 0;

		gameObject[i].spawnRegionCount = NULL;
		return defaultObjectsInit(environment, i);
	}
	return -1;
}

int createObjectProjectileDest(Region *environment, int master, ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int destroyCriteria, BOOL underGravity)
{
	return createObjectProjectileDir(environment, master, type, startX, startY, destX - startX, destY - startY, speed, lifespan, destroyCriteria, underGravity);
}

int createObjectMagicProjectileDir(Region *environment, int master, ObjectType type, double startX, double startY, double dirX, double dirY, double speed, int lifespan, int sphere, int enchant, int damage)
{
	for (int i = MAX_OBJECT - 1; i >= 0; i--)
	{
		if (gameObject[i].type != NOTHING)
			continue;		// if not empty, try next

		gameObject[i].max_endurance = 1;
		gameObject[i].max_mana = damage;
		gameObject[i].endurance = 1;
		gameObject[i].mana = damage;
		gameObject[i].type = type;
		gameObject[i].isBackground = FALSE;
		//gameObject[i].isIntangible = FALSE;
		gameObject[i].magicConductivity = 100;
		gameObject[i].x = startX;
		gameObject[i].y = startY;
		gameObject[i].dispX = dirX;
		gameObject[i].dispY = dirY;
		double disp = sqrt(gameObject[i].dispX * gameObject[i].dispX + gameObject[i].dispY * gameObject[i].dispY);
		if (fabs(disp) < 0.01)
		{
			gameObject[i].vel.x = 0.0;
			gameObject[i].vel.y = 0.0;
		}
		else {
			gameObject[i].vel.x = gameObject[i].dispX / disp * speed;
			gameObject[i].vel.y = gameObject[i].dispY / disp * speed;
		}
		gameObject[i].motiveVel.x = 0.0;
		gameObject[i].motiveVel.y = 0.0;
		gameObject[i].turnsAlive = 0;
		gameObject[i].lifespan = lifespan;

		switch (type)
		{
		case MAGIC_BLOB:
			gameObject[i].destroyCriteria = 0;
			gameObject[i].underMove = FALSE;
			gameObject[i].underGravity = FALSE;
			gameObject[i].fixedFlight = FALSE;
			break;
		case MAGIC_SPIKE:
			gameObject[i].destroyCriteria = 0;
			gameObject[i].underMove = FALSE;
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = FALSE;
			break;
		case MAGIC_LASER:
			gameObject[i].dispX /= disp;
			gameObject[i].dispY /= disp;
			gameObject[i].x += gameObject[i].dispX;
			gameObject[i].y += gameObject[i].dispY;
			gameObject[i].vel.x = 0.0;
			gameObject[i].vel.y = 0.0;
			gameObject[i].turnsAlive = -1;
			gameObject[i].lifespan = 5;
			gameObject[i].destroyCriteria = 0;
			gameObject[i].underMove = FALSE;
			gameObject[i].underGravity = FALSE;
			gameObject[i].fixedFlight = FALSE;
			break;
		case MAGIC_FLAME:
			gameObject[i].vel.x = 0.0;
			gameObject[i].vel.y = 0.0;
			gameObject[i].destroyCriteria = 0;
			gameObject[i].underMove = FALSE;
			gameObject[i].underGravity = FALSE;
			gameObject[i].fixedFlight = FALSE;
			break;
		case MAGIC_FRAGMENT:
			gameObject[i].destroyCriteria = 0;
			gameObject[i].underMove = FALSE;
			gameObject[i].underGravity = TRUE;
			gameObject[i].fixedFlight = FALSE;
			break;
		default:
			break;
		}
		gameObject[i].facingDir = 1;
		gameObject[i].master = master;

		gameObject[i].attri = sphere;
		gameObject[i].attri2 = enchant;
		if (gameObject[i].attri & SPHERE_ICE)
			gameObject[i].attri2 |= ENCHANT_COLD_SLOW;

		gameObject[i].spawnRegionCount = NULL;
		return defaultObjectsInit(environment, i);
	}
	return -1;
}

int createObjectMagicProjectile(Region *environment, int master, ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int sphere, int enchant, int damage)
{
	return createObjectMagicProjectileDir(environment, master, type, startX, startY, destX - startX, destY - startY, speed, lifespan, sphere, enchant, damage);
}

// -1 fail, otherwise id of blob
int createObjectMagicRain(Region *environment, int master, ObjectType type, double CX, double CY, int W, int H, double speed, int sphere, int enchant, int damage)
{
	if (environment == NULL) return -1;
	if (type != MAGIC_BLOB && type != MAGIC_SPIKE)
		return -1;
	double sX = CX - (W / 2) + (rand() % W);
	double sY = CY - H;
	int fX = (int)floor(sX);
	int fY = (int)floor(sY);
	if (fX < 0 || fX >= environment->width || fY < 0 || fY >= environment->height)
		return -1;
	return (createObjectMagicProjectileDir(environment, master, type, sX, sY, 0.0, 1.0, speed, -1, sphere, enchant, damage));
}

int createObjectMist(Region *environment, int master, ObjectType type, double startX, double startY, int lifespan, int sphere, int enchant)
{
	for (int i = MAX_OBJECT - 1; i >= 0; i--)
	{
		if (gameObject[i].type != NOTHING)
			continue;		// if not empty, try next

		gameObject[i].max_endurance = 1;
		gameObject[i].max_mana = 0;
		gameObject[i].endurance = 1;
		gameObject[i].mana = 0;
		gameObject[i].type = type;
		gameObject[i].isBackground = TRUE;
		//gameObject[i].isIntangible = TRUE;
		gameObject[i].magicConductivity = 0;
		gameObject[i].x = startX;
		gameObject[i].y = startY;
		gameObject[i].dispX = 0.0;
		gameObject[i].dispY = 0.0;
		gameObject[i].vel.x = 0.0;
		gameObject[i].vel.y = 0.0;
		gameObject[i].motiveVel.x = 0.0;
		gameObject[i].motiveVel.y = 0.0;
		gameObject[i].turnsAlive = 0;
		gameObject[i].lifespan = lifespan;

		gameObject[i].destroyCriteria = 0;

		switch (type)
		{
		case MIST:
			gameObject[i].destroyCriteria = 0;
			gameObject[i].underMove = FALSE;
			gameObject[i].underGravity = FALSE;
			gameObject[i].fixedFlight = TRUE;
			break;
		default:
			return -1;
		}
		gameObject[i].facingDir = 1;
		gameObject[i].master = master;

		gameObject[i].attri = sphere;
		gameObject[i].attri2 = enchant;
		if (gameObject[i].attri & SPHERE_ICE)
			gameObject[i].attri2 |= ENCHANT_COLD_SLOW;

		gameObject[i].spawnRegionCount = NULL;
		return defaultObjectsInit(environment, i);
	}
	return -1;
}

// This function is for loading sprites
// -1 fail, otherwise return objId
int defaultObjectsInit(Region *environment, int objId)
{
	if (environment == NULL) return -1;
	if (gameObject[objId].type == NOTHING) return -1;
	int fX = (int)floor(gameObject[objId].x);
	int fY = (int)floor(gameObject[objId].y);
	if (fX < 0 || fX >= environment->width || fY < 0 || fY >= environment->height)
		return -1;
	gameObject[objId].sprite = NULL;
	switch (gameObject[objId].type)
	{
	case LIFE_HUMANOID:
		if ((gameObject[objId].attri & HUMANOID_TYPE_MASK) == HUMANOID_TYPE_HUMAN)
			gameObject[objId].sprite = getImage(LIFE_HUMANOID, gameObject[objId].attri);
		else
			gameObject[objId].sprite = getImage(LIFE_HUMANOID, gameObject[objId].attri | (gameObject[objId].facingDir & 1));
		break;
	case LIFE_EYEBALL:
		gameObject[objId].sprite = getImage(LIFE_EYEBALL, rand()%3);
		break;
	case LIFE_MOSQUITOES:
		gameObject[objId].sprite = getImage(LIFE_MOSQUITOES, rand()%4);
		break;
	case LIFE_MUSHROOM:
		gameObject[objId].sprite = getImage(LIFE_MOSQUITOES, rand()%3);
		if (!registerEnvironmentObject(environment, objId))
		break;
	case LIFE_RABBIT:
		gameObject[objId].sprite = getImage(LIFE_RABBIT, gameObject[objId].facingDir & 1);
		break;
	case LIFE_SLIME:
		gameObject[objId].sprite = getImage(LIFE_SLIME, (gameObject[objId].facingDir & 1));
		break;
	case LIFE_SLUDGE:
		gameObject[objId].sprite = getImage(LIFE_SLUDGE, 2 + (gameObject[objId].facingDir & 1));
		break;
	case LIFE_GRASS:
		gameObject[objId].sprite = getImage(LIFE_GRASS, (rand()%8==0)?(4+rand()%2):rand()%4);
		break;
	case SPAWN_BEE_HIVE:
		gameObject[objId].sprite = getImage(SPAWN_BEE_HIVE, rand()%2);
		break;
	case LIFE_BEE:
		gameObject[objId].sprite = getImage(LIFE_BEE, 2 + rand() % 2);
		break;
	case SPAWN_DURIAN_TREE:
		gameObject[objId].sprite = getImage(SPAWN_DURIAN_TREE, 0);
		break;
	case LIFE_DURIAN:
		gameObject[objId].sprite = getImage(LIFE_DURIAN, 1);
		break;
	case MAGIC_FLAME:
	case MAGIC_FRAGMENT:
		if (environment->blocked[fY][fX])
		{
			deleteObject(environment, objId, TRUE);
			return -1;
		}
		break;
	default:
		break;
	}
	if (gameObject[objId].sprite != NULL)
	{
		if (!registerEnvironmentObject(environment, objId))
		{
			deleteObject(environment, objId, TRUE);
			return -1;
		}
	}
	for (int index = 0; index < TOTAL_EFFECT_COUNT; index++)
	{
		gameObject[objId].underEffect[index] = -1;
	}
	return objId;
}

void deleteObject(Region *environment, int id, BOOL silentDelete)
{
	if (environment == NULL) return;
	if (gameObject[id].type == NOTHING) return;
	removeEnvironmentObject(environment, id, gameObject[id].x, gameObject[id].y, gameObject[id].sprite);
	if (!silentDelete)
	{
		switch (gameObject[id].type)
		{
		case LIFE_MUSHROOM: {
			for (int k = 0; k < 8; k++)
			{
				createObjectMist(environment, -1, MIST, gameObject[id].x, gameObject[id].y, 1000, SPHERE_MYTH, ENCHANT_BLIND);
			}
			break;
		}
		case LIFE_RABBIT: {
			createObjectProjectileDir(environment, -1, BOMB, gameObject[id].x, gameObject[id].y, 0.0, 0.0, 0.3, 15, 0, TRUE);
			createObjectProjectileDir(environment, -1, BOMB, gameObject[id].x - 1, gameObject[id].y, -1.0, 0.0, 0.3, 15, 0, TRUE);
			createObjectProjectileDir(environment, -1, BOMB, gameObject[id].x + 1, gameObject[id].y, 1.0, 0.0, 0.3, 15, 0, TRUE);
			createObjectProjectileDir(environment, -1, BOMB, gameObject[id].x, gameObject[id].y - 1, 0.0, -1.0, 0.3, 15, 0, TRUE);
			createObjectProjectileDir(environment, -1, BOMB, gameObject[id].x - 1, gameObject[id].y - 1, -1.0, -1.0, 0.3, 15, 0, TRUE);
			createObjectProjectileDir(environment, -1, BOMB, gameObject[id].x + 1, gameObject[id].y - 1, 1.0, -1.0, 0.3, 15, 0, TRUE);
			break;
		}
		case LIFE_SLIME: {
			createObject(environment, -1, LIFE_SLIME, gameObject[id].x - 3, gameObject[id].y);
			createObject(environment, -1, LIFE_SLIME, gameObject[id].x + 3, gameObject[id].y);
			break;
		}
		case SPAWN_BEE_HIVE: {
			for (int k=0; k<10; k++) {
				createObject(environment, -1, LIFE_BEE, gameObject[id].x + getRandomOfRange(3), gameObject[id].y + getRandomOfRange(3));
			}
			break;
		}
		case MAGIC_BLOB: {
			if (gameObject[id].attri)
			{
				if (gameObject[id].attri & SPHERE_FIRE)
				{
					int diameter = 5; // must be odd number
					int radius = diameter / 2;
					int rsq = radius * radius;
					int cx = (int)floor(gameObject[id].x);
					int cy = (int)floor(gameObject[id].y);
					int ox, oy;
					for (int yc = 0, oy = cy - radius; yc < diameter; yc++, oy++)
					{
						for (int xc = 0, ox = cx - radius; xc < diameter; xc++, ox++)
						{
							if ((ox - cx) * (ox - cx) + (oy - cy) * (oy - cy) > rsq) continue;
							if (ox < 0 || ox >= environment->width || oy < 0 || oy >= environment->height) continue;
							createObjectMagicProjectile(environment, gameObject[id].master, MAGIC_FLAME, ox, oy, 0.0, 0.0, 0.0, 10, SPHERE_FIRE, 0, gameObject[id].mana / 10 + 1);
						}
					}
				}
			}
			if (gameObject[id].attri2 & ENCHANT_SHRAPNEL)
			{
				int cx = (int)floor(gameObject[id].x);
				int cy = (int)floor(gameObject[id].y);
				for (int k = 0; k < 4; k++)
				{
					int dirX = rand() % 101 - 50;
					int dirY = rand() % 101 - 50;
					// conservation of momentum -> there must be a shrapnel going in the opposite direction
					createObjectMagicProjectileDir(environment, gameObject[id].master, MAGIC_FRAGMENT, cx, cy, dirX, dirY, 0.2, 250, gameObject[id].attri, gameObject[id].attri2 & ENCHANT_EFFECT_MASK, gameObject[id].mana / 20 + 1);
					createObjectMagicProjectileDir(environment, gameObject[id].master, MAGIC_FRAGMENT, cx, cy, -dirX, -dirY, 0.2, 250, gameObject[id].attri, gameObject[id].attri2 & ENCHANT_EFFECT_MASK, gameObject[id].mana / 20 + 1);
				}
			}
			break;
		}
		case MAGIC_LASER:
		case MAGIC_SPIKE:
			break;
		case BOMB:
		{
			// spawn fragments when bombs are destroyed!
			double dirX, dirY, shrapnelV = 0.2;
			removeEnvironmentBlock(environment, gameObject[id].x, gameObject[id].y);
			for (int k = 0; k < 4; k++)
			{
				dirX = rand() % 101 - 50;
				dirY = rand() % 101 - 50;
				// conservation of momentum -> there must be a shrapnel going in the opposite direction
				createObjectProjectileDir(environment, gameObject[id].master, FRAGMENT, gameObject[id].x, gameObject[id].y, dirX, dirY, shrapnelV, 250, 0, TRUE);
				createObjectProjectileDir(environment, gameObject[id].master, FRAGMENT, gameObject[id].x, gameObject[id].y, -dirX, -dirY, shrapnelV, 250, 0, TRUE);
				removeEnvironmentBlock(environment, gameObject[id].x + DIRECTION2X[k], gameObject[id].y + DIRECTION2Y[k]);
				removeEnvironmentBlock(environment, gameObject[id].x + DIAGONALX[k], gameObject[id].y + DIAGONALY[k]);
				int ox = (int)floor(gameObject[id].x + DIRECTION2X[k]), oy = (int)floor(gameObject[id].y + DIRECTION2Y[k]);
				interactObject(gameObject[id].master, environment->objId[oy][ox], TRUE, DMG_STANDARD_BOMB_DAMAGE, gameObject[id].attri, 0);
				ox = (int)floor(gameObject[id].x + DIAGONALX[k]); oy = (int)floor(gameObject[id].y + DIAGONALY[k]);
				interactObject(gameObject[id].master, environment->objId[oy][ox], TRUE, DMG_STANDARD_BOMB_DAMAGE, gameObject[id].attri, 0);
			}
			break;
		}
		default:
			break;
		}
	}
	if (gameObject[id].spawnRegionCount != NULL) {
		*(gameObject[id].spawnRegionCount) = *(gameObject[id].spawnRegionCount) - 1;
	}
	gameObject[id].type = NOTHING;		// destroy it!
	return;
}

void displayObjects(Region *environment, int observerId, Coordinate scrTopLeftPos, int scrW, int scrH)
{
	if (environment == NULL) return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (gameObject[i].type == NOTHING)
			continue;

		if (i != observerId)
		{
			if (gameObject[observerId].underEffect[EFFECT_BLIND] >= 0)
				continue;
			if (gameObject[i].underEffect[EFFECT_INVISIBLE] >= 0)
				continue;
		}

		int screenX = (int)floor(gameObject[i].x - scrTopLeftPos.x);
		int screenY = (int)floor(gameObject[i].y - scrTopLeftPos.y);

		double tmp;
		int fcolor;
		switch (gameObject[i].type)
		{
		case DEMO_OBJ_USING_IMG_LOADER:
		case LIFE_HUMANOID:
		case LIFE_EYEBALL:
		case LIFE_MOSQUITOES:
		case LIFE_MUSHROOM:
		case LIFE_RABBIT:
		case LIFE_SLIME:
		case LIFE_SLUDGE:
		case LIFE_GRASS:
		case SPAWN_BEE_HIVE:
		case LIFE_BEE:
		case SPAWN_DURIAN_TREE:
		case LIFE_DURIAN:
			{
				if (gameObject[i].sprite == NULL) break;
				int grx, gry;
				int lx, ly;
				int fdimx = (int)floor(gameObject[i].sprite->dimension->x);
				int fdimy = (int)floor(gameObject[i].sprite->dimension->y);
				for (ly = 0, gry = -(int)floor(gameObject[i].sprite->center->y); ly < fdimy; gry++, ly++)
				{
					for (lx = 0, grx = -(int)floor(gameObject[i].sprite->center->x); lx < fdimx; grx++, lx++)
					{
						if (gameObject[i].sprite->solid[ly][lx] > 0)
						{
							if (move(screenY + gry, screenX + grx) != ERR)
							{
								fcolor = gameObject[i].sprite->color[ly][lx];
								// KEYWORD: effectcolor enchantcolor
								if (gameObject[i].underEffect[EFFECT_INVISIBLE] >= 0)
									fcolor = COLOR_PAIR(COLOR_B_BLACK);
								else if (gameObject[i].underEffect[EFFECT_STUN] >= 0)
									fcolor = COLOR_PAIR(COLOR_WHITE);
								else if (gameObject[i].underEffect[EFFECT_COLD_SLOW] >= 0)
									fcolor = COLOR_PAIR(COLOR_B_BLUE);
								attron(fcolor);
								addch(gameObject[i].sprite->display[ly][lx]);
								attroff(fcolor);
							}
						}
					}
				}
			}
			break;
		case MAGIC_BLOB:
			{
				if (move(screenY, screenX) == ERR) break;
				fcolor = COLOR_WHITE;
				char outchar = '|';
				if (gameObject[i].attri & SPHERE_FIRE)
				{
					fcolor = COLOR_B_RED;
					outchar = '@';
				}
				else if (gameObject[i].attri & SPHERE_EARTH) {
					fcolor = COLOR_YELLOW;
					outchar = 'O';
				}
				else if (gameObject[i].attri & SPHERE_ICE) {
					fcolor = COLOR_B_BLUE;
					outchar = 'O';
				}
				else if (gameObject[i].attri & SPHERE_MYTH) {
					outchar = '?';
				}
				attron(COLOR_PAIR(fcolor));
				addch(outchar);
				attroff(COLOR_PAIR(fcolor));
			}
			break;
		case MAGIC_SPIKE:
			{
				if (move(screenY, screenX) == ERR) break;
				fcolor = COLOR_WHITE;
				char outchar = 'v';
				if (gameObject[i].attri & SPHERE_ICE)
					fcolor = COLOR_B_CYAN;
				attron(COLOR_PAIR(fcolor));
				addch(outchar);
				attroff(COLOR_PAIR(fcolor));
			}
			break;
		case MAGIC_LASER: // can use middle-line algorithm here for optimization
			{
				fcolor = COLOR_WHITE;
				if (gameObject[i].attri & SPHERE_FIRE)
				{
					fcolor = COLOR_B_RED;
				}
				else if (gameObject[i].attri & SPHERE_ICE) {
					fcolor = COLOR_B_CYAN;
				}
				double curX = gameObject[i].x;
				double curY = gameObject[i].y;
				int fX, fY;
				attron(COLOR_PAIR(fcolor));
				while (1)
				{
					fX = (int)floor(curX);
					fY = (int)floor(curY);
					if (fX < 0 || fX >= environment->width || fY < 0 || fY >= environment->height)
						break;
					if (environment->blocked[fY][fX] && (environment->objId[fY][fX] != gameObject[i].master))
						break;
					if (move((int)floor(curY - scrTopLeftPos.y), (int)floor(curX - scrTopLeftPos.x)) != ERR)
						addch('*');
					curX += gameObject[i].dispX;
					curY += gameObject[i].dispY;
				}
				attroff(COLOR_PAIR(fcolor));
			}
			break;
		case MAGIC_FLAME:
			{
				if (move(screenY, screenX) == ERR) break;
				fcolor = COLOR_B_RED;
				attron(COLOR_PAIR(fcolor));
				addch('#');
				attroff(COLOR_PAIR(fcolor));
			}
			break;
		case MIST:
			{
				if (move(screenY, screenX) == ERR) break;
				fcolor = COLOR_YELLOW;
				if (gameObject[i].attri & SPHERE_ICE)
					fcolor = COLOR_B_BLUE;
				attron(COLOR_PAIR(fcolor));
				addch(97 | A_ALTCHARSET);
				attroff(COLOR_PAIR(fcolor));
			}
			break;
		case BULLET:
			if (move(screenY, screenX) == ERR) break;
			attron(COLOR_PAIR(COLOR_WHITE));
			addch('*');
			attroff(COLOR_PAIR(COLOR_WHITE));
			break;
		case BOMB:
			if (move(screenY, screenX) == ERR) break;
			addch('@');
			break;
		case FRAGMENT:
			fcolor = COLOR_WHITE;
		case MAGIC_FRAGMENT:
			if (gameObject[i].type == MAGIC_FRAGMENT)
			{
				if (gameObject[i].attri)
				{
					if (gameObject[i].attri & SPHERE_FIRE)
					{
						fcolor = COLOR_B_RED;
					}
					else if (gameObject[i].attri & SPHERE_ICE) {
						fcolor = COLOR_B_CYAN;
					}
				}
			}
			if (move(screenY, screenX) == ERR) break;
			tmp = (gameObject[i].y - floor(gameObject[i].y));
			attron(COLOR_PAIR(fcolor));
			if (tmp <= 0.5)
			{
				if (tmp <= 0.25)
					addch('`');
				else
					addch('\'');
			}
			else {
				if (tmp <= 0.75)
					addch('.');
				else
					addch(',');
			}
			attroff(COLOR_PAIR(fcolor));
			break;
		default:
			break;
		}
	}
	return;
}

void displayCrossHair(int X, int Y)
{
	attron(COLOR_PAIR(COLOR_B_CYAN));
	//addch(215 | A_ALTCHARSET);
	if (move(Y, X - 2) != ERR) addch('[');
	//if (move(Y, X - 1) != ERR) addch(' ');
	if (move(Y, X) != ERR) addch('+');
	//if (move(Y, X + 1) != ERR) addch(' ');
	if (move(Y, X + 2) != ERR) addch(']');
	attroff(COLOR_PAIR(COLOR_B_CYAN));
	return;
}

void pushObjectDir(int id, double dirX, double dirY, double speed)
{
	if (gameObject[id].type == NOTHING) return;

	gameObject[id].dispX = dirX;
	gameObject[id].dispY = dirY;
	double disp = sqrt(dirX * dirX + dirY * dirY);
	if (fabs(disp) > 0.01)
	{
		gameObject[id].vel.x += dirX / disp * speed;
		gameObject[id].vel.y += dirY / disp * speed;
	}
	speed = gameObject[id].vel.x * gameObject[id].vel.x + gameObject[id].vel.y * gameObject[id].vel.y;
	if (speed > 1.0)
	{
		speed = sqrt(speed);
		gameObject[id].vel.x /= speed;
		gameObject[id].vel.y /= speed;
	}
	return;
}

void acceObjects(Region *environment)
{
	if (environment == NULL) return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (gameObject[i].type == NOTHING)
			continue;

		BOOL onFeet = FALSE;
		BOOL underGravity = (gameObject[i].underGravity || (gameObject[i].underEffect[EFFECT_ENTANGLE] >= 0));
		if (underGravity)
		{
			gameObject[i].vel.y += GRAVITATIONAL_ACC;
			if ((gameObject[i].vel.y > FRICTION_TRIGGER) && checkObjectOnFeet(environment, i)) // friction and normal force only works if you press it against the surface
			{
				gameObject[i].y = floor(gameObject[i].y) + 0.8;
				onFeet = TRUE;
				gameObject[i].vel.x = 0.0; // friction coefficient is infinite
				gameObject[i].vel.y = 0.0; // absorb all downward momentum
				triggerObjectHitEvent(environment, i, gameObject[i].x, gameObject[i].y + 1);
			}
		}
		else if (gameObject[i].fixedFlight) {
			gameObject[i].vel.x *= 0.2;
			gameObject[i].vel.y *= 0.2;
		}

		if (gameObject[i].underMove)
		{
			// exception of physics to make player easier to control the character in flight
			if (underGravity && (!onFeet))
			{
				if (fabs(gameObject[i].vel.x + gameObject[i].motiveVel.x * 0.8) <= fabs(gameObject[i].motiveVel.x * 0.8))
					gameObject[i].vel.x += gameObject[i].motiveVel.x * 0.8;
			}

			BOOL cancelMove = (fabs(gameObject[i].dispX) + fabs(gameObject[i].dispY) <= 0.01); // finish moving
			cancelMove |= (underGravity && (!onFeet)); // unable to move on free will
			if (cancelMove)
			{
				gameObject[i].underMove = FALSE;
				gameObject[i].dispX = 0.0;
				gameObject[i].dispY = 0.0;
			}
		}
		if (gameObject[i].underMove) // try to apply motive speed until disp is exhausted
		{
			// motiveVel has the same direction as disp along the same axis
			if (fabs(gameObject[i].motiveVel.x) <= fabs(gameObject[i].dispX))
			{
				gameObject[i].vel.x += gameObject[i].motiveVel.x;
				gameObject[i].dispX -= gameObject[i].motiveVel.x;
			}
			else {
				gameObject[i].vel.x += gameObject[i].dispX;
				gameObject[i].dispX = 0.0;
			}
			if (fabs(gameObject[i].motiveVel.y) <= fabs(gameObject[i].dispY))
			{
				gameObject[i].vel.y += gameObject[i].motiveVel.y;
				gameObject[i].dispY -= gameObject[i].motiveVel.y;
			}
			else {
				gameObject[i].vel.y += gameObject[i].dispY;
				gameObject[i].dispY = 0.0;
			}
		}
		else { // regression
			if (gameObject[i].motiveVel.x < 0.5)
				gameObject[i].motiveVel.x = 0.0;
			else
				gameObject[i].motiveVel.x *= 0.99;
			gameObject[i].motiveVel.y = 0.0;
		}
	}
}

void controlObjectX(int id, double destX, double speed)
{
	if (gameObject[id].type == NOTHING)
		return;
	if (gameObject[id].underEffect[EFFECT_STUN] >= 0)
		return;

	gameObject[id].dispX = destX - gameObject[id].x;
	if (gameObject[id].underEffect[EFFECT_CONFUSE] >= 0)
		gameObject[id].dispX = -gameObject[id].dispX;
	if (gameObject[id].dispX > 0)
	{
		gameObject[id].motiveVel.x = speed;
		if (gameObject[id].facingDir == 0)
			gameObject[id].facingDir |= TURNING_UNSETTLED;
	}
	else {
		gameObject[id].motiveVel.x = -speed;
		if (gameObject[id].facingDir == 1)
			gameObject[id].facingDir |= TURNING_UNSETTLED;
	}
	gameObject[id].underMove = TRUE;
	if ((gameObject[id].underEffect[EFFECT_COLD_SLOW] >= 0) || (gameObject[id].underEffect[EFFECT_SLOW] >= 0))
		gameObject[id].motiveVel.x *= 0.5;
	return;
}

void controlObjectY(int id, double destY, double speed)
{
	if (gameObject[id].type == NOTHING)
		return;
	if (gameObject[id].underEffect[EFFECT_STUN] >= 0)
		return;

	gameObject[id].dispY = destY - gameObject[id].y;
	if (gameObject[id].underEffect[EFFECT_CONFUSE] >= 0)
		gameObject[id].dispY = -gameObject[id].dispY;
	if (gameObject[id].dispY > 0)
		gameObject[id].motiveVel.y = speed;
	else
		gameObject[id].motiveVel.y = -speed;
	gameObject[id].underMove = TRUE;
	if ((gameObject[id].underEffect[EFFECT_COLD_SLOW] >= 0) || (gameObject[id].underEffect[EFFECT_SLOW] >= 0))
		gameObject[id].motiveVel.y *= 0.5;
	return;
}

void moveObjects(Region *environment)
{
	if (environment == NULL) return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (gameObject[i].type == NOTHING)
			continue;

		BOOL toDelete = FALSE;
		BOOL silentDelete = FALSE;
		if (gameObject[i].turnsAlive != gameObject[i].lifespan)
		{
			double speed = gameObject[i].vel.x * gameObject[i].vel.x + gameObject[i].vel.y * gameObject[i].vel.y;
			if (speed > 0.9999)
			{
				speed = sqrt(speed);
				if (speed >= 0.9999)
					speed /= 0.9999;
				gameObject[i].vel.x /= speed;
				gameObject[i].vel.y /= speed;
			}
			else if (speed <= 0.01) {
				if (gameObject[i].destroyCriteria & DESTROY_CRITERIA_STOP)
					toDelete = TRUE;
			}

			double newX = gameObject[i].x + gameObject[i].vel.x;
			double newY = gameObject[i].y + gameObject[i].vel.y;
			int fnewX = (int)floor(newX);
			int fnewY = (int)floor(newY);
			if (fnewX < 0 || fnewX >= environment->width || fnewY < 0 || fnewY >= environment->height)
			{
				toDelete = TRUE;
				silentDelete = TRUE;
			}
			else if (checkObjectCollision(environment, i, newX, newY))
			{
				triggerObjectHitEvent(environment, i, newX, newY);
				gameObject[i].y = floor(gameObject[i].y) + 0.8; // still in the same grid
				gameObject[i].vel.x = 0.0;
				gameObject[i].vel.y = 0.0;
				gameObject[i].underMove = FALSE;
				if (gameObject[i].destroyCriteria & (DESTROY_CRITERIA_HIT | DESTROY_CRITERIA_STOP))
					toDelete = TRUE;
			}
			else {
				removeEnvironmentObject(environment, i, gameObject[i].x, gameObject[i].y, gameObject[i].sprite);
				gameObject[i].x = newX;
				gameObject[i].y = newY;
				registerEnvironmentObject(environment, i);
			}
		}
		else {
			toDelete = TRUE;
		}

		if (toDelete)
		{
			deleteObject(environment, i, silentDelete);
		}
	}
	return;
}

void rotateObjects(Region *environment)
{
	if (environment == NULL)
		return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (gameObject[i].type == NOTHING)
			continue;
		switch (gameObject[i].type)
		{
		case LIFE_HUMANOID:
			if (gameObject[i].facingDir & TURNING_UNSETTLED)
			{
				CharacterImage *oldImage = gameObject[i].sprite;
				if ((gameObject[i].attri & HUMANOID_TYPE_MASK) == HUMANOID_TYPE_HUMAN)
					gameObject[i].sprite = getImage(LIFE_HUMANOID, gameObject[i].attri);
				else
					gameObject[i].sprite = getImage(LIFE_HUMANOID, gameObject[i].attri | ((gameObject[i].facingDir & 1) ^ 1));
				if (gameObject[i].sprite == oldImage || gameObject[i].sprite == NULL || checkObjectCollision(environment, i, gameObject[i].x, gameObject[i].y))
				{
					gameObject[i].sprite = oldImage;
					gameObject[i].facingDir ^= TURNING_UNSETTLED;
				}
				else {
					removeEnvironmentObject(environment, i, gameObject[i].x, gameObject[i].y, oldImage);
					registerEnvironmentObject(environment, i);
					gameObject[i].facingDir ^= (TURNING_UNSETTLED | 1);
				}
			}
			break;
		case LIFE_RABBIT:
			if (gameObject[i].facingDir & TURNING_UNSETTLED)
			{
				CharacterImage *oldImage = gameObject[i].sprite;
				gameObject[i].sprite = getImage(LIFE_RABBIT, ((gameObject[i].facingDir & 1) ^ 1));
				if (gameObject[i].sprite == oldImage || gameObject[i].sprite == NULL || checkObjectCollision(environment, i, gameObject[i].x, gameObject[i].y))
				{
					gameObject[i].sprite = oldImage;
					gameObject[i].facingDir ^= TURNING_UNSETTLED;
				}
				else {
					removeEnvironmentObject(environment, i, gameObject[i].x, gameObject[i].y, oldImage);
					registerEnvironmentObject(environment, i);
					gameObject[i].facingDir ^= (TURNING_UNSETTLED | 1);
				}
			}
			break;
		case LIFE_SLIME:
			if (gameObject[i].facingDir & TURNING_UNSETTLED)
			{
				CharacterImage *oldImage = gameObject[i].sprite;
				gameObject[i].sprite = getImage(LIFE_SLIME, ((gameObject[i].facingDir & 1) ^ 1));
				if (gameObject[i].sprite == oldImage || gameObject[i].sprite == NULL || checkObjectCollision(environment, i, gameObject[i].x, gameObject[i].y))
				{
					gameObject[i].sprite = oldImage;
					gameObject[i].facingDir ^= TURNING_UNSETTLED;
				}
				else {
					removeEnvironmentObject(environment, i, gameObject[i].x, gameObject[i].y, oldImage);
					registerEnvironmentObject(environment, i);
					gameObject[i].facingDir ^= (TURNING_UNSETTLED | 1);
				}
			}
			break;
			
		case LIFE_SLUDGE:
			if (gameObject[i].facingDir & TURNING_UNSETTLED)
			{
				CharacterImage *oldImage = gameObject[i].sprite;
				gameObject[i].sprite = getImage(LIFE_SLUDGE, 2 + ((gameObject[i].facingDir & 1) ^ 1));
				if (gameObject[i].sprite == oldImage || gameObject[i].sprite == NULL || checkObjectCollision(environment, i, gameObject[i].x, gameObject[i].y))
				{
					gameObject[i].sprite = oldImage;
					gameObject[i].facingDir ^= TURNING_UNSETTLED;
				}
				else {
					removeEnvironmentObject(environment, i, gameObject[i].x, gameObject[i].y, oldImage);
					registerEnvironmentObject(environment, i);
					gameObject[i].facingDir ^= (TURNING_UNSETTLED | 1);
				}
			}
			break;
		default:
			if (gameObject[i].facingDir & TURNING_UNSETTLED)
			{
				gameObject[i].facingDir ^= TURNING_UNSETTLED;
			}
			break;
		}
	}
	return;
}

void updateObjectsStatus(Region *environment)
{
	if (environment == NULL)
		return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (gameObject[i].type == NOTHING)
			continue;
		if (gameObject[i].endurance <= 0)
		{
			deleteObject(environment, i, FALSE);
			continue;
		}
		CharacterImage *oldImage = NULL;
		CharacterImage *newImage = NULL;
		switch (gameObject[i].type)
		{
		case LIFE_HUMANOID:
			{
				oldImage = gameObject[i].sprite;
				if ((gameObject[i].attri & HUMANOID_TYPE_MASK) == HUMANOID_TYPE_HUMAN)
					newImage = getImage(LIFE_HUMANOID, gameObject[i].attri);
				else
					newImage = getImage(LIFE_HUMANOID, gameObject[i].attri | (gameObject[i].facingDir & 1));
			}
			break;
		case LIFE_EYEBALL:
			{
				oldImage = gameObject[i].sprite;
				newImage = getImage(LIFE_EYEBALL, gameObject[i].sprite->charaID);
			}
			break;
		case LIFE_MOSQUITOES:
			{
				oldImage = gameObject[i].sprite;
				newImage = getImage(LIFE_MOSQUITOES, gameObject[i].sprite->charaID);
			}
			break;
		case LIFE_MUSHROOM:
			{
				oldImage = gameObject[i].sprite;
				newImage = getImage(LIFE_MUSHROOM, gameObject[i].sprite->charaID);
			}
			break;
		case LIFE_RABBIT:
		{
			oldImage = gameObject[i].sprite;
			newImage = getImage(LIFE_RABBIT, gameObject[i].facingDir & 1);
		}
			break;
		case LIFE_SLIME:
		{
			oldImage = gameObject[i].sprite;
			newImage = getImage(LIFE_SLIME, (gameObject[i].facingDir & 1));
		}
			break;
		case LIFE_SLUDGE:
		{
			oldImage = gameObject[i].sprite;
			newImage = getImage(LIFE_SLUDGE, 2 + (gameObject[i].facingDir & 1));
		}
			break;
		case LIFE_GRASS:
		{
			oldImage = gameObject[i].sprite;
			newImage = getImage(LIFE_GRASS, gameObject[i].sprite->charaID);
		}
			break;
		case SPAWN_BEE_HIVE:
		{
			oldImage = gameObject[i].sprite;
			newImage = getImage(SPAWN_BEE_HIVE, gameObject[i].sprite->charaID);
		}
			break;
		case LIFE_BEE:
		{
			oldImage = gameObject[i].sprite;
			newImage = getImage(LIFE_BEE, gameObject[i].sprite->charaID);
		}
			break;
		case SPAWN_DURIAN_TREE:
		{
			oldImage = gameObject[i].sprite;
			newImage = getImage(SPAWN_DURIAN_TREE, 0);
		}
			break;
		case LIFE_DURIAN:
		{
			oldImage = gameObject[i].sprite;
			newImage = getImage(LIFE_DURIAN, 1);
		}
			break;
		case MAGIC_LASER: // can use middle-line algorithm here for optimization
			{
				double curX = gameObject[i].x;
				double curY = gameObject[i].y;
				int fX, fY;
				while (1)
				{
					fX = (int)floor(curX);
					fY = (int)floor(curY);
					if (fX < 0 || fX >= environment->width || fY < 0 || fY >= environment->height)
						break;
					if (environment->blocked[fY][fX] && (environment->objId[fY][fX] != gameObject[i].master))
					{
						interactObject(gameObject[i].master, environment->objId[fY][fX], gameObject[i].mana, 1, gameObject[i].attri, gameObject[i].attri2 & ENCHANT_EFFECT_MASK);
						break;
					}
					curX += gameObject[i].dispX;
					curY += gameObject[i].dispY;
				}
			}
			break;
		default:
			break;
		}
		if (oldImage != newImage)
		{
			gameObject[i].sprite = newImage;
			if (!checkObjectCollision(environment, i, gameObject[i].x, gameObject[i].y))
			{
				removeEnvironmentObject(environment, i, gameObject[i].x, gameObject[i].y, oldImage);
				registerEnvironmentObject(environment, i);
			}
			else {
				gameObject[i].sprite = oldImage;
			}
		}
		gameObject[i].turnsAlive++;
		int manaRecovery = (int)(gameObject[i].max_mana * 0.0005) + 1;
		if (!checkObjectOnFeet(environment, i))
			manaRecovery /= 2;
		if (gameObject[i].mana + manaRecovery < gameObject[i].max_mana)
			gameObject[i].mana += manaRecovery;
		else
			gameObject[i].mana = gameObject[i].max_mana;
		for (int index = 0; index < TOTAL_EFFECT_COUNT; index++)
		{
			if (gameObject[i].underEffect[index] >= 0)
				gameObject[i].underEffect[index]--;
		}
	}
	return;
}

// 0 error 1 event triggered(may not work though)
BOOL triggerObjectHitEvent(Region *environment, int objId, double newX, double newY)
{
	if (environment == NULL) return FALSE;
	if (gameObject[objId].type == NOTHING) return FALSE;
	int master = gameObject[objId].master;
	if (master == -1) master = -2;
	int tmpDmg;
	switch (gameObject[objId].type)
	{
	case LIFE_MOSQUITOES:
		tmpDmg = DMG_STANDARD_MOSQUITO_DAMAGE;
	case LIFE_BEE:
		if (gameObject[objId].type == LIFE_BEE)
			tmpDmg = DMG_STANDARD_BEE_DAMAGE;
		if (gameObject[objId].sprite != NULL)
		{
			int gax, gay;
			int lx, ly;
			int topLeftgx = (int)floor(newX) - (int)floor(gameObject[objId].sprite->center->x);
			int topLeftgy = (int)floor(newY) - (int)floor(gameObject[objId].sprite->center->y);
			int fdimx = (int)floor(gameObject[objId].sprite->dimension->x);
			int fdimy = (int)floor(gameObject[objId].sprite->dimension->y);
			for (ly = 0, gay = topLeftgy; ly < fdimy; gay++, ly++)
			{
				for (lx = 0, gax = topLeftgx; lx < fdimx; gax++, lx++)
				{
					if (gameObject[objId].sprite->solid[ly][lx] > 0)
					{
						if (gax >= 0 && gax < environment->width && gay >= 0 && gay < environment->height)
						{
							if (environment->blocked[gay][gax] && (environment->objId[gay][gax] != objId) && (environment->objId[gay][gax] != master))
							{
								int tId = environment->objId[gay][gax];
								if (tId != -1 && (gameObject[tId].type != gameObject[objId].type) && (gameObject[tId].type != SPAWN_BEE_HIVE))
									interactObject(gameObject[objId].master, environment->objId[gay][gax], FALSE, tmpDmg, 0, 0);
							}
						}
					}
				}
			}
		}
		break;
	case LIFE_SLUDGE:
	case LIFE_SLIME:
		if (gameObject[objId].sprite != NULL)
		{
			int gax, gay;
			int lx, ly;
			int topLeftgx = (int)floor(newX) - (int)floor(gameObject[objId].sprite->center->x);
			int topLeftgy = (int)floor(newY) - (int)floor(gameObject[objId].sprite->center->y);
			int fdimx = (int)floor(gameObject[objId].sprite->dimension->x);
			int fdimy = (int)floor(gameObject[objId].sprite->dimension->y);
			for (ly = 0, gay = topLeftgy; ly < fdimy; gay++, ly++)
			{
				for (lx = 0, gax = topLeftgx; lx < fdimx; gax++, lx++)
				{
					if (gameObject[objId].sprite->solid[ly][lx] > 0)
					{
						if (gax >= 0 && gax < environment->width && gay >= 0 && gay < environment->height)
						{
							if (environment->blocked[gay][gax] && (environment->objId[gay][gax] != objId) && (environment->objId[gay][gax] != master))
							{
								int tId = environment->objId[gay][gax];
								if (tId != -1 && (gameObject[tId].type != LIFE_SLUDGE) && (gameObject[tId].type != LIFE_SLIME))
									interactObject(gameObject[objId].master, environment->objId[gay][gax], TRUE, DMG_STANDARD_SLUDGE_MELEE_DAMAGE, 0, ENCHANT_SLOW);
							}
						}
					}
				}
			}
		}
	break;
	case LIFE_DURIAN: {
		if (gameObject[objId].sprite != NULL)
		{
			int gax, gay;
			int lx, ly;
			int topLeftgx = (int)floor(newX) - (int)floor(gameObject[objId].sprite->center->x);
			int topLeftgy = (int)floor(newY) - (int)floor(gameObject[objId].sprite->center->y);
			int fdimx = (int)floor(gameObject[objId].sprite->dimension->x);
			int fdimy = (int)floor(gameObject[objId].sprite->dimension->y);
			for (ly = 0, gay = topLeftgy; ly < fdimy; gay++, ly++)
			{
				for (lx = 0, gax = topLeftgx; lx < fdimx; gax++, lx++)
				{
					if (gameObject[objId].sprite->solid[ly][lx] > 0)
					{
						if (gax >= 0 && gax < environment->width && gay >= 0 && gay < environment->height)
						{
							if (environment->blocked[gay][gax] && (environment->objId[gay][gax] != objId) && (environment->objId[gay][gax] != master))
							{
								int tId = environment->objId[gay][gax];
								if (tId != -1 && (gameObject[tId].type != gameObject[objId].type))
									interactObject(gameObject[objId].master, environment->objId[gay][gax], FALSE, DMG_STANDARD_DURIAN_DAMAGE, 0, ENCHANT_CONFUSE);
							}
						}
					}
				}
			}
		}
	deleteObject(environment, objId, FALSE);
	break;
	}
	case MAGIC_BLOB:
		if (environment->blocked[(int)floor(newY)][(int)floor(newX)] && (environment->objId[(int)floor(newY)][(int)floor(newX)] != master))
			interactObject(gameObject[objId].master, environment->objId[(int)floor(newY)][(int)floor(newX)], FALSE, gameObject[objId].mana, gameObject[objId].attri, gameObject[objId].attri2 & ENCHANT_EFFECT_MASK);
		deleteObject(environment, objId, FALSE);
		break;
	case MAGIC_SPIKE:
		if (environment->blocked[(int)floor(newY)][(int)floor(newX)] && (environment->objId[(int)floor(newY)][(int)floor(newX)] != master))
			interactObject(gameObject[objId].master, environment->objId[(int)floor(newY)][(int)floor(newX)], FALSE, gameObject[objId].mana, gameObject[objId].attri, gameObject[objId].attri2 & ENCHANT_EFFECT_MASK);
		deleteObject(environment, objId, FALSE);
		break;
	case MAGIC_LASER:
		break;
	case MAGIC_FLAME:
		if (environment->blocked[(int)floor(newY)][(int)floor(newX)] && (environment->objId[(int)floor(newY)][(int)floor(newX)] != master))
			interactObject(gameObject[objId].master, environment->objId[(int)floor(newY)][(int)floor(newX)], FALSE, gameObject[objId].mana, gameObject[objId].attri, gameObject[objId].attri2 & ENCHANT_EFFECT_MASK);
		break;
	case MAGIC_FRAGMENT:
		if (environment->blocked[(int)floor(newY)][(int)floor(newX)] && (environment->objId[(int)floor(newY)][(int)floor(newX)] != master))
			interactObject(gameObject[objId].master, environment->objId[(int)floor(newY)][(int)floor(newX)], TRUE, gameObject[objId].mana, gameObject[objId].attri, gameObject[objId].attri2 & ENCHANT_EFFECT_MASK);
		deleteObject(environment, objId, FALSE);
		break;
	case MIST:
		if (environment->blocked[(int)floor(newY)][(int)floor(newX)] && (environment->objId[(int)floor(newY)][(int)floor(newX)] != master))
			interactObject(gameObject[objId].master, environment->objId[(int)floor(newY)][(int)floor(newX)], FALSE, gameObject[objId].mana, gameObject[objId].attri, gameObject[objId].attri2 & ENCHANT_EFFECT_MASK);
		break;
	case BULLET:
	case FRAGMENT:
		if (environment->blocked[(int)floor(newY)][(int)floor(newX)] && (environment->objId[(int)floor(newY)][(int)floor(newX)] != master))
			interactObject(gameObject[objId].master, environment->objId[(int)floor(newY)][(int)floor(newX)], TRUE, 10, 0, 0);
		deleteObject(environment, objId, FALSE);
		break;
	case DEMO_OBJ_USING_IMG_LOADER:
	default:
		if (gameObject[objId].sprite != NULL)
		{
			int gax, gay;
			int lx, ly;
			int topLeftgx = (int)floor(newX) - (int)floor(gameObject[objId].sprite->center->x);
			int topLeftgy = (int)floor(newY) - (int)floor(gameObject[objId].sprite->center->y);
			int fdimx = (int)floor(gameObject[objId].sprite->dimension->x);
			int fdimy = (int)floor(gameObject[objId].sprite->dimension->y);
			for (ly = 0, gay = topLeftgy; ly < fdimy; gay++, ly++)
			{
				for (lx = 0, gax = topLeftgx; lx < fdimx; gax++, lx++)
				{
					if (gameObject[objId].sprite->solid[ly][lx] > 0)
					{
						if (gax >= 0 && gax < environment->width && gay >= 0 && gay < environment->height)
						{
							if (environment->blocked[gay][gax] && (environment->objId[gay][gax] != objId) && (environment->objId[gay][gax] != master))
							{
								interactObject(gameObject[objId].master, environment->objId[gay][gax], TRUE, 0, 0, 0);
							}
						}
					}
				}
			}
		}
		break;
	}
	return TRUE;
}

// 0 error 1 interaction triggered(may not work though)
BOOL interactObject(int sourceId, int targetId, BOOL physicalTouch, int damage, int sphere, int effect)
{
	if (targetId == -1) return FALSE; // this is one of the few functions that may receive an enquiry about invalid objId
	if (gameObject[targetId].type == NOTHING) return FALSE;
	// this function is called in moveObjects, the bumped object may still haven't executed its hitEvent
	// so we cannot directly call deleteObject()
	if (physicalTouch || (damage > 5))
		if (gameObject[targetId].destroyCriteria & DESTROY_CRITERIA_HIT)
			gameObject[targetId].endurance = 0;
	if (damage > 0) // No healing by negative number
	{
		if (sphere)
		{
			damage = damage * gameObject[targetId].magicConductivity / 100;
			// as long as there is at least 1 dmg before reduction, we still need to give some damage to avoid invincibility
			if (damage <= 0)
				damage = 1;
		}
		if (gameObject[targetId].endurance > damage)
			gameObject[targetId].endurance -= damage;
		else {
			gameObject[targetId].endurance = 0;
			if (sourceId != -1 && gameObject[sourceId].type == LIFE_HUMANOID)
			{
				switch (gameObject[targetId].type) // Earn EXP
				{
				case LIFE_EYEBALL:
					gameObject[sourceId].attri2 += 1; break;
				case LIFE_MOSQUITOES:
				case LIFE_MUSHROOM:
					gameObject[sourceId].attri2 += 0; break;
				case LIFE_RABBIT:
					gameObject[sourceId].attri2 += 2; break;
				case LIFE_SLUDGE:
					gameObject[sourceId].attri2 += 2; break;
				case LIFE_SLIME:
					gameObject[sourceId].attri2 += 2; break;
				case LIFE_BEE:
					gameObject[sourceId].attri2 += 1; break;
				case SPAWN_BEE_HIVE:
					gameObject[sourceId].attri2 += 10; break;
				default:
					break;
				}
			}
		}
	}
	if (effect) // KEYWORD: hit effect addeffect addenchant effectduration enchantduration
	{
		if (effect & ENCHANT_COLD_SLOW)
			if (gameObject[targetId].underEffect[EFFECT_COLD_SLOW] < (500 * gameObject[targetId].magicConductivity / 100))
				gameObject[targetId].underEffect[EFFECT_COLD_SLOW] = (500 * gameObject[targetId].magicConductivity / 100);
		if (effect & ENCHANT_SLOW)
			if (gameObject[targetId].underEffect[EFFECT_SLOW] < (200 * gameObject[targetId].magicConductivity / 100))
				gameObject[targetId].underEffect[EFFECT_SLOW] = (200 * gameObject[targetId].magicConductivity / 100) - 1;
		if (effect & ENCHANT_BLIND)
			if (gameObject[targetId].underEffect[EFFECT_BLIND] < (500 * gameObject[targetId].magicConductivity / 100))
				gameObject[targetId].underEffect[EFFECT_BLIND] = (500 * gameObject[targetId].magicConductivity / 100);
		if (effect & ENCHANT_STUN)
			if (gameObject[targetId].underEffect[EFFECT_STUN] < (50 * gameObject[targetId].magicConductivity / 100))
				gameObject[targetId].underEffect[EFFECT_STUN] = (50 * gameObject[targetId].magicConductivity / 100);
		if (effect & ENCHANT_ENTANGLE)
			if (gameObject[targetId].underEffect[EFFECT_ENTANGLE] < (200 * gameObject[targetId].magicConductivity / 100))
			{
				gameObject[targetId].vel.x = 0.0;
				gameObject[targetId].vel.y = 0.0;
				gameObject[targetId].underEffect[EFFECT_ENTANGLE] = (200 * gameObject[targetId].magicConductivity / 100);
			}
		if (effect & ENCHANT_CONFUSE)
			if (gameObject[targetId].underEffect[EFFECT_CONFUSE] < (500 * gameObject[targetId].magicConductivity / 100))
				gameObject[targetId].underEffect[EFFECT_CONFUSE] = (500 * gameObject[targetId].magicConductivity / 100);
		if (effect & ENCHANT_CLOAK)
			if (gameObject[targetId].underEffect[EFFECT_INVISIBLE] < (1000 * gameObject[targetId].magicConductivity / 100))
				gameObject[targetId].underEffect[EFFECT_INVISIBLE] = (1000 * gameObject[targetId].magicConductivity / 100);
		if (effect & ENCHANT_SILENT)
			if (gameObject[targetId].mana <= 1000)
				gameObject[targetId].mana = 0;
			else
				gameObject[targetId].mana -= 1000;
	}
	return TRUE;
}

// 0 error/miss 1 hit
BOOL checkObjectCollision(Region *environment, int objId, double x, double y)
{
	if (environment == NULL) return FALSE;
	if (gameObject[objId].type == NOTHING) return FALSE;
	int master = gameObject[objId].master;
	if (master == -1) master = -2;
	switch (gameObject[objId].type)
	{
	case DEMO_OBJ_USING_IMG_LOADER:
	case LIFE_HUMANOID:
	case LIFE_EYEBALL:
	case LIFE_MOSQUITOES:
	case LIFE_MUSHROOM:
	case LIFE_RABBIT:
	case LIFE_SLIME:
	case LIFE_SLUDGE:
	case LIFE_GRASS:
	case SPAWN_BEE_HIVE:
	case LIFE_BEE:
	case SPAWN_DURIAN_TREE:
	case LIFE_DURIAN:
		{
			if (gameObject[objId].sprite == NULL) return FALSE;
			int gax, gay;
			int lx, ly;
			int topLeftgx = (int)floor(x) - (int)floor(gameObject[objId].sprite->center->x);
			int topLeftgy = (int)floor(y) - (int)floor(gameObject[objId].sprite->center->y);
			int fdimx = (int)round(gameObject[objId].sprite->dimension->x);
			int fdimy = (int)round(gameObject[objId].sprite->dimension->y);
			for (ly = 0, gay = topLeftgy; ly < fdimy; gay++, ly++)
			{
				for (lx = 0, gax = topLeftgx; lx < fdimx; gax++, lx++)
				{
					if (gameObject[objId].sprite->solid[ly][lx] > 0)
					{
						if (gax >= 0 && gax < environment->width && gay >= 0 && gay < environment->height)
						{
							if (environment->blocked[gay][gax] && (environment->objId[gay][gax] != objId) && (environment->objId[gay][gax] != master))
							{
								return TRUE;
							}
						}
					}
				}
			}
		}
		break;
	case MAGIC_BLOB:
	case MAGIC_SPIKE:
	case MAGIC_FLAME:
	case MAGIC_FRAGMENT:
	case MIST:
	case BULLET:
	case BOMB:
	case FRAGMENT:
		if (environment->blocked[(int)floor(y)][(int)floor(x)] && (environment->objId[(int)floor(y)][(int)floor(x)] != master))
		{
			return TRUE;
		}
		break;
	default:
		break;
	}
	return FALSE;
}

// 0 error/not on feet 1 on feet
BOOL checkObjectOnFeet(Region *environment, int objId)
{
	if (environment == NULL) return FALSE;
	if (gameObject[objId].type == NOTHING) return FALSE;
	int master = gameObject[objId].master;
	if (master == -1) master = -2;
	switch (gameObject[objId].type)
	{
	// add special cases that has no sprite
	case MAGIC_BLOB:
	case MAGIC_SPIKE:
	case MAGIC_FLAME:
	case MAGIC_FRAGMENT:
	case MIST:
	case BULLET:
	case BOMB:
	case FRAGMENT:
		{
			int fX = (int)floor(gameObject[objId].x);
			int fY = (int)floor(gameObject[objId].y) + 1;
			if (fX < 0 || fX >= environment->width || fY < 0 || fY >= environment->height)
				return FALSE;
			if (environment->blocked[(int)floor(gameObject[objId].y) + 1][(int)floor(gameObject[objId].x)])
				return TRUE;
		}
		break;
	case MAGIC_LASER:
		break;
	case DEMO_OBJ_USING_IMG_LOADER:
	default:
		{
			if (gameObject[objId].sprite == NULL) return FALSE;
			int fdimx = (int)floor(gameObject[objId].sprite->dimension->x);
			int fdimy = (int)floor(gameObject[objId].sprite->dimension->y);
			int gax = (int)floor(gameObject[objId].x) - (int)floor(gameObject[objId].sprite->center->x);
			int gyUnderFoot = (int)floor(gameObject[objId].y) - (int)floor(gameObject[objId].sprite->center->y) + fdimy;
			if (gyUnderFoot < 0 || gyUnderFoot >= environment->height)
				return FALSE;
			for (int lx = 0; lx < fdimx; gax++, lx++)
			{
				if (gameObject[objId].sprite->solid[fdimy - 1][lx] > 0)
				{
					if (gax < 0 || gax >= environment->width)
						return FALSE;
					if (environment->blocked[gyUnderFoot][gax] && (environment->objId[gyUnderFoot][gax] != master))
						return TRUE;
				}
			}
		}
		break;
	}
	return FALSE;
}

BOOL removeEnvironmentBlock(Region *environment, double x, double y)
{
	return FALSE;
	if (environment == NULL) return FALSE;
	int fX = (int)floor(x);
	int fY = (int)floor(y);
	if (fX < 0 || fX >= environment->width || fY < 0 || fY >= environment->height)
		return FALSE;
	if (environment->objId[fY][fX] == -1)
	{
		environment->appearance[fY][fX] = ' ';
		environment->blocked[fY][fX] = 0;
	}
	return TRUE;
}

BOOL registerEnvironmentObject(Region *environment, int objId)
{
	if (environment == NULL) return FALSE;
	if (gameObject[objId].sprite == NULL) return FALSE;
	if (gameObject[objId].isBackground) return TRUE;
	int gax, gay;
	int lx, ly;
	int topLeftgx = (int)floor(gameObject[objId].x) - (int)floor(gameObject[objId].sprite->center->x);
	int topLeftgy = (int)floor(gameObject[objId].y) - (int)floor(gameObject[objId].sprite->center->y);
	int fdimx = (int)floor(gameObject[objId].sprite->dimension->x);
	int fdimy = (int)floor(gameObject[objId].sprite->dimension->y);
	for (ly = 0, gay = topLeftgy; ly < fdimy; gay++, ly++)
	{
		for (lx = 0, gax = topLeftgx; lx < fdimx; gax++, lx++)
		{
			if (gameObject[objId].sprite->solid[ly][lx] > 0)
			{
				if (gax >= 0 && gax < environment->width && gay >= 0 && gay < environment->height)
				{
					if ((!environment->blocked[gay][gax]) && (environment->objId[gay][gax] == -1))
					{
						environment->blocked[gay][gax] = TRUE;
						environment->objId[gay][gax] = objId;
					}
					else {
						return FALSE;
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL removeEnvironmentObject(Region *environment, int objId, double oldX, double oldY, CharacterImage *oldImage)
{
	if (environment == NULL) return FALSE;
	if (oldImage == NULL) return FALSE;
	if (gameObject[objId].isBackground) return TRUE;
	int gax, gay;
	int lx, ly;
	int topLeftgx = (int)floor(oldX) - (int)floor(oldImage->center->x);
	int topLeftgy = (int)floor(oldY) - (int)floor(oldImage->center->y);
	int fdimx = (int)floor(oldImage->dimension->x);
	int fdimy = (int)floor(oldImage->dimension->y);
	for (ly = 0, gay = topLeftgy; ly < fdimy; gay++, ly++)
	{
		for (lx = 0, gax = topLeftgx; lx < fdimx; gax++, lx++)
		{
			if (oldImage->solid[ly][lx] > 0)
			{
				if (gax >= 0 && gax < environment->width && gay >= 0 && gay < environment->height)
				{
					if ((environment->blocked[gay][gax]) && (environment->objId[gay][gax] == objId))
					{
						environment->blocked[gay][gax] = FALSE;
						environment->objId[gay][gax] = -1;
					}
				}
			}
		}
	}
	return TRUE;
}

// 0 fail 1 now not flying 2 now flying
int setObjectFlyingState(int objId, BOOL enableFlying, BOOL stableFlight)
{
	if (objId == -1) return 0;
	gameObject[objId].underGravity = (!enableFlying);
	gameObject[objId].fixedFlight = stableFlight;
	return (1 + enableFlying);
}

void doInitialSpawn(Region *target) {
	for (int i=0; i<target->numSpawns; i++) {
		if (target->spawns[i]->mob == LIFE_GRASS) {
			for (int j=0; j<target->spawns[i]->width; j++) {
				if (rand() % 5 == 0) {
					int tempID = createObject(target, -1, LIFE_GRASS, target->spawns[i]->x+j, target->spawns[i]->y);
					if (tempID != -1) {
						target->spawns[i]->currMobSpawned++;
						gameObject[tempID].spawnRegionCount = &(target->spawns[i]->currMobSpawned);
					}
				}
			}
		} else {
			for (int j=0; j<target->spawns[i]->initial; j++) {
				if (target->spawns[i]->currMobSpawned < target->spawns[i]->max) {
					int tempID = createObject(target, -1, target->spawns[i]->mob, target->spawns[i]->x+getRandomOfRange(target->spawns[i]->width), target->spawns[i]->y+getRandomOfRange(target->spawns[i]->height));
					if (tempID != -1) {
						target->spawns[i]->currMobSpawned++;
						gameObject[tempID].spawnRegionCount = &(target->spawns[i]->currMobSpawned);
					}
				}
			}
		}
	}
}

void spawnCheck(Region *target) {
	for (int i=0; i<target->numSpawns; i++) {
		if (target->spawns[i]->currMobSpawned < target->spawns[i]->max) {
			if (((double)rand())/RAND_MAX < target->spawns[i]->chance) {
				// spawns
				int tempID = createObject(target, -1, target->spawns[i]->mob, target->spawns[i]->x+getRandomOfRange(target->spawns[i]->width), target->spawns[i]->y+getRandomOfRange(target->spawns[i]->height));
				if (tempID != -1) {
					target->spawns[i]->currMobSpawned++;
					gameObject[tempID].spawnRegionCount = &(target->spawns[i]->currMobSpawned);
				}
			}
		}
	}
}
