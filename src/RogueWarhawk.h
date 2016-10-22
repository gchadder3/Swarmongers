// RogueWarhawk.h
//
// The header file for the RogueWarhawk class.  Rogue 
// Warhawks are like normal Warhawks, only Hostile.


#ifndef _ROGUE_WARHAWK_H
#define _ROGUE_WARHAWK_H

// INCLUDES

#include "Warhawk.h"


const int ROGUE_WARHAWK_COLOR = 40;


// Line definition of the Valiant.
static int rogue_warhawk_def[] =
{
	8,
	10,0,6,24,ROGUE_WARHAWK_COLOR,
	6,24,12,16,ROGUE_WARHAWK_COLOR,
	12,16,0,24,ROGUE_WARHAWK_COLOR,
	0,24,12,8,ROGUE_WARHAWK_COLOR,
	12,8,24,24,ROGUE_WARHAWK_COLOR,
	24,24,12,16,ROGUE_WARHAWK_COLOR,
	12,16,18,24,ROGUE_WARHAWK_COLOR,
	18,24,14,0,ROGUE_WARHAWK_COLOR
}; 


class RogueWarhawk: public Warhawk
{
public:
	// Constructor function.
	RogueWarhawk();

	// Return what we think of other object types.
	AlignmentType DispositionTowards(GameObjectType obj_type)
	{
		if ((obj_type == WarhawkObj) || 
			(obj_type == StingerObj))
			return(Hostile);
		else if ((obj_type == PlayerShotObj) ||
			(obj_type == PlasmaAShotObj) ||
			(obj_type == PlasmaBShotObj) ||
			(obj_type == PlasmaCShotObj) ||
			(obj_type == PlasmaDShotObj) ||
			(obj_type == ExplosionObj) ||
			(obj_type == AsteroidObj) ||
			(obj_type == WhirlakillObj))
			return(Neutral);
		else
			return(Friendly);
	}
};

#endif