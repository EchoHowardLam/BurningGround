#pragma once

#include "shared_definition.h"

int loadImageFiles(); // Load every single needed image file. Return 1 successful 0 failure
int getImage(ObjectType objType, int ID, Dimension *dimension, char **dispChar, int **solid, int **fcolor); // Return 1 successful 0 failure
