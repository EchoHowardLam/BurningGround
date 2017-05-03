#include "image_load.h"

CharacterImage **allObjs = NULL;

int lastIndexOf(char *str, char needle) {
	int currIndex = (int) strlen(str)-1;
	while(currIndex >= 0) {
		if (str[currIndex] == needle) return currIndex;
		currIndex--;
	}
	return -1;
}

int loadImageFiles(char *path) {
	allObjs = (CharacterImage **) malloc(IMAGE_FILES * sizeof(CharacterImage *));
	
	char *filesToRead[IMAGE_FILES];
	filesToRead[0] = "\\graphics\\Player.txt";
	
	int size = ((int) strlen(path)) + 1;
	char *newpath = (char *) malloc(size * sizeof(char));
	int cut = lastIndexOf(path, '\\');
	memcpy(newpath, path, cut);
	newpath[cut] = '\0';
	
	for (int i=0; i<IMAGE_FILES; i++) {
		char *tempFileName = (char *) malloc(size * sizeof(char) + 50);
		strcpy(tempFileName, newpath);
		strcat(tempFileName, filesToRead[i]);
		
		FILE *raw = fopen(tempFileName, "r");
		size_t length = 0;
		char *line[100];
		
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
					for (int k=0; k<length; k++) {
						if (line[k] >= 161)
							loadImage->display[row][k] = ((int)line[k]) | A_ALTCHARSET;
						else if (line[k] >= 130)
							loadImage->display[row][k] = (((int)line[k]) -128+102-2) | A_ALTCHARSET;
						else if (line[k] >= 128)
							loadImage->display[row][k] = (((int)line[k]) -128+96) | A_ALTCHARSET;
						else
							loadImage->display[row][k] = line[k];
						
						loadImage->solid[row][k] = (line[k] == ' ');
					}
				} else {
					loadImage->color[row] = (int *) malloc(loadImage->dimension->x * sizeof(int));
					for (int k=0; k<length; k++) {
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
	/*CharacterImage* temp = getImage(PLAYER, 0);
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
	
	switch (objType) {
		case PLAYER: {
			CharacterImage *temp = allObjs[0];
			while (temp != NULL) {
				if (temp->charaID == ID) {
					return temp;
				} else {
					temp = temp->next;
				}
			}
			return NULL;
		}
		default:
			return NULL;
	}
}

int colorCodeToPair(char code) {
	switch (code) {
		case 'k':
			return COLOR_PAIR(0);
		case 'b':
			return COLOR_PAIR(1);
		case 'g':
			return COLOR_PAIR(2);
		case 'c':
			return COLOR_PAIR(3);
		case 'r':
			return COLOR_PAIR(4);
		case 'm':
			return COLOR_PAIR(5);
		case 'y':
			return COLOR_PAIR(6);
		case 'w':
			return COLOR_PAIR(7);
		case 'K':
			return COLOR_PAIR(8);
		case 'B':
			return COLOR_PAIR(9);
		case 'G':
			return COLOR_PAIR(10);
		case 'C':
			return COLOR_PAIR(11);
		case 'R':
			return COLOR_PAIR(12);
		case 'M':
			return COLOR_PAIR(13);
		case 'Y':
			return COLOR_PAIR(14);
		case 'W':
			return COLOR_PAIR(15);
	}
	return -1;
}

void setUpColors() {
	for (int i = 1; i < 16; i++)
		init_pair(i, i, COLOR_BLACK);
	/*init_pair(0, COLOR_BLACK, COLOR_BLACK);
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	init_pair(8, COLOR_B_BLACK, COLOR_BLACK);
	init_pair(9, COLOR_B_BLUE, COLOR_BLACK);
	init_pair(10, COLOR_B_GREEN, COLOR_BLACK);
	init_pair(11, COLOR_B_CYAN, COLOR_BLACK);
	init_pair(12, COLOR_B_RED, COLOR_BLACK);
	init_pair(13, COLOR_B_MAGENTA, COLOR_BLACK);
	init_pair(14, COLOR_B_YELLOW, COLOR_BLACK);
	init_pair(15, COLOR_B_WHITE, COLOR_BLACK);*/
	
	init_pair(20, COLOR_BLACK, COLOR_B_WHITE); // inverse
}
