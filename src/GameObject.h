// GameObject.h
//
// The header file for the GameObject class.  GameObject
// is the base class for all displayable game entities.

#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include "Vect.h"

const int DEFAULT_STRUCT_POINTS = 5;
const int DEFAULT_COLLIDE_DAMAGE = 10;
const float DEFAULT_COLLISION_MASS = 1000000.0;  
					// >= 1000000.0 means infinite mass:
					// other objects will bounce right off
const int DEFAULT_SCORE_4_BLASTING = 0;

// Types of game object.  This needs to be modified each time
// a new kind of game object is created.
enum GameObjectType
{
	HellhornetObj,  // a Grell Hellhornet drone
	BuzzBombObj,	// a buzz bomb
	WarhawkObj,		// a Terran Warhawk (could be the player's ship)
	RogueWarhawkObj,		// a rogue Terran Warhawk
	MarauderObj,	// a Grell Marauder (their weakest fighter)
	StingerObj,		// a Terran Stinger (their weakest fighter)
	HornetnestObj,	// a Grell Hornetnest
	DreadnaughtObj, // a Grell Dreadnaught
	AsteroidObj,	// an asteroid
	WhirlakillObj,	// a whirlakill
	PlasmaTurretObj, // a big plasma turret (shoots white shots)
	PlayerShotObj,	// a shot from the player's ship
	PlasmaAShotObj, // a blue plasma shot (a lot like the player's shots)
	PlasmaBShotObj,	// a red plasma shot
	PlasmaCShotObj,	// a green plasma shot
	PlasmaDShotObj,	// a big white plasma shot
	DreadnaughtShotObj, // a dreadnaught shot
	ExplosionObj,	// an explosion
	PowerUpObj      // a powerup capsule
};


class GameObject
{
public:
	GameObject()
	{
		alive = FALSE;
		has_AI = FALSE;
		position.set(0.0,0.0,FALSE);
		max_struct_points = DEFAULT_STRUCT_POINTS;
		struct_points = max_struct_points;
		collide_damage = DEFAULT_COLLIDE_DAMAGE;
		collision_mass = DEFAULT_COLLISION_MASS;
		score_for_blasting = DEFAULT_SCORE_4_BLASTING;
		game_obj_id = game_obj_count;
		game_obj_count++;
		parent_obj_id = -1;
	}

	GameObjectType GetType()
	{
		return(obj_type);
	}

	BOOL IsProjectile()
	{
		if ((obj_type == PlayerShotObj) ||
			(obj_type == PlasmaAShotObj) ||
			(obj_type == PlasmaBShotObj) ||
			(obj_type == PlasmaCShotObj) ||
			(obj_type == PlasmaDShotObj) ||
			(obj_type == DreadnaughtShotObj))
			return(TRUE);
		else
			return(FALSE);
	}

	int GetObjectID()
	{
		return(game_obj_id);
	}

	int GetParentObjectID()
	{
		return(parent_obj_id);
	}

	void SetParentObject(GameObject *obj)
	{
		parent_obj_id = obj->GetObjectID();
	}

	BOOL IsParentOf(GameObject *obj)
	{
		if (obj->GetParentObjectID() == GetObjectID())
			return(TRUE);
		else
			return(FALSE);
	}

	BOOL IsChildOf(GameObject *obj)
	{
		if (GetParentObjectID() == obj->GetObjectID())
			return(TRUE);
		else
			return(FALSE);
	}

	BOOL HasAI()
	{
		return(has_AI);
	}

	void MakeHaveAI()
	{
		has_AI = TRUE;
	}

	BOOL IsAlive()
	{
		return(alive);
	}

	void MakeAlive()
	{
		alive = TRUE;
	}

	virtual void Kill()
	{
		alive = FALSE;
	}

	void ResetDamage()
	{
		struct_points = max_struct_points;
	}

	void Damage(int dam_amount)
	{
		struct_points -= dam_amount;
		if (struct_points <= 0)
		{
			Kill();
			ResetDamage();
		}
	}

	int GetStructPoints()
	{
		return(struct_points);
	}

	void SetMaxStructPoints(int s_points)
	{
		max_struct_points = s_points;
		struct_points = s_points;
	}

	int GetCollideDamage()
	{
		return(collide_damage);
	}

	void SetCollideDamage(int dam_amount)
	{
		collide_damage = dam_amount;
	}

	float GetCollisionMass()
	{
		return(collision_mass);
	}

	void SetCollisionMass(float mass)
	{
		collision_mass = mass;
	}

	int GetKillScore()
	{
		return(score_for_blasting);
	}

	void SetKillScore(int points)
	{
		score_for_blasting = points;
	}

	virtual void Define(int def[]) = 0;

	Vect& GetPosition()
	{
		return(position);
	}

	Vect& GetVelocity()
	{
		return(velocity);
	}

	void SetScale(float s)
	{
		// Set the scale.
		scale = s;

		// Recalculate the pixel width and height, and the
		// collision radius.
		pixel_width = (int) ((float) def_pixel_width * s);
		pixel_height = (int) ((float) def_pixel_height * s);
		collision_rad = def_collision_rad * s;
	}

	void SetRotation(float ang)
	{
		rotation = Vect::radian_fix(ang);
	}

	void Rotate(float ang)
	{
		rotation = Vect::radian_fix(rotation + ang);
	}

	float GetCollisionRad()
	{
		return(collision_rad);
	}

	void SetPosition(float x, float y);

	void SetVelocity(float ang, float dist)
	{
		velocity.set(ang, dist);
	}

	virtual BOOL CanCollideWith(GameObjectType obj_type)
	{
		return(TRUE);
	}

	BOOL IsCollidedWith(GameObject *other_guy)
	{
		float COM_dist;

		if ((alive) && (other_guy->IsAlive()) &&
			(CanCollideWith(other_guy->GetType())))
		{
			COM_dist = position.distance_to(other_guy->GetPosition());
			if (COM_dist > collision_rad + other_guy->GetCollisionRad())
				return(FALSE);
			else
				return(TRUE);
		}
		else
			return(FALSE);
	}

	// Draws the object.  Will return FALSE if there is an
	// error.
	virtual BOOL Draw(BOOL ghost = FALSE) = 0;

	virtual BOOL DecalDraw(int pos_x, int pos_y) = 0;

	virtual void Update(BOOL wraparound = TRUE);

	virtual void PlayExplodeSound();

	// Use Draw_Text to debug display the vector info.
	virtual void dbg_display(int disp_x, int disp_y)
	{
		char buffer[80];

		sprintf(buffer, "Object# %d", game_obj_id);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Parent Object# %d", parent_obj_id);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		if (alive)
		{
			if (obj_type == HellhornetObj)
				sprintf(buffer, "Hellhornet (alive)"); 
			else if (obj_type == BuzzBombObj)
				sprintf(buffer, "Buzz Bomb (alive)"); 
			else if (obj_type == WarhawkObj)
				sprintf(buffer, "Warhawk (alive)"); 
			else if (obj_type == RogueWarhawkObj)
				sprintf(buffer, "Rogue Warhawk (alive)"); 
			else if (obj_type == MarauderObj)
				sprintf(buffer, "Marauder (alive)"); 
			else if (obj_type == StingerObj)
				sprintf(buffer, "Stinger (alive)"); 
			else if (obj_type == HornetnestObj)
				sprintf(buffer, "Hornetnest (alive)"); 
			else if (obj_type == DreadnaughtObj)
				sprintf(buffer, "Dreadnaught (alive)"); 
			else if (obj_type == AsteroidObj)
				sprintf(buffer, "Asteroid (alive)"); 
			else if (obj_type == WhirlakillObj)
				sprintf(buffer, "Whirlakill (alive)"); 
			else if (obj_type == PlasmaTurretObj)
				sprintf(buffer, "Plasma Turret (alive)"); 
			else if (obj_type == PlayerShotObj)
				sprintf(buffer, "Player Plasma Shot (alive)"); 
			else if (obj_type == PlasmaAShotObj)
				sprintf(buffer, "Blue Plasma Shot (alive)"); 
			else if (obj_type == PlasmaBShotObj)
				sprintf(buffer, "Red Plasma Shot (alive)"); 
			else if (obj_type == PlasmaCShotObj)
				sprintf(buffer, "Green Plasma Shot (alive)"); 
			else if (obj_type == PlasmaDShotObj)
				sprintf(buffer, "White Plasma Shot (alive)"); 
			else if (obj_type == DreadnaughtShotObj)
				sprintf(buffer, "Dreadnaught Shot (alive)"); 
			else if (obj_type == ExplosionObj)
				sprintf(buffer, "Explosion (alive)"); 
			else if (obj_type == PowerUpObj)
				sprintf(buffer, "Power-up (alive)"); 
		}
		else
		{
			if (obj_type == HellhornetObj)
				sprintf(buffer, "Hellhornet (dead)"); 
			else if (obj_type == BuzzBombObj)
				sprintf(buffer, "Buzz Bomb (dead)"); 
			else if (obj_type == WarhawkObj)
				sprintf(buffer, "Warhawk (dead)"); 
			else if (obj_type == RogueWarhawkObj)
				sprintf(buffer, "Rogue Warhawk (dead)"); 
			else if (obj_type == MarauderObj)
				sprintf(buffer, "Marauder (dead)"); 
			else if (obj_type == StingerObj)
				sprintf(buffer, "Stinger (dead)"); 
			else if (obj_type == HornetnestObj)
				sprintf(buffer, "Hornetnest (dead)"); 
			else if (obj_type == DreadnaughtObj)
				sprintf(buffer, "Dreadnaught (dead)"); 
			else if (obj_type == AsteroidObj)
				sprintf(buffer, "Asteroid (dead)"); 
			else if (obj_type == WhirlakillObj)
				sprintf(buffer, "Whirlakill (dead)"); 
			else if (obj_type == PlasmaTurretObj)
				sprintf(buffer, "Plasma Turret (dead)"); 
			else if (obj_type == PlayerShotObj)
				sprintf(buffer, "Player Plasma Shot (dead)"); 
			else if (obj_type == PlasmaAShotObj)
				sprintf(buffer, "Blue Plasma Shot (dead)"); 
			else if (obj_type == PlasmaBShotObj)
				sprintf(buffer, "Red Plasma Shot (dead)"); 
			else if (obj_type == PlasmaCShotObj)
				sprintf(buffer, "Green Plasma Shot (dead)"); 
			else if (obj_type == PlasmaDShotObj)
				sprintf(buffer, "White Plasma Shot (dead)"); 
			else if (obj_type == DreadnaughtShotObj)
				sprintf(buffer, "Dreadnaught Shot (dead)"); 
			else if (obj_type == ExplosionObj)
				sprintf(buffer, "Explosion (dead)"); 
			else if (obj_type == PowerUpObj)
				sprintf(buffer, "Power-up (dead)"); 
		}
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Location (pixels)  X: %.2f, Y: %.2f, <: %.2f, dist: %.2f", 
			position.get_x(), position.get_y(), position.get_ang(), position.get_dist());
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Velocity (pix/sec)  X: %.2f, Y: %.2f, <: %.2f, dist: %.2f", 
			velocity.get_x(), velocity.get_y(), velocity.get_ang(), velocity.get_dist());
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;
	
		sprintf(buffer,"Scale: %.2f",scale);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer,"Rotation (rad): %.2f",rotation);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Struct Points: %d/%d", struct_points, max_struct_points);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Width (pix): %d", pixel_width);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Height (pix): %d", pixel_height);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Collision Radius (pix): %f", collision_rad);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Collide Damage (per frame): %d", collide_damage);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Collision Mass: %f", collision_mass);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;

		sprintf(buffer, "Score for Kill: %d", score_for_blasting);
		Draw_Text_GDI(buffer, disp_x, disp_y, RGB(0,255,0),
			lpddsback);
		disp_y += 15;
	}

protected:
	void SetObjectType(GameObjectType typ)
	{
		obj_type = typ;
	}

	// The count of game objects.  Used for debug ID-ing.
	static int game_obj_count;

	// A unique ID number for the game object.  Used for
	// debugging.
	int game_obj_id;

	// A unique ID number identifying the object which is 
	// the object's "parent".  (For example a shot object
	// has the shooter object as it's parent.)  -1 means
	// the object has no parent.
	int parent_obj_id;

	// What kind of game object is it?  This means what is 
	// the _most specific_ game object type.
	GameObjectType obj_type;

	// Is the object an AI?
	BOOL has_AI;

	// Should the object be displayed and updated?
	BOOL alive;

	// Current position of the object (in pixels)
	Vect position;

	// Current velocity of the object (in pixels / sec.)
	Vect velocity;

	// How much is the object scaled?  1.0 is normal 
	// size.
	float scale;

	// How much is the object rotated?  Angles are 
	// clockwise and in radians.
	float rotation;

	// Width and height of the object in pixels
	int pixel_width;
	int pixel_height;

	// Collision radius (units pixels though rep. with a float)
	float collision_rad;

	// Width and height of the (unscaled) object in pixels
	int def_pixel_width;
	int def_pixel_height;

	// Unscaled collision radius (units pixels though rep. with a float)
	float def_collision_rad;

	// How many structural points does the object have max.
	// (i.e., how much damage can it take)
	int max_struct_points;

	// How many structural points does the object currently
	// have?
	int struct_points;

	// How much damage does this object do to another if it
	// collides with it?
	int collide_damage;

	// Collision mass (units irrelevant).  This is used in 
	// resolving motions after collisions.
	float collision_mass;

	// How many points does a player get for destroying the object?
	int score_for_blasting;
};

typedef GameObject *GameObjectPtr;

#endif
