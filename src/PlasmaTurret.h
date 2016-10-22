// PlasmaTurret.h
//
// The header file for the PlasmaTurret class.  This
// class defines Grell plasma turrets that shoot large
// white plasma bursts.

#ifndef _PTURRET_H
#define _PTURRET_H

// INCLUDES

#include "AIObject.h"


const int PTURRET_COLOR = 255;
const int PTURRET_HULL_POINTS = 100;
const float PTURRET_COLLISION_MASS = 20;
const int PTURRET_COLLIDE_DAMAGE = 20;
const float PTURRET_FRONT_FOV = HALF_CIRCLE;
const AIStateType PTURRET_DEF_AI_STATE = TurretCamp;
const float PTURRET_MAX_SPEED = 250; // in pixels / sec.
const float PTURRET_TURN_RATE = 2.5; // in radians / sec.
const float PTURRET_FRICTION = 5; // in pixels / sec.
const GameObjectType PTURRET_SHOT_TYPE = PlasmaDShotObj;
const int PTURRET_SHOT_RELOAD = 2000;
const float PTURRET_SHOT_SPEED = 600;
const int PTURRET_KILL_SCORE = 500;


// Line definition of a Grell plasma turret.
static int pturret_def[] =
{
	11,
	11,0,7,15,PTURRET_COLOR,
	19,0,23,15,PTURRET_COLOR,
	22,12,30,15,PTURRET_COLOR,
	30,15,15,30,PTURRET_COLOR,
	15,30,0,15,PTURRET_COLOR,
	0,15,8,12,PTURRET_COLOR,
	0,15,7,15,PTURRET_COLOR,
	7,15,15,17,PTURRET_COLOR,
	15,17,23,15,PTURRET_COLOR,
	23,15,30,15,PTURRET_COLOR,
	15,17,15,30,PTURRET_COLOR
}; 

class PlasmaTurret: public AIObject
{
public:
	// Constructor function.
	PlasmaTurret();

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
	void PlayExplodeSound();

protected:
	void PlayFireSound();
};

#endif