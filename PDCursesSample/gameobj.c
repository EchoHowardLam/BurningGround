#include "gameobj.h"

GameObject gameObject[MAX_OBJECT];

int createObject(ObjectType type, double startX, double startY)
{
	for (int i = MAX_OBJECT - 1; i >= 0; i--)
	{
		if (gameObject[i].type != NOTHING)
			continue;		// if not empty, try next

		gameObject[i].type = type;
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
		case PLAYER:
			gameObject[i].underGravity = TRUE;
			break;
		default:
			gameObject[i].underGravity = FALSE;
			break;
		}
		return i;
	}
	return -1;
}

int createObjectProjectileDir(ObjectType type, double startX, double startY, double dirX, double dirY, double speed, int lifespan, int destroyCriteria, BOOL underGravity)
{
	for (int i = MAX_OBJECT - 1; i >= 0; i--)
	{
		if (gameObject[i].type != NOTHING)
			continue;		// if not empty, try next

		gameObject[i].type = type;
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
		return i;
	}
	return -1;
}

int createObjectProjectileDest(ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int destroyCriteria, BOOL underGravity)
{
	for (int i = MAX_OBJECT - 1; i >= 0; i--)
	{
		if (gameObject[i].type != NOTHING)
			continue;		// if not empty, try next

		gameObject[i].type = type;
		gameObject[i].x = startX;
		gameObject[i].y = startY;
		gameObject[i].dispX = destX - startX;
		gameObject[i].dispY = destY - startY;
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
		gameObject[i].destroyCriteria = destroyCriteria;
		gameObject[i].underMove = FALSE;
		gameObject[i].underGravity = underGravity;
		return i;
	}
	return -1;
}

void displayObjects(Coordinate scrTopLeftPos, int scrW, int scrH)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (gameObject[i].type == NOTHING)
			continue;

		int screenX = (int)floor(gameObject[i].x - scrTopLeftPos.x);
		int screenY = (int)floor(gameObject[i].y - scrTopLeftPos.y);

		if (move(screenY, screenX) != ERR)
		{
			int tmp;
			switch (gameObject[i].type)
			{
			case PLAYER:
				attron(COLOR_PAIR(2));
				addch('|');
				if (move(screenY - 1, screenX) != ERR) {
					addch(245 | A_ALTCHARSET);
				}
				attroff(COLOR_PAIR(2));
				break;
			case BULLET:
				addch('*');
				break;
			case BOMB:
				addch('@');
				break;
			case FRAGMENT:
				tmp = (gameObject[i].y - floor(gameObject[i].y));
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
				break;
			}
		}
	}
	return;
}

void pushObjectDir(int id, double dirX, double dirY, double speed)
{
	if (gameObject[id].type == NOTHING)
		return;

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
	if (environment == NULL)
		return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (gameObject[i].type == NOTHING)
			continue;

		BOOL on_feet = FALSE;
		if (gameObject[i].underGravity)
		{
			gameObject[i].vel.y += 0.02;
			if ((gameObject[i].vel.y > 0.01) && checkObjectOnFeet(environment, i)) // friction and normal force only works if you press it against the surface
			{
				on_feet = TRUE;
				gameObject[i].vel.x = 0.0; // friction coefficient is infinite
				gameObject[i].vel.y = 0.0; // absorb all downward momentum
			}
		}

		if (gameObject[i].underMove)
		{
			/*if (gameObject[i].underGravity && (!on_feet))
			{
				gameObject[i].vel.x += gameObject[i].motiveVel.x * 0.2;
			}*/
			BOOL cancelMove = (fabs(gameObject[i].dispX) + fabs(gameObject[i].dispY) <= 0.01); // finish moving
			cancelMove |= (gameObject[i].underGravity && (!on_feet)); // unable to move on free will
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
			gameObject[i].motiveVel.x *= 0.95;
			gameObject[i].motiveVel.y = 0.0;
		}
	}
}

void controlObjectX(int id, double destX, double speed)
{
	if (gameObject[id].type == NOTHING)
		return;

	gameObject[id].dispX = destX - gameObject[id].x;
	if (gameObject[id].dispX > 0)
		gameObject[id].motiveVel.x = speed;
	else
		gameObject[id].motiveVel.x = -speed;
	gameObject[id].underMove = TRUE;
	return;
}

void controlObjectY(int id, double destY, double speed)
{
	if (gameObject[id].type == NOTHING)
		return;

	gameObject[id].dispY = destY - gameObject[id].y;
	if (gameObject[id].dispY > 0)
		gameObject[id].motiveVel.y = speed;
	else
		gameObject[id].motiveVel.y = -speed;
	gameObject[id].underMove = TRUE;
	return;
}

void moveObjects(Region *environment)
{
	if (environment == NULL)
		return;
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (gameObject[i].type == NOTHING)
			continue;

		BOOL toDelete = FALSE;
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

			double adjustedVX = gameObject[i].vel.x;
			double adjustedVY = gameObject[i].vel.y;

			double newX = gameObject[i].x + adjustedVX;
			double newY = gameObject[i].y + adjustedVY;
			int fnewX = (int)floor(newX);
			int fnewY = (int)floor(newY);
			//gameObject[i].vel.x = adjustedVX;
			//gameObject[i].vel.y = adjustedVY;
			if (fnewX < 0 || fnewX >= environment->width || fnewY < 0 || fnewY >= environment->height)
				toDelete = TRUE;
			else if (environment->appearance[fnewY][fnewX] == ' ')
			{
				gameObject[i].x = newX;
				gameObject[i].y = newY;
			}
			else {
				//gameObject[i].y = floor(gameObject[i].y) + 0.5;
				gameObject[i].vel.x = 0.0;
				gameObject[i].vel.y = 0.0;
				gameObject[i].underMove = FALSE;
				if (gameObject[i].destroyCriteria & (DESTROY_CRITERIA_HIT | DESTROY_CRITERIA_STOP))
					toDelete = TRUE;
			}
		}
		else {
			toDelete = TRUE;
		}

		if (toDelete)
		{
			switch (gameObject[i].type)
			{
			case BOMB:
			{
				// spawn fragments when bombs are destroyed!
				double dirX, dirY, shrapnelV = 0.9;
				for (int k = 0; k < 4; k++)
				{
					dirX = rand() % 101 - 50;
					dirY = rand() % 101 - 50;
					// conservation of momentum -> there must be a shrapnel going in the opposite direction
					createObjectProjectileDir(FRAGMENT, gameObject[i].x, gameObject[i].y, dirX, dirY, shrapnelV, 30, 0, TRUE);
					createObjectProjectileDir(FRAGMENT, gameObject[i].x, gameObject[i].y, -dirX, -dirY, shrapnelV, 30, 0, TRUE);
				}
				break;
			}
			default:
				break;
			}
			gameObject[i].type = NOTHING;		// destroy it!
		}
		gameObject[i].turnsAlive++;
	}
}

BOOL checkObjectOnFeet(Region *environment, int objId)
{
	if (environment == NULL) return FALSE;
	if (environment->appearance[(int)floor(gameObject[objId].y) + 1][(int)floor(gameObject[objId].x)] != ' ')
		return TRUE;
	return FALSE;
}
