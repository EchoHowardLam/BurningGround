#pragma once

#include <shared_definition.h>
#include <local_region.h>
#include <image_load.h>

#include <stdio.h>

typedef enum {TUTORIAL, TEST, TEST2, FOREST, PLATFORM, HELL} LevelName;

int loadLevel(Region *retRegion, LevelName level, Coordinate *start, Coordinate *end, char *executablePath);

ObjectType codeToType(char *code);
