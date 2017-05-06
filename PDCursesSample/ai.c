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
					createObjectMagicProjectileDir(environment, i, MAGIC_LASER, gameObject[i].x, gameObject[i].y, dx, dy, 0.0, -1, SPHERE_ICE, 0);
					//createObjectMagicProjectile(environment, i, MAGIC_LASER, gameObject[i].x, gameObject[i].y, gameObject[playerId].x, gameObject[playerId].y, 0.0, -1, SPHERE_ICE, 0);
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
						controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.2);
					else if (rand() % 10 < 3)
						controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? 1.5 : -1.5), 0.2);
					if (rand() % 10 < 3)
						controlObjectY(i, floor(gameObject[i].y) + ((gameObject[playerId].y - 1.0 - gameObject[i].y<0) ? -1.5 : 1.5), 0.2);
					else if (rand() % 10 < 3)
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
			case LIFE_RABBIT: {
				if (fabs(gameObject[playerId].x-gameObject[i].x) < 40 &&
					fabs(gameObject[playerId].x-gameObject[i].x) > 6 &&
					fabs(gameObject[playerId].y-gameObject[i].y) <= 2) {
					controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x-gameObject[i].x<0)?-1.5:1.5), 0.1); // + 0.5 is compulsory as it is the center of a grid
				} else if (fabs(gameObject[playerId].x-gameObject[i].x) <= 6 &&
						   fabs(gameObject[playerId].y-gameObject[i].y) <= 2) {
					deleteObject(environment, i, FALSE);
				}
				break;
			}
			case LIFE_SLUDGE: {
				if (fabs(gameObject[playerId].x-gameObject[i].x) < 30 &&
				fabs(gameObject[playerId].x-gameObject[i].x) > 0 &&
				fabs(gameObject[playerId].y-gameObject[i].y) <= 30) {
					controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.04); // + 0.5 is compulsory as it is the center of a grid
					if (rand() % 400 == 0 || (gameObject[playerId].y + 1.0 < gameObject[i].y)) {
						controlObjectY(i, floor(gameObject[i].y) - 0.5, 0.1);
					}
					if (rand() % 2000 == 0)
						createObjectMagicProjectile(environment, i, MAGIC_BLOB, gameObject[i].x, gameObject[i].y, gameObject[playerId].x, gameObject[playerId].y, 1.0, -1, SPHERE_EARTH, ENCHANT_SLOW | ENCHANT_ENTANGLE | ENCHANT_SILENT);
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
