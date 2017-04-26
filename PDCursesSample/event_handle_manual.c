/**
Demo for keyboard event manual

*** Reading single keypress ***

	do
	{
		keyboard_press = getSingleKeyboardPress();
		if (keyboard_press != 0)
		{
			full_refresh_count = 0;
		}
		switch (keyboard_press)
		{
		case 'w':
			direction = 0; break;
		case 's':
			direction = 2; break;
		case 'a':
			direction = 3; break;
		case 'd':
			direction = 1; break;
		case KEY_UP: //Up
			player.movedir(curregion, (0 + direction) % 4);
			//if (Y > 0){Y--;}
			break;
		case KEY_DOWN: //Down
			player.movedir(curregion, (2 + direction) % 4);
			//if (Y < maxY){Y++;}
			break;
		case KEY_LEFT: //Left
			player.movedir(curregion, (3 + direction) % 4);
			//if (X > 0){X--;}
			break;
		case KEY_RIGHT: //Right
			player.movedir(curregion, (1 + direction) % 4);
			//if (X < maxX){X++;}
			break;
		}
	} while (keyboard_press != 0);



*** Reading multiple keypresses ***
	
	BOOL keyboard_press[ACCEPTABLE_KEY_NUM] = { FALSE };
	getMultipleKeyboardPress(keyboard_press);
	combineWASDwasdKeys(keyboard_press);
	combineArrowKeys(keyboard_press);
	combinewasdArrowKeys(keyboard_press);
	if (keyboard_press['w']) { direction = 0; keyboard_press['w'] = false; }
	else if (keyboard_press['s']) { direction = 2; keyboard_press['s'] = false; }
	if (keyboard_press['a']) { direction = 3; keyboard_press['a'] = false; }
	else if (keyboard_press['d']) { direction = 1; keyboard_press['d'] = false; }
	if (keyboard_press[KB_UP_KEY]) { player.movedir(curregion, (0 + direction) % 4); keyboard_press[KB_UP_KEY] = false; }
	else if (keyboard_press[KB_DOWN_KEY]) { player.movedir(curregion, (2 + direction) % 4); keyboard_press[KB_DOWN_KEY] = false; }
	if (keyboard_press[KB_LEFT_KEY]) { player.movedir(curregion, (3 + direction) % 4); keyboard_press[KB_LEFT_KEY] = false; }
	else if (keyboard_press[KB_RIGHT_KEY]) { player.movedir(curregion, (1 + direction) % 4); keyboard_press[KB_RIGHT_KEY] = false; }

*/
