// GameObject.cpp
//
// The source for the GameObject class.

// INCLUDES 

#include "GPDUMB_includes.h"
#include "Game.h"
#include "GameObject.h"
#include "GameScreen.h"
#include "Vect.h"


// Globals defined in main.cpp
extern Game *theGame;
extern GameScreen *theScreen;


// PUBLIC FUNCTIONS

void GameObject::SetPosition(float x, float y)
{
	int x_min = theScreen->GetMinX();
	int x_max = theScreen->GetMaxX();
	int y_min = theScreen->GetMinY();
	int y_max = theScreen->GetMaxY();
	
	// Clip the position to the screen boundaries.
	if ((int) x < x_min)
		x = (float) x_min;
	if ((int) x > x_max)
		x = (float) x_max;
	if ((int) y < y_min)
		y = (float) y_min;
	if ((int) y > y_max)
		y = (float) y_max;

	position.set(x, y, FALSE);
}

void GameObject::Update(BOOL wraparound)
{
	Vect v(velocity);

	// Only do anything if the object is alive...
	if (alive)
	{
		int x_min = theScreen->GetMinX();
		int x_max = theScreen->GetMaxX();
		int y_min = theScreen->GetMinY();
		int y_max = theScreen->GetMaxY();

		v.div((float) theGame->GetCurrentFPS());
		position.add(v);

		// Handle off-the-screen motions. 
		if (position.get_x() > x_max)
		{
			if (wraparound)
				position.set((float) x_min, position.get_y(), FALSE);
			else
				position.set((float) x_max, position.get_y(), FALSE);
		}
		else if (position.get_x() < x_min)
		{
			if (wraparound)
				position.set((float) x_max, position.get_y(), FALSE);
			else
				position.set((float) x_min, position.get_y(), FALSE);
		}

		if (position.get_y() > y_max)
		{
			if (wraparound)
				position.set(position.get_x(), (float) y_min, FALSE);
			else
				position.set(position.get_x(), (float) y_max, FALSE);
		}
		else if (position.get_y() < y_min)
		{
			if (wraparound)
				position.set(position.get_x(), (float) y_max, FALSE);
			else
				position.set(position.get_x(), (float) y_min, FALSE);
		}
	}
}

void GameObject::PlayExplodeSound()
{
//	sndManager->PlayNonCombatExplode();
}
