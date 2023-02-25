#pragma once

#if defined(_WIN32) || defined(_WIN64)
#else

#include <stdio.h>

// For unknown reason, restrict keyword is not recognized
// and there are other mildly incompatible parts
// Therefore the function signature is modified in this translation
void strcpy_s(char *dest, size_t destsz, const char *src);
void strcat_s(char *dest, size_t destsz, const char *src);
char *strtok_s(char *str, const char *delim, char **saveptr);
int fopen_s(
	FILE **streamptr,
	const char *filename,
	const char *mode);

#endif
