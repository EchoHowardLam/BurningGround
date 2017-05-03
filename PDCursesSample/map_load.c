#include "map_load.h"

Region loadLevel(LevelName level, char *executablePath) {
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
			} else {
				char *aInt = strtok(line, " ");
				int num = 0;
				int tempX = 0, tempY = 0, tempW = 0, tempH = 0;
				while (aInt != NULL) {
					if (num == 0) {
						tempX = atoi(aInt);
					} else if (num == 1) {
						tempY = atoi(aInt);
					} else if (num == 2) {
						tempW = atoi(aInt);
					} else if (num == 3) {
						tempH = atoi(aInt);
						localRegionAddRect(&temp, tempX, tempY, tempW, tempH, 1);
					}
					num++;
					aInt = strtok(NULL, " ");
				}
			}
			row++;
		}
		
		if (level == TEST) {
			for (int i = 0; i < 100; i++)
				localRegionAddRect(&temp, 50 + rand() % 900, 100 + rand() % 30, 5 + rand() % 10, 5 + rand() % 10, rand()%2);
		}
		
		fclose(raw);
	}
	free(newpath);
	return temp;
}
