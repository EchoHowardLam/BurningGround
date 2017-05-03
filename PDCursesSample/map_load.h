#pragma once

#include "shared_definition.h"
#include "local_region.h"

#include <stdio.h>

typedef enum {TUTORIAL, TEST} LevelName;

Region loadLevel(LevelName level, char *eecutablePath);
