// Hellhornet.cpp
//
// Source file for the Hellhornet class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "AIObject.h"
#include "Hellhornet.h"


// Globals from main.cpp
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;


// PUBLIC FUNCTIONS

Hellhornet::Hellhornet() : AIObject()
{
	Define(hellhornet_def);
	SetObjectType(HellhornetObj);
	SetCollideDamage(HELLHORNET_DAMAGE);
	SetCollisionMass(HELLHORNET_COLLISION_MASS);
	rotation = (float) (rand() % 7);
	SetKillScore(HELLHORNET_KILL_SCORE);
	default_AIstate = HELLHORNET_DEF_AI_STATE;
	ResetAI();
}

void Hellhornet::PlayExplodeSound()
{
	sndManager->PlayGrellGenExplode();
}


// PROTECTED FUNCTIONS

void Hellhornet::PlayThrustSound()
{
	sndManager->PlayGrellGenThrust();
}