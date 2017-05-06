#pragma once

#include "shared_definition.h"

#include <stdio.h>

#define IMAGE_FILES 7

typedef struct CharacterImage CharacterImage;

struct CharacterImage {
	int charaID;
	Dimension *dimension;
	Coordinate *center;
	chtype **display;
	int **color;
	int **solid;
	CharacterImage *next;
};

int loadImageFiles(char *path); // Load every single needed image file. Return 1 successful 0 failure
CharacterImage* getImage(ObjectType objType, int ID); // Return 1 successful 0 failure
//int getImage(ObjectType objType, int ID, CharacterImage* returnImg); // Return 1 successful 0 failure

int colorCodeToPair(char code);

void setUpColors();
