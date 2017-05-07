#include "image_load.h"

CharacterImage **allObjs = NULL;

int loadImageFiles(char *path) {
	allObjs = (CharacterImage **) malloc(IMAGE_FILES * sizeof(CharacterImage *));
	
	char *filesToRead[IMAGE_FILES];
#if defined(_WIN32) || defined(_WIN64)
	filesToRead[0] = "\\graphics\\Player.txt";
	filesToRead[1] = "\\graphics\\eyeballs.txt";
	filesToRead[2] = "\\graphics\\mosquitoes.txt";
	filesToRead[3] = "\\graphics\\mushrooms.txt";
	filesToRead[4] = "\\graphics\\rabbit.txt";
	filesToRead[5] = "\\graphics\\sludge.txt";
	filesToRead[6] = "\\graphics\\grass.txt";
	filesToRead[7] = "\\graphics\\bees.txt";
	filesToRead[8] = "\\graphics\\bat.txt";
	filesToRead[9] = "\\graphics\\birds.txt";
	filesToRead[10] = "\\graphics\\durian.txt";
#else
	filesToRead[0] = "/graphics/Player.txt";
	filesToRead[1] = "/graphics/eyeballs.txt";
	filesToRead[2] = "/graphics/mosquitoes.txt";
	filesToRead[3] = "/graphics/mushrooms.txt";
	filesToRead[4] = "/graphics/rabbit.txt";
	filesToRead[5] = "/graphics/sludge.txt";
	filesToRead[6] = "/graphics/grass.txt";
	filesToRead[7] = "/graphics/bees.txt";
	filesToRead[8] = "/graphics/bat.txt";
	filesToRead[9] = "/graphics/birds.txt";
	filesToRead[10] = "/graphics/durian.txt";
#endif
	
	int size = ((int) strlen(path)) + 1;
	char *newpath = (char *) malloc(size * sizeof(char));
#if defined(_WIN32) || defined(_WIN64)
	int cut = lastIndexOf(path, '\\');
#else
	int cut = lastIndexOf(path, '/');
#endif
	memcpy(newpath, path, cut);
	newpath[cut] = '\0';
	
	for (int i=0; i<IMAGE_FILES; i++) {
		char *tempFileName = (char *) malloc(size * sizeof(char) + 50);
		strcpy(tempFileName, newpath);
		strcat(tempFileName, filesToRead[i]);
		
		FILE *raw = fopen(tempFileName, "rb");
		//size_t length = 0;
		unsigned char line[100];
		
		CharacterImage *loadImage = NULL, *lastImage = NULL;
		int sectionOpening = 1;
		int displayMode = 1;
		int row = 0;
		while (fgets(line, 100, raw) != NULL) {
			if (sectionOpening) {
				loadImage = (CharacterImage *) malloc(sizeof(CharacterImage));
				if (lastImage == NULL) allObjs[i] = loadImage;
				else lastImage->next = loadImage;
				loadImage->next = NULL;
				
				char *aInt = strtok(line, " ");
				int j=0;
				while (aInt != NULL) {
					if (j==0)
						loadImage->charaID = atoi(aInt);
					else if (j==1) {
						loadImage->dimension = (Dimension *) malloc(sizeof(Dimension));
						loadImage->dimension->x = atoi(aInt);
					} else if (j==2) {
						loadImage->dimension->y = atoi(aInt);
						loadImage->display = (chtype **) malloc(loadImage->dimension->y * sizeof(chtype *));
						loadImage->color = (int **) malloc(loadImage->dimension->y * sizeof(int *));
						loadImage->solid = (int **) malloc(loadImage->dimension->y * sizeof(int *));
					} else if (j==3) {
						loadImage->center = (Coordinate *) malloc(sizeof(Coordinate));
						loadImage->center->x = atoi(aInt);
					} else if (j==4) {
						loadImage->center->y = atoi(aInt);
						sectionOpening = 0;
					}
					j++;
					aInt = strtok(NULL, " ");
				}
			} else {
				if (displayMode) {
					loadImage->display[row] = (chtype *) malloc(loadImage->dimension->x * sizeof(chtype));
					loadImage->solid[row] = (int *) malloc(loadImage->dimension->x * sizeof(int));
					int headChar = indexNotOf(line, ' ');
					int tailChar = lastIndexNotOf(line, ' ');
					for (int k = 0; k < loadImage->dimension->x; k++) {
						if (line[k] >= 161)
							loadImage->display[row][k] = (line[k] | A_ALTCHARSET);
						else if (line[k] >= 130)
							loadImage->display[row][k] = (line[k] -128+102-2) | A_ALTCHARSET;
						else if (line[k] >= 128)
							loadImage->display[row][k] = (line[k] -128+96) | A_ALTCHARSET;
						else
							loadImage->display[row][k] = line[k];
						
						loadImage->solid[row][k] = (k >= headChar && k <= tailChar);
					}
				} else {
					loadImage->color[row] = (int *) malloc(loadImage->dimension->x * sizeof(int));
					for (int k=0; k < loadImage->dimension->x; k++) {
						loadImage->color[row][k] = colorCodeToPair(line[k]);
					}
				}
				row++;
				if (row >= loadImage->dimension->y && displayMode) {displayMode = 0; row = 0;}
				else if (row >= loadImage->dimension->y) {sectionOpening = 1; lastImage = loadImage; row = 0; displayMode = 1;}
			}
		}
		
		fclose(raw);
		//free(line);
		free(tempFileName);
	}
	free(newpath);
	return 1;
}

CharacterImage* getImage(ObjectType objType, int ID) {
	///////////////////////// Sample for main.c
	/*CharacterImage* temp = getImage(LIFE_HUMANOID, 0);
	if (temp != NULL) {
		for (int i=0; i<temp->dimension->y; i++) {
			for (int j=0; j<temp->dimension->x; j++) {
				move(i, j);
				attron(temp->color[i][j]);
				addch(temp->display[i][j]);
				attroff(temp->color[i][j]);
			}
		}
	}*/
	////////////////////////
	
	
	if (allObjs == NULL)
		return NULL;
	
	CharacterImage *temp = NULL;
	switch (objType) {
		case LIFE_HUMANOID:
			temp = allObjs[0];
			break;
		case LIFE_EYEBALL:
			temp = allObjs[1];
			break;
		case LIFE_MOSQUITOES:
			temp = allObjs[2];
			break;
		case LIFE_MUSHROOM:
			temp = allObjs[3];
			break;
		case LIFE_RABBIT:
			temp = allObjs[4];
			break;
		case LIFE_SLIME:
			temp = allObjs[5];
			break;
		case LIFE_SLUDGE:
			//temp = allObjs[5];
			temp = allObjs[5]->next->next;
			break;
		case LIFE_GRASS:
			temp = allObjs[6];
			break;
		case LIFE_BEE:
			temp = allObjs[7]->next->next;
			break;
		case SPAWN_BEE_HIVE:
			temp = allObjs[7];
			break;
		case LIFE_BAT:
			temp = allObjs[8];
			break;
		case LIFE_BIRD:
			temp = allObjs[9];
			break;
		case SPAWN_DURIAN_TREE:
			temp = allObjs[10];
			break;
		case LIFE_DURIAN:
			temp = allObjs[10]->next;
			break;
		default:
			return NULL;
	}
	while (temp != NULL) {
		if (temp->charaID == ID) {
			return temp;
		} else {
			temp = temp->next;
		}
	}
	return NULL;
}

int colorCodeToPair(char code) {
	switch (code) {
		case 'k':
			return COLOR_PAIR(COLOR_BLACK);
		case 'b':
			return COLOR_PAIR(COLOR_BLUE);
		case 'g':
			return COLOR_PAIR(COLOR_GREEN);
		case 'c':
			return COLOR_PAIR(COLOR_CYAN);
		case 'r':
			return COLOR_PAIR(COLOR_RED);
		case 'm':
			return COLOR_PAIR(COLOR_MAGENTA);
		case 'y':
			return COLOR_PAIR(COLOR_YELLOW);
		case 'w':
			return COLOR_PAIR(COLOR_WHITE);
		case 'K':
			return COLOR_PAIR(COLOR_B_BLACK);
		case 'B':
			return COLOR_PAIR(COLOR_B_BLUE);
		case 'G':
			return COLOR_PAIR(COLOR_B_GREEN);
		case 'C':
			return COLOR_PAIR(COLOR_B_CYAN);
		case 'R':
			return COLOR_PAIR(COLOR_B_RED);
		case 'M':
			return COLOR_PAIR(COLOR_B_MAGENTA);
		case 'Y':
			return COLOR_PAIR(COLOR_B_YELLOW);
		case 'W':
			return COLOR_PAIR(COLOR_B_WHITE);
	}
	return -1;
}

void setUpColors() {
	for (int i = 1; i < 16; i++)
		init_pair(i, i, COLOR_BLACK);
	
	init_pair(20, COLOR_BLACK, COLOR_B_WHITE); // inverse
}
