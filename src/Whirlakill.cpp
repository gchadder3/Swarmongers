// Whirlakill.cpp
//
// Source file for the Whirlakill class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "Game.h"
#include "Whirlakill.h"


// Globals from main.cpp
extern Game *theGame;


// PUBLIC FUNCTIONS

Whirlakill::Whirlakill() : VectObject()
{
	Define(whirlakill_def);
	SetObjectType(WhirlakillObj);
	SetMaxStructPoints(WHIRLAKILL_HULL_POINTS);
	SetCollideDamage(WHIRLAKILL_DAMAGE);
	rotation = (float) (rand() % 7);
}

void Whirlakill::Update(BOOL wraparound)
{
	// Spin to the right.
	Rotate(WHIRLAKILL_ROTATE_SPEED / theGame->GetCurrentFPS());

	VectObject::Update(wraparound);
}