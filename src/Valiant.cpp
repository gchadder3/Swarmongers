// Valiant.cpp
//
// Source file for the Valiant class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "AIObject.h"
#include "Warhawk.h"
#include "Valiant.h"


// Globals from main.cpp
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;


// PUBLIC FUNCTIONS

Valiant::Valiant() : Warhawk()
{
	Define(valiant_def);
	SetKillScore(-1000); // for blowing yourself up
}


// PROTECTED FUNCTIONS

void Valiant::PlayThrustSound()
{
	sndManager->PlayValiantThrust();
}

void Valiant::PlayFireSound()
{
	sndManager->PlayValiantFire();
}