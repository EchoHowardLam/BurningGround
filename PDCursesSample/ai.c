#include "ai.h"

void aiRun(Region *environment) {
	
	if (environment == NULL) return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		switch (gameObject[i].type) {
			case LIFE_EYEBALL: {
				double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
				double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
				controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.2); // + 0.5 is compulsory as it is the center of a grid
				controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.2);
				break;
			}
			case LIFE_MOSQUITOES: {
				double dx = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
				double dy = ((double)rand()) / RAND_MAX * (((rand() % 2 == 0) ? 1 : -1));
				controlObjectX(i, floor(gameObject[i].x) + dx + 0.5, 0.2); // + 0.5 is compulsory as it is the center of a grid
				controlObjectY(i, floor(gameObject[i].y) + dy + 0.5, 0.2);
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
