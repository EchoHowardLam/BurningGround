#include "ai.h"

void aiRun(Region *environment, int playerId) {
	
	if (environment == NULL) return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		switch (gameObject[i].type) {
			case LIFE_HUMANOID:
				if (i != playerId)
				{
					double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
					double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
					controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.15); // + 0.5 is compulsory as it is the center of a grid
					controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.15);
				}
				break;
			case LIFE_EYEBALL: {
				double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
				double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
				if (gameObject[i].attri <= 0)
				{
					if (rand() % 10000 == 0)
					{
						gameObject[i].attri = 3600;
						gameObject[i].attri2 = rand() % 3600;
					}
				}
				if (gameObject[i].attri > 0)
				{
					gameObject[i].attri--;
					gameObject[i].attri2 = (++gameObject[i].attri2) % 3600;
					dx = cos((gameObject[i].attri2 / 10.0 / 180.0) * M_PI);
					dy = sin((gameObject[i].attri2 / 10.0 / 180.0) * M_PI);
					createObjectMagicProjectileDir(environment, i, MAGIC_LASER, gameObject[i].x, gameObject[i].y, dx, dy, 0.0, -1, SPHERE_ICE, 0, DMG_STANDARD_ICELASER_DAMAGE);
				}
				else {
					controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.2); // + 0.5 is compulsory as it is the center of a grid
					controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.2);
				}
				break;
			}
			case LIFE_MOSQUITOES: {
				if (fabs(gameObject[playerId].x - gameObject[i].x) < 30 && fabs(gameObject[playerId].y - gameObject[i].y) <= 30)
				{
					if (rand() % 10 < 3)
						controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.15);
					else if (rand() % 10 < 3)
						controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? 1.5 : -1.5), 0.15);
					if (rand() % 10 < 3)
						controlObjectY(i, floor(gameObject[i].y) + ((gameObject[playerId].y - 1.0 - gameObject[i].y<0) ? -1.5 : 1.5), 0.15);
					else if (rand() % 10 < 3)
						controlObjectY(i, floor(gameObject[i].y) + ((gameObject[playerId].y - 1.0 - gameObject[i].y<0) ? 1.5 : -1.5), 0.15);
				}
				else {
					double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
					double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
					controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.15); // + 0.5 is compulsory as it is the center of a grid
					controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.15);
				}
				break;
			}
			case LIFE_RABBIT: {
				if (fabs(gameObject[playerId].x-gameObject[i].x) < 40 &&
					fabs(gameObject[playerId].x-gameObject[i].x) > 2 &&
					fabs(gameObject[playerId].y-gameObject[i].y) <= 2) {
					controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x-gameObject[i].x<0)?-1.5:1.5), 0.1); // + 0.5 is compulsory as it is the center of a grid
				} else if (fabs(gameObject[playerId].x-gameObject[i].x) <= 2 &&
						   fabs(gameObject[playerId].y-gameObject[i].y) <= 2) {
					deleteObject(environment, i, FALSE);
				}
				break;
			}
			case LIFE_SLIME: {
				if (fabs(gameObject[playerId].x-gameObject[i].x) < 30 &&
					fabs(gameObject[playerId].y-gameObject[i].y) < 30) {
					controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.04); // + 0.5 is compulsory as it is the center of a grid
					if (rand() % 400 == 0 || (gameObject[playerId].y + 1.0 < gameObject[i].y)) {
						controlObjectY(i, floor(gameObject[i].y) - 0.5, 0.1);
					}
				}
				break;
			}
			case LIFE_SLUDGE: {
				if (fabs(gameObject[playerId].x-gameObject[i].x) < 30 &&
					fabs(gameObject[playerId].y-gameObject[i].y) < 30) {
					controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.04); // + 0.5 is compulsory as it is the center of a grid
					if (rand() % 400 == 0 || (gameObject[playerId].y + 1.0 < gameObject[i].y)) {
						controlObjectY(i, floor(gameObject[i].y) - 0.5, 0.08);
					}
				}
				if (fabs(gameObject[playerId].x - gameObject[i].x) < 50 && fabs(gameObject[playerId].y - gameObject[i].y) < 100)
					if (rand() % 500 == 0)
						createObjectMagicProjectile(environment, i, MAGIC_BLOB, gameObject[i].x, gameObject[i].y, gameObject[playerId].x, gameObject[playerId].y, 0.2, -1, SPHERE_EARTH, ENCHANT_SLOW | ENCHANT_STUN | ENCHANT_ENTANGLE | ENCHANT_SILENT, DMG_STANDARD_DIRTBALL_DAMAGE);
				break;
			}
			case SPAWN_BEE_HIVE: {
				if (fabs(gameObject[playerId].x - gameObject[i].x) < 50 && fabs(gameObject[playerId].y - gameObject[i].y) <= 50 && rand() % 100 == 0)
					createObject(environment, -1, LIFE_BEE, gameObject[i].x + getRandomOfRange(3), gameObject[i].y + getRandomOfRange(3));
				break;
			}
			case LIFE_BEE: {
				if (fabs(gameObject[playerId].x - gameObject[i].x) < 40 && fabs(gameObject[playerId].y - gameObject[i].y) < 40)
				{
					if (rand() % 2)
						controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.2);
					else if (rand() % 3)
						controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? 1.5 : -1.5), 0.2);
					if (rand() % 2)
						controlObjectY(i, floor(gameObject[i].y) + ((gameObject[playerId].y - 1.0 - gameObject[i].y<0) ? -1.5 : 1.5), 0.2);
					else if (rand() % 3)
						controlObjectY(i, floor(gameObject[i].y) + ((gameObject[playerId].y - 1.0 - gameObject[i].y<0) ? 1.5 : -1.5), 0.2);
				}
				else {
					double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
					double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
					controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.2); // + 0.5 is compulsory as it is the center of a grid
					controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.2);
				}
				break;
			}
			case SPAWN_DURIAN_TREE: {
				if (rand() % 200 == 0) {
					createObject(environment, i, LIFE_DURIAN, gameObject[i].x + getRandomOfRange(10), gameObject[i].y-10);
				}
				break;
			}
			case MIST: {
				double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
				double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
				controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.2); // + 0.5 is compulsory as it is the center of a grid
				controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.2);
				break;
			}
			default:
				break;
		}
	}
}
