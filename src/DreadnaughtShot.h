// DreadnaughtShot.h
//
// The header file for the DreadnaughtShot class.  DreadnaughtShot 
// defines and manages the shots fired by Grell Dreadnaughts.

#ifndef _DREADNAUGHT_SHOT_H
#define _DREADNAUGHT_SHOT_H

#include "VectObject.h"


const int DREADNAUGHT_SHOT_COLOR = 56;
const int DREADNAUGHT_SHOT_HULL_POINTS = 1000;
const float DREADNAUGHT_SHOT_DAMAGE = 600;  // how much damage does the shot do?
const float DREADNAUGHT_SHOT_COLLISION_MASS = 1000; 
const int DREADNAUGHT_SHOT_MAX_AGE = 2000; // in msec.

// Point definition of a dreadnaught shot.
static int dreadnaught_shot_def[] =
{
	6,
	2,0,10,8,DREADNAUGHT_SHOT_COLOR,
	0,2,8,10,DREADNAUGHT_SHOT_COLOR,
	0,8,8,0,DREADNAUGHT_SHOT_COLOR,
	2,10,10,2,DREADNAUGHT_SHOT_COLOR,
	5,0,5,10,DREADNAUGHT_SHOT_COLOR,
	0,5,10,5,DREADNAUGHT_SHOT_COLOR
}; 


class DreadnaughtShot: public VectObject
{
public:
	// Constructor function.
	DreadnaughtShot();

	void Kill()
	{
		life_timer = DREADNAUGHT_SHOT_MAX_AGE;
		VectObject::Kill();
	}

	void Update(BOOL wraparound = TRUE);

	// Use Draw_Text to debug display object info.
	void dbg_display(int disp_x, int disp_y)
	{
		char buffer[80];

		VectObject::dbg_display(disp_x, disp_y);
		disp_y += 225;

		sprintf(buffer, "Life Timer: %d", life_timer);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;
	}

private:
	int life_timer;
};

#endif