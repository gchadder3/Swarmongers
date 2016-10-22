// ActionEventManager.h
//
// The header file for the ActionEvents object.

#ifndef _ACTION_EVENT_MANAGER_H
#define _ACTION_EVENT_MANAGER_H


// INCLUDES

#include "GPDUMB_includes.h"


const int MOUSE_COUNTER_CLIP = 10;

enum ActionKeys
{
	LeftSteerKey,
	RightSteerKey,
	MainThrustKey,
	FireForwardKey,
	HelpKey,
	AutopilotKey,
	TripleFireCheatKey,
	RapidFireCheatKey,
	GodModeCheatKey,
	ForceLevelRestartKey,
	PauseKey,
	DebugDispKey,
	DebugLogKey,
	PaletteKey,
	ActionKeysCount
};

class ActionEventManager
{
public:
	ActionEventManager()
	{
		key_bindings[LeftSteerKey] = DIK_LEFT;
		key_bindings[RightSteerKey] = DIK_RIGHT;
		key_bindings[MainThrustKey] = DIK_UP;
		key_bindings[FireForwardKey] = DIK_SPACE;
		key_bindings[HelpKey] = DIK_RETURN;
		key_bindings[AutopilotKey] = DIK_DOWN;
		key_bindings[TripleFireCheatKey] = DIK_F1;
		key_bindings[RapidFireCheatKey] = DIK_F2;
		key_bindings[GodModeCheatKey] = DIK_F3;
		key_bindings[ForceLevelRestartKey] = DIK_F4;
		key_bindings[PauseKey] = DIK_F5;
		key_bindings[DebugDispKey] = DIK_F6;
		key_bindings[DebugLogKey] = DIK_F7;
		key_bindings[PaletteKey] = DIK_F8;

		keys_down_last_frame[LeftSteerKey] = FALSE;
		keys_down_last_frame[RightSteerKey] = FALSE;
		keys_down_last_frame[MainThrustKey] = FALSE;
		keys_down_last_frame[FireForwardKey] = FALSE;
		keys_down_last_frame[HelpKey] = FALSE;
		keys_down_last_frame[AutopilotKey] = FALSE;
		keys_down_last_frame[TripleFireCheatKey] = FALSE;
		keys_down_last_frame[RapidFireCheatKey] = FALSE;
		keys_down_last_frame[GodModeCheatKey] = FALSE;
		keys_down_last_frame[ForceLevelRestartKey] = FALSE;
		keys_down_last_frame[PauseKey] = FALSE;
		keys_down_last_frame[DebugDispKey] = FALSE;
		keys_down_last_frame[DebugLogKey] = FALSE;
		keys_down_last_frame[PaletteKey] = FALSE;

		mouse_player_on = TRUE;
		mouse_counter = 0;
	}

	BOOL KeyDown(ActionKeys key_type)
	{
		UCHAR the_key;

		// Handle special cases of left and right turns, 
		// trapping mouse motions.
		if (mouse_player_on)
		{
			if (key_type == LeftSteerKey)
			{
				if (mouse_counter < 0)
					return(TRUE);
			}
			else if (key_type == RightSteerKey)
			{
				if (mouse_counter > 0)
					return(TRUE);
			}
			else if (key_type == FireForwardKey)
			{
				if (mouse_state.rgbButtons[0])
					return(TRUE);
			}
			else if (key_type == MainThrustKey)
			{
				if (mouse_state.rgbButtons[1])
					return(TRUE);
			}
		}

		the_key = key_bindings[key_type];
		return(keyboard_state[the_key]);
	}

	BOOL KeyPressed(ActionKeys key_type)
	{
		UCHAR the_key;

		the_key = key_bindings[key_type];
		if ((keyboard_state[the_key]) &&
			(!keys_down_last_frame[key_type]))
			return(TRUE);
		else
			return(FALSE);
	}

	BOOL KeyReleased (ActionKeys key_type)
	{
		UCHAR the_key;

		the_key = key_bindings[key_type];
		if ((!keyboard_state[the_key]) &&
			(keys_down_last_frame[key_type]))
			return(TRUE);
		else
			return(FALSE);
	}

	void SetMousePlayerOff()
	{
		mouse_player_on = FALSE;
	}

	void SetMousePlayerOn()
	{
		mouse_player_on = TRUE;
	}

	void Update()
	{
		keys_down_last_frame[LeftSteerKey] = KeyDown(LeftSteerKey);
		keys_down_last_frame[RightSteerKey] = KeyDown(RightSteerKey);
		keys_down_last_frame[MainThrustKey] = KeyDown(MainThrustKey);
		keys_down_last_frame[FireForwardKey] = KeyDown(FireForwardKey);
		keys_down_last_frame[HelpKey] = KeyDown(HelpKey);
		keys_down_last_frame[AutopilotKey] = KeyDown(AutopilotKey);
		keys_down_last_frame[TripleFireCheatKey] = KeyDown(TripleFireCheatKey);
		keys_down_last_frame[RapidFireCheatKey] = KeyDown(RapidFireCheatKey);
		keys_down_last_frame[GodModeCheatKey] = KeyDown(GodModeCheatKey);
		keys_down_last_frame[ForceLevelRestartKey] = KeyDown(ForceLevelRestartKey);
		keys_down_last_frame[PauseKey] = KeyDown(PauseKey);
		keys_down_last_frame[DebugDispKey] = KeyDown(DebugDispKey);
		keys_down_last_frame[DebugLogKey] = KeyDown(DebugLogKey);
		keys_down_last_frame[PaletteKey] = KeyDown(PaletteKey);

		mouse_counter += mouse_state.lX;
		if (mouse_counter > MOUSE_COUNTER_CLIP)
			mouse_counter = MOUSE_COUNTER_CLIP;
		else if (mouse_counter < -MOUSE_COUNTER_CLIP)
			mouse_counter = -MOUSE_COUNTER_CLIP;
		if (mouse_counter > 0)
			mouse_counter--;
		else if (mouse_counter < 0)
			mouse_counter++;
	}

private:
	UCHAR key_bindings[ActionKeysCount];
	BOOL keys_down_last_frame[ActionKeysCount];
	BOOL mouse_player_on;

	int mouse_counter;
};

#endif
