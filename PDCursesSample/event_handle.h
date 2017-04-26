#pragma once

#include "shared_definition.h"

/**
Reference:
https://msdn.microsoft.com/en-us/library/windows/desktop/ms684199(v=vs.85).aspx
http://stackoverflow.com/questions/7798242/keyboard-mouse-input-in-c
Input record struct: https://msdn.microsoft.com/en-us/library/windows/desktop/ms683499(v=vs.85).aspx
i.e. InRec.Event.KeyEvent.uChar.AsciiChar

This code is migrated from Echo's old project so the coding style is different from this project,
however to maintain the stability of the code, the coding style in event_handle.h/c follows its original project.
Prefix WIN_EVENT_ is added to the variables to isolate the variables used by this file with exterior files. (concept: namespace in C++)
Only the function interface should be used by exterior files.

Check the event_handle_manual.c for code examples
*/

#define ACCEPTABLE_KEY_NUM 132
#define KB_UP_KEY 128
#define KB_DOWN_KEY 129
#define KB_LEFT_KEY 130
#define KB_RIGHT_KEY 131
//128 + 4

void initializeKeyboardControl(void);
int getSingleKeyboardPress(void);
void getMultipleKeyboardPress(BOOL *output);

// The following three functions are only for getMultipleKeyboardPress's output
// Supposed to be called in the order as declaration
void combineWASDwasdKeys(BOOL *output); // combine lower case wasd and upper case WASD into the slot of lower case wasd, eliminate conflicting keys
void combineArrowKeys(BOOL *output); // eliminate conflicting arrow keys, i.e. up and down are cancelled against each other
void combinewasdArrowKeys(BOOL *output); // combine lower case wasd and Arrow keys into the slot of arrow keys, DOES NOT eliminate conflicting arrow and lower case wasd keys

extern BOOL KBHistory[ACCEPTABLE_KEY_NUM];

#if defined(_WIN32) || defined(_WIN64)
// Declare internal variables for Windows OS here
extern HANDLE WIN_EVENT_hIn;
extern INPUT_RECORD WIN_EVENT_InRec;
extern DWORD WIN_EVENT_NumUnread;
extern DWORD WIN_EVENT_NumRead;
extern int WIN_EVENT_KBpress;

#else
// Declare internal variables for Macs OS here

#endif
