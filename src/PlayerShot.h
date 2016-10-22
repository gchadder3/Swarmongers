// PlayerShot.h
//
// The header file for the PlayerShot class.  PlayerShot 
// defines and manages shot from the player's weapon.

#ifndef _PLAYER_SHOT_H
#define _PLAYER_SHOT_H

#include "PointObject.h"


const int PLAYER_SHOT_COLOR = 128;
const float PLAYER_SHOT_SPEED = 600;  // in pixels / second
const float PLAYER_SHOT_DAMAGE = 20;  // how much damage does the shot do?
const float PLAYER_SHOT_COLLISION_MASS = 5; 
const int PLAYER_SHOT_MAX_AGE = 2000; // in msec.

// Point definition of a player shot.
static int player_shot_def[] =
{
	5,
	1,0,PLAYER_SHOT_COLOR,
	0,1,PLAYER_SHOT_COLOR,
	1,1,PLAYER_SHOT_COLOR,
	2,1,PLAYER_SHOT_COLOR,
	1,2,PLAYER_SHOT_COLOR
}; 


class PlayerShot: public PointObject
{
public:
	// Constructor function.
	PlayerShot();

	float GetShotSpeed()
	{
		return(PLAYER_SHOT_SPEED);
	}

	void Kill()
	{
		life_timer = PLAYER_SHOT_MAX_AGE;
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