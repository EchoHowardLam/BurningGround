#include <cross_platform_bullshit.h>

#if defined(_WIN32) || defined(_WIN64)
#else

#include <string.h>
#include <stddef.h>

void strcpy_s(char *dest, size_t destsz, const char *src) {
	// For unknown reason, restrict keyword is not recognized
	// and there are other mildly incompatible parts
	// Therefore the strcpy_s function signature is modified
	strncpy(dest, src, destsz);
}
void strcat_s(char *dest, size_t destsz, const char *src) {
	// For unknown reason, restrict keyword is not recognized
	// and there are other mildly incompatible parts
	// Therefore the strcat_s function signature is modified
	strncat(dest, src, destsz);
}
char *strtok_s(char *str, const char *delim, char **saveptr) {
	return strtok_r(str, delim, saveptr);
}
int fopen_s(
	FILE **streamptr,
	const char *filename,
	const char *mode) {
	*streamptr = fopen(filename, mode);
	return (*streamptr == NULL);
}

#endif
