// PlasmaDShot.h
//
// The header file for the PlasmaDShot class.  PlasmaDShot 
// defines and manages large white plasma shots from a 
// non-player's weapon.

#ifndef _PLASMA_D_SHOT_H
#define _PLASMA_D_SHOT_H

#include "VectObject.h"


const int PLASMA_D_SHOT_COLOR = 255;
const float PLASMA_D_SHOT_DAMAGE = 30;  // how much damage does the shot do?
const float PLASMA_D_SHOT_COLLISION_MASS = 10; 
const int PLASMA_D_SHOT_MAX_AGE = 2000; // in msec.

// Point definition of a red plasma shot.
static int plasma_d_shot_def[] =
{
	4,
	0,2,4,2,PLASMA_D_SHOT_COLOR,
	2,0,2,4,PLASMA_D_SHOT_COLOR,
	1,1,3,3,PLASMA_D_SHOT_COLOR,
	1,3,3,1,PLASMA_D_SHOT_COLOR
}; 


class PlasmaDShot: public VectObject
{
public:
	// Constructor function.
	PlasmaDShot();

	void Kill()
	{
		life_timer = PLASMA_D_SHOT_MAX_AGE;
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