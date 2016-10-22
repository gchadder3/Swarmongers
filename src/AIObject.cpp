// AIObject.cpp
//
// Source file for the AIObject class.  AIObject
// is the base class for all AI-controlled game entities.


// INCLUDES

#include "GPDUMB_includes.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "Vect.h"
#include "GameObject.h"
#include "VectObject.h"
#include "Game.h"
#include "AIObject.h"


// Globals defined in main.cpp
extern Game *theGame;
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;


// PUBLIC FUNCTIONS

void AIObject::Update(BOOL wraparound)
{
	float speed;

	// Only update if alive.
	if (alive)
	{	
		// Go to the appropriate function based on the 
		// AI state.
		switch(AIstate)
		{
		case DoNothing:
			break;
		case Avatar:
			AvatarUpdate();
			break;
		case GoToDefaultState:
			AIstate = default_AIstate;
			break;
		case RamNearestFoe:
			RamNearestFoeUpdate();
			break;
		case SmartRamNearestFoe:
			SmartRamNearestFoeUpdate();
			break;
		case TurretCamp:
			TurretCampUpdate();
			break;
		case FighterAssault:
			FighterAssaultUpdate();
			break;
		case Explore:
			ExploreUpdate();
			break;
		case SmartExplore:
			SmartExploreUpdate();
			break;
		case FollowNearestFriend:
			FollowNearestFriendUpdate();
			break;
		case ManeuverLeft:
			ManeuverLeftUpdate();
			break;
		case ManeuverRight:
			ManeuverRightUpdate();
			break;
		case DodgeLeft:
			DodgeLeftUpdate();
			break;
		case DodgeRight:
			DodgeRightUpdate();
			break;
		case PressForward:
			PressForwardUpdate();
			break;
		}

		// Decrement the state timer.
		if (state_timer > 0)
			theGame->CountdownTickTimer(state_timer);

		// Decrement the shot timer.
		if (shot_timer > 0)
			theGame->CountdownTickTimer(shot_timer);

		// Apply "friction" to the AI's current
		// velocity.
		speed = velocity.get_dist();
		if (speed > friction)
		{
			speed -= friction;
			velocity.set_dist(speed);
		}
		else
		{
			SetVelocity(Vect::radian_fix(rotation - QUARTER_CIRCLE),0);
		} 

		VectObject::Update(wraparound);
	}
}

void AIObject::AddPossibleDetect(GameObject *target)
{
	int target_x;
	int target_y;
	GameObjectType obj_type;
	int obj_id;
	float bearing;
	float range;
	float angle_diff;

	target_x = target->GetPosition().get_x();
	target_y = target->GetPosition().get_y();
	obj_type = target->GetType();
	obj_id = target->GetObjectID();

	// If we have a free detect available and the target
	// is in the AIObject's field-of-vision, add the 
	// detect to the list.
	if ((num_detects < MAX_AI_DETECTS) &&
		(InFrontFOV(target_x, target_y)))
	{
		// Calculate range.
		range = position.distance_to(target_x, target_y);

		// Calculate bearing.
		angle_diff = position.angle_to(target_x, target_y) 
			- rotation;
		if (angle_diff > HALF_CIRCLE)
			bearing = angle_diff - FULL_CIRCLE;
		else if (angle_diff < -HALF_CIRCLE)
			bearing = FULL_CIRCLE + angle_diff;
		else
			bearing = angle_diff;

		// Store the detect and advance the count.
		detects[num_detects].det_type = obj_type;
		detects[num_detects].obj_id = obj_id;
		detects[num_detects].alignment = DispositionTowards(obj_type);
		detects[num_detects].bearing = bearing;
		detects[num_detects].range = range;
		detects[num_detects].location.set(target_x, target_y, FALSE);
		detects[num_detects].velocity.set(target->GetVelocity());
		num_detects++;
	}
}


// PROTECTED FUNCTIONS

// Is a point in our front field-of-vision?
BOOL AIObject::InFrontFOV(float target_x, float target_y)
{
	float range;
	float diff_angle;

	range = position.distance_to(target_x, target_y);
	diff_angle = (float) fabs((double) position.angle_to(target_x, target_y) 
		- rotation);
	if (diff_angle > FULL_CIRCLE - diff_angle)
		diff_angle = FULL_CIRCLE - diff_angle;
	if ((range < sight_dist) &&
		(diff_angle < front_FOV / 2.0))
	{
		return(TRUE);
	}
	else
		return(FALSE); 
}

BOOL AIObject::InFrontFOV(Vect& pos)
{
	return(InFrontFOV(pos.get_x(), pos.get_y()));
}

// Is a point in our front field-of-fire?
BOOL AIObject::InFrontFOF(float target_x, float target_y)
{
	float range;
	float diff_angle;

	range = position.distance_to(target_x, target_y);
	diff_angle = (float) fabs((double) position.angle_to(target_x, target_y) 
		- rotation);
	if (diff_angle > FULL_CIRCLE - diff_angle)
		diff_angle = FULL_CIRCLE - diff_angle;
	if ((range < sight_dist) &&
		(diff_angle < front_FOF / 2.0))
	{
		return(TRUE);
	}
	else
		return(FALSE); 
}

BOOL AIObject::InFrontFOF(Vect& pos)
{
	return(InFrontFOF(pos.get_x(), pos.get_y()));
}

int AIObject::GetNearestFriendly()
{
	float min_range = 2000.0;
	int friend_ind = -1;

	if (num_detects > 0)
	{
		for (int i = 0; i < num_detects; i++)
		{
			if ((detects[i].alignment == Friendly) &&
				(detects[i].range < min_range))
			{
				friend_ind = i;
				min_range = detects[i].range;
			}
		}
	}

	return(friend_ind);
}

int AIObject::GetNearestHostile()
{
	float min_range = 2000.0;
	int hostile_ind = -1;

	if (num_detects > 0)
	{
		for (int i = 0; i < num_detects; i++)
		{
			if ((detects[i].alignment == Hostile) &&
				(detects[i].range < min_range))
			{
				hostile_ind = i;
				min_range = detects[i].range;
			}
		}
	}

	return(hostile_ind);
}

int AIObject::GetNearestKin()
{
	float min_range = 2000.0;
	int kin_ind = -1;

	if (num_detects > 0)
	{
		for (int i = 0; i < num_detects; i++)
		{
			if ((detects[i].det_type == obj_type) &&
				(detects[i].range < min_range))
			{
				kin_ind = i;
				min_range = detects[i].range;
			}
		}
	}

	return(kin_ind);
}

int AIObject::GetNearestDetect()
{
	float min_range = 2000.0;
	int det_ind = -1;

	if (num_detects > 0)
	{
		for (int i = 0; i < num_detects; i++)
		{
			if (detects[i].range < min_range)
			{
				det_ind = i;
				min_range = detects[i].range;
			}
		}
	}

	return(det_ind);
}

// Get the detect index for the nearest collision threat.
// A detect is a collision threat if it is within the AI's
// scare field (both within the angles and within the scare
// range), it can collide with the AI, and it is actually 
// moving towards the AI (relative to how the AI is moving).
int AIObject::GetNearestCollisionThreat()
{
	float min_range = 2000.0;
	int threat_ind = -1;
	Vect detect_to_ai;
	Vect relative_velocity;

	if (num_detects > 0)
	{
		for (int i = 0; i < num_detects; i++)
		{
			detect_to_ai.set(position);
			detect_to_ai.subtract(detects[i].location);
			relative_velocity.set(detects[i].velocity);
			relative_velocity.subtract(velocity);
			if ((detects[i].range < min_range) &&
				(detects[i].range < collision_scare_dist) &&
				(detects[i].bearing < collision_scare_field / 2.0) &&
				(detects[i].bearing > -collision_scare_field / 2.0) &&
				(CanCollideWith(detects[i].det_type)) &&
				(relative_velocity.dot_product(detect_to_ai) > 0.0))
			{
				threat_ind = i;
				min_range = detects[i].range;
			}
		}
	}

	return(threat_ind);
}

int AIObject::LeftCollisionThreat()
{
	int near_threat_ind;

	near_threat_ind = GetNearestCollisionThreat();
	if ((near_threat_ind != -1) &&
		(detects[near_threat_ind].bearing <= 0.0))
		return(near_threat_ind);
	else
		return(-1);
}

int AIObject::RightCollisionThreat()
{
	int near_threat_ind;

	near_threat_ind = GetNearestCollisionThreat();
	if ((near_threat_ind != -1) &&
		(detects[near_threat_ind].bearing > 0.0))
		return(near_threat_ind);
	else
		return(-1);
}

BOOL AIObject::HostileInFrontFOF()
{
	if (num_detects > 0)
	{
		for (int i = 0; i < num_detects; i++)
		{
			if ((detects[i].alignment == Hostile) &&
				(InFrontFOF(detects[i].location)))
				return(TRUE);
		}
	}

	return(FALSE);
}

BOOL AIObject::FriendlyInFrontFOF()
{
	if (num_detects > 0)
	{
		for (int i = 0; i < num_detects; i++)
		{
			if ((detects[i].alignment == Friendly) &&
				(InFrontFOF(detects[i].location)))
				return(TRUE);
		}
	}

	return(FALSE);
}

void AIObject::AvatarUpdate()
{
	if (actionManager->KeyDown(LeftSteerKey))
		Spin();
	else if (actionManager->KeyDown(RightSteerKey))
		Spin(FALSE);
	if (actionManager->KeyDown(MainThrustKey))
		MainThrust(); 
}

void AIObject::RamNearestFoeUpdate()
{
	int nearest_foe_ind;

	// Turn towards the nearest hostile and thrust.
	nearest_foe_ind = GetNearestHostile();
	if (nearest_foe_ind != -1)
	{
		Steer(detects[nearest_foe_ind].location);
		MainThrust();
	}
	// If no enemies are visible spin a little to the left.
	else
	{
		Spin();
	}
}

void AIObject::SmartRamNearestFoeUpdate()
{
	int threat_ind;

	// If a left collision seems imminent, thrust right
	// (unless it's a warhawk which we want to hit).
	if ((threat_ind = LeftCollisionThreat()) != -1)
	{
		if (!ShouldRam(detects[threat_ind].det_type))
		{
			PushAIState(AIstate);
			AIstate = DodgeRight;
			state_timer = 50;
		}
	}
	// Else if a right collision seems imminent, thrust left.
	// (unless it's a warhawk which we want to hit).
	else if ((threat_ind = RightCollisionThreat()) != -1)
	{
		if (!ShouldRam(detects[threat_ind].det_type))
		{
			PushAIState(AIstate);
			AIstate = DodgeLeft;
			state_timer = 50;
		}
	}
	// Else proceed to ram the nearest foe.
	else
		RamNearestFoeUpdate();
}

void AIObject::TurretCampUpdate()
{
	int nearest_foe_ind;

	// Turn towards the nearest hostile and fire.
	nearest_foe_ind = GetNearestHostile();
	if (nearest_foe_ind != -1)
	{
		Steer(detects[nearest_foe_ind].location);
		FireFrontWeapon();
	}
	// If no enemies are visible spin a little to the left.
	else
	{
		Spin();
	}
}

void AIObject::FighterAssaultUpdate()
{
	int threat_ind;
	int nearest_foe_ind;
	int roll;

	nearest_foe_ind = GetNearestHostile();

	// If a left collision seems imminent, maneuver right
	// (unless it's a warhawk which we want to hit).
	if ((threat_ind = LeftCollisionThreat()) != -1)
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
	else if (nearest_foe_ind != -1)
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

void AIObject::ExploreUpdate()
{
	MainThrust();
	if (explore_timer <= 0)
	{
		explore_timer = explore_leg_duration;
		if (rand() % 100 < 50)
		{
			PushAIState(AIstate);
			AIstate = ManeuverLeft;
			state_timer = 500; // in msec
		}
		else
		{
			PushAIState(AIstate);
			AIstate = ManeuverRight;
			state_timer = 500; // in msec
		}
	}

	// Decrement the explore timer.
	if (explore_timer > 0)
		theGame->CountdownTickTimer(explore_timer);
}

void AIObject::SmartExploreUpdate()
{
	int threat_ind;

	// If a left collision seems imminent, thrust right
	// (unless it's a warhawk which we want to hit).
	if ((threat_ind = LeftCollisionThreat()) != -1)
	{
		if (!ShouldRam(detects[threat_ind].det_type))
		{
			PushAIState(AIstate);
			AIstate = DodgeRight;
			state_timer = 50;
		}
	}
	// Else if a right collision seems imminent, thrust left.
	// (unless it's a warhawk which we want to hit).
	else if ((threat_ind = RightCollisionThreat()) != -1)
	{
		if (!ShouldRam(detects[threat_ind].det_type))
		{
			PushAIState(AIstate);
			AIstate = DodgeLeft;
			state_timer = 50;
		}
	}
	// Else proceed to explore.
	else
		ExploreUpdate();
}

void AIObject::FollowNearestFriendUpdate()
{
	int threat_ind;
	int nearest_friend_ind = GetNearestFriendly();

	// If a left collision seems imminent, thrust right
	// (unless it's a warhawk which we want to hit).
	if ((threat_ind = LeftCollisionThreat()) != -1)
	{
		if (!ShouldRam(detects[threat_ind].det_type))
		{
			PushAIState(AIstate);
			AIstate = DodgeRight;
			state_timer = 50;
		}
	}
	// Else if a right collision seems imminent, thrust left.
	// (unless it's a warhawk which we want to hit).
	else if ((threat_ind = RightCollisionThreat()) != -1)
	{
		if (!ShouldRam(detects[threat_ind].det_type))
		{
			PushAIState(AIstate);
			AIstate = DodgeLeft;
			state_timer = 50;
		}
	}
	// Else proceed to follow the nearest friendly (if
	// there is one in sight).
	else if (nearest_friend_ind != -1)
	{
		Steer(detects[nearest_friend_ind].location);

		// Unless the guys already too close, thrust.
		if ((detects[nearest_friend_ind].range > follow_distance) &&
			(velocity.get_dist() < 400.0))
			MainThrust();
	}
	// If no friends are visible spin a little to the left.
	else
	{
		Spin();
	}
}

void AIObject::ManeuverLeftUpdate()
{
	Spin();
	if (fabs(velocity.get_dist()) < 400.0)
		MainThrust();
	if (state_timer <= 0)
	{
		AIstate = PopAIState();
	}
}

void AIObject::ManeuverRightUpdate()
{
	Spin(FALSE);
	if (fabs(velocity.get_dist()) < 400.0)
		MainThrust();
	if (state_timer <= 0)
	{
		AIstate = PopAIState();
	}
}

void AIObject::DodgeLeftUpdate()
{
	LeftThrust();
	if (state_timer <= 0)
	{
		AIstate = PopAIState();
	}
}

void AIObject::DodgeRightUpdate()
{
	RightThrust();
	if (state_timer <= 0)
	{
		AIstate = PopAIState();
	}
}

void AIObject::PressForwardUpdate()
{
	MainThrust();
	if (state_timer <= 0)
	{
		AIstate = PopAIState();
	}
}

void AIObject::Steer(float steer_x, float steer_y)
{
	float diff_angle;
	float turn_rate_rpf;  // turn rate in radians / frame

	// turn towards the steer-target
	diff_angle = position.angle_to(steer_x, steer_y) 
		- rotation;
	turn_rate_rpf = turn_rate / theGame->GetCurrentFPS();
	if ((diff_angle < turn_rate_rpf) && 
		(diff_angle > -turn_rate_rpf) &&
		(precision_steering))
		Rotate(diff_angle);
	else
	{
		if(diff_angle < -HALF_CIRCLE || 
			(diff_angle > 0 && diff_angle < HALF_CIRCLE))
			Rotate(turn_rate_rpf);	// turn right
		else
			Rotate(-turn_rate_rpf); // turn left
	}
}

void AIObject::Steer(Vect& v)
{
	Steer(v.get_x(), v.get_y());
}

void AIObject::Spin(BOOL left)
{
	if (left)
		Rotate(-turn_rate / theGame->GetCurrentFPS());
	else
		Rotate(turn_rate / theGame->GetCurrentFPS());
}

void AIObject::MainThrust()
{
	float speed;

	velocity.add(Vect::radian_fix(rotation - QUARTER_CIRCLE), main_thrust_speed);
	speed = velocity.get_dist();
	if (speed > max_speed)
	{
		speed = max_speed;
		velocity.set_dist(speed);
	}
	PlayThrustSound();
}

void AIObject::LeftThrust()
{
	float speed;

	velocity.add(Vect::radian_fix(rotation - HALF_CIRCLE), main_thrust_speed);
	speed = velocity.get_dist();
	if (speed > max_speed)
	{
		speed = max_speed;
		velocity.set_dist(speed);
	}
	PlayThrustSound();
}

void AIObject::RightThrust()
{
	float speed;

	velocity.add(Vect::radian_fix(rotation), main_thrust_speed);
	speed = velocity.get_dist();
	if (speed > max_speed)
	{
		speed = max_speed;
		velocity.set_dist(speed);
	}
	PlayThrustSound();
}

void AIObject::FireFrontWeapon()
{
	int shot_x;
	int shot_y;
	float shot_ang;
	Vect shot_vect;
	char buffer[80];

	// If we've passed the correct reload time.
	if (shot_timer <= 0)
	{
		// Set the shot initial position and angle.
		shot_ang = rotation - QUARTER_CIRCLE;
		shot_vect.set(shot_ang, collision_rad + theGame->GetObjectCollisionRad(shot_type) + 1.0);
//		shot_vect.dbg_display(20,30);
		shot_vect.add(position.get_x(), position.get_y(), FALSE);
//		shot_vect.dbg_display(20,70);
		shot_x = (int) shot_vect.get_x();
		shot_y = (int) shot_vect.get_y();

		// If we are avatar try to spawn the shot on 
		// the player's shot list.
		if ((IsAvatar()) && (GetType() == WarhawkObj))
		{
			// If we successfully spawn a shot...
			if (theGame->SpawnPlayerShot(shot_x, shot_y, shot_ang, this))
			{
				// Set the shot clock for a countdown.
				shot_timer = shot_reload_time;

				// Play a shot sound.
				PlayFireSound();
			}
		}
		// Otherwise, spawn the appropriate shot object
		else
		{
			// If we successfully spawn a shot...
			if (theGame->SpawnGameObject(shot_type, shot_x, shot_y, shot_ang, shot_speed, this))
			{
				// Set the shot clock for a countdown.
				shot_timer = shot_reload_time;

				// Play a shot sound.
				PlayFireSound();
			}
		}

		// If we're in triple-fire mode add two more 
		// shots at +- 30 degrees.
		if (triple_fire)
		{
			// Aim the second shot for -30 degrees.

			// Set the shot initial position and angle.
			shot_ang = rotation - QUARTER_CIRCLE - TWELFTH_CIRCLE;
			shot_vect.set(shot_ang, collision_rad + theGame->GetObjectCollisionRad(shot_type) + 1.0);
//			shot_vect.dbg_display(20,30);
			shot_vect.add(position.get_x(), position.get_y(), FALSE);
//			shot_vect.dbg_display(20,70);
			shot_x = (int) shot_vect.get_x();
			shot_y = (int) shot_vect.get_y();

			// If we are avatar try to spawn the shot on 
			// the player's shot list.
			if ((IsAvatar()) && (GetType() == WarhawkObj))
			{
				// If we successfully spawn a shot...
				if (theGame->SpawnPlayerShot(shot_x, shot_y, shot_ang, this))
				{
					// Set the shot clock for a countdown.
					shot_timer = shot_reload_time;
				}
			}
			// Otherwise, spawn the appropriate shot object
			else
			{
				// If we successfully spawn a shot...
				if (theGame->SpawnGameObject(shot_type, shot_x, shot_y, shot_ang, shot_speed, this))
				{
					// Set the shot clock for a countdown.
					shot_timer = shot_reload_time;
				}
			}

			// Aim the third shot for +30 degrees.

			// Set the shot initial position and angle.
			shot_ang = rotation - QUARTER_CIRCLE + TWELFTH_CIRCLE;
			shot_vect.set(shot_ang, collision_rad + theGame->GetObjectCollisionRad(shot_type) + 1.0);
//			shot_vect.dbg_display(20,30);
			shot_vect.add(position.get_x(), position.get_y(), FALSE);
//			shot_vect.dbg_display(20,70);
			shot_x = (int) shot_vect.get_x();
			shot_y = (int) shot_vect.get_y();

			// If we are avatar try to spawn the shot on 
			// the player's shot list.
			if ((IsAvatar()) && (GetType() == WarhawkObj))
			{
				// If we successfully spawn a shot...
				if (theGame->SpawnPlayerShot(shot_x, shot_y, shot_ang, this))
				{
					// Set the shot clock for a countdown.
					shot_timer = shot_reload_time;
				}
			}
			// Otherwise, spawn the appropriate shot object
			else
			{
				// If we successfully spawn a shot...
				if (theGame->SpawnGameObject(shot_type, shot_x, shot_y, shot_ang, shot_speed, this))
				{
					// Set the shot clock for a countdown.
					shot_timer = shot_reload_time;
				}
			}
		}
	}
}

void AIObject::PlayThrustSound()
{
	// Play nothing.  Default is silent thrust.
}

void AIObject::PlayFireSound()
{
	sndManager->PlayGrellGenFire();
}

