#include "image_load.h"

int lastIndexOf(char *str, char needle) {
	int currIndex = (int) strlen(str)-1;
	while(currIndex >= 0) {
		if (str[currIndex] == needle) return currIndex;
		currIndex--;
	}
	return -1;
}

int loadImageFiles(char *path) {
	char newpath[strlen(path)+50];
	int cut = lastIndexOf(path, '/');
	memcpy(newpath, path, cut);
	newpath[cut] = '\0';
	strcat(newpath, "/graphics/Player.txt");
	FILE *temp = fopen(newpath, "r");
	if (temp != NULL) {
		size_t length = 0;
		char *line = NULL;
		int i=0;
		while (getline(&line, &length, temp) != -1) {
			mvprintw(i, 0, line);
			i++;
		}
		fclose(temp);
		free(line);
	} else {
		mvprintw(0, 0, "%s\n%s", path, newpath);
	}
	return 1;
}

int getImage(ObjectType objType, int ID, Dimension *dimension, char **dispChar, int **solid, int **color) {
	
	return 1;
}
