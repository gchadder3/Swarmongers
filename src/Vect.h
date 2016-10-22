// Vect.h
//
// The header file for a Vect object.
// Note that angles run clockwise beginning with the X axis
// as the origin.  All angles are in radians.

#ifndef _VECT_H
#define _VECT_H

// INCLUDES

#include "misc_constants.h"


class Vect
{
public:
	// Default constructor
	Vect()
	{
		x = 0;
		y = 0;
		ang = 0;
		dist = 0;
	}

	// This constructor allows a vector to be defined
	// in either rectangular or polar (the default) 
	// coordinates.
	Vect(float val1, float val2, BOOL polar = TRUE)
	{
		if (polar)
		{
			ang = val1;
			dist = val2;
			calc_polar2rect();
		}
		else
		{
			x = val1;
			y = val2;
			calc_rect2polar();
		}
	}

	// This constructor allows you to copy an existing 
	// vector.
	Vect(Vect& v)
	{
		x = v.get_x();
		y = v.get_y();
		ang = v.get_ang();
		dist = v.get_dist();
	}

	float get_x()
	{
		return(x);
	}

	float get_y()
	{
		return(y);
	}

	float get_ang()
	{
		return ang;
	}

	float get_dist()
	{
		return dist;
	}

	void set_dist(float new_dist)
	{
		dist = new_dist;
		calc_polar2rect();
	}

	// Set a vector to some desired value.
	void set(float val1, float val2, BOOL polar = TRUE)
	{
		if (polar)
		{
			ang = val1;
			dist = val2;
			calc_polar2rect();
		}
		else
		{
			x = val1;
			y = val2;
			calc_rect2polar();
		}
	}

	void set(Vect& v)
	{
		x = v.get_x();
		y = v.get_y();
		ang = v.get_ang();
		dist = v.get_dist();
	}

	void add(Vect& v)
	{
		x += v.get_x();
		y += v.get_y();
		calc_rect2polar();
	}

	void add(float val1,float val2, BOOL polar = TRUE)
	{
		float new_x;
		float new_y;

		if (polar)
		{
			new_x = (float)cos(val1) * val2;
			new_y = (float)sin(val1) * val2;
			add(new_x, new_y, FALSE);
		}
		else
		{
			x += val1;
			y += val2;
			calc_rect2polar();
		}
	}

	void subtract(Vect& v)
	{
		x -= v.get_x();
		y -= v.get_y();
		calc_rect2polar();
	}

	void subtract(float val1,float val2, BOOL polar = TRUE)
	{
		float new_x;
		float new_y;

		if (polar)
		{
			new_x = (float)cos(val1) * val2;
			new_y = (float)sin(val2) * val2;
			subtract(new_x, new_y, FALSE);
		}
		else
		{
			x -= val1;
			y -= val2;
			calc_rect2polar();
		}
	}

	void div(float val1)
	{
		x /= val1;
		y /= val1;
		calc_rect2polar();
	}

	float dot_product(Vect &v)
	{
		return(x * v.get_x() + y * v.get_y());
	}

	float angle_to(float tx, float ty)
	{
		double dx = (double) (tx - x);
		double dy = (double) (ty - y);
		return(radian_fix((float)(atan2(dy,dx) + QUARTER_CIRCLE)));
	}

	float angle_to(Vect& v)
	{
		double dx = (double) (v.get_x() - x);
		double dy = (double) (v.get_y() - y);
		return(radian_fix((float)(atan2(dy,dx) + QUARTER_CIRCLE)));
	}

	float distance_to(float tx, float ty)
	{
		double dx = tx - x;
		double dy = ty - y;;
		return((float) sqrt((float) (dx * dx) + (float) (dy * dy)));
	}

	float distance_to(Vect& v)
	{
		double dx = v.get_x() - x;
		double dy = v.get_y() - y;;
		return((float) sqrt((float) (dx * dx) + (float) (dy * dy)));
	}

	// Take radian values that are negative or over 2*PI and 
	// return the equivalent radian value between 0 and 2*PI.
	// Note that this only works if the angle is between
	// -4*PI and 4*PI.
	static float radian_fix(float ang)
	{
		if (ang < 0.0)
			return(FULL_CIRCLE + ang);
		else if (ang > FULL_CIRCLE)
			return(ang - FULL_CIRCLE);
		else
			return(ang);
	}

	// Use Draw_Text to debug display the vector info.
	void dbg_display(int disp_x, int disp_y)
	{
		char buffer[80];

		sprintf(buffer, "X: %.2f, Y: %.2f, <: %.2f, dist: %.2f", 
			get_x(), get_y(), get_ang(), get_dist());
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
	}

private:
	// Update the polar values to match the rectangular
	// values.
	void calc_rect2polar()
	{
		ang = (float)atan2(y,x);
		if(x == 0 && y == 0)
			dist = 0;
		else
			dist = (float)sqrt((float) (x * x) + (float) (y * y));
	}

	// Update the rectangular values to match the polar
	// values.
	void calc_polar2rect()
	{
		if(dist == 0)
		{
			x = 0;
			y = 0;
		}
		else
		{
			x = (float)cos(ang) * dist;
			y = (float)sin(ang) * dist;
		}
	}

	float x;	// rectangular coords
	float y;
	float ang;	// polar coords
	float dist;
};

#endif