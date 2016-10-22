// Dreadnaught.cpp
//
// Source file for the Dreadnaught class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "AIObject.h"
#include "Dreadnaught.h"


// Globals from main.cpp
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;


// PUBLIC FUNCTIONS

Dreadnaught::Dreadnaught() : AIObject()
{
	Define(dreadnaught_def);
	SetObjectType(DreadnaughtObj);
	SetMaxStructPoints(DREADNAUGHT_HULL_POINTS);
	SetCollisionMass(DREADNAUGHT_COLLISION_MASS);
	collide_damage = DREADNAUGHT_COLLIDE_DAMAGE;
	rotation = (float) (rand() % 7);
	front_FOV = DREADNAUGHT_FRONT_FOV;
	collision_scare_field = DREADNAUGHT_COLLISION_SCARE_FIELD;
	collision_scare_dist = DREADNAUGHT_COLLISION_SCARE_DIST;
	default_AIstate = DREADNAUGHT_DEF_AI_STATE;
	ResetAI();
	follow_distance = DREADNAUGHT_FOLLOW_DIST;
	turn_rate = DREADNAUGHT_TURN_RATE;
	precision_steering = FALSE;
	max_speed = DREADNAUGHT_MAX_SPEED;
	main_thrust_speed = DREADNAUGHT_MAIN_THRUST_SPEED;
	friction = DREADNAUGHT_FRICTION;
	SetShotType(DREADNAUGHT_SHOT_TYPE);
	shot_reload_time = DREADNAUGHT_SHOT_RELOAD;
	SetShotSpeed(DREADNAUGHT_SHOT_SPEED);
	SetKillScore(DREADNAUGHT_KILL_SCORE); 
	SetScale(4.0);
}

void Dreadnaught::AvatarUpdate()
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

void Dreadnaught::FighterAssaultUpdate()
{
	int threat_ind;
	int nearest_foe_ind;
	int roll;

	nearest_foe_ind = GetNearestHostile();

	// If a left collision seems imminent, maneuver right
	// (unless it's a warhawk which we want to hit).
/*	if ((threat_ind = LeftCollisionThreat()) != -1)
	{
		if (!ShouldRam(detects[threat_ind].det_type))
		{
			PushAIState(AIstate);
			AIstate = ManeuverRight;
			state_timer = 50; // in msec
		}
	}
	// Else if a right collision seems imminent, maneuver left.
	// (unless it's a warhawk which we want to hit).
	else if ((threat_ind = RightCollisionThreat()) != -1)
	{
		if (!ShouldRam(detects[threat_ind].det_type))
		{
			PushAIState(AIstate);
			AIstate = ManeuverLeft;
			state_timer = 50; // in msec
		}
	}
	// Turn towards the nearest hostile and either fire 
	// (if there are no friendlies in the FOF) or
	// (if the target isn't too close) thrust.
	else*/ if (nearest_foe_ind != -1)
	{
		Steer(detects[nearest_foe_ind].location);
		roll = rand() % 100;
		// 30% of the time, try to fire.
		if (roll < 30)
			FireFrontWeapon();
		// 10% of the time try to move forward.
		else if ((roll < 40) && 
			(detects[nearest_foe_ind].range > collision_scare_dist))
			MainThrust();
	}
	// If no enemies are visible spin a little to the left.
	else
	{
		Spin();
	}
}

void Dreadnaught::PlayExplodeSound()
{
	if (!IsAlive())
		sndManager->PlayDreadnaughtExplode();
	else if (struct_points > max_struct_points / 2)
		sndManager->PlayDreadnaughtLtDamage();
	else
		sndManager->PlayDreadnaughtHvDamage(); 
}


// PROTECTED FUNCTIONS

void Dreadnaught::PlayThrustSound()
{
	sndManager->PlayGrellGenThrust();
}

void Dreadnaught::PlayFireSound()
{
	sndManager->PlayDreadnaughtFire();
}