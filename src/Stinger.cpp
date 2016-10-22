// Stinger.cpp
//
// Source file for the Stinger class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "AIObject.h"
#include "Stinger.h"


// Globals from main.cpp
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;


// PUBLIC FUNCTIONS

Stinger::Stinger() : AIObject()
{
	Define(stinger_def);
	SetObjectType(StingerObj);
	SetMaxStructPoints(STINGER_HULL_POINTS);
	SetCollisionMass(STINGER_COLLISION_MASS);
	collide_damage = STINGER_COLLIDE_DAMAGE;
	rotation = (float) (rand() % 7);
	front_FOV = STINGER_FRONT_FOV;
	collision_scare_field = STINGER_COLLISION_SCARE_FIELD;
	collision_scare_dist = STINGER_COLLISION_SCARE_DIST;
	default_AIstate = STINGER_DEF_AI_STATE;
	ResetAI();
	follow_distance = STINGER_FOLLOW_DIST;
	turn_rate = STINGER_TURN_RATE;
	precision_steering = FALSE;
	max_speed = STINGER_MAX_SPEED;
	main_thrust_speed = STINGER_MAIN_THRUST_SPEED;
	friction = STINGER_FRICTION;
	SetShotType(STINGER_SHOT_TYPE);
	shot_reload_time = STINGER_SHOT_RELOAD;
	SetShotSpeed(STINGER_SHOT_SPEED);
	SetKillScore(STINGER_KILL_SCORE); 
}

void Stinger::AvatarUpdate()
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

void Stinger::FighterAssaultUpdate()
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
		if ((roll < 30) &&
			(!FriendlyInFrontFOF()))
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

void Stinger::PlayExplodeSound()
{
	sndManager->PlayTerranGenExplode();
}


// PROTECTED FUNCTIONS

void Stinger::PlayThrustSound()
{
	sndManager->PlayStingerThrust();
}

void Stinger::PlayFireSound()
{
	sndManager->PlayTerranGenFire();
}