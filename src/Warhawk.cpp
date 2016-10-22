// Warhawk.cpp
//
// Source file for the Warhawk class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "AIObject.h"
#include "Warhawk.h"


// Globals from main.cpp
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;


// PUBLIC FUNCTIONS

Warhawk::Warhawk() : AIObject()
{
	Define(warhawk_def);
	SetObjectType(WarhawkObj);
	SetMaxStructPoints(WARHAWK_HULL_POINTS);
	SetCollisionMass(WARHAWK_COLLISION_MASS);
	collide_damage = WARHAWK_COLLIDE_DAMAGE;
	rotation = (float) (rand() % 7);
	front_FOV = WARHAWK_FRONT_FOV;
	collision_scare_field = WARHAWK_COLLISION_SCARE_FIELD;
	collision_scare_dist = WARHAWK_COLLISION_SCARE_DIST;
	default_AIstate = WARHAWK_DEF_AI_STATE;
	ResetAI();
	follow_distance = WARHAWK_FOLLOW_DIST;
	turn_rate = WARHAWK_TURN_RATE;
	precision_steering = FALSE;
	max_speed = WARHAWK_MAX_SPEED;
	main_thrust_speed = WARHAWK_MAIN_THRUST_SPEED;
	friction = WARHAWK_FRICTION;
	norm_shot_reload_time = WARHAWK_SHOT_RELOAD_TIME;
	shot_reload_time = norm_shot_reload_time;
	SetShotType(WARHAWK_SHOT_TYPE);
	SetShotSpeed(WARHAWK_SHOT_SPEED);
	SetKillScore(WARHAWK_KILL_SCORE);  // for blowing up your buddy
}

void Warhawk::AvatarUpdate()
{
	if (actionManager->KeyDown(LeftSteerKey))
		Spin();
	else if (actionManager->KeyDown(RightSteerKey))
		Spin(FALSE);
	if (actionManager->KeyDown(MainThrustKey))
	{
		MainThrust();
	}
	if (actionManager->KeyDown(FireForwardKey))
	{
		FireFrontWeapon();
	} 
}

void Warhawk::PlayExplodeSound()
{
	sndManager->PlayTerranGenExplode();
}


// PROTECTED FUNCTIONS

void Warhawk::PlayThrustSound()
{
	sndManager->PlayWarhawkThrust();
}

void Warhawk::PlayFireSound()
{
	sndManager->PlayTerranGenFire();
}

