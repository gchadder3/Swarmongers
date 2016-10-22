// Warhawk.h
//
// The header file for the Warhawk class.  Typically, the 
// player's ship is a Warhawk.

#ifndef _WARHAWK_H
#define _WARHAWK_H

// INCLUDES

#include "AIObject.h"


const int WARHAWK_COLOR = 133;
const int WARHAWK_HULL_POINTS = 100;
const float WARHAWK_COLLISION_MASS = 20;
const float WARHAWK_COLLIDE_DAMAGE = 20;
const float WARHAWK_FRONT_FOV = FULL_CIRCLE;
const float WARHAWK_COLLISION_SCARE_FIELD = FULL_CIRCLE;
const float WARHAWK_COLLISION_SCARE_DIST = 150;
const AIStateType WARHAWK_DEF_AI_STATE = FighterAssault;
const float WARHAWK_FOLLOW_DIST = 250;
const float WARHAWK_TURN_RATE = 4.0; // in radians / sec.
const float WARHAWK_MAX_SPEED = 600;
const float WARHAWK_MAIN_THRUST_SPEED = 25; // in pixels / sec.
const float WARHAWK_FRICTION = 3; // in pixels / sec.
const int WARHAWK_SHOT_RELOAD_TIME = 250;
const GameObjectType WARHAWK_SHOT_TYPE = PlasmaAShotObj;
const float WARHAWK_SHOT_SPEED = 600;
const int WARHAWK_KILL_SCORE = -1000;


// Line definition of a player shot.
static int warhawk_def[] =
{
	8,
	10,0,6,24,WARHAWK_COLOR,
	6,24,12,16,WARHAWK_COLOR,
	12,16,0,24,WARHAWK_COLOR,
	0,24,12,8,WARHAWK_COLOR,
	12,8,24,24,WARHAWK_COLOR,
	24,24,12,16,WARHAWK_COLOR,
	12,16,18,24,WARHAWK_COLOR,
	18,24,14,0,WARHAWK_COLOR
}; 


class Warhawk: public AIObject
{
public:
	// Constructor function.
	Warhawk();

	// Return what we think of other object types.
	virtual AlignmentType DispositionTowards(GameObjectType obj_type)
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
	virtual void PlayExplodeSound();

protected:
	virtual void PlayThrustSound();
	virtual void PlayFireSound();
};

#endif