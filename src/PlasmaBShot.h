// PlasmaBShot.h
//
// The header file for the PlasmaBShot class.  PlasmaBShot 
// defines and manages red plasma shots from a non-player's
// weapon.

#ifndef _PLASMA_B_SHOT_H
#define _PLASMA_B_SHOT_H

#include "PointObject.h"


const int PLASMA_B_SHOT_COLOR = 32;
const float PLASMA_B_SHOT_DAMAGE = 10;  // how much damage does the shot do?
const float PLASMA_B_SHOT_COLLISION_MASS = 2; 
const int PLASMA_B_SHOT_MAX_AGE = 2000; // in msec.

// Point definition of a red plasma shot.

static int plasma_b_shot_def[] =
{
	5,
	1,0,PLASMA_B_SHOT_COLOR,
	0,1,PLASMA_B_SHOT_COLOR,
	1,1,PLASMA_B_SHOT_COLOR,
	2,1,PLASMA_B_SHOT_COLOR,
	1,2,PLASMA_B_SHOT_COLOR
}; 
/*
static int plasma_b_shot_def[] =
{
	1,
	0,0,PLASMA_B_SHOT_COLOR
}; 
*/

class PlasmaBShot: public PointObject
{
public:
	// Constructor function.
	PlasmaBShot();

	void Kill()
	{
		life_timer = PLASMA_B_SHOT_MAX_AGE;
		PointObject::Kill();
	}

	void Update(BOOL wraparound = TRUE);

	// Use Draw_Text to debug display object info.
	void dbg_display(int disp_x, int disp_y)
	{
		char buffer[80];

		PointObject::dbg_display(disp_x, disp_y);
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