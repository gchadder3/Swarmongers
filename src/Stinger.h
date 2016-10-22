// Stinger.h
//
// The header file for the Stinger class.  The Stinger
// is the lowest class of Terran fighter.

#ifndef _STINGER_H
#define _STINGER_H

// INCLUDES

#include "AIObject.h"


const int STINGER_COLOR = 144;
const int STINGER_HULL_POINTS = 20;
const float STINGER_COLLISION_MASS = 10;
const int STINGER_COLLIDE_DAMAGE = 20;
const float STINGER_FRONT_FOV = HALF_CIRCLE;
const float STINGER_COLLISION_SCARE_FIELD = HALF_CIRCLE;
const float STINGER_COLLISION_SCARE_DIST = 250;
const AIStateType STINGER_DEF_AI_STATE = FighterAssault;
const float STINGER_FOLLOW_DIST = 150;
const float STINGER_TURN_RATE = 3.0; // in radians / sec.
const float STINGER_MAX_SPEED = 350;
const float STINGER_MAIN_THRUST_SPEED = 12; // in pixels / sec.
const float STINGER_FRICTION = 5; // in pixels / sec.
const GameObjectType STINGER_SHOT_TYPE = PlasmaBShotObj;
const int STINGER_SHOT_RELOAD = 1200;
const float STINGER_SHOT_SPEED = 450;
const int STINGER_KILL_SCORE = -200;


// Line definition of a stinger.
static int stinger_def[] =
{
	10,
	1,16,8,0,STINGER_COLOR,
	5,12,8,0,STINGER_COLOR,
	8,12,8,0,STINGER_COLOR,
	11,12,8,0,STINGER_COLOR,
	15,16,8,0,STINGER_COLOR,
	1,16,8,12,STINGER_COLOR,
	8,12,15,16,STINGER_COLOR,
	1,16,5,12,STINGER_COLOR,
	5,12,11,12,STINGER_COLOR,
	11,12,15,16,STINGER_COLOR
};  

class Stinger: public AIObject
{
public:
	// Constructor function.
	Stinger();

	// Return what we think of other object types.
	AlignmentType DispositionTowards(GameObjectType obj_type)
	{
		if ((obj_type == WarhawkObj) ||
			(obj_type == StingerObj))
			return(Friendly);
		else if ((obj_type == PlayerShotObj) ||
			(obj_type == PlasmaAShotObj) ||
			(obj_type == PlasmaBShotObj) ||
			(obj_type == PlasmaCShotObj) ||
			(obj_type == PlasmaDShotObj) ||
			(obj_type == ExplosionObj) ||
			(obj_type == AsteroidObj) ||
			(obj_type == PowerUpObj) ||
			(obj_type == WhirlakillObj))
			return(Neutral);
		else
			return(Hostile);
	}

	void AvatarUpdate();
	void FighterAssaultUpdate();
	void PlayExplodeSound();

protected:
	void PlayThrustSound();
	void PlayFireSound();
};

#endif