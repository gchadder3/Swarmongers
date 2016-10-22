// BuzzBomb.cpp
//
// Source file for the BuzzBomb class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "AIObject.h"
#include "BuzzBomb.h"


// Globals from main.cpp
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;


// PUBLIC FUNCTIONS

BuzzBomb::BuzzBomb() : AIObject()
{
	Define(buzzbomb_def);
	SetObjectType(BuzzBombObj);
	SetMaxStructPoints(BUZZBOMB_HULL_POINTS);
	SetCollideDamage(BUZZBOMB_DAMAGE);
	SetCollisionMass(BUZZBOMB_COLLISION_MASS);
	rotation = (float) (rand() % 7);
	front_FOV = BUZZBOMB_FRONT_FOV;
	default_AIstate = BUZZBOMB_DEF_AI_STATE;
	ResetAI();
	SetKillScore(BUZZBOMB_KILL_SCORE);
}

void BuzzBomb::PlayExplodeSound()
{
	sndManager->PlayGrellGenExplode();
}


// PROTECTED FUNCTIONS

void BuzzBomb::PlayThrustSound()
{
	sndManager->PlayBuzzbombThrust();
}