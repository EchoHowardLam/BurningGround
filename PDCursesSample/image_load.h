#pragma once

#include "shared_definition.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//int *playerData;

int lastIndexOf(char *str, char needle);
int loadImageFiles(char *path); // Load every single needed image file. Return 1 successful 0 failure
int getImage(ObjectType objType, int ID, Dimension *dimension, char **dispChar, int **solid, int **color); // Return 1 successful 0 failure
