// PlasmaCShot.h
//
// The header file for the PlasmaCShot class.  PlasmaCShot 
// defines and manages green plasma shots from a non-player's
// weapon.

#ifndef _PLASMA_C_SHOT_H
#define _PLASMA_C_SHOT_H

#include "PointObject.h"


const int PLASMA_C_SHOT_COLOR = 103;
const float PLASMA_C_SHOT_DAMAGE = 15;  // how much damage does the shot do?
const float PLASMA_C_SHOT_COLLISION_MASS = 3; 
const int PLASMA_C_SHOT_MAX_AGE = 2000; // in msec.

// Point definition of a green plasma shot.
static int plasma_c_shot_def[] =
{
	5,
	1,0,PLASMA_C_SHOT_COLOR,
	0,1,PLASMA_C_SHOT_COLOR,
	1,1,PLASMA_C_SHOT_COLOR,
	2,1,PLASMA_C_SHOT_COLOR,
	1,2,PLASMA_C_SHOT_COLOR
}; 


class PlasmaCShot: public PointObject
{
public:
	// Constructor function.
	PlasmaCShot();

	void Kill()
	{
		life_timer = PLASMA_C_SHOT_MAX_AGE;
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