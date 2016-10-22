// Asteroid.cpp
//
// Source file for the Asteroid class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "Game.h"
#include "Asteroid.h"


// Globals from main.cpp
extern Game *theGame;


// PUBLIC FUNCTIONS

Asteroid::Asteroid() : VectObject()
{
	Define(asteroid_def);
	SetObjectType(AsteroidObj);
	SetMaxStructPoints(ASTEROID_HULL_POINTS);
	SetCollideDamage(ASTEROID_DAMAGE);
	collision_mass = ASTEROID_COLLISION_MASS;
	rotation = (float) (rand() % 7);
	SetVelocity((float) (rand() % 7),
		rand() % ((int) ASTEROID_MAX_SPEED));
	SetKillScore(ASTEROID_KILL_SCORE);
}

void Asteroid::Kill()
{
	VectObject::Kill();

	rotation = (float) (rand() % 7);
	SetVelocity((float) (rand() % 7),
		rand() % ((int) ASTEROID_MAX_SPEED));
}

void Asteroid::Update(BOOL wraparound)
{
	// Spin to the right.
	Rotate(ASTEROID_ROTATE_SPEED / theGame->GetCurrentFPS());

	VectObject::Update(wraparound);
}