// Asteroid.h
//
// The header file for the Asteroid class.  


#ifndef _ASTEROID_H
#define _ASTEROID_H


// INCLUDES

#include "VectObject.h"


const int ASTEROID_COLOR = 255;
const int ASTEROID_HULL_POINTS = 100;
const float ASTEROID_COLLISION_MASS = 100;
const int ASTEROID_DAMAGE = 20;
const float ASTEROID_ROTATE_SPEED = 2.5;  // in radians / sec.
const float ASTEROID_MAX_SPEED = 450;
const int ASTEROID_KILL_SCORE = 50;

// Line definition of a buzz bomb.
static int asteroid_def[] =
{
	15,
	11,0,23,3,ASTEROID_COLOR,
	23,3,30,18,ASTEROID_COLOR,
	30,18,17,30,ASTEROID_COLOR,
	17,30,13,25,ASTEROID_COLOR,
	13,25,4,27,ASTEROID_COLOR,
	4,27,0,15,ASTEROID_COLOR,
	0,15,9,8,ASTEROID_COLOR,
	9,8,11,0,ASTEROID_COLOR,
	13,6,17,9,ASTEROID_COLOR,
	17,9,17,15,ASTEROID_COLOR,
	11,12,8,15,ASTEROID_COLOR,
	8,15,12,19,ASTEROID_COLOR,
	24,13,26,15,ASTEROID_COLOR,
	16,22,18,21,ASTEROID_COLOR,
	18,21,20,22,ASTEROID_COLOR
}; 

class Asteroid: public VectObject
{
public:
	Asteroid();

	void Kill();

	void Update(BOOL wraparound = TRUE);
};

#endif