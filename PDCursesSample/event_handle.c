#include "event_handle.h"

BOOL KBHistory[ACCEPTABLE_KEY_NUM] = { FALSE };

#if defined(_WIN32) || defined(_WIN64)
HANDLE WIN_EVENT_hIn;
INPUT_RECORD WIN_EVENT_InRec;
DWORD WIN_EVENT_NumUnread;
DWORD WIN_EVENT_NumRead;
int WIN_EVENT_KBpress;

void initializeKeyboardControl(void)
{
	for (int i = 0; i < ACCEPTABLE_KEY_NUM; i++)
		KBHistory[i] = FALSE;
	return;
}

int getSingleKeyboardPress(void)
{
	WIN_EVENT_hIn = GetStdHandle(STD_INPUT_HANDLE);

	GetNumberOfConsoleInputEvents(WIN_EVENT_hIn, &WIN_EVENT_NumUnread);

	if (WIN_EVENT_NumUnread >= 1)
	{
		ReadConsoleInput(WIN_EVENT_hIn, &WIN_EVENT_InRec, 1, &WIN_EVENT_NumRead);
		switch (WIN_EVENT_InRec.EventType)
		{
		case KEY_EVENT:
			if (WIN_EVENT_InRec.Event.KeyEvent.bKeyDown)
			{
				switch (WIN_EVENT_InRec.Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_UP:
					return KEY_UP; break;
				case VK_DOWN:
					return KEY_DOWN; break;
				case VK_LEFT:
					return KEY_LEFT; break;
				case VK_RIGHT:
					return KEY_RIGHT; break;
				default:
					WIN_EVENT_KBpress = WIN_EVENT_InRec.Event.KeyEvent.uChar.AsciiChar;
					WIN_EVENT_InRec.Event.KeyEvent.uChar.AsciiChar = 0;
					return WIN_EVENT_KBpress;
				}
			}
			break;
		case MOUSE_EVENT:
			break;
		}
		if (WIN_EVENT_NumUnread >= 2) { return 1; } //After reading, actual Unread = NumUnread - 1
	}
	return 0;
}

void getMultipleKeyboardPress(BOOL *output)
{
	WIN_EVENT_hIn = GetStdHandle(STD_INPUT_HANDLE);

	GetNumberOfConsoleInputEvents(WIN_EVENT_hIn, &WIN_EVENT_NumUnread);

	while (WIN_EVENT_NumUnread >= 1)
	{
		ReadConsoleInput(WIN_EVENT_hIn, &WIN_EVENT_InRec, 1, &WIN_EVENT_NumRead);
		switch (WIN_EVENT_InRec.EventType)
		{
		case KEY_EVENT:
			switch (WIN_EVENT_InRec.Event.KeyEvent.wVirtualKeyCode)
			{
			case VK_UP:
				if (WIN_EVENT_InRec.Event.KeyEvent.bKeyDown) { output[KB_UP_KEY] = TRUE; }
				KBHistory[KB_UP_KEY] = WIN_EVENT_InRec.Event.KeyEvent.bKeyDown;
				break;
			case VK_DOWN:
				if (WIN_EVENT_InRec.Event.KeyEvent.bKeyDown) { output[KB_DOWN_KEY] = TRUE; }
				KBHistory[KB_DOWN_KEY] = WIN_EVENT_InRec.Event.KeyEvent.bKeyDown;
				break;
			case VK_LEFT:
				if (WIN_EVENT_InRec.Event.KeyEvent.bKeyDown) { output[KB_LEFT_KEY] = TRUE; }
				KBHistory[KB_LEFT_KEY] = WIN_EVENT_InRec.Event.KeyEvent.bKeyDown;
				break;
			case VK_RIGHT:
				if (WIN_EVENT_InRec.Event.KeyEvent.bKeyDown) { output[KB_RIGHT_KEY] = TRUE; }
				KBHistory[KB_RIGHT_KEY] = WIN_EVENT_InRec.Event.KeyEvent.bKeyDown;
				break;
			default:
				WIN_EVENT_KBpress = WIN_EVENT_InRec.Event.KeyEvent.uChar.AsciiChar;
				WIN_EVENT_InRec.Event.KeyEvent.uChar.AsciiChar = 0;
				if (WIN_EVENT_KBpress >= 0 && WIN_EVENT_KBpress < 128)
				{
					if (WIN_EVENT_InRec.Event.KeyEvent.bKeyDown) { output[WIN_EVENT_KBpress] = TRUE; }
					KBHistory[WIN_EVENT_KBpress] = WIN_EVENT_InRec.Event.KeyEvent.bKeyDown;
				}
				break;
			}
			break;
		case MOUSE_EVENT:
			break;
		}
		WIN_EVENT_NumUnread--;
	}
	for (int i = 0; i < ACCEPTABLE_KEY_NUM; i++)
	{
		output[i] |= KBHistory[i];
	}
	return;
}

#else
void initializeKeyboardControl(void)
{
	return;
}

int getSingleKeyboardPress(void)
{
	return getch();
}

void getMultipleKeyboardPress(BOOL *output)
{
	int ch = ' ';
	do {
		ch = getch();
		if (ch == KEY_UP) { output[KB_UP_KEY] = TRUE; }
		else if (ch == KEY_DOWN) { output[KB_DOWN_KEY] = TRUE; }
		else if (ch == KEY_LEFT) { output[KB_LEFT_KEY] = TRUE; }
		else if (ch == KEY_RIGHT) { output[KB_RIGHT_KEY] = TRUE; }
		else if (ch >= 0 && ch < 128) {
			output[ch] = TRUE;
		}
	} while (ch != ERR);
	return;
}

#endif

void combineWASDwasdKeys(BOOL *output)
{
	output['w'] |= output['W']; output['W'] = FALSE;
	output['s'] |= output['S']; output['S'] = FALSE;
	output['a'] |= output['A']; output['A'] = FALSE;
	output['d'] |= output['D']; output['D'] = FALSE;
	if (output['w'] && output['s']) { output['w'] = FALSE; output['s'] = FALSE; }
	if (output['a'] && output['d']) { output['a'] = FALSE; output['d'] = FALSE; }
	return;
}
void combineArrowKeys(BOOL *output)
{
	if (output[KB_UP_KEY] && output[KB_DOWN_KEY]) { output[KB_UP_KEY] = FALSE; output[KB_DOWN_KEY] = FALSE; }
	if (output[KB_LEFT_KEY] && output[KB_RIGHT_KEY]) { output[KB_LEFT_KEY] = FALSE; output[KB_RIGHT_KEY] = FALSE; }
	return;
}
void combinewasdArrowKeys(BOOL *output)
{
	if (output['w'] || output[KB_UP_KEY]) { output['w'] = FALSE; output[KB_UP_KEY] = TRUE; }
	if (output['s'] || output[KB_DOWN_KEY]) { output['s'] = FALSE; output[KB_DOWN_KEY] = TRUE; }
	if (output['a'] || output[KB_LEFT_KEY]) { output['a'] = FALSE; output[KB_LEFT_KEY] = TRUE; }
	if (output['d'] || output[KB_RIGHT_KEY]) { output['d'] = FALSE; output[KB_RIGHT_KEY] = TRUE; }
	return;
}
