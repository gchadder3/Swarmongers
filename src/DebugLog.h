// DebugLog.h
//
// The header file for the DebugLog object.  This object
// manages a debug log.

#ifndef _DEBUG_LOG_H
#define _DEBUG_LOG_H

#include "GPDUMB_includes.h"  


const int NUM_DBG_LINES = 20;

class DebugLog
{
public:
	DebugLog()
	{
		int i;

		curr_line = 0;
		for (i = 0; i < NUM_DBG_LINES; i++)
		{
			strcpy(lines[i], " ");
		}
	}

	void post_msg(char s[])
	{
		strcpy(lines[curr_line], s);
		curr_line++;
		if (curr_line == NUM_DBG_LINES)
			curr_line = 0;
	}

	void display(int disp_x, int disp_y)
	{
		int i;

		for(i = 0; i < NUM_DBG_LINES; i++)
		{
			Draw_Text_GDI(lines[(curr_line + i) % NUM_DBG_LINES], disp_x, disp_y, RGB(0,255,0),
				lpddsback);
			disp_y += 15;
		}
	}

private:
	int curr_line;
	char lines[NUM_DBG_LINES][100];
};

#endif