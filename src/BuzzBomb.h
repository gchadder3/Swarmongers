// BuzzBomb.h
//
// The header file for the BuzzBomb class.  BuzzBomb 
// defines and manages BuzzBomb objects in the game.

#ifndef _BUZZBOMB_H
#define _BUZZBOMB_H

#include "AIObject.h"
//#include "SoundManager.h"

const int BUZZBOMB_COLOR = 255;
const int BUZZBOMB_HULL_POINTS = 10;
const int BUZZBOMB_DAMAGE = 100;
const float BUZZBOMB_COLLISION_MASS = 5; 
const float BUZZBOMB_FRONT_FOV = FULL_CIRCLE;
const AIStateType BUZZBOMB_DEF_AI_STATE = RamNearestFoe;
const int BUZZBOMB_KILL_SCORE = 100;

//extern SoundManager *sndManager;


// Line definition of a buzz bomb.
static int buzzbomb_def[] =
{
	3,
	0,0,8,8,BUZZBOMB_COLOR,
	8,0,0,8,BUZZBOMB_COLOR,
	4,0,4,8,BUZZBOMB_COLOR
};


class BuzzBomb: public AIObject
{
public:
	BuzzBomb();

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