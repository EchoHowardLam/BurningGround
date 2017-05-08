#include "ai.h"

void aiRun(Region *environment, int playerId) {
	
	if (environment == NULL) return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		switch (gameObject[i].type) {
			case LIFE_HUMANOID:
				if (i != playerId)
				{
					if (gameObject[i].attri == HUMANOID_TYPE_CORRUPTED_WINGMAN) {
						double dx, dy;
						dx = gameObject[playerId].x - gameObject[i].x;
						dy = gameObject[playerId].y - gameObject[i].y;
						if (fabs(dx) < 40 && fabs(dy) < 40) {
							if (rand() % 150 == 0)
							createObjectMagicProjectile(environment, i, MAGIC_BLOB, gameObject[i].x, gameObject[i].y, gameObject[playerId].x, gameObject[playerId].y, 0.2, -1, SPHERE_FIRE, ENCHANT_SHRAPNEL, DMG_STANDARD_DEMON_DAMAGE);
							if (fabs(dx) < 10 && fabs(dy) < 10) {
								controlObjectX(i, floor(gameObject[i].x) - dx / 10 + 0.5, 0.2);
								controlObjectY(i, floor(gameObject[i].y) - dy / 10 + 0.5, 0.2);
							} else {
								double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
								double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
								controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.2);
								controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.2);
							}
						} else {
							double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
							double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
							controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.1);
							controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.1);
						}
					} else {
						double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
						double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
						controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.15); // + 0.5 is compulsory as it is the center of a grid
						controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.15);
					}
				}
				break;
			case LIFE_EYEBALL: {
				if (gameObject[i].attri != 0)
				{
					int direction;
					if (gameObject[i].attri > 0)
						direction = -1;
					else
						direction = 1;
					double dx, dy;
					gameObject[i].attri += (direction * 2);
					gameObject[i].attri2 = (gameObject[i].attri2 + direction * 2) % 3600;
					dx = cos((gameObject[i].attri2 / 10.0 / 180.0) * M_PI);
					dy = sin((gameObject[i].attri2 / 10.0 / 180.0) * M_PI);
					if (gameObject[i].sprite->charaID < 1)
						createObjectMagicProjectileDir(environment, i, MAGIC_LASER, gameObject[i].x, gameObject[i].y, dx, dy, 0.0, -1, SPHERE_ICE, 0, DMG_STANDARD_ICELASER_DAMAGE);
					else
						createObjectMagicProjectileDir(environment, i, MAGIC_LASER, gameObject[i].x, gameObject[i].y, dx, dy, 0.0, -1, SPHERE_FIRE, 0, DMG_STANDARD_FIRELASER_DAMAGE);
				}
				else if (fabs(gameObject[playerId].x - gameObject[i].x) > 200 ||
						 fabs(gameObject[playerId].y - gameObject[i].y) > 200) {
					double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
					double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
					controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.2);
					controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.2);
				}
				else if (fabs(gameObject[playerId].x - gameObject[i].x) > 20 ||
						 fabs(gameObject[playerId].y - gameObject[i].y) > 20) {
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
					if (rand() % 300 == 0)
					{
						gameObject[i].attri = 450; // scan for 45 deg
						int direction = (rand() % 2 ? -1 : 1);
						gameObject[i].attri *= direction;
						int angle = (int)(atan((gameObject[playerId].y - gameObject[i].y) / (gameObject[playerId].x - gameObject[i].x)) / M_PI * 180);
						if (gameObject[playerId].x - gameObject[i].x < 0.0)
							angle = ((angle + 360 + 180) % 360);
						else
							angle = ((angle + 360) % 360);
						gameObject[i].attri2 = (angle * 10 + 225 * direction);
					}
					else {
						double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
						double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
						controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.2);
						controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.2);
					}
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
				if (gameObject[i].attri != 0) // hiding
				{
					if ((fabs(gameObject[playerId].x - gameObject[i].x) < 20 && // Ambush!
						 fabs(gameObject[playerId].x - gameObject[i].x) > 2 && // Ambush!
						 fabs(gameObject[playerId].y - gameObject[i].y) <= 15) || // Ambush!
						 (rand() % 2000 == 0)) { // Poke the head around
						if (checkObjectSubmergedInGround(environment, i))
							controlObjectY(i, floor(gameObject[i].y) - 0.5, 0.1);
						else {
							gameObject[i].attri = 0;
							gameObject[i].submergeGround = FALSE;
						}
					}
				}
				else if (gameObject[i].attri == 0) { // not hiding
					if (fabs(gameObject[playerId].x - gameObject[i].x) < 80 &&
						fabs(gameObject[playerId].x - gameObject[i].x) > 2 &&
						fabs(gameObject[playerId].y - gameObject[i].y) <= 30) {
						if (gameObject[playerId].y + 0.5 < gameObject[i].y) {
							if (rand() % 3)
								controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.2);
							if (rand() % 2)
								controlObjectY(i, floor(gameObject[i].y) - 0.5, 0.1);
						}
						else {
							controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.1); // + 0.5 is compulsory as it is the center of a grid
						}
					}
					else if (fabs(gameObject[playerId].x - gameObject[i].x) <= 2 &&
						fabs(gameObject[playerId].y - gameObject[i].y) <= 2) {
						deleteObject(environment, i, FALSE);
					}
					else if (rand() % 1000 == 0) {
						gameObject[i].attri = 1; // Back to hiding
						gameObject[i].submergeGround = TRUE;
						controlObjectY(i, floor(gameObject[i].y) + 2.5, 0.1);
					}
					else {
						if (rand() % 1000 == 0)
							gameObject[i].attri2 = (rand() % 3) - 1;
						controlObjectX(i, floor(gameObject[i].x) + gameObject[i].attri2 + 0.5, 0.01);
						if (rand() % 800 == 0) controlObjectY(i, floor(gameObject[i].y) - 0.5, 0.08);
					}
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
				else {
					if (rand() % 1000 == 0)
						gameObject[i].attri2 = (rand() % 3) - 1;
					controlObjectX(i, floor(gameObject[i].x) + gameObject[i].attri2 + 0.5, 0.01);
					if (rand() % 800 == 0) controlObjectY(i, floor(gameObject[i].y) - 0.5, 0.04);
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
				{
					if (rand() % 500 == 0)
						createObjectMagicProjectile(environment, i, MAGIC_BLOB, gameObject[i].x, gameObject[i].y, gameObject[playerId].x, gameObject[playerId].y, 0.2, -1, SPHERE_EARTH, ENCHANT_SLOW | ENCHANT_STUN | ENCHANT_ENTANGLE | ENCHANT_SILENT, DMG_STANDARD_DIRTBALL_DAMAGE);
				}
				else {
					if (rand() % 1000 == 0)
						gameObject[i].attri2 = (rand() % 3) - 1;
					controlObjectX(i, floor(gameObject[i].x) + gameObject[i].attri2 + 0.5, 0.01);
					if (rand() % 800 == 0) controlObjectY(i, floor(gameObject[i].y) - 0.5, 0.04);
				}
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
				if (fabs(gameObject[playerId].x - gameObject[i].x) < 5 && fabs(gameObject[playerId].y - gameObject[i].y) < 10)
				{
					if (rand() % 50 == 0) {
						createObject(environment, i, LIFE_DURIAN, gameObject[i].x + getRandomOfRange(10), gameObject[i].y - 10);
					}
				}
				else if (rand() % 500 == 0) {
					createObject(environment, i, LIFE_DURIAN, gameObject[i].x + getRandomOfRange(10), gameObject[i].y-10);
				}
				break;
			}
			case PROFESSOR_HTRAHDIS:
			{
				double dx, dy;
				dx = gameObject[playerId].x - gameObject[i].x;
				dy = gameObject[playerId].y - gameObject[i].y;
				if (fabs(dx) >= 40 || fabs(dy) >= 60) {
					if (rand() % 10 < 7)
						controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.4);
					else if (rand() % 10 < 3)
						controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? 1.5 : -1.5), 0.4);
					if (rand() % 10 < 7)
						controlObjectY(i, floor(gameObject[i].y) + ((gameObject[playerId].y - 1.0 - gameObject[i].y<0) ? -1.5 : 1.5), 0.4);
					else if (rand() % 10 < 3)
						controlObjectY(i, floor(gameObject[i].y) + ((gameObject[playerId].y - 1.0 - gameObject[i].y<0) ? 1.5 : -1.5), 0.4);
				}
				else if (fabs(dx) >= 20 || fabs(dy) >= 20) {
					if (rand() % 150 == 0)
						createObjectMagicProjectile(environment, i, MAGIC_BLOB, gameObject[i].x, gameObject[i].y, gameObject[playerId].x, gameObject[playerId].y, 0.2, -1, SPHERE_FIRE, ENCHANT_SHRAPNEL, DMG_STANDARD_DEMON_DAMAGE);
					if (fabs(dx) < 10 && fabs(dy) < 10) {
						controlObjectX(i, floor(gameObject[i].x) - dx / 10 + 0.5, 0.2);
						controlObjectY(i, floor(gameObject[i].y) - dy / 10 + 0.5, 0.2);
					}
					else {
						double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
						double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
						controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.2);
						controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.2);
					}
				}
				else {
					controlObjectX(i, floor(gameObject[i].x) + ((gameObject[playerId].x - gameObject[i].x<0) ? -1.5 : 1.5), 0.2);
					controlObjectY(i, floor(gameObject[i].y) + ((gameObject[playerId].y - gameObject[i].y<0) ? -1.5 : 1.5), 0.2);
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
