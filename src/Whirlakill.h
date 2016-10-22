// Whirlakill.h
//
// The header file for the Whirlakill class.  


#ifndef _WHIRLAKILL_H
#define _WHIRLAKILL_H


// INCLUDES

#include "VectObject.h"


const int WHIRLAKILL_COLOR = 128;
const int WHIRLAKILL_HULL_POINTS = 500;
const int WHIRLAKILL_DAMAGE = 30;
const float WHIRLAKILL_ROTATE_SPEED = 2.5;  // in radians / sec.


// Line definition of a buzz bomb.
static int whirlakill_def[] =
{
	12,
	15,0,30,15,WHIRLAKILL_COLOR,
	30,15,15,30,WHIRLAKILL_COLOR,
	15,30,0,15,WHIRLAKILL_COLOR,
	0,15,15,0,WHIRLAKILL_COLOR,
	0,7,30,7,WHIRLAKILL_COLOR,
	0,23,30,23,WHIRLAKILL_COLOR,
	8,0,8,30,WHIRLAKILL_COLOR,
	22,0,22,30,WHIRLAKILL_COLOR,
	0,7,8,0,WHIRLAKILL_COLOR,
	22,0,30,7,WHIRLAKILL_COLOR,
	0,23,8,30,WHIRLAKILL_COLOR,
	22,30,30,23,WHIRLAKILL_COLOR
};


class Whirlakill: public VectObject
{
public:
	Whirlakill();

	void Update(BOOL wraparound = TRUE);
};

#endif