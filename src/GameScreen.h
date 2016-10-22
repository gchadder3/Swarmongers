// GameScreen.h
//
// The header file for the GameScreen object.  This object
// contains information about the screen in the game.

#ifndef _GAME_SCREEN_H
#define _GAME_SCREEN_H

#include "GPDUMB_includes.h"  // for BOOL type

const int LINE_PIXEL_HEIGHT = 15;

class GameScreen
{
public:
	GameScreen(int w, int h, int minx, int maxx, int miny,
				int maxy, BOOL wrap = TRUE)
	{
		width = w;
		height = h;
		min_x = minx;
		max_x = maxx;
		min_y = miny;
		max_y = maxy;
		wraparound = wrap;
		curr_text_line = 1;
	}

	// Reader functions

	int GetWidth()
	{
		return(width);
	}

	int GetHeight()
	{
		return(height);
	}

	int GetMinX()
	{
		return(min_x);
	}

	int GetMaxX()
	{
		return(max_x);
	}

	int GetMinY()
	{
		return(min_y);
	}

	int GetMaxY()
	{
		return(max_y);
	}

	BOOL WrapsAround()
	{
		return(wraparound);
	}

	void DisplayText(char buffer[], int x_pos, int line)
	{
		if ((x_pos < 0) || (x_pos >= width))
			return;
		if ((line < 1) || (line > height / LINE_PIXEL_HEIGHT))
			return;
		Draw_Text_GDI(buffer, x_pos, (line - 1) * LINE_PIXEL_HEIGHT,
			RGB(0,255,0),lpddsback);

		curr_text_line = line + 1;
	}

	void DisplayText(char buffer[], int x_pos)
	{
		DisplayText(buffer, x_pos, curr_text_line);
	}

	void DisplayHCenterText(char buffer[], int line)
	{
		DisplayText(buffer, (width / 2) - 4 * strlen(buffer), line);
	}

	void DisplayHCenterText(char buffer[])
	{
		DisplayHCenterText(buffer, curr_text_line);
	}

	void Update()
	{
		curr_text_line = 1;
	}

private:
	// Pixel width and height of the screen (including the
	// border, however).
	int width;
	int height;

	// These values set a border at the edge of the screen 
	// where game objects can't go.  This is to avoid trying to
	// display objects outside the screen.
	int min_x;
	int max_x;
	int min_y;
	int max_y;

	// Does the screen wrap around (toroidally).
	BOOL wraparound;

	// The current text line number.
	int curr_text_line;
};

#endif