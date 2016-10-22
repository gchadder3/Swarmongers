// PointExplosion.cpp
//
// Source for the PointExplosion object.


// INCLUDES

#include "GPDUMB_includes.h"
#include "Game.h"
#include "GameObject.h"
#include "GameScreen.h"
#include "Vect.h"

#include "PointExplosion.h"


// Globals defined in main.cpp
extern Game *theGame;
extern GameScreen *theScreen;


// PUBLIC FUNCTIONS

PointExplosion::PointExplosion() : PointObject()
{
	Define(explosion_def);
	SetObjectType(ExplosionObj);
	SetCollideDamage(EXPLOSION_DAMAGE);
	SetCollisionMass(EXPLOSION_COLLISION_MASS);
	explode_timer = DEFAULT_EXPLOSION_LENGTH;
}

void PointExplosion::Update(BOOL wraparound)
{
	Vect new_pos;
	Vect v(velocity);
	float new_scale;

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

		// Randomly move the points.
		for (int i = 0; i < num_pts; i++)
		{
			points[i].x += ((rand() % 3) - 1);
			points[i].y += ((rand() % 3) - 1);
		}

		// Scale the explosion.
		new_scale = scale + EXPLOSION_SCALE_SPEED / theGame->GetCurrentFPS();
		SetScale(new_scale);

		// Decrement the explosion timer.
		if (explode_timer > 0)
			theGame->CountdownTickTimer(explode_timer);
		if (explode_timer <= 0)
		{
			explode_timer = DEFAULT_EXPLOSION_LENGTH;
			Kill();
			SetScale(1.0);
		}

		PointObject::Update(wraparound);
	}
}