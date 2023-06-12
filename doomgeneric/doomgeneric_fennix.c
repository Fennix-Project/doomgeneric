#include "doomkeys.h"

#include "doomgeneric.h"

#include <stdio.h>
#include <fennix/syscall.h>

#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char convertToDoomKey(unsigned char key)
{
	// switch (key)
	// {
	// case VK_RETURN:
	// 	key = KEY_ENTER;
	// 	break;
	// case VK_ESCAPE:
	// 	key = KEY_ESCAPE;
	// 	break;
	// case VK_LEFT:
	// 	key = KEY_LEFTARROW;
	// 	break;
	// case VK_RIGHT:
	// 	key = KEY_RIGHTARROW;
	// 	break;
	// case VK_UP:
	// 	key = KEY_UPARROW;
	// 	break;
	// case VK_DOWN:
	// 	key = KEY_DOWNARROW;
	// 	break;
	// case VK_CONTROL:
	// 	key = KEY_FIRE;
	// 	break;
	// case VK_SPACE:
	// 	key = KEY_USE;
	// 	break;
	// case VK_SHIFT:
	// 	key = KEY_RSHIFT;
	// 	break;
	// default:
	// 	key = tolower(key);
	// 	break;
	// }

	return key;
}

static void addKeyToQueue(int pressed, unsigned char keyCode)
{
	unsigned char key = convertToDoomKey(keyCode);

	unsigned short keyData = (pressed << 8) | key;

	s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
	s_KeyQueueWriteIndex++;
	s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

void DG_Init()
{
	memset(s_KeyQueue, 0, KEYQUEUE_SIZE * sizeof(unsigned short));
}

void DG_DrawFrame()
{
	syscall1(sys_Print, 'H');
		// StretchDIBits(s_Hdc, 0, 0, DOOMGENERIC_RESX, DOOMGENERIC_RESY, 0, 0, DOOMGENERIC_RESX, DOOMGENERIC_RESY, DG_ScreenBuffer);
}

void DG_SleepMs(uint32_t ms)
{
	syscall1(sys_Sleep, ms);
}

uint32_t DG_GetTicksMs()
{
	static uint32_t stub;
	return ++stub; // TODO
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
	if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex)
	{
		//key queue is empty

		return 0;
	}
	else
	{
		unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
		s_KeyQueueReadIndex++;
		s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

		*pressed = keyData >> 8;
		*doomKey = keyData & 0xFF;

		return 1;
	}
}

void DG_SetWindowTitle(const char * title)
{
	for (int i = 0; i < 14; i++)
		syscall1(sys_Print, "Window Title: "[i]);

	for (int i = 0; title[i] != '\0'; i++)
	{
		syscall1(sys_Print, title[i]);
	}
	syscall1(sys_Print, '\n');
}

int main(int argc, char **argv)
{
    doomgeneric_Create(argc, argv);

    for (int i = 0; ; i++)
    {
        doomgeneric_Tick();
    }
    

    return 0;
}