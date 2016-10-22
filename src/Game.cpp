// Game.cpp
//
// The source for the Game object.

// INCLUDES

#include "GPDUMB_includes.h"
#include "StarField.h"
#include "GameObject.h"
#include "AIObject.h"
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
#include "SoundManager.h"
#include "GameScreen.h"
#include "ActionEventManager.h"
#include "DebugLog.h"
#include "Game.h"
#include "Vect.h"


// Globals from main.cpp
extern HWND main_window_handle;
extern GameScreen *theScreen;
extern StarField *theStars; 
extern SoundManager *sndManager;
extern ActionEventManager *actionManager;
extern DebugLog *dbgLog;


int GameObject::game_obj_count = 0;


// PUBLIC FUNCTIONS

Game::Game()
{
	// This function is where you do all the initialization 
	// for your game

	// Start up DirectDraw (replace the parms as you desire)
	DD_Init(GM_SCREEN_WIDTH, GM_SCREEN_HEIGHT, SCREEN_BPP);

	// Load a very good palette from disk based on the Deluxe
	// paint palette
	Load_Palette_From_File("DPAINT.PAL",palette);
	Set_Palette(palette);

	// Set clipping rectangle to screen extents so objects dont
	// mess up at edges
	RECT screen_rect = {0,0,GM_SCREEN_WIDTH,GM_SCREEN_HEIGHT};
//	lpddclipper = DD_Attach_Clipper(lpddsback,1,&screen_rect);

	// Initilize DirectSound
	DSound_Init();

	// Create the game's SoundManager and load all of the 
	// sounds.
	sndManager = new SoundManager();
	sndManager->LoadSounds();

	// Create a debug log object
	dbgLog = new DebugLog();

	// Initialize DirectInput
	DInput_Init();

	// Initialize keyboard
	DI_Init_Keyboard();

	// Initialize mouse
	DI_Init_Mouse();

	// Initialize joystick
	DI_Init_Joystick();

	// Hide the cursor.
	ShowCursor(FALSE);

	// Set up the screen parameters.
	// BEWARE!  Any objects which extend over 
	// GM_SCREEN_BORDER_WIDTH from their centers-of-mass could 
	// potentially crash the game.
	theScreen = new GameScreen(GM_SCREEN_WIDTH, 
		GM_SCREEN_HEIGHT, GM_SCREEN_BORDER_WIDTH, 
		GM_SCREEN_WIDTH - GM_SCREEN_BORDER_WIDTH,
		GM_SCREEN_BORDER_WIDTH + MESSAGE_PANE_HEIGHT, 
		GM_SCREEN_HEIGHT - GM_SCREEN_BORDER_WIDTH, TRUE);

	// Prepare the logo bitmap.
	Load_Bitmap_File(&bitmap8bit, "Swarmongers 1C.bmp");
	Create_Bitmap(&logo_bmp,212,75,600,398);
	Load_Image_Bitmap(&logo_bmp, &bitmap8bit,0,0,BITMAP_EXTRACT_MODE_CELL);

	// Prepare the star field.
	theStars = new StarField();

	// Set up the action manager.
	actionManager = new ActionEventManager();

	// Set the game in intro mode.
	mode = IntroMode;

	// Default the game difficulty to Easy.
	difficulty = Easy;

	// Turn off pause mode.
	pause_mode = FALSE;

	// Set the start-of-level pause timer to nothing (so
	// pause won't happen in the intro).
	start_pause_timer = 0;

	// Turn off debug display mode.
	dbg_disp_mode = FALSE;

	// Turn off debug log mode.
	dbg_log_mode = FALSE;

	// Turn off cheats.
	god_mode = FALSE;
	
	// Reset level start sound
	played_level_start_snd = FALSE;
	
	// Create the player ship "decal" used for showing the 
	// number of ships.
	ship_count_decal.SetRotation(0.0);
	ship_count_decal.SetScale(0.475);
	ship_count_decal.MakeAlive();

	// Set the level for the intro level.
	level = 0;

	// Set this to a garbage value (to avoid a division by 0).
	current_fps = 1;

	// Prepare the intro "level".
	PrepareLevel();
}


Game::~Game()
{
	// This function is where you shutdown your game and
	// release all resources that you allocated

	// Release the logo bitmap and bitmap file.
	Unload_Bitmap_File(&bitmap8bit);
	Destroy_Bitmap(&logo_bmp);

	// Shut down directdraw first.
	DD_Shutdown();

	// Now directsound
	DSound_Shutdown();

	// Finally directinput
	DI_Release_Keyboard();
	DI_Release_Mouse();
	DI_Release_Joystick();

	DInput_Shutdown();
}


void Game::MainLoop()
{
	DWORD new_frame_time;
	DWORD last_frame_duration;
	char buffer[80];             // used to print text

	// If we need to restart the game, prepare a new level
	if (restart_game)
	{
		level++;
		new_level = TRUE;
		played_level_start_snd = FALSE;
		mode = IntermissionMode;

		restart_game = FALSE;
		
		num_player_ships = 4;
		player_ship.Kill();
		restart_game = FALSE; 
	}

	// Check of user is trying to exit
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(main_window_handle, WM_DESTROY,0,0);

	// Start the timing clock, and save the starting time
	// as the new frame start time.
	new_frame_time = Start_Clock();

	// Get the duration of the last frame.
	last_frame_duration = new_frame_time - old_frame_time;

	// Increment the elapsed time and the frame count timer.
	elapsed_time += last_frame_duration;
	frame_count_timer += last_frame_duration;

	// Increment the frame count.
	frame_count++;

	// If the frame count timer has counted at least a second,
	// Recalculate the fps and reset the frame count and 
	// frame_count_timer.
	if (frame_count_timer > 1000)
	{
		current_fps = (frame_count * 1000) / frame_count_timer;
		if (current_fps == 0)
			current_fps = 80;   // fix this so that we don't get divide by zero errors
		frame_count = 0;
		frame_count_timer = 0;
	}

	// Clear the drawing surface
	DD_Fill_Surface(lpddsback, 0);

	// Get input
	DI_Read_Keyboard();
	DI_Read_Mouse();
	DI_Read_Joystick();

	// See if we need to toggle palette mode.
	if (actionManager->KeyReleased(PaletteKey))
	{
		if (mode == PlayMode)
		{
			mode = PaletteMode;
			ShowCursor(TRUE);
		}
		else if (mode == PaletteMode)
		{
			mode = PlayMode;
			ShowCursor(FALSE);
		}
	}

	// Depending on which mode the game is in, call the 
	// proper loop function.
	switch (mode)
	{
	case IntroMode:
		IntroModeLoop();
		break;
	case AttractMode:
		AttractModeLoop();
		break;
	case InstructMode:
		InstructionsModeLoop();
		break;
	case ChapterMode:
		ChapterLoop();
		break;
	case IntermissionMode:
		IntermissionLoop();
		break;
	case PlayMode:
		PlayModeLoop();
		break;
	case WonMode:
		WonModeLoop();
		break;
	case PaletteMode:
		PaletteModeLoop();
		break;
	}

	// Draw the frame rate at the top of the screen.
//	sprintf(buffer, "%d fps", current_fps);
//	Draw_Text_GDI(buffer,1,1,RGB(0,255,255),lpddsback);

	// If we're in the right mode, display the debug log.
	if (dbg_log_mode)
	{
		dbgLog->display(300,300);
	}

	// Flip the surfaces
	DD_Flip();

	// Update the GameScreen object.
	theScreen->Update();

	// Update the ActionEventManager object.
	actionManager->Update();

	// Update the SoundManager object.
	sndManager->Update();

	// Remember the start time for this frame.
	old_frame_time = new_frame_time;

	// If we're in debug mode, we might want to slow down,
	// the frame rate.
	if (dbg_disp_mode)
	{
//		Wait_Clock(100);
	}
}


float Game::GetObjectCollisionRad(GameObjectType obj_type)
{
	for (int i = 0; i < num_game_objects; i++)
	{
		if (game_objects[i]->GetType() == obj_type)
			return game_objects[i]->GetCollisionRad();
	}
}


BOOL Game::SpawnGameObject(GameObjectType obj_type, int pos_x, int pos_y, float ang, float speed, GameObjectPtr parent)
{
	int the_obj = -1;
	int i;

	// Look for the first available object of the correct type.
	for (i = 0; i < num_game_objects; i++)
	{
		if ((game_objects[i]->GetType() == obj_type) && 
			(!game_objects[i]->IsAlive()))
		{
			the_obj = i;
			break;
		}
	}

	// If we can't find a free object, return a failure.
	if (the_obj == -1)
		return(FALSE);
	// Otherwise, spawn the object.
	else
	{
		game_objects[i]->SetParentObject(parent);
		game_objects[i]->SetPosition((float) pos_x, (float) pos_y);
		game_objects[i]->SetVelocity(ang, speed);
		if ((obj_type != PlasmaAShotObj) &&
			(obj_type != PlasmaBShotObj) &&
			(obj_type != PlasmaCShotObj))
			game_objects[i]->SetRotation(ang + QUARTER_CIRCLE);
		game_objects[i]->MakeAlive();
		return(TRUE);
	}
}

BOOL Game::SpawnPlayerShot(int shot_x, int shot_y, float shot_ang, GameObjectPtr parent)
{
	int theShot = -1;
	float shot_speed;
	int i;

	// Look for the first available PlayerShot (i.e., 
	// the first "dead" shot).
	for (i = 0; i < num_pl_shots; i++)
	{
		if (!player_shots[i].IsAlive())
		{
			theShot = i;
			break;
		}
	}

	// If we can't find a free shot, return a failure.
	if (theShot == -1)
		return(FALSE);
	// Otherwise, spawn the shot.
	else
	{
		player_shots[i].SetParentObject(parent);
		shot_speed = player_shots[i].GetShotSpeed();
		player_shots[i].SetPosition((float) shot_x, (float) shot_y);
		player_shots[i].SetVelocity(shot_ang, shot_speed);
		player_shots[i].MakeAlive();
		return(TRUE);
	} 
}


// PRIVATE FUNCTIONS

BOOL Game::PlaceObjectRandCenter(GameObjectPtr obj)
{
	BOOL collision_problem = FALSE;
	int place_x;
	int place_y;
	int place_tries = 0;
	int x_min = theScreen->GetMinX();
	int x_max = theScreen->GetMaxX();
	int y_min = theScreen->GetMinY();
	int y_max = theScreen->GetMaxY();
	int i;

	// Pick starting places until you find one without a 
	// collision or you time out.
	do
	{
		// Pick a random location.
		place_x = x_min + (rand() % (x_max - x_min + 1));
		place_y = y_min + (rand() % (y_max - y_min + 1));
		obj->SetPosition((float) place_x, (float) place_y);

		// See if that location collides with anything.
		for (i = 0; i < num_game_objects; i++)
		{
			if ((obj->GetObjectID() != game_objects[i]->GetObjectID()) && 
				(obj->IsCollidedWith(game_objects[i])))
				collision_problem = TRUE;
		}
		place_tries++;
	} while((collision_problem) && (place_tries < 10000));
	
	if (!collision_problem)
		return(TRUE);
	else
		return(FALSE);
}

BOOL Game::PlaceObjectFirstHole(GameObjectPtr obj)
{
	BOOL collision_problem = TRUE;
	int x, y;
	int i, j;
	int x_min = theScreen->GetMinX();
	int x_max = theScreen->GetMaxX();
	int y_min = theScreen->GetMinY();
	int y_max = theScreen->GetMaxY();

	// Probe every possible space on the screen for a place to 
	// put the game object.
	for (x = x_min; x <= x_max; x++)
	{
		for (y = y_min; y <= y_max; y++)
		{
			// If we haven't found the right place yet...
			if (collision_problem)
			{
				collision_problem = FALSE;
				obj->SetPosition((float) x, (float) y);

				// See if that location collides with anything.
				for (i = 0; i < num_game_objects; i++)
				{
					if ((obj->GetObjectID() != game_objects[i]->GetObjectID()) && 
						(obj->IsCollidedWith(game_objects[i])))
						collision_problem = TRUE;
				}
			}
		}
	}

	if (collision_problem)
		return(FALSE);
	else
		return(TRUE);
}

BOOL Game::PlaceObjectRandEdge(GameObjectPtr obj)
{
	return(FALSE);
}

void Game::PrepareLevel()
{
	int i;
	BOOL obj_placed;

	// Set the game not to keep restarting.
	restart_game = FALSE;

	// Set the elapsed time to zero ticks (milliseconds).
	ResetElapsedTime();

	// Initialize the variables for calculating frame rate.
	frame_count = 0;
	frame_count_timer = 0;
	old_frame_time = Start_Clock();

	// Make sure the starfield is stationary.
	theStars->SetVelocity(0.0, 0.0);

	// Set the default numbers of game objects.
	num_warhawks = 0;
	num_rwarhawks = 0;
	num_marauders = 0;
	num_stingers = 0;
	num_hhornets = 0;
	num_hnests = 0;
	num_dreadnaughts = 0;
	num_bombs = 0;
	num_asteroids = 0;
	num_wkills = 0;
	num_pturrets = 0;
	num_pl_shots = 30;
	num_pA_shots = 10;
	num_pB_shots = 10;
	num_pC_shots = 10;
	num_pD_shots = 10;
	num_dreadnaught_shots = 2;
	num_pexplosions = 10;

	// Reset the kill counts.
	num_warhawks_killed = 0;
	num_rwarhawks_killed = 0;
	num_marauders_killed = 0;
	num_stingers_killed = 0;
	num_hhornets_killed = 0;
	num_hnests_killed = 0;
	num_dreadnaughts_killed = 0;
	num_bombs_killed = 0;
	num_asteroids_killed = 0;
	num_wkills_killed = 0;
	num_pturrets_killed = 0;

	// Now, set the numbers according to what level we're
	// on.
	if (level == 0)
	{
		num_stingers = 3;
		num_hhornets = 30;
		num_hnests = 2;
		num_pB_shots = 10;
	}
	else if (level == 1)
	{
		num_asteroids = 5;
	}
	else if (level == 2)
	{
		num_asteroids = 7;
		num_bombs = 1;
	}
	else if (level == 3)
	{
		num_asteroids = 10;
		num_bombs = 2;
	}
	else if (level == 4)
	{
		num_asteroids = 5;
		num_marauders = 2;
	}
	else if (level == 5)
	{
		num_asteroids = 5;
		num_marauders = 5;
	}
	else if (level == 6)
	{
		num_asteroids = 3;
		num_bombs = 2;
		num_marauders = 5;
		num_stingers = 5;
	}
	else if (level == 7)
	{
		num_marauders = 10;
		num_stingers = 8;
		num_bombs = 2;
	}
	else if (level == 8)
	{
		num_hhornets = 20;
		num_bombs = 1;
	}
	else if (level == 9)
	{
		num_hhornets = 20;
		num_bombs = 1;
		num_wkills = 5;
	}
	else if (level == 10)
	{
		num_hhornets = 20;
		num_marauders = 3;
		num_stingers = 4;
		num_bombs = 1;
		num_wkills = 8;
	}
	else if (level == 11)
	{
		num_hhornets = 30;
		num_marauders = 3;
		num_wkills = 8;
	}
	else if (level == 12)
	{
		num_hnests = 1;
		num_wkills = 10;
		num_pturrets = 2;
		num_hhornets = 30;
	}
	else if (level == 13)
	{
		num_rwarhawks = 1;
	}
	else if (level == 14)
	{
		num_pturrets = 5;
		num_warhawks = 1;
	}
	else if (level == 15)
	{
		num_pturrets = 2;
		num_warhawks = 1;
		num_hhornets = 25;
	}
	else if (level == 16)
	{
		num_pturrets = 2;
		num_warhawks = 1;
		num_hnests = 2;
		num_marauders = 5;
		num_hhornets = 30;
	}
	else if (level == 17)
	{
		num_pturrets = 3;
		num_wkills = 10;
		num_bombs = 1;
		num_warhawks = 1;
	}
	else if (level == 18)
	{
		num_pturrets = 2;
		num_wkills = 10;
		num_hhornets = 10;
	}
	else if (level == 19)
	{
		num_pturrets = 2;
		num_wkills = 3;
		num_hnests = 2;
		num_marauders = 5;
		num_hhornets = 30;
	}
	else if (level == 20)
	{
		num_dreadnaughts = 1;
		num_hnests = 3;
		num_hhornets = 30;
	}

	// Clear all of the old positions by setting them to
	// (-500.0, -500.0).
	for (i = 0; i < num_game_objects; i++)
	{
		game_objects[i]->SetPosition(-500.0, -500.0);
	}

	// Load all game objects...  Ships, then shots.
	num_game_objects = 0;

	// Set the player's (the Valiant's) position, make him
	// alive, put him on the game object list, and randomly
	// place him.
	player_ship.SetVelocity(0.0, 0.0);
	player_ship.MakeAlive();
	if (level == 1)
		player_ship.ResetDamage();
	game_objects[num_game_objects] = &player_ship;	
	num_game_objects++;
	obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
	if (!obj_placed)
	{
		obj_placed = PlaceObjectFirstHole(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_warhawks_killed++;
	dbgLog->post_msg("Valiant Placement Failed!");
			player_ship.Kill();
		}
	}

	// Add dreadnaughts to the object list
	for (i = 0; i < num_dreadnaughts; i++)
	{
		dreadnaughts[i].SetVelocity(0.0, 0.0);
		dreadnaughts[i].MakeAlive();
		dreadnaughts[i].ResetDamage();
		dreadnaughts[i].ResetAI();
		game_objects[num_game_objects] = &dreadnaughts[i];
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_dreadnaughts_killed++;
	dbgLog->post_msg("Dreadnaught Placement Failed!");
			dreadnaughts[i].Kill();
		}
	}

	// Add hornetnests to the object list
	for (i = 0; i < num_hnests; i++)
	{
		hnests[i].SetVelocity(0.0, 0.0);
		hnests[i].MakeAlive();
		hnests[i].ResetDamage();
		hnests[i].ResetAI();
		game_objects[num_game_objects] = &hnests[i];
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_hnests_killed++;
	dbgLog->post_msg("Hornetnest Placement Failed!");
			hnests[i].Kill();
		}
	}

	// Add warhawks to the object list
	for (i = 0; i < num_warhawks; i++)
	{
		warhawks[i].SetVelocity(0.0, 0.0);
		warhawks[i].MakeAlive();
		warhawks[i].ResetDamage();
		warhawks[i].ResetAI();
		warhawks[i].TripleFireOff();
		warhawks[i].RapidFireOff();
		game_objects[num_game_objects] = &warhawks[i];
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_warhawks_killed++;
	dbgLog->post_msg("Warhawk Placement Failed!");
			warhawks[i].Kill();
		}
	}

	// Add rogue warhawks to the object list
	for (i = 0; i < num_rwarhawks; i++)
	{
		rwarhawks[i].SetVelocity(0.0, 0.0);
		rwarhawks[i].MakeAlive();
		rwarhawks[i].ResetDamage();
		rwarhawks[i].ResetAI();
		rwarhawks[i].TripleFireOn();  // note, triple fire is ON
		rwarhawks[i].RapidFireOff();
		game_objects[num_game_objects] = &rwarhawks[i];
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_rwarhawks_killed++;
	dbgLog->post_msg("Rogue Warhawk Placement Failed!");
			rwarhawks[i].Kill();
		}
	}

	// Add marauders to the object list
	for (i = 0; i < num_marauders; i++)
	{
		marauders[i].SetVelocity(0.0, 0.0);
		marauders[i].MakeAlive();
		marauders[i].ResetDamage();
		marauders[i].ResetAI();
		game_objects[num_game_objects] = &marauders[i];
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_marauders_killed++;
	dbgLog->post_msg("Marauder Placement Failed!");
			marauders[i].Kill();
		}
	}

	// Add stingers to the object list
	for (i = 0; i < num_stingers; i++)
	{
		stingers[i].SetVelocity(0.0, 0.0);
		stingers[i].MakeAlive();
		stingers[i].ResetDamage();
		stingers[i].ResetAI();
		game_objects[num_game_objects] = &stingers[i];
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_stingers_killed++;
	dbgLog->post_msg("Stinger Placement Failed!");
			stingers[i].Kill();
		}
	}

	// Position the hellhornets and make them alive.
	for (i = 0; i < num_hhornets; i++)
	{
		hhornets[i].SetVelocity(0.0, 0.0);
		hhornets[i].MakeAlive();
		hhornets[i].ResetDamage();
		hhornets[i].ResetAI();
		game_objects[num_game_objects] = &(hhornets[i]);
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_hhornets_killed++;
	dbgLog->post_msg("Hellhornet Placement Failed!");
			hhornets[i].Kill();
		}
	} 

	// Generally, if there are hornetnests around, there are 
	// no free hellhornets at the beginning of the level.
	if ((level > 0) && (num_hnests > 0))
	{
		for (i = 0; i < num_hhornets; i++)
		{
			hhornets[i].Kill();
		}
	}

	// Generally, if there are Dreadnaughts around, there are 
	// no free Buzz Bombs at the beginning of the level.
	if ((level > 0) && (num_dreadnaughts > 0))
	{
		for (i = 0; i < num_bombs; i++)
		{
			bombs[i].Kill();
		}
	}


	// Position the buzz bombs and make them alive.
	for (i = 0; i < num_bombs; i++)
	{
		bombs[i].SetVelocity(0.0, 0.0);
		bombs[i].MakeAlive();
		bombs[i].ResetDamage();
		bombs[i].ResetAI();
		game_objects[num_game_objects] = &(bombs[i]);
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_bombs_killed++;
	dbgLog->post_msg("Buzz Bomb Placement Failed!");
			bombs[i].Kill();
		}
	}

	// Position the asteroids and make them alive.
	for (i = 0; i < num_asteroids; i++)
	{
//		asteroids[i].SetVelocity(0.0,0.0);
		asteroids[i].MakeAlive();
		asteroids[i].ResetDamage();
		game_objects[num_game_objects] = &(asteroids[i]);
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_asteroids_killed++;
	dbgLog->post_msg("Asteroid Placement Failed!");
			asteroids[i].Kill();
		}
	}

	// Position the whirlakills and make them alive.
	for (i = 0; i < num_wkills; i++)
	{
		wkills[i].SetVelocity(0.0, 0.0);
		wkills[i].MakeAlive();
		wkills[i].ResetDamage();
		game_objects[num_game_objects] = &(wkills[i]);
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_wkills_killed++;
	dbgLog->post_msg("Whirlakill Placement Failed!");
			wkills[i].Kill();
		}
	}

	// Position the plasma turrets and make them alive.
	for (i = 0; i < num_pturrets; i++)
	{
		pturrets[i].SetVelocity(0.0, 0.0);
		pturrets[i].MakeAlive();
		pturrets[i].ResetDamage();
		pturrets[i].ResetAI();
		game_objects[num_game_objects] = &(pturrets[i]);
		num_game_objects++;
		obj_placed = PlaceObjectRandCenter(game_objects[num_game_objects - 1]);
		if (!obj_placed)
		{
			num_pturrets_killed++;
	dbgLog->post_msg("Plasma Turret Placement Failed!");
			pturrets[i].Kill();
		}
	}

	// Add player shots to the object list
	for (i = 0; i < num_pl_shots; i++)
	{
		player_shots[i].Kill();
		game_objects[num_game_objects] = &player_shots[i];
		num_game_objects++;
	}

	// Add other shots to the object list
	for (i = 0; i < num_pA_shots; i++)
	{
		plasma_a_shots[i].Kill();
		game_objects[num_game_objects] = &plasma_a_shots[i];
		num_game_objects++;
	}
	for (i = 0; i < num_pB_shots; i++)
	{
		plasma_b_shots[i].Kill();
		game_objects[num_game_objects] = &plasma_b_shots[i];
		num_game_objects++;
	}
	for (i = 0; i < num_pC_shots; i++)
	{
		plasma_c_shots[i].Kill();
		game_objects[num_game_objects] = &plasma_c_shots[i];
		num_game_objects++;
	}
	for (i = 0; i < num_pD_shots; i++)
	{
		plasma_d_shots[i].Kill();
		game_objects[num_game_objects] = &plasma_d_shots[i];
		num_game_objects++;
	}
	for (i = 0; i < num_dreadnaught_shots; i++)
	{
		dreadnaught_shots[i].Kill();
		game_objects[num_game_objects] = &dreadnaught_shots[i];
		num_game_objects++;
	}

	// Add point explosions to the list
	for (i = 0; i < num_pexplosions; i++)
	{
		pexplosions[i].Kill();
		game_objects[num_game_objects] = &pexplosions[i];
		num_game_objects++;
	}

	// Add the (dormant) powerup capsule to the list.
	powerup_capsule.Kill();
	game_objects[num_game_objects] = &powerup_capsule;
	num_game_objects++;

	// Reset the power-up timer.
	powerup_timer = MAX_POWERUP_INTERVAL - level * 500;

	// Reset the power-ups delivered counter.
	powerups_delivered = 0;

	if (level == 0)
	{
		// Set the player for non-alive.
		player_ship.Kill();

		// Set the three intro Stingers for non-alive.
		for (i = 0; i < num_stingers; i++)
		{
			stingers[i].Kill();
		}

		// Position the three intro stingers.
		stingers[0].SetPosition((float) 200, (float) 360);
		stingers[0].SetRotation(QUARTER_CIRCLE);
		stingers[0].SetAIState(DoNothing);
		stingers[1].SetPosition((float) 100, (float) 310);
		stingers[1].SetRotation(QUARTER_CIRCLE);
		stingers[1].SetAIState(DoNothing);
		stingers[2].SetPosition((float) 100, (float) 410);
		stingers[2].SetRotation(QUARTER_CIRCLE);
		stingers[2].SetAIState(DoNothing);

		// Set the intro Hellhornets for non-alive.
		for (i = 0; i < num_hhornets; i++)
		{
			hhornets[i].Kill();
		}

		// Set the intro Hellhornetnests for non-alive.
		for (i = 0; i < num_hnests; i++)
		{
			hnests[i].Kill();
		}

		// Position the two intro Hornetnests.
		hnests[0].SetPosition((float) 900, (float) 220);
		hnests[0].SetRotation(FULL_CIRCLE - QUARTER_CIRCLE);
		hnests[1].SetPosition((float) 900, (float) 500);
		hnests[1].SetRotation(FULL_CIRCLE - QUARTER_CIRCLE);
	}
	else if (level == 20)
	{
		// Place the dreadnaught manually.
		dreadnaughts[0].SetPosition(300,360);
		if (!dreadnaughts[0].IsAlive())
		{
			dreadnaughts[0].MakeAlive();
			num_dreadnaughts_killed--;
		}

		// Place the three hornetnests manually.
		hnests[0].SetPosition(100,100);
		if (!hnests[0].IsAlive())
		{
			hnests[0].MakeAlive();
			num_hnests_killed--;
		}
		hnests[1].SetPosition(100,600);
		if (!hnests[1].IsAlive())
		{
			hnests[1].MakeAlive();
			num_hnests_killed--;
		}
		hnests[2].SetPosition(900,150);
		if (!hnests[2].IsAlive())
		{
			hnests[2].MakeAlive();
			num_hnests_killed--;
		}

		// Place the player manually
		player_ship.SetPosition(900,700);
		if (!player_ship.IsAlive())
		{
			player_ship.MakeAlive();
			num_warhawks_killed--;
		}
	}

	if (level > 0)
	{
		// Make the first warhawk the avatar.
		player_ship.MakeAvatar();

		// Start the level music.
		switch(level)
		{
			case 1:
				sndManager->PlayMusic(2);
				break;
			case 2:
				sndManager->PlayMusic(3);
				break;
			case 3:
				sndManager->PlayMusic(4);
				break;
			case 4:
				sndManager->PlayMusic(5);
				break;
			case 5:
				sndManager->PlayMusic(6);
				break;
			case 6:
				sndManager->PlayMusic(7);
				break;
			case 7:
				sndManager->PlayMusic(8);
				break;
			case 8:
				sndManager->PlayMusic(9);
				break;
			case 9:
				sndManager->PlayMusic(10);
				break;
			case 10:
				sndManager->PlayMusic(11);
				break;
			case 11:
				sndManager->PlayMusic(12);
				break;
			case 12:
				sndManager->PlayMusic(13);
				break;
			case 13:
				sndManager->PlayMusic(14);
				break;
			case 14:
				sndManager->PlayMusic(15);
				break;
			case 15:
				sndManager->PlayMusic(16);
				break;
			case 16:
				sndManager->PlayMusic(17);
				break;
			case 17:
				sndManager->PlayMusic(18);
				break;
			case 18:
				sndManager->PlayMusic(19);
				break;
			case 19:
				sndManager->PlayMusic(20);
				break;
			case 20:
				sndManager->PlayMusic(21);
				break;
		} 
	} 
} 

BOOL Game::CheckVictory()
{
	GameObjectType obj_type;
	int i;

	// If the kill-count is as high or higher than the 
	// number of predicted guys, victory.
	if (level > 0)
	{
		if ((num_marauders_killed >= num_marauders) &&
			(num_rwarhawks_killed >= num_rwarhawks) &&
			(num_bombs_killed >= num_bombs) &&
			(num_hhornets_killed >= num_hhornets) &&
			(num_hnests_killed >= num_hnests) &&
			(num_dreadnaughts_killed >= num_dreadnaughts) &&
			(num_asteroids_killed >= num_asteroids) &&
			(num_pturrets_killed >= num_pturrets))
			return(TRUE);
	}

	// If there are no living bad-guys on the game list, 
	// victory.
	for (i = 0; i < num_game_objects; i++)
	{
		obj_type = game_objects[i]->GetType();
		if ((game_objects[i]->IsAlive()) &&
			((obj_type == MarauderObj) ||
			(obj_type == RogueWarhawkObj) ||
			(obj_type == BuzzBombObj) ||
			(obj_type == HellhornetObj) ||
			(obj_type == HornetnestObj) ||
			(obj_type == DreadnaughtObj) ||
			(obj_type == AsteroidObj) ||
			(obj_type == PlasmaTurretObj)))
			return(FALSE);
	}
}

void Game::HandleCollision(GameObjectPtr obj1, GameObjectPtr obj2)
{
	float old_vx1;
	float old_vy1;
	float old_vx2;
	float old_vy2;
	float new_vx1;
	float new_vy1;
	float new_vx2;
	float new_vy2;
	BOOL ugly_math = TRUE;
	float m1,m2;		// masses
	float v1,v2;        // velocities
	float p1,p2,pt;     // momenta (m*v)
	float ke1,ke2,ket;	// (actually 2 times) kinetic energy (m*v^2)
	float quad_a;		// a coeff for quadratic formula
	float quad_b;		// b coeff for quadratic formula
	float quad_c;		// c coeff for quadratic formula
	float q1;			// solution 1 for quadratic formula
	float q2;			// solution 2 for quadratic formula

	BOOL obj1_toasted = FALSE;
	BOOL obj2_toasted = FALSE;

	float expl_x;
	float expl_y;
	float expl_ang;
	float expl_speed;

	char buffer[80];

	int powerup_roll;


	// Update damage on objects and handle power-up pickups.

	// For now, collisions with explosions shouldn't do damage.
	if (obj1->GetType() == ExplosionObj)
	{
//		if ((!god_mode) || (obj2->GetType() != WarhawkObj))
//			obj2->Damage(obj1->GetCollideDamage());
	}
	else if (obj2->GetType() == ExplosionObj)
	{
//		if ((!god_mode) || (obj1->GetType() != WarhawkObj))
//			obj1->Damage(obj2->GetCollideDamage());
	}
	// Warhawks colliding with power-up means the warhawks 
	// get either triple-fire or rapid-fire.
	else if ((obj1->GetType() == WarhawkObj) && 
		(obj2->GetType() == PowerUpObj))
	{
		// 60% of the time turn on triple-fire.
		// 30% of the time turn on rapid-fire.
		// 10% of the time repair the ship.
		powerup_roll = rand() % 100;
		if (powerup_roll < 60)
		{
			((AIObject *) obj1)->TripleFireOn();
			sndManager->PlayPowerupTripleFire();
		}
		else if (powerup_roll < 90)
		{
			((AIObject *) obj1)->RapidFireOn();
			sndManager->PlayPowerupRapidFire();
		}
		else
		{
			obj1->ResetDamage();
			sndManager->PlayPowerupRepair();
		}
		obj2->Kill();
	}
	else if ((obj2->GetType() == WarhawkObj) &&
		(obj1->GetType() == PowerUpObj))	
	{
		// 60% of the time turn on triple-fire.
		// 30% of the time turn on rapid-fire.
		// 10% of the time repair the ship.
		powerup_roll = rand() % 100;
		if (powerup_roll < 60)
		{
			((AIObject *) obj2)->TripleFireOn();
			sndManager->PlayPowerupTripleFire();
		}
		else if (powerup_roll < 90)
		{
			((AIObject *) obj2)->RapidFireOn();
			sndManager->PlayPowerupRapidFire();
		}
		else
		{
			obj2->ResetDamage();
			sndManager->PlayPowerupRepair();
		}
		obj1->Kill();
	}	
	// Warhawks (including the player) are damaged if they 
	// collide with anything (except powerup capsules).
	else if (obj1->GetType() == WarhawkObj)
	{
		if (!god_mode)
			obj1->Damage(obj2->GetCollideDamage());
		if ((!god_mode) || (obj2->GetType() != WarhawkObj))
			obj2->Damage(obj1->GetCollideDamage());
		obj1->PlayExplodeSound();
	}
	else if (obj2->GetType() == WarhawkObj)
	{
		if (!god_mode)
			obj2->Damage(obj1->GetCollideDamage());
		if ((!god_mode) || (obj1->GetType() != WarhawkObj))
			obj1->Damage(obj2->GetCollideDamage());
		obj2->PlayExplodeSound();
	}
	else
	{
		obj1->Damage(obj2->GetCollideDamage());
		obj2->Damage(obj1->GetCollideDamage());
		if ((obj1->GetType() != ExplosionObj) &&
			(obj1->GetType() != PlayerShotObj) &&
			(obj1->GetType() != PlasmaAShotObj) &&
			(obj1->GetType() != PlasmaBShotObj) &&
			(obj1->GetType() != PlasmaCShotObj) &&
			(obj1->GetType() != PlasmaDShotObj))
			obj1->PlayExplodeSound();
		if ((obj2->GetType() != ExplosionObj) &&
			(obj2->GetType() != PlayerShotObj) &&
			(obj2->GetType() != PlasmaAShotObj) &&
			(obj2->GetType() != PlasmaBShotObj) &&
			(obj2->GetType() != PlasmaCShotObj) &&
			(obj2->GetType() != PlasmaDShotObj))
			obj2->PlayExplodeSound();
	}

	// Update the variables that say whether obj1 and obj2
	// are still alive.
	obj1_toasted = !obj1->IsAlive();
	obj2_toasted = !obj2->IsAlive();


	// Update velocities of the objects.  The key is 
	// conservation of momentum and kinetic energy: 
	// m1*v1 + m2*v2 and 0.5*m1*v1^2 + 0.5*m2*v2^2 stay the
	// same, where m1 and m2 are masses and v1 and v2 are
	// the velocities.

	// Get the x and y components of the current velocities.
	old_vx1 = obj1->GetVelocity().get_x();
	old_vy1 = obj1->GetVelocity().get_y();
	old_vx2 = obj2->GetVelocity().get_x();
	old_vy2 = obj2->GetVelocity().get_y();

	// Initialize the new velocities to the old.
	new_vx1 = old_vx1;
	new_vy1 = old_vy1;
	new_vx2 = old_vx2;
	new_vy2 = old_vy2;

	// Get v1 first (if obj1 is still alive).
	if (!obj1_toasted)
	{
		// First, avoid the ugly math if we can.  If obj2's
		// collision mass >= 1000000.0, count it as an 
		// infinite mass.
		if (obj2->GetCollisionMass() >= 1000000.0)
		{
			// If obj2 is still, just make obj1 bounce back
			// the opposite way from which it came.
			if (obj2->GetVelocity().get_dist() == 0.0)
			{
				new_vx1 = -old_vx1;
				new_vy1 = -old_vy1;
			}
			// Else add the heavy mass' velocity to the 
			// opposite of yours.  (This may be a fudge.
			// Check the math.)
			else if (obj1->GetCollisionMass() < 1000000.0)
			{
				new_vx1 = -old_vx1 + old_vx2;
				new_vy1 = -old_vy1 + old_vy2;
			}
			ugly_math = FALSE;
		}
	}

	// Get v2 (if obj2 is still alive).
	if (!obj2_toasted)
	{
		// First, avoid the ugly math if we can.  If obj1's
		// collision mass >= 1000000.0, count it as an 
		// infinite mass.
		if (obj1->GetCollisionMass() >= 1000000.0)
		{
			// If obj1 is still, just make obj2 bounce back
			// the opposite way from which it came.
			if (obj1->GetVelocity().get_dist() == 0.0)
			{
				new_vx2 = -old_vx2;
				new_vy2 = -old_vy2;
			}
			// Else add the heavy mass' velocity to the 
			// opposite of yours.  (This may be a fudge.
			// Check the math.)
			else if (obj2->GetCollisionMass() < 1000000.0)
			{
				new_vx2 = -old_vx2 + old_vx1;
				new_vy2 = -old_vy2 + old_vy1;
			}
			ugly_math = FALSE;
		}
	}

	// If we haven't already covered the collision bounce,
	// do the math.
	if ((ugly_math) && ((!obj1_toasted) || (!obj2_toasted)))
	{
		m1 = obj1->GetCollisionMass();
		m2 = obj2->GetCollisionMass();

		// Do the x components first.
		v1 = obj1->GetVelocity().get_x();
		v2 = obj2->GetVelocity().get_x();
		p1 = m1*v1;
		p2 = m2*v2;
		pt = p1 + p2;
		ke1 = m1*v1*v1;
		ke2 = m2*v2*v2;
		ket = ke1 + ke2;
		quad_a = (m1*m2+m1*m1)/m2;
		quad_b = (-2.0*m1*pt)/m2;
		quad_c = (pt*pt-ket*m2)/m2;

// BUG: We're getting an imaginary number sometimes.  I need
// to track this down.  It appears that when this happens,
// the velocities are equal
		/*
if (quad_b*quad_b - 4.0*quad_a*quad_c < 0.0)
{
	sprintf(buffer,"X Imaginary!  a = %.2f, b = %.2f, c = %.2f",
		quad_a, quad_b, quad_c);
	dbgLog->post_msg(buffer);
	sprintf(buffer,"m1 = %.2f, m2 = %.2f, v1 = %.2f, v2 = %.2f",
		m1,m2,v1,v2);
	dbgLog->post_msg(buffer);
}
else
{
//	sprintf(buffer,"Normal  a = %.2f, b = %.2f, c = %.2f",
//		quad_a, quad_b, quad_c);
//	dbgLog->post_msg(buffer);
} */

		// Solve for v1.
		q1 = (-quad_b + sqrt(quad_b*quad_b - 4.0*quad_a*quad_c))
			/(2.0*quad_a);
		q2 = (-quad_b - sqrt(quad_b*quad_b - 4.0*quad_a*quad_c))
			/(2.0*quad_a);

		// Pick the solution that isn't what we already have,
		// and solve for v2.
		if (fabs(old_vx1 - q1) < fabs(old_vx1 - q2))
			new_vx1 = q2;
		else
			new_vx1 = q1;
		new_vx2 = (pt-m1*new_vx1)/m2;

		// If the original velocities were equal, forget 
		// about the calculated results because they'll 
		// contain imaginary numbers.
		if (v1 == v2)
		{
			new_vx1 = v1;
			new_vx2 = v1;
		}

		// Now do the y components.
		v1 = obj1->GetVelocity().get_y();
		v2 = obj2->GetVelocity().get_y();
		p1 = m1*v1;
		p2 = m2*v2;
		pt = p1 + p2;
		ke1 = m1*v1*v1;
		ke2 = m2*v2*v2;
		ket = ke1 + ke2;
		quad_a = (m1*m2+m1*m1)/m2;
		quad_b = (-2.0*m1*pt)/m2;
		quad_c = (pt*pt-ket*m2)/m2;

		// Solve for v1.
		q1 = (-quad_b + sqrt(quad_b*quad_b-4.0*quad_a*quad_c))
			/(2.0*quad_a);
		q2 = (-quad_b - sqrt(quad_b*quad_b-4.0*quad_a*quad_c))
			/(2.0*quad_a);

		// Pick the solution that isn't what we already have,
		// and solve for v2.
		if (fabs(old_vy1 - q1) < fabs(old_vy1 - q2))
			new_vy1 = q2;
		else
			new_vy1 = q1;
		new_vy2 = (pt-m1*new_vy1)/m2;

		// If the original velocities were equal, forget 
		// about the calculated results because they'll 
		// contain imaginary numbers.
		if (v1 == v2)
		{
			new_vy1 = v1;
			new_vy2 = v1;
		}

	}

	// Finally, update the velocities (provided the object
	// in question isn't an explosion or a shot).
	if (!obj1_toasted && 
		(obj1->GetType() != ExplosionObj) &&
		(obj1->GetType() != PlayerShotObj) &&
		(obj1->GetType() != PlasmaAShotObj) &&
		(obj1->GetType() != PlasmaBShotObj) &&
		(obj1->GetType() != PlasmaCShotObj) &&
		(obj1->GetType() != PlasmaDShotObj))
		obj1->GetVelocity().set(new_vx1,new_vy1,FALSE);
	if (!obj2_toasted && 
		(obj2->GetType() != ExplosionObj) &&
		(obj2->GetType() != PlayerShotObj) &&
		(obj2->GetType() != PlasmaAShotObj) &&
		(obj2->GetType() != PlasmaBShotObj) &&
		(obj2->GetType() != PlasmaCShotObj) &&
		(obj2->GetType() != PlasmaDShotObj))
		obj2->GetVelocity().set(new_vx2,new_vy2,FALSE);


	// Update score if player has destroyed something.
	if ((obj1->GetType() == PlayerShotObj) && 
		(obj2_toasted))
	{
		AddScorePoints(obj2->GetKillScore());
	}
	else if ((obj2->GetType() == PlayerShotObj) && 
		(obj1_toasted))
	{
		AddScorePoints(obj1->GetKillScore());
	}


	// Update kill counts if something has been destroyed.
	if (obj1_toasted)
	{
		// Up a kill counter if appropriate.
		if (obj1->GetType() == WarhawkObj)
			num_warhawks_killed++;
		else if (obj1->GetType() == RogueWarhawkObj)
			num_rwarhawks_killed++;
		else if (obj1->GetType() == MarauderObj)
			num_marauders_killed++;
		else if (obj1->GetType() == StingerObj)
			num_stingers_killed++;
		else if (obj1->GetType() == HellhornetObj)
			num_hhornets_killed++;
		else if (obj1->GetType() == HornetnestObj)
			num_hnests_killed++;
		else if (obj1->GetType() == DreadnaughtObj)
			num_dreadnaughts_killed++;
		else if (obj1->GetType() == BuzzBombObj)
			num_bombs_killed++;
		else if (obj1->GetType() == AsteroidObj)
			num_asteroids_killed++;
		else if (obj1->GetType() == WhirlakillObj)
			num_wkills_killed++;
		else if (obj1->GetType() == PlasmaTurretObj)
			num_pturrets_killed++;
	}

	if (obj2_toasted)
	{
		// Up a kill counter if appropriate.
		if (obj2->GetType() == WarhawkObj)
			num_warhawks_killed++;
		else if (obj2->GetType() == RogueWarhawkObj)
			num_rwarhawks_killed++;
		else if (obj2->GetType() == MarauderObj)
			num_marauders_killed++;
		else if (obj2->GetType() == StingerObj)
			num_stingers_killed++;
		else if (obj2->GetType() == HellhornetObj)
			num_hhornets_killed++;
		else if (obj2->GetType() == HornetnestObj)
			num_hnests_killed++;
		else if (obj2->GetType() == DreadnaughtObj)
			num_dreadnaughts_killed++;
		else if (obj2->GetType() == BuzzBombObj)
			num_bombs_killed++;
		else if (obj2->GetType() == AsteroidObj)
			num_asteroids_killed++;
		else if (obj2->GetType() == WhirlakillObj)
			num_wkills_killed++;
		else if (obj2->GetType() == PlasmaTurretObj)
			num_pturrets_killed++;
	}


	// Create explosion objects if appropriate.
	if (obj1_toasted)
	{
		// Create an explosion object if appropriate.
		if ((obj1->GetType() != PlayerShotObj) &&
			(obj1->GetType() != PlasmaAShotObj) &&
			(obj1->GetType() != PlasmaBShotObj) &&
			(obj1->GetType() != PlasmaCShotObj) &&
			(obj1->GetType() != PlasmaDShotObj) &&
			(obj1->GetType() != ExplosionObj) &&
			((obj1->GetType() != PowerUpObj) || 
			(obj2->GetType() != WarhawkObj)))
		{
			expl_x = obj1->GetPosition().get_x();
			expl_y = obj1->GetPosition().get_y();
			expl_ang = obj1->GetVelocity().get_ang();
			expl_speed = obj1->GetVelocity().get_dist();
			SpawnGameObject(ExplosionObj, expl_x, expl_y, expl_ang, expl_speed, obj1);
		}
	}

	if (obj2_toasted)
	{
		// Create an explosion object if appropriate.
		if ((obj2->GetType() != PlayerShotObj) &&
			(obj2->GetType() != PlasmaAShotObj) &&
			(obj2->GetType() != PlasmaBShotObj) &&
			(obj2->GetType() != PlasmaCShotObj) &&
			(obj2->GetType() != PlasmaDShotObj) &&
			(obj2->GetType() != ExplosionObj) &&
			((obj2->GetType() != PowerUpObj) || 
			(obj1->GetType() != WarhawkObj)))
		{
			expl_x = obj2->GetPosition().get_x();
			expl_y = obj2->GetPosition().get_y();
			expl_ang = obj2->GetVelocity().get_ang();
			expl_speed = obj2->GetVelocity().get_dist();
			SpawnGameObject(ExplosionObj, expl_x, expl_y, expl_ang, expl_speed, obj2);
		}
	} 
}

void Game::IntroModeLoop()
{
	static BOOL stingers_added = FALSE;
	static BOOL hnests_added = FALSE;
	static BOOL stingers_attack = FALSE;
	char buffer[150];
	int i;

	// Allow the player to skip the intro (the fire key).
	if (actionManager->KeyReleased(FireForwardKey))
	{
		// Start the player on the attract screen.
		Set_Palette(bitmap8bit.palette);
		mode = AttractMode;

		// Play Inferno music.
		if (sndManager->GetTrackPlaying() != 1)
			sndManager->PlayMusic(1);
	}

	if (elapsed_time < 31500)
	{
		if ((elapsed_time > 1000) && (elapsed_time < 15000))
			theScreen->DisplayHCenterText("It had begun as a routine mission.");
	
		if ((elapsed_time > 3000) && (elapsed_time < 15000))
		{
			if (!stingers_added)
			{
				// Set three stingers for alive
				for (i = 0; i < 3; i++)
				{
					stingers[i].MakeAlive();
				}
				stingers_added = TRUE;

				// Make the starfield move to the left.
				theStars->SetVelocity(-200.0, 0.0);
			}

			theScreen->DisplayHCenterText("Three Stingers had been sent by the Terran Union to scout the Thur' Gal",3);
			theScreen->DisplayHCenterText("system in the Gamma sector for any sign of the pirates that occasionally");
			theScreen->DisplayHCenterText("plague this region of the galaxy.  All scans were clear... ");
		}

		if ((elapsed_time > 13000) && (elapsed_time < 15000))
		{
			theScreen->DisplayHCenterText("...and then they appeared...",7);
		}

		if (elapsed_time > 15000)
		{
			if (!hnests_added)
			{
				for (i = 0; i < 2; i++)
				{
					hnests[i].MakeAlive();
				}
				hnests_added = TRUE;

				// Position 15 of the Hellhornets and make them alive.
				for (i = 0; i < 15; i++)
				{
					hhornets[i].SetPosition((float) 1000, (float) (rand() % 500 - 250 + 360));
					hhornets[i].MakeAlive();
				} 

				// Stop the starfield.
				theStars->SetVelocity(0.0, 0.0);
			}

			theScreen->DisplayHCenterText("S W A R M O N G E R S",1);
		}

		if (elapsed_time > 15500)
		{
			// Play Inferno music.
			if (sndManager->GetTrackPlaying() != 1)
				sndManager->PlayMusic(1);
		}

		if ((elapsed_time > 16000) && (!stingers_attack))
		{
			// Wake the stingers up.
			stingers[0].ResetAI();
			stingers[1].ResetAI();
			stingers[2].ResetAI();
			stingers_attack = TRUE; 
		}

		// Do the level cycle for the intro.
		LevelCycle();
	}
	else
	{
		// Set the mode for the attract screen.
		Set_Palette(bitmap8bit.palette);
		mode = AttractMode;
	}
}

void Game::AttractModeLoop()
{
	int top_margin = 37;
//	theScreen->DisplayHCenterText("S W A R M O N G E R S",1);
//	theScreen->DisplayHCenterText("ADD COOL LOGO HERE, SOMEONE!!",20);

	theScreen->DisplayHCenterText("Constructed and designed by George Chadderdon", top_margin);
	theScreen->DisplayHCenterText("with special thanks going to:", top_margin + 2);
	theScreen->DisplayHCenterText("Magic Lantern Playware and 'Windows Game Programming for Dummies', by Andre LaMothe", top_margin + 4);

	// Display the logo
	DD_Lock_Back_Surface();
	Draw_Bitmap(&logo_bmp,back_buffer, back_lpitch, 0);
	DD_Unlock_Back_Surface();

	theScreen->DisplayHCenterText("Hit Spacebar to Start, Enter for Instructions",top_margin + 7);

	// See if we need to start the game (the fire key).
	if (actionManager->KeyReleased(FireForwardKey))
	{
		// Stop the intro music
		sndManager->StopMusic();

		// Reset the score.
		ResetScore();

		// Set the level and number of ships.
		level = 1;
		num_player_ships = 3;
		new_level = TRUE;
		player_just_exploded = FALSE;

		// Set the elapsed time to zero ticks (milliseconds).
		ResetElapsedTime();

		// Set the mode for intermission.
		Load_Palette_From_File("DPAINT.PAL",palette);
		Set_Palette(palette);
		mode = ChapterMode;
	}

	// See if you need to load instructions.  (the Help Key)
	if (actionManager->KeyReleased(HelpKey))
	{
		// Set the mode to instructions.
		Load_Palette_From_File("DPAINT.PAL",palette);
		Set_Palette(palette);
		mode = InstructMode;
	}
}

void Game::InstructionsModeLoop()
{
	int left_margin = 100;
	int first_line_num = 5;
	int obj_y_offset = 30;

	theScreen->DisplayHCenterText("S W A R M O N G E R S",1);

	theScreen->DisplayText("CONTROLS",500,17);
	theScreen->DisplayText("Left Turn",525,19);
	theScreen->DisplayText("Right Turn",525,21);
	theScreen->DisplayText("Fire",525,23);
	theScreen->DisplayText("Thrust",525,25);
	theScreen->DisplayText("Toggle Autopilot",525,27);

	theScreen->DisplayText("Left Arrow or Mouse Movement",700,19);
	theScreen->DisplayText("Right Arrow or Mouse Movement",700,21);
	theScreen->DisplayText("Spacebar or Left-Mouse",700,23);
	theScreen->DisplayText("Up Arrow or Right-Mouse",700,25);
	theScreen->DisplayText("Down Arrow",700,27);

	theScreen->DisplayText("THE VALIANT (YOU)",left_margin,first_line_num);
	theScreen->DisplayText("NEUTRAL PARTIES",left_margin,first_line_num + 4);
	theScreen->DisplayText("ENEMY FORCES",left_margin,first_line_num + 12);
	theScreen->DisplayText("FRIENDLY FORCES",left_margin,first_line_num + 31);

	theScreen->DisplayText("Whirlakill (0 pts.)",left_margin+125,first_line_num + 6);
	theScreen->DisplayText("Asteroid (50 pts.)",left_margin+125,first_line_num + 9);
	theScreen->DisplayText("Hellhornet (50 pts.)",left_margin+125,first_line_num + 14);
	theScreen->DisplayText("Buzz Bomb (100 pts.)",left_margin+125,first_line_num + 17);
	theScreen->DisplayText("Marauder (100 pts.)",left_margin+125,first_line_num + 20);
	theScreen->DisplayText("Plasma Turret (500 pts.)",left_margin+125,first_line_num + 23);
	theScreen->DisplayText("Hornetnest (1,000 pts.)",left_margin+125,first_line_num + 27);
	theScreen->DisplayText("Stinger (-200 pts.)",left_margin+125,first_line_num + 33);
	theScreen->DisplayText("Warhawk (-1,000 pts.)",left_margin+125,first_line_num + 36);
	theScreen->DisplayText("Power-up Capsule (-100 pts.)",left_margin+125,first_line_num + 39);

	// Position your ship.
	player_ship.SetPosition(left_margin+50, 65+obj_y_offset);
	player_ship.SetRotation(0.0);
	player_ship.MakeAlive();

	// Position a Whirlakill.
	wkills[0].SetPosition(left_margin+50,130+obj_y_offset);
	wkills[0].SetRotation(0.0);
	wkills[0].MakeAlive();

	// Position an Asteroid.
	asteroids[0].SetPosition(left_margin+50,175+obj_y_offset);
	asteroids[0].SetRotation(0.0);
	asteroids[0].MakeAlive();

	// Position a Hellhornet.
	hhornets[0].SetPosition(left_margin+50,248+obj_y_offset);
	hhornets[0].SetRotation(0.0);
	hhornets[0].MakeAlive();

	// Position a Buzz Bomb.
	bombs[0].SetPosition(left_margin+50,294+obj_y_offset);
	bombs[0].SetRotation(0.0);
	bombs[0].MakeAlive();

	// Position a Marauder.
	marauders[0].SetPosition(left_margin+50,338+obj_y_offset);
	marauders[0].SetRotation(0.0);
	marauders[0].MakeAlive();

	// Position a Plasma Turret.
	pturrets[0].SetPosition(left_margin+50,385+obj_y_offset);
	pturrets[0].SetRotation(0.0);
	pturrets[0].MakeAlive();

	// Position a Hornetnest.
	hnests[0].SetPosition(left_margin+50,442+obj_y_offset);
	hnests[0].SetRotation(0.0);
	hnests[0].MakeAlive();

	// Position a Stinger.
	stingers[0].SetPosition(left_margin+50,531+obj_y_offset);
	stingers[0].SetRotation(0.0);
	stingers[0].MakeAlive();

	// Position a Warhawk.
	warhawks[0].SetPosition(left_margin+50,577+obj_y_offset);
	warhawks[0].SetRotation(0.0);
	warhawks[0].MakeAlive();

	// Position a Power-up.
	powerup_capsule.SetPosition(left_margin+50,624+obj_y_offset);
	powerup_capsule.SetRotation(-QUARTER_CIRCLE);
	powerup_capsule.MakeAlive();

	// Lock back surface
	DD_Lock_Back_Surface();
	
	// Draw sample ships.
	player_ship.Draw();
	wkills[0].Draw();
	asteroids[0].Draw();
	hhornets[0].Draw();
	bombs[0].Draw();
	marauders[0].Draw();
	pturrets[0].Draw();
	hnests[0].Draw();
	stingers[0].Draw();
	warhawks[0].Draw();
	powerup_capsule.Draw();

	// Unlock the secondary surface
	DD_Unlock_Back_Surface();

	theScreen->DisplayHCenterText("Hit Spacebar to Go Back",48);

	// See if we need to go back to the attractor screen (the fire key).
	if (actionManager->KeyReleased(FireForwardKey))
	{
		// Kill any objects we made alive for display.
		player_ship.Kill();
		wkills[0].Kill();
		asteroids[0].Kill();
		hhornets[0].Kill();
		bombs[0].Kill();
		marauders[0].Kill();
		pturrets[0].Kill();
		hnests[0].Kill();
		dreadnaughts[0].Kill();
		stingers[0].Kill();
		warhawks[0].Kill();
		rwarhawks[0].Kill();
		powerup_capsule.Kill();

		// Set the mode to attract.
		Set_Palette(bitmap8bit.palette);
		mode = AttractMode;
	}
}

void Game::ChapterLoop()
{
	char buffer[80];

	// Allow the player to skip the chapter screen.
	if ((elapsed_time > 1000) && (actionManager->KeyReleased(FireForwardKey)))
	{
		// Set the elapsed time to zero ticks (milliseconds).
		ResetElapsedTime();

		sndManager->StopMusic();
		mode = IntermissionMode;
	} 

	// Display the appropriate text until we're ready to
	// continue game-play.
	if (elapsed_time < 20000)
	{
		// Start the chapter music if it isn't already started
		if (sndManager->GetTrackPlaying() != 22)
		{
			sndManager->StopMusic();
			sndManager->PlayMusic(22);
		}

		// Display the appropriate text and splash screens
		if (level == 1)
		{
			theScreen->DisplayHCenterText("[Picture of Valiant struggling through asteroid field]",20);

			theScreen->DisplayHCenterText("A large number of Grell vessels have been sighted in the Thur' Gal system near the planet Throk.",41);
			theScreen->DisplayHCenterText("Proceed there immediately and eliminate them before they invade the planet's surface.  To avoid",42);
			theScreen->DisplayHCenterText("attracting unwanted attention, Lieutenant, travel through the asteroid belt.",43);
		}
		else if (level == 7)
		{
			theScreen->DisplayHCenterText("[Picture of Valiant blasting Hellhornets near Throk]",20);

			theScreen->DisplayHCenterText("Lieutenant.  You're almost in orbiting range of the planet Throk.  The Grell",41);
			theScreen->DisplayHCenterText("fleet has deployed several Hellhornets that are headed for the surface of",42);
			theScreen->DisplayHCenterText("the planet.  Proceed there immediately and neutralize the invading forces.",43);
		}
		else if (level == 13)
		{
			theScreen->DisplayHCenterText("[Picture of battle between Valiant and Rogue Warhawk]",20);

			theScreen->DisplayHCenterText("The Grell have captured one of our Warhawks, and have used this to destroy the entire Fifth",41);
			theScreen->DisplayHCenterText("Wing.  Using its auto-pilot coordinates, the captured Warhawk has set a direct course to our",42);
			theScreen->DisplayHCenterText("base in orbit around the planet Oliphans.  Intercept and destroy this rogue ship.",43);
		}
		else if (level == 14)
		{
			theScreen->DisplayHCenterText("[Picture of 4 Warhawks pursuing the fleeing Grell fleet]",20);

			theScreen->DisplayHCenterText("With the relief of the planet Throk, the Grell fleet appears to be withdrawing to the perimeter",41);
			theScreen->DisplayHCenterText("of the Thur' Gal system.  Pursue and destroy all of the enemy vessels.  Watch out for the Plasma",42);
			theScreen->DisplayHCenterText("Turrets the Grell have deployed to cover their withdrawal.",43);
		}
		else if (level == 20)
		{
			theScreen->DisplayHCenterText("[Picture of Grell Dreadnaught]",20);

			theScreen->DisplayHCenterText("The Grell fleet has nearly been eliminated.  However, their flagship and its escort of 3 Hornetnests",41);
			theScreen->DisplayHCenterText("have decided to turn and fight.  Neutralize them and end the Grell presence in the Thur' Gal ",42);
			theScreen->DisplayHCenterText("system.  Avoid at all costs the Dreadnaught's forward positron cannon.  Good luck, Lieutenant!",43);
		}
	}
	else
	{
		// Set the elapsed time to zero ticks (milliseconds).
		ResetElapsedTime();

		sndManager->StopMusic();
		mode = IntermissionMode;
	}
}

void Game::IntermissionLoop()
{
	char buffer[80];

	// Display the appropriate text until we're ready to
	// continue game-play.
	if (elapsed_time < 3000)
	{
		// If we're starting a new level, kill the old
		// level's music.
		if ((sndManager->GetTrackPlaying() != 0) &&
			(new_level))
			sndManager->StopMusic(); 

		if ((new_level) && (!played_level_start_snd))
		{
			sndManager->PlayLevelStart();
			played_level_start_snd = TRUE;
		}

		// If we're starting a new level and haven't just
		// died, show the level.
		if ((new_level) && (!player_just_exploded))
		{
			sprintf(buffer,"LEVEL %d", level);
			theScreen->DisplayHCenterText(buffer,25);
			if (level % 4 == 0)
				theScreen->DisplayHCenterText("BONUS SHIP!",27);
		}
		// If we're starting a new level, but have also 
		// died, show the level and ships remaining.
		else if (new_level)
		{
			if (num_player_ships > 0)
			{
				sprintf(buffer,"LEVEL %d", level);
				theScreen->DisplayHCenterText(buffer,24);
				if (num_player_ships > 1)
				{
					sprintf(buffer,"%d ships remaining", num_player_ships);
					theScreen->DisplayHCenterText(buffer,26);
				}
				else
				{
					theScreen->DisplayHCenterText("1 ship remaining",26);
				}
				if (level % 4 == 0)
					theScreen->DisplayHCenterText("BONUS SHIP!",28);
			}
			else
			{
				theScreen->DisplayHCenterText("GAME OVER",24);
				sprintf(buffer,"SCORE %d", score);
				theScreen->DisplayHCenterText(buffer,26);
			}	
		}
		// If the player died in the middle of the level,
		// show just the number of ships left.
		else if (player_just_exploded)
		{
			if (num_player_ships > 0)
			{
				if (num_player_ships > 1)
				{
					sprintf(buffer,"%d ships remaining", num_player_ships);
					theScreen->DisplayHCenterText(buffer,25);
				}
				else
				{
					theScreen->DisplayHCenterText("1 ship remaining",25);
				}
			}
			else
			{
				theScreen->DisplayHCenterText("GAME OVER",24);
				sprintf(buffer,"SCORE %d", score);
				theScreen->DisplayHCenterText(buffer,26);
			}	
		}
	}
	// We're ready to re-enter game-play.
	else
	{
		// If we still have ships left, enter Play Mode.
		if (num_player_ships > 0)
		{
			// If the player is starting a new_level,
			// prepare it.
			if (new_level)
				PrepareLevel(); 
	
			// Switch to play mode.
			new_level = FALSE;
			player_just_exploded = FALSE;
			mode = PlayMode;

			// Give a pause so player can get bearings.
			start_pause_timer = LEVEL_START_PAUSE;
		}
		// Otherwise, go to the attractor screen.
		else
		{
			sndManager->StopMusic();
			Set_Palette(bitmap8bit.palette);
			mode = AttractMode;
		}
	}
}


void Game::PlayModeLoop()
{
	char buffer[80];
	POINT curs_pt;
	int curs_obj_ind;
	static BOOL draw_error = FALSE;
	Vect p;
	int i, j;
	BOOL any_player_children;
	BOOL obj_placed;
	int hull_left;

	// Process cheat keys...

	// See if we need to toggle triple-fire mode.
	if (actionManager->KeyReleased(TripleFireCheatKey))
		player_ship.ToggleTripleFire();

	// See if we need to toggle rapid-fire mode.
	if (actionManager->KeyReleased(RapidFireCheatKey))
	{
		player_ship.ToggleRapidFire();
	}

	// See if we need to toggle god mode.
	if (actionManager->KeyReleased(GodModeCheatKey))
	{
		god_mode = !god_mode;
		if (god_mode)
			dbgLog->post_msg("God Mode On");
		else
			dbgLog->post_msg("God Mode Off");
	}

	// See if we need to do an unconditional restart.
	if (actionManager->KeyReleased(ForceLevelRestartKey))
	{
		restart_game = TRUE;
	}

	// See if we need to toggle autopilot.
	if (actionManager->KeyReleased(AutopilotKey))
	{
		if (player_ship.IsAvatar())
		{
			player_ship.MakeNotAvatar();
			dbgLog->post_msg("Autopilot Off");
		}
		else
		{
			player_ship.MakeAvatar();
			dbgLog->post_msg("Autopilot On");
		}
	}

	// See if we need to toggle pause-mode.
	if (actionManager->KeyReleased(PauseKey))
	{
		pause_mode = !pause_mode;
		if (pause_mode)
			dbgLog->post_msg("Pause On");
		else
			dbgLog->post_msg("Pause Off");
	}

	// See if we need to toggle debug display mode.
	if (actionManager->KeyReleased(DebugDispKey))
	{
		dbg_disp_mode = !dbg_disp_mode;
		if (dbg_disp_mode)
		{
			ShowCursor(TRUE);
			actionManager->SetMousePlayerOff();
		}
		else
		{
			ShowCursor(FALSE);
			actionManager->SetMousePlayerOn();
		}
	}

	// See if we need to toggle debug log mode.
	if (actionManager->KeyReleased(DebugLogKey))
	{
		dbg_log_mode = !dbg_log_mode;
	}

	// Check for victory.
	if ((CheckVictory()) && (!new_level))
	{
		level++;
		if (level % 4 == 0)
			num_player_ships++;

		new_level = TRUE;
		played_level_start_snd = FALSE;
		if (player_ship.IsAlive())
		{
			// Set the elapsed time to zero ticks (milliseconds).
			ResetElapsedTime();

			if ((level == 1) ||
				(level == 7) || 
				(level == 13) ||
				(level == 14) ||
				(level == 20))
				mode = ChapterMode;
			else
				mode = IntermissionMode;
		}

		if (level == 21)
		{
			// Set the elapsed time to zero ticks (milliseconds).
			ResetElapsedTime();

			mode = WonMode;
		}
	}

	// Check to see if the player has just died.
	if (!player_ship.IsAlive())
	{
		any_player_children = FALSE;

		// Look for any live objects spawned by the player (shots
		// or explosions).
		for (i = 0; i < num_game_objects; i++)
			if ((game_objects[i]->IsChildOf(&player_ship)) &&
				(game_objects[i]->IsAlive()))
				any_player_children = TRUE;

		// If there are no player children floating around,
		// get ready for the next ship.
		if (!any_player_children)
		{
			num_player_ships--;

			// Make the player alive, put him on the game 
			// object list, and set his position, 
			player_ship.SetVelocity(0.0, 0.0);
			player_ship.MakeAlive();
			player_ship.ResetDamage();
			obj_placed = PlaceObjectRandCenter(&player_ship);  // must be done after he is made alive
			if (!obj_placed)
			{
				obj_placed = PlaceObjectFirstHole(game_objects[num_game_objects - 1]);
				if (!obj_placed)
				{
					num_warhawks_killed++;
			dbgLog->post_msg("Valiant Placement Failed!");
					player_ship.Kill();
				}
			}

			if (level == 20)
			{
				// Place the player manually
				player_ship.SetPosition(900,700);
				if (!player_ship.IsAlive())
				{
					player_ship.MakeAlive();
					num_warhawks_killed--;
				}
			}

			player_just_exploded = TRUE;

			// Player loses triple-fire and rapid-fire.  Doh!!
			player_ship.TripleFireOff();
			player_ship.RapidFireOff();

			// Set the elapsed time to zero ticks (milliseconds).
			ResetElapsedTime();
	
			mode = IntermissionMode;
		}
	}

	// Do the update, draw level cycle.
	LevelCycle();

	// Draw the number of ships the player has left at
	// the top of the screen.
	Draw_Text_GDI("SHIPS:",0,1,RGB(0,255,255),lpddsback);

	// Lock back surface
	DD_Lock_Back_Surface();

	for (i = 0; i < num_player_ships; i++)
	{
		ship_count_decal.DecalDraw(i * 17 + 58,9);
	}

	// Unlock back surface
	DD_Unlock_Back_Surface();

	// Draw an indicator for triple-fire at the top of the screen.
	if (((AIObject) player_ship).IsTripleFireOn())
	{
		Draw_Text_GDI("Triple-Fire",175,1,RGB(255,255,0),lpddsback);
	}

	// Draw an indicator for rapid-fire at the top of the screen.
	if (((AIObject) player_ship).IsRapidFireOn())
	{
		Draw_Text_GDI("Rapid-Fire",320,1,RGB(255,0,255),lpddsback);
	} 

	// Draw the score at the top of the screen.
	sprintf(buffer, "SCORE: %d", score);
	Draw_Text_GDI(buffer,theScreen->GetWidth()  / 2 - 4 *strlen(buffer),
		1,RGB(255,255,255),lpddsback);

	// Draw the player's hull points.
	if (player_ship.IsAlive())
	{
		hull_left = player_ship.GetStructPoints();
		sprintf(buffer, "HULL: %d", hull_left);
		
		if (hull_left > 60)
			Draw_Text_GDI(buffer,850,1,RGB(0,255,0),lpddsback);
		else if (hull_left > 25)
			Draw_Text_GDI(buffer,850,1,RGB(255,255,0),lpddsback);
		else
			Draw_Text_GDI(buffer,850,1,RGB(255,0,0),lpddsback);	
	}
	else
	{
		Draw_Text_GDI("Destroyed",800,
			1,RGB(128,0,0),lpddsback);
	} 
}

void Game::WonModeLoop()
{
	char buffer[80];

	// Display the appropriate text until we're ready to stop the game.
	if (elapsed_time < 20000)
	{
		// Start the chapter music if it isn't already started
		if (sndManager->GetTrackPlaying() != 22)
		{
			sndManager->StopMusic();
			sndManager->PlayMusic(22);
		} 

		theScreen->DisplayHCenterText("[Picture of Dreadnaught exploding]",20);
		theScreen->DisplayHCenterText("YOU WON!  YOU RULE!",25);

		theScreen->DisplayHCenterText("MISSION SUCCESS!!  With the destruction of the Grell invasion fleet, the habitable worlds of",41);
		theScreen->DisplayHCenterText("the Thur' Gal system have been secured.  Brilliant work, Commander!  Return to base...",42);
	}
	// Otherwise, go to the attractor screen.
	else
	{
		sndManager->StopMusic();
		Set_Palette(bitmap8bit.palette);
		mode = AttractMode;
	}

	// Allow the player to skip the win screen.
	if ((elapsed_time > 1000) && (actionManager->KeyReleased(FireForwardKey)))
	{
		sndManager->StopMusic();
		Set_Palette(bitmap8bit.palette);
		mode = AttractMode;
	}

}

void Game::PaletteModeLoop()
{
	char buffer[80];
	POINT curs_pt;
	int c, j;

	// Lock back surface
	DD_Lock_Back_Surface();
	
	for (c = 0; c < 256; c++)
	{
		for (j = 0; j < 4; j++)
		{
			Draw_Line((c*4)+j, 0, (c*4)+j,
				700, c, back_buffer, 
				back_lpitch);
		}
	}

	// Unlock the secondary surface
	DD_Unlock_Back_Surface();

	// Get the cursor position so we point to the color and 
	// read it's value
	GetCursorPos(&curs_pt);

	// Display Mouse
	sprintf(buffer,"Color = %d",curs_pt.x / 4);
	Draw_Text_GDI(buffer,1,710,RGB(255,0,0),lpddsback);
}

void Game::LevelCycle()
{
	char buffer[80];
	POINT curs_pt;
	int curs_obj_ind;
	static BOOL draw_error = FALSE;
	BOOL obj_placed;
	Vect p;
	int i, j;

	// If we are paused, skip to drawing...
	if ((!pause_mode) && (start_pause_timer <= 0))
	{ 
		// Check to see if we need to add a power-up to the 
		// screen.  If so, add it.  If not, count-down the 
		// timer.
		if ((!powerup_capsule.IsAlive()) && 
			(powerup_timer <= 0) && (level != 0) &&
			(powerups_delivered < POWERUPS_PER_LEVEL))
		{ 
			powerups_delivered++;

			// Randomly place the power-up, and make it 
			// alive.
			powerup_capsule.MakeAlive();
			obj_placed = PlaceObjectRandCenter(&powerup_capsule);
			if (!obj_placed)
			{
				powerup_capsule.Kill();
	dbgLog->post_msg("Powerup Placement Failed!");
			}

			// Reset the power-up timer.
			powerup_timer = MAX_POWERUP_INTERVAL - level * 500;
		}
		else if (powerup_timer > 0)
			CountdownTickTimer(powerup_timer);  /* this line crashes release mode */

		// Update detects for all AI-controlled game objects.
		for (i = 0; i < num_game_objects; i++)
		{
			// Only update if game object is alive and has AI
			if ((game_objects[i]->IsAlive()) && 
				(game_objects[i]->HasAI()))
			{
				// First clear all detects.
				((AIObject *) game_objects[i])->ClearDetects(); 

				// Add detects for any other objects.
				for (j = 0; j < num_game_objects; j++)
				{
					// No need to detect yourself or things no
					// longer alive.
					if ((i != j) && (game_objects[j]->IsAlive()))
					{
						((AIObject *) game_objects[i])->AddPossibleDetect(game_objects[j]);
					}
				}
			}
		}
	
		// Update the star field.
		theStars->Update();

		// Update each game object...
		for (i = 0; i < num_game_objects; i++)
			game_objects[i]->Update(); 

		// Process collisions (killing off entities as 
		// necessary).
		for (i = 0; i < num_game_objects; i++)
		{
			for (j = i + 1; j < num_game_objects; j++)
			{
				// Check for a collision.
				if (game_objects[i]->IsCollidedWith(game_objects[j]))
					HandleCollision(game_objects[i], game_objects[j]);
			}
		}
	}

	// Update the start pause timer if necessary.
	if (start_pause_timer > 0)
		CountdownTickTimer(start_pause_timer);

	// Handle the debugging mode. 
	if (dbg_disp_mode)
	{
		// Get the cursor position so we point to objects
		GetCursorPos(&curs_pt);

		// Determine which object (if any) the cursor is on.
		curs_obj_ind = -1;
		for (i = 0; i < num_game_objects; i++)
		{
			if ((game_objects[i]->GetPosition().distance_to(curs_pt.x, curs_pt.y)
				< game_objects[i]->GetCollisionRad()) &&
				(game_objects[i]->IsAlive()))
				curs_obj_ind = i;
		}
		if (curs_obj_ind != -1)
			game_objects[curs_obj_ind]->dbg_display(50,50);
	} 

	// Draw all of the game objects...

	// Lock back surface
	DD_Lock_Back_Surface();

	// If we're at the start of a level, draw everything the 
	// "ghost-color".  Otherwise, draw everything normally.
	if (start_pause_timer > 0)
	{
		// Draw the star field.
		theStars->Draw(TRUE);

		// Draw the game objects ghostly (except the player).
		for (i = 0; i < num_game_objects; i++)
		{
			if (game_objects[i]->GetObjectID() == player_ship.GetObjectID())
			{
				if (!(game_objects[i]->Draw()))
					draw_error = TRUE;
			}
			else
			{
				if (!(game_objects[i]->Draw(TRUE)))
					draw_error = TRUE;
			}
		}
	}
	else
	{
		// Draw the star field.
		theStars->Draw();

		// Draw the game objects.
		for (i = 0; i < num_game_objects; i++)
		{
			if (!(game_objects[i]->Draw()))
				draw_error = TRUE;
		} 
	}

	// Unlock the secondary surface
	DD_Unlock_Back_Surface();

	if (draw_error)
		Draw_Text_GDI("OWWW!! I tried to draw off the screen!", 200, 200, RGB(255,0,0),lpddsback);	
}
