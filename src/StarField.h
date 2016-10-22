// StarField.h
//
// The header file for the StarField object.  This object 
// manages the decorative background star field in the game.

#ifndef _STARFIELD_H
#define _STARFIELD_H


// INCLUDES
#include "GPDUMB_includes.h"
#include "Vect.h"


const int NUM_STARS = 100;
const int STAR_COLOR = 128;
const int STAR_GHOST_COLOR = 23;

class StarField
{
public:
	StarField();
	
	void SetVelocity(float vx, float vy);

	void Update(BOOL wraparound = TRUE);

	BOOL Draw(BOOL ghost = FALSE);

private:
	// Current position of each star. (in pixels)
	Vect position[NUM_STARS];

	// Current velocity of the entire star field (in pixels / sec.)
	Vect velocity;
};

#endif

