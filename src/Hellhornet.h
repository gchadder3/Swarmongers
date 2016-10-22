// Hellhornet.h
//
// The header file for the Hellhornet class.  

#ifndef _HELLHORNET_H
#define _HELLHORNET_H

#include "AIObject.h"
//#include "SoundManager.h"

const int HELLHORNET_COLOR = 32;
const int HELLHORNET_DAMAGE = 25;
const float HELLHORNET_COLLISION_MASS = 5; 
const AIStateType HELLHORNET_DEF_AI_STATE = SmartRamNearestFoe;
const int HELLHORNET_KILL_SCORE = 50;

// Line definition of a buzz bomb.
static int hellhornet_def[] =
{
	4,
	2,2,8,8,HELLHORNET_COLOR,
	6,2,0,8,HELLHORNET_COLOR,
	4,0,4,4,HELLHORNET_COLOR,
	0,8,8,8,HELLHORNET_COLOR
};


class Hellhornet: public AIObject
{
public:
	Hellhornet();

	BOOL CanCollideWith(GameObjectType obj_type)
	{
		if ((obj_type == HellhornetObj) ||
			(obj_type == HornetnestObj))
			return(FALSE);
		else
			return(TRUE);
	}

	BOOL ShouldRam(GameObjectType obj_type)
	{
		if (DispositionTowards(obj_type) == Hostile)
			return(TRUE);
		else
			return(FALSE);
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

	void PlayExplodeSound();

protected:
	void PlayThrustSound();
};

#endif