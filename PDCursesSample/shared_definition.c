#include "shared_definition.h"

double DIRECTION2X[4] = { 0, 1, 0, -1 };
double DIRECTION2Y[4] = { -1, 0, 1, 0 };
double DIAGONALX[4] = { 1, 1, -1, -1 };
double DIAGONALY[4] = { 1, -1, 1, -1 };
int EXP_NEEDED_TO_LV_UP[MAX_LV] = { 1 };

void setupDisplay() {
#if defined(_WIN32) || defined(_WIN64)
	system("chcp 437");		// Windows-only
#endif
}

#if defined(_WIN32) || defined(_WIN64)
void threadSleep(unsigned ms) {
	Sleep(ms);
}
#else
void threadSleep(unsigned ms) {
	usleep(ms * 1000);
}
#endif

int lastIndexOf(char *str, char needle) {
	int currIndex = (int) strlen(str)-1;
	while(currIndex >= 0) {
		if (str[currIndex] == needle) return currIndex;
		currIndex--;
	}
	return -1;
}

int indexNotOf(char *str, char needle) {
	int currIndex = 0;
	while(currIndex < strlen(str)-1) {
		if (str[currIndex] == needle) currIndex++;
		else return currIndex;
	}
	return -1;
}

int lastIndexNotOf(char *str, char needle) {
	int currIndex = (int) strlen(str)-1;
	while(currIndex >= 0) {
		if (str[currIndex] == needle || str[currIndex] == '\n' || str[currIndex] == '\r') currIndex--;
		else return currIndex;
	}
	return -1;
}


double getRandomOfRange(int base) {
	return ((double)rand())/RAND_MAX * base * ((rand()%2==0)?1:-1);
}
