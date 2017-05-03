/**

int ch = getch();
BOOL fired = FALSE;
while (ch != ERR)
{
	if (ch == KEY_UP) { controlObjectY(playerId, floor(gameObject[playerId].y) - 0.5, 0.5); playerFacing = UP; }
	else if (ch == KEY_DOWN) { controlObjectY(playerId, floor(gameObject[playerId].y) + 1.5, 0.5); playerFacing = DOWN; }
	else if (ch == KEY_LEFT) { controlObjectX(playerId, floor(gameObject[playerId].x) - 0.5, 0.5); playerFacing = WEST; }
	else if (ch == KEY_RIGHT) { controlObjectX(playerId, floor(gameObject[playerId].x) + 1.5, 0.5); playerFacing = EAST; }
	//if (ch == KEY_UP) { pushObjectDir(playerId, 0.0, -1.0, 0.5); playerFacing = NORTH; }
	//else if (ch == KEY_DOWN) { pushObjectDir(playerId, 0.0, 1.0, 0.2); playerFacing = SOUTH; }
	//else if (ch == KEY_LEFT) { pushObjectDir(playerId, -1.0, 0.0, 0.2); playerFacing = WEST; }
	//else if (ch == KEY_RIGHT) { pushObjectDir(playerId, 1, 0.0, 0.2); playerFacing = EAST; }
	else if (fired) {} // avoid multiplt fire in one go
	else if (ch == ' ') {
		// shoot!
		fired = TRUE;
		double destX = gameObject[playerId].x + DIRECTION2X[playerFacing];
		double destY = gameObject[playerId].y + DIRECTION2Y[playerFacing];
		int bulletId = createObjectProjectileDest(BULLET, gameObject[playerId].x, gameObject[playerId].y, destX, destY, 0.8, -1, DESTROY_CRITERIA_HIT, FALSE);
	}
	else if (ch == 'b') {
		fired = TRUE;
		double destX = gameObject[playerId].x + DIRECTION2X[playerFacing];
		double destY = gameObject[playerId].y + DIRECTION2Y[playerFacing] - 0.4;
		int bombId = createObjectProjectileDest(BOMB, gameObject[playerId].x, gameObject[playerId].y, destX, destY, 0.8, 20, 0, TRUE);
	}
	ch = getch();
}

*/