// Game.h
//
// The header file for the Game object.

#ifndef _GAME_H
#define _GAME_H

//INCLUDES
#include "GPDUMB_includes.h"
#include "StarField.h"
#include "GameObject.h"
#include "Valiant.h"
#include "Warhawk.h"
#include "RogueWarhawk.h"
#include "Marauder.h"
#include "Stinger.h"
#include "PlayerShot.h"
#include "PlasmaAShot.h"
#include "PlasmaBShot.h"
#include "PlasmaCShot.h"
#include "PlasmaDShot.h"
#include "DreadnaughtShot.h"
#include "PointExplosion.h"
#include "Hellhornet.h"
#include "Hornetnest.h"
#include "Dreadnaught.h"
#include "BuzzBomb.h"
#include "PowerUp.h"
#include "Asteroid.h"
#include "Whirlakill.h"
#include "PlasmaTurret.h"


const int GM_SCREEN_WIDTH = 1024;
const int GM_SCREEN_HEIGHT = 768;
const int GM_SCREEN_BORDER_WIDTH = 40;
const int MESSAGE_PANE_HEIGHT = 20;

const int MAX_NUM_WARHAWKS = 2;
const int MAX_NUM_RWARHAWKS = 1;
const int MAX_NUM_MARAUDERS = 20;
const int MAX_NUM_STINGERS = 20;
const int MAX_NUM_HELLHORNETS = 30;
const int MAX_NUM_HORNETNESTS = 3;
const int MAX_NUM_DREADNAUGHTS = 1;
const int MAX_NUM_BOMBS = 5;
const int MAX_NUM_ASTEROIDS = 10;
const int MAX_NUM_WHIRLAKILLS = 20;
const int MAX_NUM_PLASMA_TURRETS = 8;
const int MAX_NUM_PLAYER_SHOTS = 30;
const int MAX_NUM_PLASMA_A_SHOTS = 20;
const int MAX_NUM_PLASMA_B_SHOTS = 10;
const int MAX_NUM_PLASMA_C_SHOTS = 10;
const int MAX_NUM_PLASMA_D_SHOTS = 10;
const int MAX_NUM_DREADNAUGHT_SHOTS = 2;
const int MAX_NUM_POINT_EXPLOSIONS = 10;
const int MAX_NUM_GAME_OBJECTS = 2 + MAX_NUM_WARHAWKS + MAX_NUM_RWARHAWKS
	+ MAX_NUM_MARAUDERS + MAX_NUM_STINGERS + MAX_NUM_HELLHORNETS
	+ MAX_NUM_HORNETNESTS + MAX_NUM_DREADNAUGHTS + MAX_NUM_BOMBS + MAX_NUM_ASTEROIDS
	+ MAX_NUM_WHIRLAKILLS + MAX_NUM_PLASMA_TURRETS 
	+ MAX_NUM_PLAYER_SHOTS + MAX_NUM_PLASMA_A_SHOTS
	+ MAX_NUM_PLASMA_B_SHOTS + MAX_NUM_PLASMA_C_SHOTS
	+ MAX_NUM_PLASMA_D_SHOTS + MAX_NUM_DREADNAUGHT_SHOTS 
	+ MAX_NUM_POINT_EXPLOSIONS;
const int LEVEL_START_PAUSE = 1500;  // How long do we pause before the fray begins on a level? (in ms.)
const int MAX_POWERUP_INTERVAL = 20000;  // How long maximum do we wait before putting a power-up capsule on the screen for the player?
const int POWERUPS_PER_LEVEL = 2;    // How many powerups are offered max. per level?

enum GameMode
{
	IntroMode,
	AttractMode,
	InstructMode,
	ChapterMode,
	IntermissionMode,
	PlayMode,
	WonMode,
	PaletteMode   // for displaying the color palette
};

enum GameDifficulty
{
	Easy,
	Hard,
	ReallyHard,
	Absurd
};

class Game
{
public:
	// Constructor
	Game();

	// Destructor
	~Game();

	// The main game loop call.
	void MainLoop();

 	float GetObjectCollisionRad(GameObjectType obj_type);

	// Spawn a game object if possible.  If not possible,
	// return FALSE.
 	BOOL SpawnGameObject(GameObjectType obj_type, int pos_x, int pos_y, float ang, float speed, GameObjectPtr parent);

	// Spawn a player shot if possible.  If not possible,
	// return FALSE.
 	BOOL SpawnPlayerShot(int shot_x, int shot_y, float shot_ang, GameObjectPtr parent);

	// Return the player's current score.
	int GetScore()
	{
		return(score);
	}

	// Increment (or sometimes decrement) the player's score.
	void AddScorePoints(int points)
	{
		score += points;
	}

	// Reset the score to zero.
	void ResetScore()
	{
		score = 0;
	}

	DWORD GetElapsedTime()
	{
		return(elapsed_time);
	}

	void ResetElapsedTime()
	{
		elapsed_time = 0;
	}

	// Get the current frames-per-second count.
	int GetCurrentFPS()
	{
		return(current_fps);
	}

	// Countdown a tick (millisecond) timer to the next 
	// (supposed) frame value, according to our current FPS
	// estimate.
	void CountdownTickTimer(int& current_timer)
	{
		current_timer -= (1000 / current_fps);
	}

private:
 	BOOL PlaceObjectRandCenter(GameObjectPtr obj);
 	BOOL PlaceObjectFirstHole(GameObjectPtr obj);
 	BOOL PlaceObjectRandEdge(GameObjectPtr obj);
	void PrepareLevel();
	BOOL CheckVictory();
 	void HandleCollision(GameObjectPtr obj1, GameObjectPtr obj2);
	void IntroModeLoop();
	void AttractModeLoop();
	void InstructionsModeLoop();
	void ChapterLoop();
	void IntermissionLoop();
	void PlayModeLoop();
	void WonModeLoop();
	void PaletteModeLoop();
	void LevelCycle();

	// Logo bitmap
	BITMAP_IMAGE logo_bmp; // used to hold backgrounds

	// What mode is the game in?
	GameMode mode;

	// Restart the game?
	BOOL restart_game;

	// How difficult is the game?
	GameDifficulty difficulty;

	// The current score.
	int score;

	// Game level.
	int level;

	// Number of player ships.
	int num_player_ships;

	// Game object counts.
	int num_warhawks;
	int num_rwarhawks;
	int num_marauders;
	int num_stingers;
	int num_hhornets;
	int num_hnests;
	int num_dreadnaughts;
	int num_bombs;
	int num_asteroids;
	int num_wkills;
	int num_pturrets;
	int num_pl_shots;
	int num_pA_shots;
	int num_pB_shots;
	int num_pC_shots;
	int num_pD_shots;
	int num_dreadnaught_shots;
	int num_pexplosions;

	// Kill counts (on the current level).
	int num_warhawks_killed;
	int num_rwarhawks_killed;
	int num_marauders_killed;
	int num_stingers_killed;
	int num_hhornets_killed;
	int num_hnests_killed;
	int num_dreadnaughts_killed;
	int num_bombs_killed;
	int num_asteroids_killed;
	int num_wkills_killed;
	int num_pturrets_killed;

 	Valiant player_ship;
 	Warhawk warhawks[MAX_NUM_WARHAWKS];
 	RogueWarhawk rwarhawks[MAX_NUM_RWARHAWKS];
 	Marauder marauders[MAX_NUM_MARAUDERS];
 	Stinger stingers[MAX_NUM_STINGERS];
 	Hellhornet hhornets[MAX_NUM_HELLHORNETS];
 	Hornetnest hnests[MAX_NUM_HORNETNESTS];	
 	Dreadnaught dreadnaughts[MAX_NUM_DREADNAUGHTS];	
 	BuzzBomb bombs[MAX_NUM_BOMBS];
 	Asteroid asteroids[MAX_NUM_ASTEROIDS];
 	Whirlakill wkills[MAX_NUM_WHIRLAKILLS];
 	PlasmaTurret pturrets[MAX_NUM_PLASMA_TURRETS];
 	PlayerShot player_shots[MAX_NUM_PLAYER_SHOTS];
 	PlasmaAShot plasma_a_shots[MAX_NUM_PLASMA_A_SHOTS];
 	PlasmaBShot plasma_b_shots[MAX_NUM_PLASMA_B_SHOTS];
 	PlasmaCShot plasma_c_shots[MAX_NUM_PLASMA_C_SHOTS];
 	PlasmaDShot plasma_d_shots[MAX_NUM_PLASMA_D_SHOTS];
 	DreadnaughtShot dreadnaught_shots[MAX_NUM_DREADNAUGHT_SHOTS];
 	PointExplosion pexplosions[MAX_NUM_POINT_EXPLOSIONS];
 	PowerUp powerup_capsule;

	int num_game_objects;
 	GameObjectPtr game_objects[MAX_NUM_GAME_OBJECTS];

 	Valiant ship_count_decal;

	// Game elapsed time (in milliseconds).
	DWORD  elapsed_time;

	// Have we played the level start sound yet?
	BOOL played_level_start_snd;

	// Begin a new level from intermission mode?
	BOOL new_level;

	// Did the player's ship just explode?
	BOOL player_just_exploded;

	// Values used for calculating the frames / second 
	// (fps) count.
	int frame_count;
	DWORD frame_count_timer;
	DWORD old_frame_time;

	// The current frames / second count.
	int current_fps;

	// The time left on a pause at the start of the level.
	// (in milliseconds).
	int start_pause_timer;

	// Power-up timer.  We want power-up capsules to appear every
	// so often on the screen.
	int powerup_timer;

	// The count of power-ups that have been delivered during this level.
	int powerups_delivered;

	// Game pause mode
	BOOL pause_mode;

	// Debug display mode.  This very often crashes the game,
	// unfortunately.
	BOOL dbg_disp_mode;

	// Debug log mode.  
	BOOL dbg_log_mode;

	// Cheats
	BOOL god_mode;
};

#endif