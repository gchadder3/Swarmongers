// DreadnaughtShot.cpp
//
// Source for the DreadnaughtShot class.


// INCLUDES

#include "GPDUMB_includes.h"
#include "Game.h"
#include "GameObject.h"
#include "GameScreen.h"
#include "Vect.h"
#include "DreadnaughtShot.h"


// Globals defined in main.cpp
extern Game *theGame;
extern GameScreen *theScreen;


// PUBLIC FUNCTIONS

DreadnaughtShot::DreadnaughtShot() : VectObject()
{
	Define(dreadnaught_shot_def);
	max_struct_points = DREADNAUGHT_SHOT_HULL_POINTS;
	struct_points = max_struct_points;
	SetObjectType(DreadnaughtShotObj);
	SetCollideDamage(DREADNAUGHT_SHOT_DAMAGE);
	SetCollisionMass(DREADNAUGHT_SHOT_COLLISION_MASS);
	life_timer = DREADNAUGHT_SHOT_MAX_AGE;
	SetScale(4.0);
}

void DreadnaughtShot::Update(BOOL wraparound)
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

		VectObject::Update(wraparound);
	}
}