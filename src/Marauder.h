// Marauder.h
//
// The header file for the Marauder class.  The Marauder
// is the lowest class of Grell fighter.

#ifndef _MARAUDER_H
#define _MARAUDER_H

// INCLUDES

#include "AIObject.h"


const int MARAUDER_COLOR = 103;
const int MARAUDER_HULL_POINTS = 20;
const float MARAUDER_COLLISION_MASS = 10;
const int MARAUDER_COLLIDE_DAMAGE = 20;
const float MARAUDER_FRONT_FOV = HALF_CIRCLE;
const float MARAUDER_COLLISION_SCARE_FIELD = HALF_CIRCLE;
const float MARAUDER_COLLISION_SCARE_DIST = 250;
const AIStateType MARAUDER_DEF_AI_STATE = FighterAssault;
const float MARAUDER_FOLLOW_DIST = 150;
const float MARAUDER_TURN_RATE = 3.0; // in radians / sec.
const float MARAUDER_MAX_SPEED = 350;
const float MARAUDER_MAIN_THRUST_SPEED = 12; // in pixels / sec.
const float MARAUDER_FRICTION = 5; // in pixels / sec.
const GameObjectType MARAUDER_SHOT_TYPE = PlasmaBShotObj;
const int MARAUDER_SHOT_RELOAD = 1200;
const float MARAUDER_SHOT_SPEED = 450;
const int MARAUDER_KILL_SCORE = 100;


// Line definition of a marauder.
static int marauder_def[] =
{
	15,
	8,0,10,3,MARAUDER_COLOR,
	10,3,10,7,MARAUDER_COLOR,
	10,7,16,12,MARAUDER_COLOR,
	16,12,16,16,MARAUDER_COLOR,
	16,16,10,12,MARAUDER_COLOR,
	10,12,6,12,MARAUDER_COLOR,
	6,12,0,16,MARAUDER_COLOR,
	0,16,0,12,MARAUDER_COLOR,
	0,12,6,7,MARAUDER_COLOR,
	6,7,6,3,MARAUDER_COLOR,
	6,3,8,0,MARAUDER_COLOR,
	6,7,8,3,MARAUDER_COLOR,
	8,3,10,7,MARAUDER_COLOR,
	6,12,8,9,MARAUDER_COLOR,
	8,9,10,12,MARAUDER_COLOR
}; 

class Marauder: public AIObject
{
public:
	// Constructor function.
	Marauder();

	// Return what we think of other object types.
	AlignmentType DispositionTowards(GameObjectType obj_type)
	{
		if ((obj_type == WarhawkObj) ||
			(obj_type == StingerObj))
			return(Hostile);
		else if ((obj_type == PlayerShotObj) ||
			(obj_type == PlasmaAShotObj) ||
			(obj_type == PlasmaBShotObj) ||
			(obj_type == PlasmaCShotObj) ||
			(obj_type == PlasmaDShotObj) ||
			(obj_type == ExplosionObj) ||
			(obj_type == AsteroidObj) ||
			(obj_type == WhirlakillObj))
			return(Neutral);
		else
			return(Friendly);
	}

	void AvatarUpdate();
	void FighterAssaultUpdate();
	void PlayExplodeSound();

protected:
	void PlayThrustSound();
	void PlayFireSound();
};

#endif