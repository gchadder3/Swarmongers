// PowerUp.cpp
//
// Source file for the PowerUp class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "Game.h"
#include "PowerUp.h"


// Globals from main.cpp
extern Game *theGame;


// PUBLIC FUNCTIONS

PowerUp::PowerUp() : VectObject()
{
	Define(powerup_def);
	SetObjectType(PowerUpObj);
	SetMaxStructPoints(POWERUP_HULL_POINTS);
	SetCollideDamage(POWERUP_DAMAGE);
	collision_mass = POWERUP_COLLISION_MASS;
	rotation = (float) (rand() % 7);
	SetVelocity((float) (rand() % 7),
		rand() % ((int) POWERUP_MAX_SPEED));
	SetKillScore(POWERUP_KILL_SCORE);
}

void PowerUp::Kill()
{
	VectObject::Kill();

	rotation = (float) (rand() % 7);
	SetVelocity((float) (rand() % 7),
		rand() % ((int) POWERUP_MAX_SPEED));
}

void PowerUp::Update(BOOL wraparound)
{
	// Spin to the right.
	Rotate(POWERUP_ROTATE_SPEED / theGame->GetCurrentFPS());

	VectObject::Update(wraparound);
}