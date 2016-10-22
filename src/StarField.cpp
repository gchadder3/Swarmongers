// StarField.cpp
//
// The source for the StarField class.


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

StarField::StarField()
{
	int x_min = theScreen->GetMinX();
	int x_max = theScreen->GetMaxX();
	int y_min = theScreen->GetMinY();
	int y_max = theScreen->GetMaxY();
	int new_x;
	int new_y;

	for (int i = 0; i < NUM_STARS; i++)
	{
		new_x = x_min + (rand() % (x_max - x_min + 1));
		new_y = y_min + (rand() % (y_max - y_min + 1));
		position[i].set((float) new_x,(float) new_y, FALSE);
	}
	velocity.set(0.0,0.0);
}

void StarField::SetVelocity(float vx, float vy)
{
	velocity.set(vx,vy, FALSE);
}
	
void StarField::Update(BOOL wraparound)
{
	Vect v(velocity);

	int x_min = theScreen->GetMinX();
	int x_max = theScreen->GetMaxX();
	int y_min = theScreen->GetMinY();
	int y_max = theScreen->GetMaxY();

	v.div((float) theGame->GetCurrentFPS());

	// For each star...
	for (int i = 0; i < NUM_STARS; i++)
	{
		position[i].add(v);

		// Handle off-the-screen motions. 
		if (position[i].get_x() > x_max)
		{
			if (wraparound)
				position[i].set((float) x_min, position[i].get_y(), FALSE);
			else
				position[i].set((float) x_max, position[i].get_y(), FALSE);
		}
		else if (position[i].get_x() < x_min)
		{
			if (wraparound)
				position[i].set((float) x_max, position[i].get_y(), FALSE);
			else
				position[i].set((float) x_min, position[i].get_y(), FALSE);
		}

		if (position[i].get_y() > y_max)
		{
			if (wraparound)
				position[i].set(position[i].get_x(), (float) y_min, FALSE);
			else
				position[i].set(position[i].get_x(), (float) y_max, FALSE);
		}
		else if (position[i].get_y() < y_min)
		{
			if (wraparound)
				position[i].set(position[i].get_x(), (float) y_max, FALSE);
			else
				position[i].set(position[i].get_x(), (float) y_min, FALSE);
		}
	}
}

BOOL StarField::Draw(BOOL ghost)
{
	int pos_x;
	int pos_y;
	int draw_x;
	int draw_y;

	for (int i = 0; i < NUM_STARS; i++)
	{
		pos_x = (int) position[i].get_x();
		pos_y = (int) position[i].get_y();

		// Keep this routine from trying to draw outside
		// the screen!  Maybe remove this later, when all
		// is copasetic.
		draw_x = pos_x;
		draw_y = pos_y;
		if ((draw_x >= 0) && (draw_x < SCREEN_WIDTH) &&
			(draw_y >= 0) && (draw_y < SCREEN_HEIGHT))
		{
			if (!ghost)
				Draw_Pixel(draw_x, draw_y, STAR_COLOR, back_buffer, back_lpitch);
			else
				Draw_Pixel(draw_x, draw_y, STAR_GHOST_COLOR, back_buffer, back_lpitch);
		}
		else
		{
			return(FALSE);
		}	
	}

	return(TRUE);
}