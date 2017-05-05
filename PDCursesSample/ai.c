#include "ai.h"

void aiRun(Region *environment) {
	
	if (environment == NULL) return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		switch (gameObject[i].type) {
			case LIFE_EYEBALL: {
				gameObject[i].vel.x += ((double) rand())/RAND_MAX/20*((rand()%2==0)?1:-1);
				gameObject[i].vel.y += ((double) rand())/RAND_MAX/20*((rand()%2==0)?1:-1);
				break;
			}
			default:
				break;
		}
	}
}
