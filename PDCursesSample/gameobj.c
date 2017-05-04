#include "gameobj.h"

GameObject gameObject[MAX_OBJECT];

void initializeObjects(void)
{
	for (int i = 0; i < MAX_OBJECT; i++)
		gameObject[i].type = NOTHING;
	return;
}

int createObject(Region *environment, int master, ObjectType type, double startX, double startY)
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
		case LIFE_HUMANOID:
			gameObject[i].underGravity = TRUE;
			break;
		default:
			gameObject[i].underGravity = FALSE;
			break;
		}
		gameObject[i].facingDir = 1;
		gameObject[i].sprite = NULL;
		gameObject[i].master = master;
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
		gameObject[i].facingDir = 1;
		gameObject[i].sprite = NULL;
		gameObject[i].master = master;
		return defaultObjectsInit(environment, i);
	}
	return -1;
}

int createObjectProjectileDest(Region *environment, int master, ObjectType type, double startX, double startY, double destX, double destY, double speed, int lifespan, int destroyCriteria, BOOL underGravity)
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
		gameObject[i].facingDir = 1;
		gameObject[i].sprite = NULL;
		gameObject[i].master = master;
		return defaultObjectsInit(environment, i);
	}
	return -1;
}

// -1 fail, otherwise return objId
int defaultObjectsInit(Region *environment, int objId)
{
	if (environment == NULL) return;
	if (gameObject[objId].type == NOTHING) return;
	switch (gameObject[objId].type)
	{
	case LIFE_HUMANOID:
		gameObject[objId].sprite = getImage(LIFE_HUMANOID, 6 | (gameObject[objId].facingDir & 1));
		if (!registerEnvironmentObject(environment, objId)) return -1;
		break;
	default:
		break;
	}
	return objId;
}

void deleteObject(Region *environment, int id, BOOL silentDelete)
{
	if (environment == NULL) return;
	if (!silentDelete)
	{
		switch (gameObject[id].type)
		{
		case BOMB:
		{
			// spawn fragments when bombs are destroyed!
			double dirX, dirY, shrapnelV = 0.9;
			removeEnvironmentBlock(environment, gameObject[id].x, gameObject[id].y);
			for (int k = 0; k < 4; k++)
			{
				dirX = rand() % 101 - 50;
				dirY = rand() % 101 - 50;
				// conservation of momentum -> there must be a shrapnel going in the opposite direction
				createObjectProjectileDir(environment, -1, FRAGMENT, gameObject[id].x, gameObject[id].y, dirX, dirY, shrapnelV, 30, 0, TRUE);
				createObjectProjectileDir(environment, -1, FRAGMENT, gameObject[id].x, gameObject[id].y, -dirX, -dirY, shrapnelV, 30, 0, TRUE);
				removeEnvironmentBlock(environment, gameObject[id].x + DIRECTION2X[k], gameObject[id].y + DIRECTION2Y[k]);
				removeEnvironmentBlock(environment, gameObject[id].x + DIAGONALX[k], gameObject[id].y + DIAGONALY[k]);
			}
			break;
		}
		default:
			break;
		}
	}
	removeEnvironmentObject(environment, id, gameObject[id].x, gameObject[id].y, gameObject[id].sprite);
	gameObject[id].type = NOTHING;		// destroy it!
	return;
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
			double tmp;
			switch (gameObject[i].type)
			{
			case LIFE_HUMANOID:
				{
					if (gameObject[i].sprite == NULL) break;
					int grx, gry;
					int lx, ly;
					int fdimx = (int)floor(gameObject[i].sprite->dimension->x);
					int fdimy = (int)floor(gameObject[i].sprite->dimension->y);
					int fcolor;
					for (ly = 0, gry = -(int)floor(gameObject[i].sprite->center->y); ly < fdimy; gry++, ly++)
					{
						for (lx = 0, grx = -(int)floor(gameObject[i].sprite->center->x); lx < fdimx; grx++, lx++)
						{
							if (gameObject[i].sprite->solid[ly][lx] > 0)
							{
								if (move(screenY + gry, screenX + grx) != ERR)
								{
									fcolor = gameObject[i].sprite->color[ly][lx];
									attron(fcolor);
									addch(gameObject[i].sprite->display[ly][lx]);
									attroff(fcolor);
								}
							}
						}
					}
				}
				break;
			case BULLET:
				attron(COLOR_PAIR(COLOR_WHITE));
				addch('*');
				attroff(COLOR_PAIR(COLOR_WHITE));
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
			default:
				break;
			}
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

		BOOL on_feet = FALSE;
		if (gameObject[i].underGravity)
		{
			gameObject[i].vel.y += 0.01;
			if ((gameObject[i].vel.y > 0.001) && checkObjectOnFeet(environment, i)) // friction and normal force only works if you press it against the surface
			{
				gameObject[i].y = floor(gameObject[i].y) + 0.8;
				on_feet = TRUE;
				gameObject[i].vel.x = 0.0; // friction coefficient is infinite
				gameObject[i].vel.y = 0.0; // absorb all downward momentum
			}
		}

		if (gameObject[i].underMove)
		{
			// exception of physics to make player easier to control the character in flight
			if (gameObject[i].underGravity && (!on_feet))
			{
				if (fabs(gameObject[i].vel.x + gameObject[i].motiveVel.x * 0.8) <= fabs(gameObject[i].motiveVel.x * 0.8))
					gameObject[i].vel.x += gameObject[i].motiveVel.x * 0.8;
			}

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
			if (gameObject[i].motiveVel.x < 0.5)
				gameObject[i].motiveVel.x = 0.0;
			else
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

			double adjustedVX = gameObject[i].vel.x;
			double adjustedVY = gameObject[i].vel.y;

			double newX = gameObject[i].x + adjustedVX;
			double newY = gameObject[i].y + adjustedVY;
			int fnewX = (int)floor(newX);
			int fnewY = (int)floor(newY);
			//gameObject[i].vel.x = adjustedVX;
			//gameObject[i].vel.y = adjustedVY;
			if (fnewX < 0 || fnewX >= environment->width || fnewY < 0 || fnewY >= environment->height)
			{
				toDelete = TRUE;
				silentDelete = TRUE;
			}
			else if (checkObjectCollision(environment, i, newX, newY))
			{
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
				gameObject[i].sprite = getImage(LIFE_HUMANOID, 6 | ((gameObject[i].facingDir & 1) ^ 1));
				if (!checkObjectCollision(environment, i, gameObject[i].x, gameObject[i].y))
				{
					removeEnvironmentObject(environment, i, gameObject[i].x, gameObject[i].y, oldImage);
					registerEnvironmentObject(environment, i);
					gameObject[i].facingDir ^= (TURNING_UNSETTLED | 1);
				}
				else {
					gameObject[i].sprite = oldImage;
					gameObject[i].facingDir ^= TURNING_UNSETTLED;
				}
			}
			break;
		default:
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
		switch (gameObject[i].type)
		{
		case LIFE_HUMANOID:
			{
				CharacterImage *oldImage = gameObject[i].sprite;
				CharacterImage *newImage = getImage(LIFE_HUMANOID, 6 | (gameObject[i].facingDir & 1));
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
			}
			break;
		case BULLET:
			break;
		case BOMB:
			break;
		case FRAGMENT:
			break;
		default:
			break;
		}
		gameObject[i].turnsAlive++;
	}
	return;
}

BOOL checkObjectCollision(Region *environment, int objId, double x, double y)
{
	if (environment == NULL) return FALSE;
	if (gameObject[objId].type == NOTHING) return FALSE;
	double tmp;
	int master = gameObject[objId].master;
	if (master == -1) master = -2;
	switch (gameObject[objId].type)
	{
	case LIFE_HUMANOID:
		{
			if (gameObject[objId].sprite == NULL) return FALSE;
			int gax, gay;
			int lx, ly;
			int topLeftgx = (int)floor(x) - (int)floor(gameObject[objId].sprite->center->x);
			int topLeftgy = (int)floor(y) - (int)floor(gameObject[objId].sprite->center->y);
			int fdimx = (int)floor(gameObject[objId].sprite->dimension->x);
			int fdimy = (int)floor(gameObject[objId].sprite->dimension->y);
			int fcolor;
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
	case BULLET:
		if (environment->blocked[(int)floor(y)][(int)floor(x)] && (environment->objId[(int)floor(y)][(int)floor(x)] != master))
		{
			return TRUE;
		}
		break;
	case BOMB:
		if (environment->blocked[(int)floor(y)][(int)floor(x)] && (environment->objId[(int)floor(y)][(int)floor(x)] != master))
		{
			return TRUE;
		}
		break;
	case FRAGMENT:
		if (environment->blocked[(int)floor(y)][(int)floor(x)] && (environment->objId[(int)floor(y)][(int)floor(x)] != master))
		{
			return TRUE;
		}
		/*tmp = (gameObject[objId].y - floor(gameObject[objId].y));
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
		}*/
		break;
	default:
		break;
	}
	return FALSE;
}

BOOL checkObjectOnFeet(Region *environment, int objId)
{
	if (environment == NULL) return FALSE;
	if (gameObject[objId].type == NOTHING) return FALSE;
	int master = gameObject[objId].master;
	if (master == -1) master = -2;
	switch (gameObject[objId].type)
	{
	// add special cases that has no sprite
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
	if (environment == NULL) return FALSE;
	int fX = (int)floor(x);
	int fY = (int)floor(y);
	if (fX < 0 || fX >= environment->width || fY < 0 || fY >= environment->height)
		return FALSE;
	environment->appearance[fY][fX] = ' ';
	environment->blocked[fY][fX] = 0;
	return TRUE;
}

BOOL registerEnvironmentObject(Region *environment, int objId)
{
	if (environment == NULL) return FALSE;
	if (gameObject[objId].sprite == NULL) return FALSE;
	int gax, gay;
	int lx, ly;
	int topLeftgx = (int)floor(gameObject[objId].x) - (int)floor(gameObject[objId].sprite->center->x);
	int topLeftgy = (int)floor(gameObject[objId].y) - (int)floor(gameObject[objId].sprite->center->y);
	int fdimx = (int)floor(gameObject[objId].sprite->dimension->x);
	int fdimy = (int)floor(gameObject[objId].sprite->dimension->y);
	int fcolor;
	for (ly = 0, gay = topLeftgy; ly < fdimy; gay++, ly++)
	{
		for (lx = 0, gax = topLeftgx; lx < fdimx; gax++, lx++)
		{
			if (gameObject[objId].sprite->solid[ly][lx] > 0)
			{
				if (gax >= 0 && gax < environment->width && gay >= 0 && gay < environment->height)
				{
					if (environment->blocked[gay][gax] == FALSE)
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
	int gax, gay;
	int lx, ly;
	int topLeftgx = (int)floor(oldX) - (int)floor(oldImage->center->x);
	int topLeftgy = (int)floor(oldY) - (int)floor(oldImage->center->y);
	int fdimx = (int)floor(oldImage->dimension->x);
	int fdimy = (int)floor(oldImage->dimension->y);
	int fcolor;
	for (ly = 0, gay = topLeftgy; ly < fdimy; gay++, ly++)
	{
		for (lx = 0, gax = topLeftgx; lx < fdimx; gax++, lx++)
		{
			if (oldImage->solid[ly][lx] > 0)
			{
				if (gax >= 0 && gax < environment->width && gay >= 0 && gay < environment->height)
				{
					if (environment->objId[gay][gax] == objId)
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
