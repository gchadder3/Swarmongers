// PointExplosion.h
//
// The header file for the PointExplosion class.  
// PointExplosion is the class for explosions which consist
// of points.

#ifndef _POINT_EXPLOSION_H
#define _POINT_EXPLOSION_H


// INCLUDES

#include "GPDUMB_includes.h"
#include "PointObject.h"

const int EXPLOSION_COLOR = 58;
const int DEFAULT_EXPLOSION_LENGTH = 500;  // in milliseconds
const int EXPLOSION_DAMAGE = 2;
const float EXPLOSION_COLLISION_MASS = 7;
const float EXPLOSION_SCALE_SPEED = 4;  // in scales / second

// Point definition of a point explosion.
static int explosion_def[] =
{
	20,
	4,0,EXPLOSION_COLOR,
	3,1,EXPLOSION_COLOR,
	4,1,EXPLOSION_COLOR,
	5,1,EXPLOSION_COLOR,
	4,2,EXPLOSION_COLOR,

	1,3,EXPLOSION_COLOR,
	0,4,EXPLOSION_COLOR,
	1,4,EXPLOSION_COLOR,
	2,4,EXPLOSION_COLOR,
	1,5,EXPLOSION_COLOR,

	6,3,EXPLOSION_COLOR,
	5,4,EXPLOSION_COLOR,
	6,4,EXPLOSION_COLOR,
	7,4,EXPLOSION_COLOR,
	6,5,EXPLOSION_COLOR,

	4,6,EXPLOSION_COLOR,
	3,7,EXPLOSION_COLOR,
	4,7,EXPLOSION_COLOR,
	5,7,EXPLOSION_COLOR,
	4,8,EXPLOSION_COLOR
}; 


class PointExplosion: public PointObject
{
public:
	PointExplosion();

	void Update(BOOL wraparound = TRUE);

protected:
	int explode_timer;
};


#endif