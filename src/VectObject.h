// VectObject.h
//
// The header file for the VectObject class.

#ifndef _VECT_OBJECT_H
#define _VECT_OBJECT_H

#include "GPDUMB_includes.h"
#include "misc_constants.h"
#include "GameObject.h"

const int VCT_OBJ_MAX_LINES = 30;
const int VCT_OBJ_GHOST_COLOR = 23;

struct PixelLine
{
	int x1;
	int y1;
	int x2;
	int y2;
	int color;
};

// TODO Break most of these function out into a
// VectObject.cpp file.

class VectObject : public GameObject
{
public:
	VectObject() : GameObject()
	{
		num_lines = 0;
		scale = 1.0;
		rotation = 0.0;
	}

	// Read the lines of the VectObject in from a line 
	// definition array.  Then calculate the width, height, 
	// and collision radius of the object.  Finally, translate
	// the object so that the center-of-mass is at (0,0).  
	void Define(int def[])
	{
		int min_x = 10000;
		int max_x = -10000;
		int min_y = 10000;
		int max_y = -10000;
		int center_def_x;
		int center_def_y;
		int i;

		// Read all of the lines in.
		num_lines = def[0];
		for (i = 0; i < num_lines; i++)
		{
			lines[i].x1 = def[i * 5 + 1];
			lines[i].y1 = def[i * 5 + 2];
			lines[i].x2 = def[i * 5 + 3];
			lines[i].y2 = def[i * 5 + 4];
			lines[i].color = def[i * 5 + 5];
		}

		// Calculate the min and max x and y coordinates.
		for (i = 0; i < num_lines; i++)
		{
			if (lines[i].x1 < min_x)
				min_x = lines[i].x1;
			if (lines[i].x1 > max_x)
				max_x = lines[i].x1;
			if (lines[i].x2 < min_x)
				min_x = lines[i].x2;
			if (lines[i].x2 > max_x)
				max_x = lines[i].x2;
			if (lines[i].y1 < min_y)
				min_y = lines[i].y1;
			if (lines[i].y1 > max_y)
				max_y = lines[i].y1;
			if (lines[i].y2 < min_y)
				min_y = lines[i].y2;
			if (lines[i].y2 > max_y)
				max_y = lines[i].y2;
		}

		// Calculate the pixel width and height.
		def_pixel_width = max_x - min_x + 1;
		def_pixel_height = max_y - min_y + 1;

		// Calculate the definition center-of-mass.
		center_def_x = (min_x + max_x) / 2;
		center_def_y = (min_y + max_y) / 2;

		// Calculate the collision radius. 
		def_collision_rad = (float) sqrt((float) (center_def_x * center_def_x) + 
									(float) (center_def_y * center_def_y));

		// Shift all of the lines left and up so that the 
		// COM of the object is now at (0,0) in model space.
		// This makes drawing, scaling, rotating, etc. easier.
		for (i = 0; i < num_lines; i++)
		{
			lines[i].x1 -= center_def_x;
			lines[i].x2 -= center_def_x;
			lines[i].y1 -= center_def_y;
			lines[i].y2 -= center_def_y;
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
		int draw_x1;
		int draw_y1;
		int draw_x2;
		int draw_y2;
		int tx,ty;
		int i;

		// Only draw the object if it's "alive".
		if (alive)
		{
			// Get the position of the whole object.
			pos_x = (int) position.get_x();
			pos_y = (int) position.get_y();

			// Draw each line...
			for (i = 0; i < num_lines; i++)
			{
				// Get the model points
				draw_x1 = lines[i].x1;
				draw_y1 = lines[i].y1;
				draw_x2 = lines[i].x2;
				draw_y2 = lines[i].y2;

				// Apply scaling.
				draw_x1 = (int) (((float) draw_x1) * scale);
				draw_y1 = (int) (((float) draw_y1) * scale);
				draw_x2 = (int) (((float) draw_x2) * scale);
				draw_y2 = (int) (((float) draw_y2) * scale);

				// Apply rotation
				tx = draw_x1;
				ty = draw_y1;
				draw_x1 = (int) ((float) tx * cos(rotation) - 
					(float) ty * sin(rotation));
				draw_y1 = (int) ((float) tx * sin(rotation) + 
					(float) ty * cos(rotation));

				tx = draw_x2;
				ty = draw_y2;
				draw_x2 = (int) ((float) tx * cos(rotation) - 
					(float) ty * sin(rotation));
				draw_y2 = (int) ((float) tx * sin(rotation) + 
					(float) ty * cos(rotation));

				// Move the model points out to the object
				// position (world coordinates).
				draw_x1 += pos_x;
				draw_y1 += pos_y;
				draw_x2 += pos_x;
				draw_y2 += pos_y;

				// Keep this routine from trying to draw outside
				// the screen!  Maybe remove the if later, when all
				// is copasetic.
				if ((draw_x1 >= 0) && (draw_x1 < SCREEN_WIDTH) &&
					(draw_y1 >= 0) && (draw_y1 < SCREEN_HEIGHT) &&
					(draw_x2 >= 0) && (draw_x2 < SCREEN_WIDTH) &&
					(draw_y2 >= 0) && (draw_y2 < SCREEN_HEIGHT))
				{
					if (!ghost)
						Draw_Line(draw_x1, draw_y1, draw_x2,
							draw_y2, lines[i].color, back_buffer, 
							back_lpitch);
					else
						Draw_Line(draw_x1, draw_y1, draw_x2,
							draw_y2, VCT_OBJ_GHOST_COLOR, back_buffer, 
							back_lpitch);
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
		BOOL draw_off_screen = FALSE;
		int draw_x1;
		int draw_y1;
		int draw_x2;
		int draw_y2;
		int tx,ty;
		int i;

		// Draw each line...
		for (i = 0; i < num_lines; i++)
		{
			// Get the model points
			draw_x1 = lines[i].x1;
			draw_y1 = lines[i].y1;
			draw_x2 = lines[i].x2;
			draw_y2 = lines[i].y2;

			// Apply scaling.
			draw_x1 = (int) (((float) draw_x1) * scale);
			draw_y1 = (int) (((float) draw_y1) * scale);
			draw_x2 = (int) (((float) draw_x2) * scale);
			draw_y2 = (int) (((float) draw_y2) * scale);

			// Apply rotation
			tx = draw_x1;
			ty = draw_y1;
			draw_x1 = (int) ((float) tx * cos(rotation) - 
				(float) ty * sin(rotation));
			draw_y1 = (int) ((float) tx * sin(rotation) + 
				(float) ty * cos(rotation));

			tx = draw_x2;
			ty = draw_y2;
			draw_x2 = (int) ((float) tx * cos(rotation) - 
				(float) ty * sin(rotation));
			draw_y2 = (int) ((float) tx * sin(rotation) + 
				(float) ty * cos(rotation));

			// Move the model points out to the object
			// position (world coordinates).
			draw_x1 += pos_x;
			draw_y1 += pos_y;
			draw_x2 += pos_x;
			draw_y2 += pos_y;

			// Keep this routine from trying to draw outside
			// the screen!  Maybe remove the if later, when all
			// is copasetic.
			if ((draw_x1 >= 0) && (draw_x1 < SCREEN_WIDTH) &&
				(draw_y1 >= 0) && (draw_y1 < SCREEN_HEIGHT) &&
				(draw_x2 >= 0) && (draw_x2 < SCREEN_WIDTH) &&
				(draw_y2 >= 0) && (draw_y2 < SCREEN_HEIGHT))
			{
				Draw_Line(draw_x1, draw_y1, draw_x2,
					draw_y2, lines[i].color, back_buffer, 
					back_lpitch);
			}
			else
			{
				draw_off_screen = TRUE;
			}	
		}

//		if (!draw_off_screen)
			return(TRUE);
//		else
//			return(FALSE);
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
	// Number of lines in the object definition
	int num_lines;

	// Point definitions
	struct PixelLine lines[VCT_OBJ_MAX_LINES];
};

#endif
