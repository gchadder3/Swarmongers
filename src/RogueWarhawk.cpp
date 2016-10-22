// RogueWarhawk.cpp
//
// Source file for the RogueWarhawk class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "AIObject.h"
#include "Warhawk.h"
#include "RogueWarhawk.h"


// Globals from main.cpp
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;


// PUBLIC FUNCTIONS

RogueWarhawk::RogueWarhawk() : Warhawk()
{
	Define(rogue_warhawk_def);
	SetObjectType(RogueWarhawkObj);
	SetKillScore(2000);
}
