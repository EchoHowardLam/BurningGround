#include "map_load.h"

Region loadLevel(LevelName level, Coordinate *start, Coordinate *end, char *executablePath) {
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
			strcat(newpath, "\\maps\\tutorial.txt");
			break;
		case TEST:
			strcat(newpath, "\\maps\\test.txt");
			break;
		default:
			return temp;
	}
#else
	switch (level) {
		case TUTORIAL:
			strcat(newpath, "/maps/tutorial.txt");
			break;
		case TEST:
			strcat(newpath, "/maps/test.txt");
			break;
		default:
			return temp;
	}
#endif
	
	FILE *raw = fopen(newpath, "rb");
	
	if (raw != NULL) {
		unsigned char line[100];
		
		int row = 0;
		while (fgets(line, 100, raw) != NULL) {
			if (row == 0) {
				char *aInt = strtok(line, " ");
				int num = 0;
				int tempX = 0, tempY = 0;
				while (aInt != NULL) {
					if (num == 0) {
						tempX = atoi(aInt);
					} else if (num == 1) {
						tempY = atoi(aInt);
						temp = generateEmptyLocalRegion(tempX, tempY);
					}
					num++;
					aInt = strtok(NULL, " ");
				}
			} else if (row == 1 || row == 2) {
				char *aInt = strtok(line, " ");
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
					aInt = strtok(NULL, " ");
				}
			} else {
				if (line[0] != '/' && line[1] != '/') {
					char *aInt = strtok(line, " ");
					int num = 0;
					BOOL del = 0;
					int tempX = 0, tempY = 0, tempW = 0, tempH = 0, tempFill = 1, tempBlock = 1;
					chtype displayChar = 97 | A_ALTCHARSET;
					// 0 - rect
					// 1 - circle (W -> radius, H -> height from bottom)
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
							char *modifier = strtok(aInt, "\\");
							while (modifier != NULL) {
								if (strcmp(modifier, "h") == 0)
									tempFill = 0;
								else if (strcmp(modifier, "d") == 0)
									del = 1;
								else if (strcmp(modifier, "e") == 0)
									tempBlock = 0;
								else if (strcmp(modifier, "R") == 0)
									shape = 1;
								else if (modifier[0] == '.')
									displayChar = modifier[1];
								modifier = strtok(NULL, "\\");
							}
						}
						num++;
						aInt = strtok(NULL, " ");
					}
					if (shape == 0) {
						if (del)
							localRegionDelRect(&temp, tempX, tempY, tempW, tempH, tempFill);
						else
							localRegionAddRectWithChar(&temp, tempX, tempY, tempW, tempH, tempFill, tempBlock, displayChar);
					} else if (shape == 1) {
						if (del)
							localRegionDelCircle(&temp, tempX, tempY, tempW, tempH, tempFill);
						else
							localRegionAddCircleWithChar(&temp, tempX, tempY, tempW, tempH, tempFill, tempBlock, displayChar);
					}
				}
			}
			row++;
		}
		
		if (level == TEST) {
			for (int i = 0; i < 100; i++) {
				localRegionAddRect(&temp, 50 + rand() % 900, 100 + rand() % 30, 5 + rand() % 10, 5 + rand() % 10, rand()%2, 1);
			}
		}
		
		fclose(raw);
	}
	free(newpath);
	return temp;
}
