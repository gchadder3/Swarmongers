// Dreadnaught.h
//
// The header file for the Dreadnaught class.  The Dreadnaught
// is a big, nasty Grell ship.


#ifndef _DREADNAUGHT_H
#define _DREADNAUGHT_H

// INCLUDES

#include "AIObject.h"


const int DREADNAUGHT_COLOR1 = 100;
const int DREADNAUGHT_COLOR2 = 33;
const int DREADNAUGHT_HULL_POINTS = 2000;
const float DREADNAUGHT_COLLISION_MASS = 1000;
const int DREADNAUGHT_COLLIDE_DAMAGE = 30;
const float DREADNAUGHT_FRONT_FOV = HALF_CIRCLE;
const float DREADNAUGHT_COLLISION_SCARE_FIELD = HALF_CIRCLE;
const float DREADNAUGHT_COLLISION_SCARE_DIST = 500;
const AIStateType DREADNAUGHT_DEF_AI_STATE = FighterAssault;
const float DREADNAUGHT_FOLLOW_DIST = 500;
const float DREADNAUGHT_TURN_RATE = 0.75; // in radians / sec.
const float DREADNAUGHT_MAX_SPEED = 350;
const float DREADNAUGHT_MAIN_THRUST_SPEED = 12; // in pixels / sec.
const float DREADNAUGHT_FRICTION = 10; // in pixels / sec.
const GameObjectType DREADNAUGHT_SHOT_TYPE = DreadnaughtShotObj;
const int DREADNAUGHT_SHOT_RELOAD = 1000;
const float DREADNAUGHT_SHOT_SPEED = 600;
const int DREADNAUGHT_KILL_SCORE = 10000;

// Line definition of a hornetnest.
static int dreadnaught_def[] =
{
	28,
	35,0,45,0,DREADNAUGHT_COLOR1,
	45,0,55,10,DREADNAUGHT_COLOR1,
	55,10,48,25,DREADNAUGHT_COLOR1,
	48,25,60,35,DREADNAUGHT_COLOR1,
	60,35,60,80,DREADNAUGHT_COLOR1,
	60,80,54,80,DREADNAUGHT_COLOR1,
	54,80,54,50,DREADNAUGHT_COLOR1,
	54,50,48,55,DREADNAUGHT_COLOR1,
	48,55,43,50,DREADNAUGHT_COLOR1,
	43,50,43,80,DREADNAUGHT_COLOR1,
	43,80,37,80,DREADNAUGHT_COLOR1,
	37,80,37,50,DREADNAUGHT_COLOR1,
	37,50,32,55,DREADNAUGHT_COLOR1,
	32,55,26,50,DREADNAUGHT_COLOR1,
	26,50,26,80,DREADNAUGHT_COLOR1,
	26,80,20,80,DREADNAUGHT_COLOR1,
	20,80,20,35,DREADNAUGHT_COLOR1,
	20,35,32,25,DREADNAUGHT_COLOR1,
	32,25,25,10,DREADNAUGHT_COLOR1,
	25,10,35,0,DREADNAUGHT_COLOR1,
	28,16,22,22,DREADNAUGHT_COLOR1,
	22,22,31,22,DREADNAUGHT_COLOR1,
	52,16,58,22,DREADNAUGHT_COLOR1,
	58,22,49,22,DREADNAUGHT_COLOR1,
	36,3,44,3,DREADNAUGHT_COLOR1,
	44,3,42,13,DREADNAUGHT_COLOR1,
	42,13,38,13,DREADNAUGHT_COLOR1,
	38,13,36,3,DREADNAUGHT_COLOR1
}; 

class Dreadnaught: public AIObject
{
public:
	// Constructor function.
	Dreadnaught();

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
			(obj_type == DreadnaughtShotObj) ||
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