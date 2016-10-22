// PlasmaTurret.cpp
//
// Source file for the PlasmaTurret class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "AIObject.h"
#include "PlasmaTurret.h"


// Globals from main.cpp
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;


// PUBLIC FUNCTIONS

PlasmaTurret::PlasmaTurret() : AIObject()
{
	Define(pturret_def);
	SetObjectType(PlasmaTurretObj);
	SetMaxStructPoints(PTURRET_HULL_POINTS);
	SetCollisionMass(PTURRET_COLLISION_MASS);
	collide_damage = PTURRET_COLLIDE_DAMAGE;
	rotation = (float) (rand() % 7);
	front_FOV = PTURRET_FRONT_FOV;
	default_AIstate = PTURRET_DEF_AI_STATE;
	ResetAI();
	turn_rate = PTURRET_TURN_RATE;
	precision_steering = FALSE;
	max_speed = PTURRET_MAX_SPEED;
	friction = PTURRET_FRICTION;
	SetShotType(PTURRET_SHOT_TYPE);
	shot_reload_time = PTURRET_SHOT_RELOAD;
	SetShotSpeed(PTURRET_SHOT_SPEED);
	SetKillScore(PTURRET_KILL_SCORE); 
}

void PlasmaTurret::AvatarUpdate()
{ 
	if (actionManager->KeyDown(LeftSteerKey))
		Spin();
	else if (actionManager->KeyDown(RightSteerKey))
		Spin(FALSE);
	if (actionManager->KeyDown(FireForwardKey))
	{
		FireFrontWeapon();
	} 
}

void PlasmaTurret::PlayExplodeSound()
{
	sndManager->PlayGrellGenExplode();
}


// PROTECTED FUNCTIONS

void PlasmaTurret::PlayFireSound()
{
	sndManager->PlayGrellGenFire();
}