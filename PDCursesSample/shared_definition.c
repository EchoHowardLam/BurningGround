#include "shared_definition.h"

double DIRECTION2X[4] = { 0, 1, 0, -1 };
double DIRECTION2Y[4] = { -1, 0, 1, 0 };
double DIAGONALX[4] = { 1, 1, -1, -1 };
double DIAGONALY[4] = { 1, -1, 1, -1 };

void setupDisplay() {
#if defined(_WIN32) || defined(_WIN64)
	system("chcp 437");		// Windows-only
#endif
}

#if defined(_WIN32) || defined(_WIN64)
void sleep(unsigned ms) {
	Sleep(ms);
}
#else
void sleep(unsigned ms) {
	usleep(ms * 1000);		// supposedly this should work on Mac, but I don't have a Mac, so I am not sure
}
#endif
