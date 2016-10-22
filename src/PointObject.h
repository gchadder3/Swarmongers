// PointObject.h
//
// The header file for the PointObject class.  GameObject
// is the class for entities in the game which are made up
// only of points.  These entities can't be rotated or 
// scaled.

#ifndef _POINT_OBJECT_H
#define _POINT_OBJECT_H

#include "GPDUMB_includes.h"
#include "GameObject.h"
#include "Vect.h"

const int PT_OBJ_MAX_PTS = 40;
const int PT_OBJ_GHOST_COLOR = 23;

struct PixelPoint
{
	int x;
	int y;
	int color;
};

// TODO Break most of these function out into a
// PointObject.cpp file.

class PointObject: public GameObject
{
public:
	PointObject() : GameObject()
	{
		num_pts = 0;
		scale = 1.0;
		rotation = 0.0;
	}

	// Read the points of the PointObject in from a point 
	// definition array.  Then calculate the width, height, 
	// and collision radius of the object.  Finally, translate
	// the object so that the center-of-mass is at (0,0).  
	// (It is assumed that the definition array has (0,0) as 
	// the upper-leftmost point.)
	void Define(int def[])
	{
		int center_def_x;
		int center_def_y;
		int i;

		// Read all of the points in.
		num_pts = def[0];
		for (i = 0; i < num_pts; i++)
		{
			points[i].x = def[i * 3 + 1];
			points[i].y = def[i * 3 + 2];
			points[i].color = def[i * 3 + 3];
		}

		// Calculate the pixel width and height.
		def_pixel_width = 0;
		def_pixel_height = 0;
		for (i = 0; i < num_pts; i++)
		{
			if (points[i].x > def_pixel_width)
				def_pixel_width = points[i].x;
			if (points[i].y > def_pixel_height)
				def_pixel_height = points[i].y;
		}

		// Calculate the definition center-of-mass.
		center_def_x = def_pixel_width / 2;
		center_def_y = def_pixel_height / 2;

		// Calculate the collision radius. 
		def_collision_rad = (float) sqrt((float) (center_def_x * center_def_x) + 
									(float) (center_def_y * center_def_y));

		// Shift all of the points left and up so that the 
		// COM of the object is now at (0,0) in model space.
		// This makes drawing, scaling, rotating, etc. easier.
		for (i = 0; i < num_pts; i++)
		{
			points[i].x -= center_def_x;
			points[i].y -= center_def_y;
		}

		// Set the current width, height, and collision 
		// radius to the definition values.
		pixel_width = def_pixel_width;
		pixel_height = def_pixel_height;
		collision_rad = def_collision_rad;
	}


	// Draw the object.
	BOOL Draw(BOOL ghost = FALSE)
	{
		BOOL draw_off_screen = FALSE;
		int pos_x;
		int pos_y;
		int draw_x;
		int draw_y;
		int tx,ty;
		int i;

		// Only draw the object if it's "alive".
		if (alive)
		{
			pos_x = (int) position.get_x();
			pos_y = (int) position.get_y();
			for (i = 0; i < num_pts; i++)
			{
				// Get the model points
				draw_x = points[i].x;
				draw_y = points[i].y;

				// Apply scaling.
				draw_x = (int) (((float) draw_x) * scale);
				draw_y = (int) (((float) draw_y) * scale);

				// Apply rotation
				tx = draw_x;
				ty = draw_y;
				draw_x = (int) ((float) tx * cos(rotation) - 
					(float) ty * sin(rotation));
				draw_y = (int) ((float) tx * sin(rotation) + 
					(float) ty * cos(rotation));

				// Move the model points out to the object
				// position (world coordinates).
				draw_x += pos_x;
				draw_y += pos_y;

				// Keep this routine from trying to draw outside
				// the screen!  Maybe remove this later, when all
				// is copasetic.
				if ((draw_x >= 0) && (draw_x < SCREEN_WIDTH) &&
					(draw_y >= 0) && (draw_y < SCREEN_HEIGHT))
				{ 
					if (!ghost)
						Draw_Pixel(draw_x, draw_y, 
							points[i].color, back_buffer, back_lpitch);
					else
						Draw_Pixel(draw_x, draw_y, 
							PT_OBJ_GHOST_COLOR, back_buffer, back_lpitch);
				}
				else
				{
					draw_off_screen = TRUE;
				} 	
			}
		}

//		if (!draw_off_screen)
			return(TRUE);
//		else
//			return(FALSE);
	}

	// Draw the object as a "decal", ignoring screen boundaries.
	BOOL DecalDraw(int pos_x, int pos_y)
	{
		return(FALSE);
	}

	virtual void Update(BOOL wraparound = TRUE)
	{ 
		GameObject::Update(wraparound);
	}

	virtual void PlayExplodeSound()
	{ 
		GameObject::PlayExplodeSound();
	}

protected:
	// Number of points in the object definition
	int num_pts;

	// Point definitions
	struct PixelPoint points[PT_OBJ_MAX_PTS];
};

#endif