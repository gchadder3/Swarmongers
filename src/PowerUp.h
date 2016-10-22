// PowerUp.h
//
// The header file for the PowerUp class.  


#ifndef _POWERUP_H
#define _POWERUP_H


// INCLUDES

#include "VectObject.h"


const int POWERUP_COLOR = 183;
const int POWERUP_HULL_POINTS = 2;
const float POWERUP_COLLISION_MASS = 1;
const int POWERUP_DAMAGE = 0;
const float POWERUP_ROTATE_SPEED = 1.0;  // in radians / sec.
const float POWERUP_MAX_SPEED = 100;
const int POWERUP_KILL_SCORE = -100;

// Line definition of a powerup.
static int powerup_def[] =
{
	16,
	9,0,16,0,POWERUP_COLOR,
	16,0,19,7,POWERUP_COLOR,
	19,7,19,24,POWERUP_COLOR,
	19,24,6,24,POWERUP_COLOR,
	6,24,6,7,POWERUP_COLOR,
	6,7,9,0,POWERUP_COLOR,
	15,3,15,9,POWERUP_COLOR,
	10,6,14,6,POWERUP_COLOR,
	10,10,15,10,POWERUP_COLOR,
	15,10,13,13,POWERUP_COLOR,
	13,13,15,16,POWERUP_COLOR,
	15,16,10,16,POWERUP_COLOR,
	10,18,15,18,POWERUP_COLOR,
	15,18,15,22,POWERUP_COLOR,
	15,22,10,22,POWERUP_COLOR,
	13,18,13,22,POWERUP_COLOR
}; 

class PowerUp: public VectObject
{
public:
	PowerUp();

	void Kill();

	void Update(BOOL wraparound = TRUE);
};

#endif