// Valiant.h
//
// The header file for the Valiant class.  The Valiant is 
// the ship the player will typically play.  It is a 
// Warhawk of the Confederation.

#ifndef _VALIANT_H
#define _VALIANT_H

// INCLUDES

#include "Warhawk.h"


const int VALIANT_COLOR = 128;


// Line definition of the Valiant.
static int valiant_def[] =
{
	8,
	10,0,6,24,VALIANT_COLOR,
	6,24,12,16,VALIANT_COLOR,
	12,16,0,24,VALIANT_COLOR,
	0,24,12,8,VALIANT_COLOR,
	12,8,24,24,VALIANT_COLOR,
	24,24,12,16,VALIANT_COLOR,
	12,16,18,24,VALIANT_COLOR,
	18,24,14,0,VALIANT_COLOR
}; 


class Valiant: public Warhawk
{
public:
	// Constructor function.
	Valiant();

protected:
	void PlayThrustSound();
	void PlayFireSound();
};

#endif