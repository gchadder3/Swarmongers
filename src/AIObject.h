// AIObject.h
//
// The header file for the AIObject class.  AIObject
// is the base class for all AI-controlled game entities.
// One main thing the AIObjects have that generic VectObjects
// don't is a sensory input.  VectObjects may enact behaviors
// but these behaviors cannot be influenced by the environment
// (save the occasion when something collides with them).

#ifndef _AI_OBJECT_H
#define _AI_OBJECT_H

// INCLUDES

#include "GameObject.h"
#include "VectObject.h"
#include "Vect.h"


const int MAX_AI_DETECTS = 20;
const float DEFAULT_FRONT_FOV = QUARTER_CIRCLE;  // in radians
const float DEFAULT_FRONT_FOF = TWELFTH_CIRCLE;  // in radians
const float DEFAULT_SIGHT_DIST = 1024;  // in pixels
const float DEFAULT_TURN_RATE = 2.0;  // in radians / sec.
const float DEFAULT_MAX_SPEED = 150;  // in pps
const float DEFAULT_FRICTION = 0;  // in pps
const int NUM_STACK_AI_STATES = 5;
const int DEFAULT_EXPLORE_LEG_DURATION = 3000; // in msec.
const float DEFAULT_FOLLOW_DIST = 150; // in pixels
const float DEFAULT_MAIN_THRUST_SPEED = 5.0;  // in pps
const float DEFAULT_SIDE_THRUST_SPEED = 10.0;  // in pps
const float DEFAULT_COLLISION_SCARE_DIST = 100; // in pixels
const int DEFAULT_SHOT_RELOAD_TIME = 300;  // in milliseconds (ticks)
const int DEFAULT_RSHOT_RELOAD_TIME = 60;  // same for rapid-fire mode
const GameObjectType DEFAULT_SHOT_TYPE = PlasmaBShotObj;
const int DEFAULT_SHOT_SPEED = 400;  // in pps


// The types of states the AI can be in.  The behavior 
// described here can be overridden by AIObject subclasses.
enum AIStateType
{
	DoNothing,
		// Sit and wait without even thrusting or turning.
	Avatar,
		// Move according to player commands.
	GoToDefaultState,
		// Change to the default state.  Use at initialization
		// or when autopilot is engaged (and the AIObject is
		// therefore coming out of Avatar mode).
	RamNearestFoe, 
		// Move towards the nearest foe and crash into him.
		// If you don't see a foe, spin a little to the left.
	SmartRamNearestFoe,
		// Same as above, except if you avoid see neutrals
		// (which include shots) or friendlies, you dodge 
		// to avoid hitting them.
	TurretCamp,
		// Sit tight and turn towards the nearest hostile 
		// and fire.  If no enemies are in sight, spin a 
		// little to the left.
	FighterAssault,
		// If there are collisions, dodge to avoid them.
		// Otherwise, turn towards the nearest hostile and 
		// randomly fire or thrust.
	Explore,
		// Move in a straight line for a certain period of 
		// time, then maneuver randomly left or right, and 
		// repeat the cycle.
	SmartExplore,
		// Same as above, except if you avoid see neutrals
		// (which include shots) or friendlies, you dodge 
		// to avoid hitting them.
	FollowNearestFriend,
		// Follow the nearest friend you see, trying to 
		// maintain a particular following distance.
	ManeuverLeft,
		// Turn left a specified number of radians.
	ManeuverRight,
		// Turn right a specified number of radians.
	DodgeLeft,
		// Slip left using a leftwards side-thrust.
	DodgeRight,
		// Slip right using a leftwards side-thrust.
	PressForward
		// Move forward for a specified duration.
};

// What sides can a ship be on?
enum AlignmentType
{
	Hostile,	// attacks the player and friendlies
	Neutral,	// ignores everybody else
	Friendly	// attacks hostiles
};

// The structure for a detect.  A detect represents an AI's 
// perception of another game object in their field of vision.
struct Detect
{
	GameObjectType det_type;	// what is it?
	int obj_id;					// what's its ID?
	AlignmentType alignment;	// whose side is it on?
	float bearing;				// what angle is it at relative
								// to our heading?
	float range;				// how far is it away?
	Vect location;				// where exactly is it?
	Vect velocity;				// what direction and speed is it
								// moving?
};


class AIObject: public VectObject
{
public:
	AIObject() : VectObject()
	{
		MakeHaveAI();

		SetVelocity(0.0, 0.0);

		num_detects = 0;

		front_FOV = DEFAULT_FRONT_FOV;
		front_FOF = DEFAULT_FRONT_FOF;
		sight_dist = DEFAULT_SIGHT_DIST;

		default_AIstate = DoNothing;
		AIstate = GoToDefaultState;
		num_stack_states = 0;
		state_timer = 0;
		explore_timer = 0;
		explore_leg_duration = DEFAULT_EXPLORE_LEG_DURATION;
		follow_distance = DEFAULT_FOLLOW_DIST;

		turn_rate = DEFAULT_TURN_RATE;
		precision_steering = TRUE;

		max_speed = DEFAULT_MAX_SPEED;
		friction = DEFAULT_FRICTION;
		main_thrust_speed = DEFAULT_MAIN_THRUST_SPEED;
		side_thrust_speed = DEFAULT_SIDE_THRUST_SPEED;
		collision_scare_dist = DEFAULT_COLLISION_SCARE_DIST;
		collision_scare_field = front_FOV;  
			// Default the AI to using its full field of vision
			// in looking for collision threats.

		norm_shot_reload_time = DEFAULT_SHOT_RELOAD_TIME;
		rapid_shot_reload_time = DEFAULT_RSHOT_RELOAD_TIME;
		shot_type = DEFAULT_SHOT_TYPE;
		shot_speed = DEFAULT_SHOT_SPEED;
		shot_reload_time = norm_shot_reload_time;
		shot_timer = 0;
		triple_fire = FALSE;
		rapid_fire = FALSE;
	}

	void SetAIState(AIStateType state)
	{
		AIstate = state;
	}

	void ResetAI()
	{
		AIstate = GoToDefaultState;
	}

	void PushAIState(AIStateType state)
	{
		if (num_stack_states < NUM_STACK_AI_STATES)
		{
			state_stack[num_stack_states] = state;
			num_stack_states++;
		}
	}

	AIStateType PopAIState()
	{
		AIStateType state;

		if (num_stack_states > 0)
		{
			num_stack_states--;
			return(state_stack[num_stack_states]);
		}
	}

	BOOL IsAvatar()
	{
		return(AIstate == Avatar);
	}

	void MakeAvatar()
	{
		AIstate = Avatar;
	}

	void MakeNotAvatar()
	{
		AIstate = GoToDefaultState;
	}

	void ClearDetects()
	{
		num_detects = 0;
	}

	void AddPossibleDetect(GameObject *target);

	virtual void Update(BOOL wraparound = TRUE);

	void SetShotType(GameObjectType obj_type)
	{
		shot_type = obj_type;
	}

	void SetShotSpeed(float speed)
	{
		shot_speed = speed;
	}

	BOOL IsTripleFireOn()
	{
		return triple_fire;
	}

	BOOL IsRapidFireOn()
	{
		return rapid_fire;
	}

	void ToggleTripleFire()
	{
		triple_fire = !triple_fire;
	}

	void ToggleRapidFire()
	{
		rapid_fire = !rapid_fire;
		if (rapid_fire)
			shot_reload_time = rapid_shot_reload_time;
		else
			shot_reload_time = norm_shot_reload_time;
	}

	void TripleFireOn()
	{
		triple_fire = TRUE;
	}

	void TripleFireOff()
	{
		triple_fire = FALSE;
	}

	void RapidFireOn()
	{
		rapid_fire = TRUE;
		shot_reload_time = rapid_shot_reload_time;
	}

	void RapidFireOff()
	{
		rapid_fire = FALSE;
		shot_reload_time = norm_shot_reload_time;
	}

	virtual void PlayExplodeSound()
	{ 
		VectObject::PlayExplodeSound();
	}

	// Use Draw_Text to debug display the AI info.
	virtual void dbg_display(int disp_x, int disp_y)
	{
		char buffer[80];
		int old_disp_y = disp_y;

		VectObject::dbg_display(disp_x, disp_y);
		disp_y += 225;

		sprintf(buffer,"Front FOV (rad): %.2f",front_FOV);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;
		
		sprintf(buffer,"Sight Distance (pix): %.2f",sight_dist);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Collision Scare Field (rad): %.2f",collision_scare_field);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;
		
		sprintf(buffer,"Collision Scare Distance (pix): %.2f",collision_scare_dist);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		if (AIstate == DoNothing)
			sprintf(buffer, "AI State: Do Nothing"); 
		else if (AIstate == Avatar)
			sprintf(buffer, "AI State: Avatar"); 
		else if (AIstate == GoToDefaultState)
			sprintf(buffer, "AI State: Go To Default State"); 
		else if (AIstate == RamNearestFoe)
			sprintf(buffer, "AI State: Ram Nearest Foe"); 
		else if (AIstate == SmartRamNearestFoe)
			sprintf(buffer, "AI State: Smart Ram Nearest Foe"); 
		else if (AIstate == TurretCamp)
			sprintf(buffer, "AI State: Turret Camp"); 
		else if (AIstate == FighterAssault)
			sprintf(buffer, "AI State: Fighter Assault"); 
		else if (AIstate == Explore)
			sprintf(buffer, "AI State: Explore"); 
		else if (AIstate == SmartExplore)
			sprintf(buffer, "AI State: Smart Explore"); 
		else if (AIstate == FollowNearestFriend)
			sprintf(buffer, "AI State: Follow Nearest Friend"); 
		else if (AIstate == ManeuverLeft)
			sprintf(buffer, "AI State: Maneuver Left"); 
		else if (AIstate == ManeuverRight)
			sprintf(buffer, "AI State: Maneuver Right"); 
		else if (AIstate == DodgeLeft)
			sprintf(buffer, "AI State: Dodge Left"); 
		else if (AIstate == DodgeRight)
			sprintf(buffer, "AI State: Dodge Right"); 
		else if (AIstate == PressForward)
			sprintf(buffer, "AI State: Press Forward"); 
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Turn Rate (rad / sec): %.2f",turn_rate);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		if (precision_steering)
			sprintf(buffer,"Prec. Steering: Yes",scale);
		else
			sprintf(buffer,"Prec. Steering: No",scale);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Max Speed (pix/sec): %.2f",max_speed);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Friction (pix/sec): %.2f",friction);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Main Thrust Speed (pix/sec): %.2f",main_thrust_speed);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Side Thrust Speed (pix/sec): %.2f",side_thrust_speed);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Curr. Shot Reload Time (msec): %d",shot_reload_time);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Norm. Shot Reload Time (msec): %d",norm_shot_reload_time);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Rapid Shot Reload Time (msec): %d",rapid_shot_reload_time);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		if (shot_type == HellhornetObj)
			sprintf(buffer, "Shot Type: Hellhornet"); 
		else if (shot_type == BuzzBombObj)
			sprintf(buffer, "Shot Type: Buzz Bomb"); 
		else if (shot_type == WarhawkObj)
			sprintf(buffer, "Shot Type: Warhawk"); 
		else if (shot_type == RogueWarhawkObj)
			sprintf(buffer, "Shot Type: Rogue Warhawk"); 
		else if (shot_type == MarauderObj)
			sprintf(buffer, "Shot Type: Marauder"); 
		else if (shot_type == StingerObj)
			sprintf(buffer, "Shot Type: Stinger"); 
		else if (shot_type == HornetnestObj)
			sprintf(buffer, "Shot Type: Hornetnest"); 
		else if (shot_type == DreadnaughtObj)
			sprintf(buffer, "Shot Type: Dreadnaught"); 
		else if (shot_type == AsteroidObj)
			sprintf(buffer, "Shot Type: Asteroid"); 
		else if (shot_type == WhirlakillObj)
			sprintf(buffer, "Shot Type: Whirlakill"); 
		else if (shot_type == PlayerShotObj)
			sprintf(buffer, "Shot Type: Player Plasma Shot"); 
		else if (shot_type == PlasmaAShotObj)
			sprintf(buffer, "Shot Type: Blue Plasma Shot"); 
		else if (shot_type == PlasmaBShotObj)
			sprintf(buffer, "Shot Type: Red Plasma Shot"); 
		else if (shot_type == PlasmaCShotObj)
			sprintf(buffer, "Shot Type: Green Plasma Shot"); 
		else if (shot_type == PlasmaDShotObj)
			sprintf(buffer, "Shot Type: White Plasma Shot"); 
		else if (shot_type == DreadnaughtShotObj)
			sprintf(buffer, "Shot Type: Dreadnaught Shot"); 
		else if (shot_type == ExplosionObj)
			sprintf(buffer, "Shot Type: Explosion"); 
		else if (shot_type == PowerUpObj)
			sprintf(buffer, "Shot Type: Power-up"); 
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Shot Speed (pix/sec): %.2f",shot_speed);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		if (triple_fire)
			sprintf(buffer,"Triple-fire: Yes",scale);
		else
			sprintf(buffer,"Triple-fire: No",scale);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		if (rapid_fire)
			sprintf(buffer,"Rapid-fire: Yes",scale);
		else
			sprintf(buffer,"Rapid-fire: No",scale);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		disp_x += 400;
		disp_y = old_disp_y;
		sprintf(buffer,"# Detects: %d",num_detects);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 30;

		for (int i = 0; i < num_detects; i++)
		{
			sprintf(buffer,"Detect #%d",i+1);
			Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
				lpddsback);
			disp_y += 15;

			if (detects[i].det_type == HellhornetObj)
				sprintf(buffer, "   Type: Hellhornet (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == BuzzBombObj)
				sprintf(buffer, "   Type: Buzz Bomb (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == WarhawkObj)
				sprintf(buffer, "   Type: Warhawk (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == RogueWarhawkObj)
				sprintf(buffer, "   Type: Rogue Warhawk (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == MarauderObj)
				sprintf(buffer, "   Type: Marauder (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == StingerObj)
				sprintf(buffer, "   Type: Stinger (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == HornetnestObj)
				sprintf(buffer, "   Type: Hornetnest (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == DreadnaughtObj)
				sprintf(buffer, "   Type: Dreadnaught (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == AsteroidObj)
				sprintf(buffer, "   Type: Asteroid (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == WhirlakillObj)
				sprintf(buffer, "   Type: Whirlakill (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == PlasmaTurretObj)
				sprintf(buffer, "   Type: Plasma Turret (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == PlayerShotObj)
				sprintf(buffer, "   Type: Player Plasma Shot (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == PlasmaAShotObj)
				sprintf(buffer, "   Type: Blue Plasma Shot (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == PlasmaBShotObj)
				sprintf(buffer, "   Type: Red Plasma Shot (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == PlasmaCShotObj)
				sprintf(buffer, "   Type: Green Plasma Shot (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == PlasmaDShotObj)
				sprintf(buffer, "   Type: White Plasma Shot (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == DreadnaughtShotObj)
				sprintf(buffer, "   Type: Dreadnaught Shot (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == ExplosionObj)
				sprintf(buffer, "   Type: Explosion (Object# %d)",detects[i].obj_id); 
			else if (detects[i].det_type == PowerUpObj)
				sprintf(buffer, "   Type: Power-up (Object# %d)",detects[i].obj_id); 
			Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
				lpddsback);
			disp_y += 15;

			if (detects[i].alignment == Hostile)
				sprintf(buffer, "   Alignment: Hostile"); 
			else if (detects[i].alignment == Neutral)
				sprintf(buffer, "   Alignment: Neutral"); 
			else if (detects[i].alignment == Friendly)
				sprintf(buffer, "   Alignment: Friendly"); 
			Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
				lpddsback);
			disp_y += 15;

			sprintf(buffer,"   Bearing (rad): %.2f",detects[i].bearing);
			Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
				lpddsback);
			disp_y += 15;

			sprintf(buffer,"   Range (pix): %.2f",detects[i].range);
			Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
				lpddsback);
			disp_y += 15;

			sprintf(buffer, "   Location (pix)  X: %.2f, Y: %.2f, <: %.2f, dist: %.2f", 
				detects[i].location.get_x(), detects[i].location.get_y(), detects[i].location.get_ang(), detects[i].location.get_dist());
			Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
				lpddsback);
			disp_y += 15;

			sprintf(buffer, "   Velocity (pix/sec)  X: %.2f, Y: %.2f, <: %.2f, dist: %.2f", 
			detects[i].velocity.get_x(), detects[i].velocity.get_y(), detects[i].velocity.get_ang(), detects[i].velocity.get_dist());
			Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
				lpddsback);
			disp_y += 30;

			if (i == 4)
				break;
		}
	}

protected:
	BOOL InFrontFOV(float target_x, float target_y);
	BOOL InFrontFOV(Vect& pos);
	BOOL InFrontFOF(float target_x, float target_y);
	BOOL InFrontFOF(Vect& pos);
	int GetNearestFriendly();
	int GetNearestHostile();
	int GetNearestKin();
	int GetNearestDetect();
	int GetNearestCollisionThreat();
	virtual AlignmentType DispositionTowards(GameObjectType obj_type)
	{
		return Neutral;
	}
	virtual BOOL ShouldRam(GameObjectType obj_type)
	{
		return(FALSE);
	}
	int LeftCollisionThreat();
	int RightCollisionThreat();
	BOOL HostileInFrontFOF();
	BOOL FriendlyInFrontFOF();

	virtual void AvatarUpdate();
	void RamNearestFoeUpdate();
	void SmartRamNearestFoeUpdate();
	void TurretCampUpdate();
	virtual void FighterAssaultUpdate();
	void ExploreUpdate();
	void SmartExploreUpdate();
	void FollowNearestFriendUpdate();
	void ManeuverLeftUpdate();
	void ManeuverRightUpdate();
	void DodgeLeftUpdate();
	void DodgeRightUpdate();
	void PressForwardUpdate();

	void Steer(float steer_x, float steer_y);
	void Steer(Vect& v);
	void Spin(BOOL left = TRUE);
	void MainThrust();
	void LeftThrust();
	void RightThrust();
	void FireFrontWeapon();

	virtual void PlayThrustSound();
	virtual void PlayFireSound();

	// Sensory variables

	int num_detects;
	struct Detect detects[MAX_AI_DETECTS];

	float front_FOV;  // front field-of-view angle (in radians)
	float front_FOF;  // front field-of-fire angle (in radians)
					  // This gets used to see if friendlies 
	                  // are in the way or if it's worthwhile
					  // to let off a shot.
	float sight_dist; // maximum range of sight (in pixels)

	float collision_scare_dist;  // how close must an object be, before I want to get away?
	float collision_scare_field; 

	// Planning variables

	AIStateType AIstate;		  // current state
	AIStateType default_AIstate;  // the default
	int num_stack_states;
	AIStateType state_stack[NUM_STACK_AI_STATES];
	int state_timer;	// countdown timer for the current state
	int explore_timer;  // countdown timer for the Explore state.
						// Every time this hits zero, we do a 
						// a maneuver (if we're exploring).
	int explore_leg_duration;  // how long do we keep going straight between maneuvers? (ms)
	float follow_distance;  // what's the ideal distance (in pix) for following other objects?

	// "Motor" variables

	BOOL avatar;   // is the AIObject being controlled by the player?

	// Steering
	float turn_rate; // how fast can the AIObject turn? (in radians / sec.)
	BOOL precision_steering; // can the AIObject understeer its turn rate?

	// Thrusting
	float max_speed; // the maximum speed of the AIObject (pixels / sec.)
	float friction; // the "friction" decelerate speed of the AIObject (pixels / sec.)
	float main_thrust_speed; // how much speed does the AIObjects main thruster add (pps)
	float side_thrust_speed; // how much speed do the AIObjects side thrusters add (pps)

	// Shooting
	int shot_reload_time; // how long before we can fire again in milliseconds (ticks)
	int norm_shot_reload_time; // default for above
	int rapid_shot_reload_time; // value for rapid-fire mode
	int shot_timer;  // a countdown timer for our shots
	GameObjectType shot_type;  // the kind of GameObject spawned with a shot
	float shot_speed;  // how fast is the shot
	BOOL triple_fire;  // triple fire mode on
	BOOL rapid_fire;   // rapid fire mode on
};

#endif