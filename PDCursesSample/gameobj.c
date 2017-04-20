#include "gameobj.h"

GameObject gameObject[100];

int createObject(ObjectType type, double startX, double startY)
{
	for (int i = MAX_OBJECT - 1; i >= 0; i--) {
		if (gameObject[i].type != NOTHING)
			continue;		// if not empty, try next

		gameObject[i].type = type;
		gameObject[i].x = startX;
		gameObject[i].y = startY;
		gameObject[i].dispX = 0.0;
		gameObject[i].dispY = 0.0;
		gameObject[i].speed.x = 0.0;
		gameObject[i].speed.y = 0.0;
		gameObject[i].turnsAlive = 0;
		gameObject[i].lifespan = -1;
		gameObject[i].destroyCriteria = 0;
		gameObject[i].holdPos = FALSE;
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

int createObjectProjectileDest(ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int destroyCriteria, BOOL holdPos, BOOL underGravity)
{
	for (int i = MAX_OBJECT - 1; i >= 0; i--) {
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
			gameObject[i].speed.x = 0.0;
			gameObject[i].speed.y = 0.0;
		}
		else {
			gameObject[i].speed.x = gameObject[i].dispX / disp * speed;
			gameObject[i].speed.y = gameObject[i].dispY / disp * speed;
		}
		gameObject[i].turnsAlive = 0;
		gameObject[i].lifespan = lifespan;
		gameObject[i].destroyCriteria = destroyCriteria;
		gameObject[i].holdPos = holdPos;
		gameObject[i].underGravity = underGravity;
		return i;
	}
	return -1;
}

void displayObjects(Coordinate scrTopLeftPos, int scrW, int scrH)
{
	for (int i = 0; i < MAX_OBJECT; i++) {
		if (gameObject[i].type == NOTHING)
			continue;

		int screenX = (int)floor(gameObject[i].x - scrTopLeftPos.x);
		int screenY = (int)floor(gameObject[i].y - scrTopLeftPos.y);

		if (move(screenY, screenX) != ERR) {
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
			}
		}
	}
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
		gameObject[id].speed.x += dirX / disp * speed;
		gameObject[id].speed.y += dirY / disp * speed;
	}
	speed = gameObject[id].speed.x * gameObject[id].speed.x + gameObject[id].speed.y * gameObject[id].speed.y;
	if (speed > 1.0)
	{
		speed = sqrt(speed);
		gameObject[id].speed.x /= speed;
		gameObject[id].speed.y /= speed;
	}
	gameObject[id].holdPos = FALSE;
}

void pushObjectDest(int id, double destX, double destY, double speed, BOOL holdPos)
{
	if (gameObject[id].type == NOTHING)
		return;

	gameObject[id].dispX = destX - gameObject[id].x;
	gameObject[id].dispY = destY - gameObject[id].y;
	double disp = sqrt(gameObject[id].dispX * gameObject[id].dispX + gameObject[id].dispY * gameObject[id].dispY);
	if (fabs(disp) > 0.01)
	{
		gameObject[id].speed.x += gameObject[id].dispX / disp * speed;
		gameObject[id].speed.y += gameObject[id].dispY / disp * speed;
	}
	speed = gameObject[id].speed.x * gameObject[id].speed.x + gameObject[id].speed.y * gameObject[id].speed.y;
	if (speed > 1.0)
	{
		speed = sqrt(speed);
		gameObject[id].speed.x /= speed;
		gameObject[id].speed.y /= speed;
	}
	gameObject[id].holdPos = holdPos;
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
			double adjustedVX = gameObject[i].speed.x;
			double adjustedVY = gameObject[i].speed.y;
			if (gameObject[i].holdPos) // try to truncate speed to hold position
			{
				if (gameObject[i].dispX > 0 && (gameObject[i].speed.x > gameObject[i].dispX))
					adjustedVX = gameObject[i].dispX;
				else if (gameObject[i].dispX < 0 && (gameObject[i].speed.x < gameObject[i].dispX))
					adjustedVX = gameObject[i].dispX;
				if (gameObject[i].dispY > 0 && (gameObject[i].speed.y > gameObject[i].dispY))
					adjustedVY = gameObject[i].dispY;
				else if (gameObject[i].dispY < 0 && (gameObject[i].speed.y < gameObject[i].dispY))
					adjustedVY = gameObject[i].dispY;
			}

			double newX = gameObject[i].x + adjustedVX;
			double newY = gameObject[i].y + adjustedVY;
			int fnewX = (int)floor(newX);
			int fnewY = (int)floor(newY);
			gameObject[i].speed.x = adjustedVX;
			gameObject[i].speed.y = adjustedVY;
			if (fnewX < 0 || fnewX >= environment->width || fnewY < 0 || fnewY >= environment->height)
				toDelete = TRUE;
			else if (environment->appearance[fnewY][fnewX] == ' ')
			{
				gameObject[i].x = newX;
				gameObject[i].y = newY;
			}
			else {
				gameObject[i].speed.x = 0.0;
				gameObject[i].speed.y = 0.0;
				if (gameObject[i].destroyCriteria & (DESTROY_CRITERIA_HIT | DESTROY_CRITERIA_STOP))
					toDelete = TRUE;
			}

			gameObject[i].dispX -= adjustedVX;
			gameObject[i].dispY -= adjustedVY;

			if (gameObject[i].underGravity)
			{
				gameObject[i].speed.y += 0.02;
				if (environment->appearance[(int)floor(gameObject[i].y) + 1][(int)floor(gameObject[i].x)] != ' ')
				{
					gameObject[i].speed.y -= 0.02;
				}
			}

			double speed = gameObject[i].speed.x * gameObject[i].speed.x + gameObject[i].speed.y * gameObject[i].speed.y;
			if (speed > 1.0)
			{
				speed = sqrt(speed);
				gameObject[i].speed.x /= speed;
				gameObject[i].speed.y /= speed;
			}

			if (gameObject[i].holdPos)
			{
				if ((fabs(gameObject[i].dispX) <= 0.01) && (fabs(gameObject[i].dispY) <= 0.01)) // reached point of stopping
				{
					gameObject[i].holdPos = FALSE;
					gameObject[i].speed.x = 0.0;
					gameObject[i].speed.y = 0.0;
					if (gameObject[i].destroyCriteria & DESTROY_CRITERIA_STOP)
						toDelete = TRUE;
				}
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
				double destX, destY;
				for (int k = 0; k < 4; k++)
				{
					destX = gameObject[i].x + DIAGONALX[k] * 10;
					destY = gameObject[i].y + DIAGONALY[k] * 10;
					int bulletId = createObjectProjectileDest(BULLET, gameObject[i].x, gameObject[i].y, destX, destY, 0.5, 20, DESTROY_CRITERIA_HIT, FALSE, FALSE);
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
