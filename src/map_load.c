#include <map_load.h>

#include <cross_platform_bullshit.h>

// 0 fail, 1 success
int loadLevel(Region *retRegion, LevelName level, Coordinate *start, Coordinate *end, char *executablePath) {
	Region temp = {0};
	
	int size = ((int) strlen(executablePath)) + 1;
	char *newpath = (char *) malloc((size+50) * sizeof(char));
#if defined(_WIN32) || defined(_WIN64)
	int cut = lastIndexOf(executablePath, '\\');
#else
	int cut = lastIndexOf(executablePath, '/');
#endif
	memcpy(newpath, executablePath, cut);
	newpath[cut] = '\0';
	
	
#if defined(_WIN32) || defined(_WIN64)
	switch (level) {
		case TUTORIAL:
			strcat_s(newpath, size + 50, "\\..\\maps\\tutorial.txt");
			break;
		case TEST:
			strcat_s(newpath, size + 50, "\\..\\maps\\test.txt");
			break;
		case TEST2:
			strcat_s(newpath, size + 50, "\\..\\maps\\test2.txt");
			break;
		case FOREST:
			strcat_s(newpath, size + 50, "\\..\\maps\\forest.txt");
			break;
		case PLATFORM:
			strcat_s(newpath, size + 50, "\\..\\maps\\platform.txt");
			break;
		case HELL:
			strcat_s(newpath, size + 50, "\\..\\maps\\hell.txt");
			break;
		default:
			(*retRegion) = temp;
			return 0;
	}
#else
	switch (level) {
		case TUTORIAL:
			strcat_s(newpath, size + 50, "/../maps/tutorial.txt");
			break;
		case TEST:
			strcat_s(newpath, size + 50, "/../maps/test.txt");
			break;
		case TEST2:
			strcat_s(newpath, size + 50, "/../maps/test2.txt");
			break;
		case FOREST:
			strcat_s(newpath, size + 50, "/../maps/forest.txt");
			break;
		case PLATFORM:
			strcat_s(newpath, size + 50, "/../maps/platform.txt");
			break;
		case HELL:
			strcat_s(newpath, size + 50, "/../maps/hell.txt");
			break;
		default:
			(*retRegion) = temp;
			return 0;
	}
#endif
	
	FILE *raw;
	if (fopen_s(&raw, newpath, "rb") != 0)
		return 0;
	
	if (raw != NULL) {
		char line[100];
		
		int row = 0;
		int mobSpawnArea = 0;
		while (fgets(line, 100, raw) != NULL) {
			if (row == 0) {
				char *context;
				char *aInt = strtok_s(line, " ", &context);
				int num = 0;
				int tempX = 0, tempY = 0, tempSpawn = 0;
				while (aInt != NULL) {
					if (num == 0) {
						tempX = atoi(aInt);
					} else if (num == 1) {
						tempY = atoi(aInt);
					} else if (num == 2) {
						tempSpawn = atoi(aInt);
						temp = generateEmptyLocalRegion(tempX, tempY, tempSpawn);
					}
					num++;
					aInt = strtok_s(NULL, " ", &context);
				}
			} else if (row == 1 || row == 2) {
				char *context;
				char *aInt = strtok_s(line, " ", &context);
				int num = 0;
				while (aInt != NULL) {
					if (num == 0) {
						if (row == 1)
							start->x = atoi(aInt);
						else
							end->x = atoi(aInt);
					} else if (num == 1) {
						if (row == 1)
							start->y = atoi(aInt);
						else
							end->y = atoi(aInt);
					}
					num++;
					aInt = strtok_s(NULL, " ", &context);
				}
			} else {
				if (line[0] == '\\' && line[1] == 'm' && line[2] == 'o' && line[3] == 'b') {
					// create mob spawning area
					char *context;
					char *param = strtok_s(line, " ", &context);
					int num = 0;
					SpawnRegion *tempSpawn = (SpawnRegion *) malloc(sizeof(SpawnRegion));
					while (param != NULL) {
						switch (num) {
							case 1:
								tempSpawn->x = atoi(param);
								break;
							case 2:
								tempSpawn->y = atoi(param);
								break;
							case 3:
								tempSpawn->width = atoi(param);
								break;
							case 4:
								tempSpawn->height = atoi(param);
								break;
							case 5:
								tempSpawn->mob = codeToType(param);
								break;
							case 6:
								tempSpawn->chance = atoi(param)/1000.0;
								break;
							case 7:
								tempSpawn->initial = atoi(param);
								break;
							case 8:
								tempSpawn->max = atoi(param);
								tempSpawn->currMobSpawned = 0;
								temp.spawns[mobSpawnArea] = tempSpawn;
								break;
						}
						num++;
						param = strtok_s(NULL, " ", &context);
					}
					mobSpawnArea++;
				} else if (line[0] == '\\' && line[1] == 'g' && line[2] == 'r' && line[3] == 's') {
					// add grass
					char *context;
					char *param = strtok_s(line, " ", &context);
					int num = 0;
					SpawnRegion *tempSpawn = (SpawnRegion *) malloc(sizeof(SpawnRegion));
					while (param != NULL) {
						switch (num) {
							case 1:
								tempSpawn->x = atoi(param);
								break;
							case 2:
								tempSpawn->y = atoi(param);
								break;
							case 3:
								tempSpawn->width = atoi(param);
								tempSpawn->height = 1;
								tempSpawn->mob = LIFE_GRASS;
								tempSpawn->currMobSpawned = 0;
								temp.spawns[mobSpawnArea] = tempSpawn;
								break;
						}
						num++;
						param = strtok_s(NULL, " ", &context);
					}
					mobSpawnArea++;
				} else if (line[0] != '/' && line[1] != '/') {
					char *context;
					char *aInt = strtok_s(line, " ", &context);
					int num = 0;
					BOOL del = 0;
					int tempX = 0, tempY = 0, tempW = 0, tempH = 0, tempFill = 1, tempBlock = 1;
					chtype displayChar = 97 | A_ALTCHARSET;
					int tempColor = COLOR_PAIR(COLOR_WHITE);
					// 0 - rect
					// 1 - circle (W -> radius, H -> height from bottom)
					// 2 - upper tri
					int shape = 0;
					while (aInt != NULL) {
						if (num == 0) {
							tempX = atoi(aInt);
						} else if (num == 1) {
							tempY = atoi(aInt);
						} else if (num == 2) {
							tempW = atoi(aInt);
						} else if (num == 3) {
							tempH = atoi(aInt);
						} else if (num == 4) {
							char *modContext;
							char *modifier = strtok_s(aInt, "\\", &modContext);
							while (modifier != NULL) {
								if (strcmp((char *) modifier, "h") == 0)
									tempFill = 0;
								else if (strcmp((char *) modifier, "d") == 0)
									del = 1;
								else if (strcmp((char *) modifier, "e") == 0)
									tempBlock = 0;
								else if (modifier[0] == 'c')
									tempColor = colorCodeToPair(modifier[1]);
								else if (strcmp((char *) modifier, "R") == 0)
									shape = 1;
								else if (strcmp((char *) modifier, "T") == 0)
									shape = 2;
								else if (modifier[0] == '.')
									displayChar = modifier[1];
								else if (modifier[0] == ',') {
									displayChar = modifier[1]-'0';
									displayChar *= 10;
									displayChar += modifier[2]-'0';
									if (strlen(modifier) == 4) {
										displayChar *= 10;
										displayChar += modifier[3]-'0';
									}
									displayChar |= A_ALTCHARSET;
								}
								modifier = strtok_s(NULL, "\\", &modContext);
							}
						}
						num++;
						aInt = strtok_s(NULL, " ", &context);
					}
					if (shape == 0) {
						if (del)
							localRegionDelRect(&temp, tempX, tempY, tempW, tempH, tempFill);
						else
							localRegionAddRectWithChar(&temp, tempX, tempY, tempW, tempH, tempFill, tempBlock, displayChar, tempColor);
					} else if (shape == 1) {
						if (del)
							localRegionDelCircle(&temp, tempX, tempY, tempW, tempH, tempFill);
						else
							localRegionAddCircleWithChar(&temp, tempX, tempY, tempW, tempH, tempFill, tempBlock, displayChar, tempColor);
					} else if (shape == 2) {
						if (del)
							localRegionDelUTri(&temp, tempX, tempY, tempW, tempH, tempFill);
						else
							localRegionAddUTriWithChar(&temp, tempX, tempY, tempW, tempH, tempFill, tempBlock, displayChar, tempColor);
					}
				}
			}
			row++;
		}
		
		if (level == TEST) {
			for (int i = 0; i < 100; i++) {
				localRegionAddRect(&temp, 50 + rand() % 900, 100 + rand() % 30, 5 + rand() % 10, 5 + rand() % 10, rand()%2, 1, COLOR_PAIR(COLOR_WHITE));
			}
		}
		
		fclose(raw);
	}
	free(newpath);

	(*retRegion) = temp;
	return 1;
}

ObjectType codeToType(char *code) {
	if (strcmp(code, "eye") == 0)
		return LIFE_EYEBALL;
	else if (strcmp(code, "mos") == 0)
		return LIFE_MOSQUITOES;
	else if (strcmp(code, "mush") == 0)
		return LIFE_MUSHROOM;
	else if (strcmp(code, "rab") == 0)
		return LIFE_RABBIT;
	else if (strcmp(code, "slime") == 0)
		return LIFE_SLIME;
	else if (strcmp(code, "slu") == 0)
		return LIFE_SLUDGE;
	else if (strcmp(code, "hive") == 0)
		return SPAWN_BEE_HIVE;
	else if (strcmp(code, "bee") == 0)
		return LIFE_BEE;
	else if (strcmp(code, "bat") == 0)
		return LIFE_BAT;
	else if (strcmp(code, "bird") == 0)
		return LIFE_BIRD;
	else if (strcmp(code, "dur") == 0)
		return SPAWN_DURIAN_TREE;
	else if (strcmp(code, "demon") == 0)
		return LIFE_HUMANOID;
	else
		return NOTHING;
}
