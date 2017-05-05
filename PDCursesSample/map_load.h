#pragma once

#include "shared_definition.h"
#include "local_region.h"
#include "image_load.h"

#include <stdio.h>

typedef enum {TUTORIAL, TEST} LevelName;

Region loadLevel(LevelName level, Coordinate *start, Coordinate *end, char *executablePath);

ObjectType codeToType(char *code);
