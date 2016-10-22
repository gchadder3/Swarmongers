// Hornetnest.h
//
// The header file for the Hornetnest class.  The Hornetnest
// is a Grell ship that spawns nasty Hellhornets.


#ifndef _HORNETNEST_H
#define _HORNETNEST_H

// INCLUDES

#include "AIObject.h"


const int HORNETNEST_COLOR1 = 71;
const int HORNETNEST_COLOR2 = 33;
const int HORNETNEST_HULL_POINTS = 150;
const float HORNETNEST_COLLISION_MASS = 50;
const int HORNETNEST_COLLIDE_DAMAGE = 25;
const float HORNETNEST_FRONT_FOV = HALF_CIRCLE;
const float HORNETNEST_COLLISION_SCARE_FIELD = HALF_CIRCLE;
const float HORNETNEST_COLLISION_SCARE_DIST = 250;
const AIStateType HORNETNEST_DEF_AI_STATE = FighterAssault;
const float HORNETNEST_FOLLOW_DIST = 150;
const float HORNETNEST_TURN_RATE = 2.0; // in radians / sec.
const float HORNETNEST_MAX_SPEED = 350;
const float HORNETNEST_MAIN_THRUST_SPEED = 12; // in pixels / sec.
const float HORNETNEST_FRICTION = 5; // in pixels / sec.
const GameObjectType HORNETNEST_SHOT_TYPE = HellhornetObj;
const int HORNETNEST_SHOT_RELOAD = 3000;
const float HORNETNEST_SHOT_SPEED = 350;
const int HORNETNEST_KILL_SCORE = 1000;


// Line definition of a hornetnest.
static int hornetnest_def[] =
{
	22,
	8,0,8,2,HORNETNEST_COLOR1,
	8,0,10,2,HORNETNEST_COLOR1,
	14,2,16,0,HORNETNEST_COLOR1,
	16,0,16,2,HORNETNEST_COLOR1,
	8,2,16,2,HORNETNEST_COLOR1,
	8,2,10,5,HORNETNEST_COLOR1,
	10,5,6,9,HORNETNEST_COLOR1,
	6,9,6,24,HORNETNEST_COLOR1,
	6,24,9,22,HORNETNEST_COLOR1,
	9,22,15,22,HORNETNEST_COLOR1,
	15,22,18,24,HORNETNEST_COLOR1,
	18,24,18,9,HORNETNEST_COLOR1,
	18,9,14,5,HORNETNEST_COLOR1,
	14,5,16,2,HORNETNEST_COLOR1,
	6,13,3,20,HORNETNEST_COLOR1,
	3,20,6,20,HORNETNEST_COLOR1,
	18,13,21,20,HORNETNEST_COLOR1,
	21,20,18,20,HORNETNEST_COLOR1,
	8,17,14,11,HORNETNEST_COLOR2,
	16,17,10,11,HORNETNEST_COLOR2,
	8,17,16,17,HORNETNEST_COLOR2,
	12,9,12,13,HORNETNEST_COLOR2
}; 

class Hornetnest: public AIObject
{
public:
	// Constructor function.
	Hornetnest();

	BOOL CanCollideWith(GameObjectType obj_type)
	{
		if (obj_type == HellhornetObj)
			return(FALSE);
		else
			return(TRUE);
	}

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