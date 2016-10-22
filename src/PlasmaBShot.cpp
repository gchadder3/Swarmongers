// PlasmaBShot.cpp
//
// Source for the PlasmaBShot class.


// INCLUDES

#include "GPDUMB_includes.h"
#include "Game.h"
#include "GameObject.h"
#include "GameScreen.h"
#include "Vect.h"

#include "PlasmaBShot.h"


// Globals defined in main.cpp
extern Game *theGame;
extern GameScreen *theScreen;


// PUBLIC FUNCTIONS

PlasmaBShot::PlasmaBShot() : PointObject()
{
	Define(plasma_b_shot_def);
	SetObjectType(PlasmaBShotObj);
	SetCollideDamage(PLASMA_B_SHOT_DAMAGE);
	SetCollisionMass(PLASMA_B_SHOT_COLLISION_MASS);
	life_timer = PLASMA_B_SHOT_MAX_AGE;
}

void PlasmaBShot::Update(BOOL wraparound)
{
	Vect new_pos;
	Vect v(velocity);

	if (alive)
	{
		int x_min = theScreen->GetMinX();
		int x_max = theScreen->GetMaxX();
		int y_min = theScreen->GetMinY();
		int y_max = theScreen->GetMaxY();

		// Kill shots if they go off the screen. 
		new_pos.set(position.get_x(), position.get_y(), FALSE);
		v.div((float) theGame->GetCurrentFPS());
		new_pos.add(v);
		if ((new_pos.get_x() > x_max) ||
			(new_pos.get_x() < x_min) ||
			(new_pos.get_y() > y_max) ||
			(new_pos.get_y() < y_min))
			Kill();

		// Decrement the life timer, and kill the shot
		// if we've timed out.
		if (life_timer > 0)
			theGame->CountdownTickTimer(life_timer);
		if (life_timer <= 0)
			Kill();

		PointObject::Update(wraparound);
	}
}