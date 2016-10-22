// SoundManager.cpp
//
// Source file for the SoundManager class.  

// INCLUDES

#include "GPDUMB_includes.h"
#include "Game.h"
#include "SoundManager.h"



// Globals from main.cpp
extern Game *theGame;


// PUBLIC FUNCTIONS

SoundManager::SoundManager()
{
	terran_gen_fire_snd_id = -1;
	terran_gen_explode_snd_id = -1;
	stinger_thrust_snd_id = -1;
	warhawk_thrust_snd_id = -1;
	valiant_thrust_snd_id = -1;
	valiant_fire_snd_id = -1;

	grell_gen_thrust_snd_id = -1;
	grell_gen_fire_snd_id = -1;
	grell_gen_explode_snd_id = -1;
	buzzbomb_thrust_snd_id = -1;
	hhornet_spawn_snd_id = -1;
	dnaught_fire_snd_id = -1;
	dnaught_light_dam_snd_id = -1;
	dnaught_heavy_dam_snd_id = -1;
	dnaught_explode_snd_id = -1;

	non_com_explode_snd_id = -1;
	powerup_triple_snd_id = -1;
	powerup_rapid_snd_id = -1;
	powerup_repair_snd_id = -1;
	level_start_snd_id = -1;

	stinger_thrust_timer = 0;  
	warhawk_thrust_timer = 0;  
	valiant_thrust_timer = 0;  
	grell_gen_thrust_timer = 0;   
	buzzbomb_thrust_timer = 0; 

	music_trk1_id = -1;
	music_trk2_id = -1;
	music_trk3_id = -1;
	music_trk4_id = -1;
	music_trk5_id = -1;
	music_trk6_id = -1;
	music_trk7_id = -1;
	music_trk8_id = -1;
	music_trk9_id = -1;
	music_trk10_id = -1;
	music_trk11_id = -1;
	music_trk12_id = -1;
	music_trk13_id = -1;
	music_trk14_id = -1;
	music_trk15_id = -1;
	music_trk16_id = -1;
	music_trk17_id = -1;
	music_trk18_id = -1;
	music_trk19_id = -1;
	music_trk20_id = -1;
	music_trk21_id = -1;
	music_trk22_id = -1;

	music_trk_playing = 0;
}

BOOL SoundManager::LoadSounds()
{
	if ((terran_gen_fire_snd_id = Load_WAV("New Sounds/Terran/Terran Generic Fire 01.wav")) == -1)
		return(FALSE);

	if ((terran_gen_explode_snd_id = Load_WAV("New Sounds/Terran/Terran Generic Explosion 01.wav")) == -1)
		return(FALSE);

	if ((stinger_thrust_snd_id = Load_WAV("New Sounds/Terran/Stinger Thrust 01.wav")) == -1)
		return(FALSE);

	if ((warhawk_thrust_snd_id = Load_WAV("New Sounds/Terran/Warhawk Thrust 01.wav")) == -1)
		return(FALSE);

	if ((valiant_thrust_snd_id = Load_WAV("New Sounds/Terran/Valiant Thrust 01.wav")) == -1)
		return(FALSE);

	if ((valiant_fire_snd_id = Load_WAV("New Sounds/Terran/Valiant Fire 01.wav")) == -1)
		return(FALSE);

	if ((grell_gen_thrust_snd_id = Load_WAV("New Sounds/Grell/Generic Thrust 01.wav")) == -1)
		return(FALSE);

	if ((grell_gen_fire_snd_id = Load_WAV("New Sounds/Grell/Generic Fire 01.wav")) == -1)
		return(FALSE);

	if ((grell_gen_explode_snd_id = Load_WAV("New Sounds/Grell/Generic Explosion 01.wav")) == -1)
		return(FALSE);

	if ((buzzbomb_thrust_snd_id = Load_WAV("New Sounds/Grell/Buzz Bomb Thrust 01.wav")) == -1)
		return(FALSE);

	if ((hhornet_spawn_snd_id = Load_WAV("New Sounds/Grell/Hellhornet Spawn 01.wav")) == -1)
		return(FALSE);

	if ((dnaught_fire_snd_id = Load_WAV("New Sounds/Grell/Dreadnaught Fire 01.wav")) == -1)
		return(FALSE);

	if ((dnaught_light_dam_snd_id = Load_WAV("New Sounds/Grell/Dreadnaught Damage Light 01.wav")) == -1)
		return(FALSE);

	if ((dnaught_heavy_dam_snd_id = Load_WAV("New Sounds/Grell/Dreadnaught Damage Heavy 01.wav")) == -1)
		return(FALSE);

	if ((dnaught_explode_snd_id = Load_WAV("New Sounds/Grell/Dreadnaught Explosion 01.wav")) == -1)
		return(FALSE);


	if ((non_com_explode_snd_id = Load_WAV("New Sounds/Non-Combat/Non-Combat Explosion 01.wav")) == -1)
		return(FALSE);

	if ((powerup_triple_snd_id = Load_WAV("New Sounds/Non-Combat/Power Up Triple Fire 01.wav")) == -1)
		return(FALSE);

	if ((powerup_rapid_snd_id = Load_WAV("New Sounds/Non-Combat/Power Up Fast Fire 01.wav")) == -1)
		return(FALSE);

	if ((powerup_repair_snd_id = Load_WAV("New Sounds/Non-Combat/Power Up Repair 01.wav")) == -1)
		return(FALSE);

	if ((level_start_snd_id = Load_WAV("New Sounds/Non-Combat/Level Start Sound.wav")) == -1)
		return(FALSE);


	if ((music_trk1_id = Load_WAV("Music/InfernoIntro.wav")) == -1)
		return(FALSE);

	if ((music_trk2_id = Load_WAV("Music/Level 01 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk3_id = Load_WAV("Music/Level 02 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk4_id = Load_WAV("Music/Level 03 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk5_id = Load_WAV("Music/Level 04 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk6_id = Load_WAV("Music/Level 05 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk7_id = Load_WAV("Music/Level 06 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk8_id = Load_WAV("Music/Level 07 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk9_id = Load_WAV("Music/Level 08 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk10_id = Load_WAV("Music/Level 09 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk11_id = Load_WAV("Music/Level 10 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk12_id = Load_WAV("Music/Level 11 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk13_id = Load_WAV("Music/Level 12 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk14_id = Load_WAV("Music/Level 13 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk15_id = Load_WAV("Music/Level 14 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk16_id = Load_WAV("Music/Level 15 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk17_id = Load_WAV("Music/Level 16 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk18_id = Load_WAV("Music/Level 17 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk19_id = Load_WAV("Music/Level 18 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk20_id = Load_WAV("Music/Level 19 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk21_id = Load_WAV("Music/Level 20 Placeholder.wav")) == -1)
		return(FALSE);

	if ((music_trk22_id = Load_WAV("Music/Shost10_2red.wav")) == -1)
		return(FALSE);

	return(TRUE);
}

int SoundManager::GetTrackPlaying()
{
	return(music_trk_playing);
}

void SoundManager::PlayMusic(int track)
{
	int gen_tone_down_percent = 70;
	int intro_tone_down_percent = 70;

	switch(track)
	{
	case 1:
		Play_Sound(music_trk1_id);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,intro_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,intro_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,intro_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,intro_tone_down_percent);

		music_trk_playing = 1;
		break;
	case 2:
		Play_Sound(music_trk2_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 2;
		break;
	case 3:
		Play_Sound(music_trk3_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 3;
		break;
	case 4:
		Play_Sound(music_trk4_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 4;
		break;
	case 5:
		Play_Sound(music_trk5_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 5;
		break;
	case 6:
		Play_Sound(music_trk6_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 6;
		break;
	case 7:
		Play_Sound(music_trk7_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 7;
		break;
	case 8:
		Play_Sound(music_trk8_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 8;
		break;
	case 9:
		Play_Sound(music_trk9_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 9;
		break;
	case 10:
		Play_Sound(music_trk10_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 10;
		break;
	case 11:
		Play_Sound(music_trk11_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 11;
		break;
	case 12:
		Play_Sound(music_trk12_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 12;
		break;
	case 13:
		Play_Sound(music_trk13_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 13;
		break;
	case 14:
		Play_Sound(music_trk14_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 14;
		break;
	case 15:
		Play_Sound(music_trk15_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 15;
		break;
	case 16:
		Play_Sound(music_trk16_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 16;
		break;
	case 17:
		Play_Sound(music_trk17_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 17;
		break;
	case 18:
		Play_Sound(music_trk18_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 18;
		break;
	case 19:
		Play_Sound(music_trk19_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 19;
		break;
	case 20:
		Play_Sound(music_trk20_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 20;
		break;
	case 21:
		Play_Sound(music_trk21_id,DSBPLAY_LOOPING);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 21;
		break;
	case 22:
		Play_Sound(music_trk22_id);
		
		// Tone down the other game sounds.
		Set_Sound_Volume(terran_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(terran_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(stinger_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(warhawk_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(valiant_fire_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(grell_gen_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(grell_gen_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(buzzbomb_thrust_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(hhornet_spawn_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_fire_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_light_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_heavy_dam_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(dnaught_explode_snd_id,gen_tone_down_percent);

		Set_Sound_Volume(non_com_explode_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_triple_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_rapid_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(powerup_repair_snd_id,gen_tone_down_percent);
		Set_Sound_Volume(level_start_snd_id,gen_tone_down_percent);
		music_trk_playing = 22;
		break;
	} 
}

void SoundManager::StopMusic()
{
	switch(music_trk_playing)
	{
	case 1:
		Stop_Sound(music_trk1_id);
		break;
	case 2:
		Stop_Sound(music_trk2_id);
		break;
	case 3:
		Stop_Sound(music_trk3_id);
		break;
	case 4:
		Stop_Sound(music_trk4_id);
		break;
	case 5:
		Stop_Sound(music_trk5_id);
		break;
	case 6:
		Stop_Sound(music_trk6_id);
		break;
	case 7:
		Stop_Sound(music_trk7_id);
		break;
	case 8:
		Stop_Sound(music_trk8_id);
		break;		
	case 9:
		Stop_Sound(music_trk9_id);
		break;
	case 10:
		Stop_Sound(music_trk10_id);
		break;
	case 11:
		Stop_Sound(music_trk11_id);
		break;
	case 12:
		Stop_Sound(music_trk12_id);
		break;
	case 13:
		Stop_Sound(music_trk13_id);
		break;
	case 14:
		Stop_Sound(music_trk14_id);
		break;
	case 15:
		Stop_Sound(music_trk15_id);
		break;
	case 16:
		Stop_Sound(music_trk16_id);
		break;	
	case 17:
		Stop_Sound(music_trk17_id);
		break;
	case 18:
		Stop_Sound(music_trk18_id);
		break;		
	case 19:
		Stop_Sound(music_trk19_id);
		break;
	case 20:
		Stop_Sound(music_trk20_id);
		break;
	case 21:
		Stop_Sound(music_trk21_id);
		break;
	case 22:
		Stop_Sound(music_trk22_id);
		break;
	}
	music_trk_playing = 0;

	ResetSoundVolumes();
}

void SoundManager::PlayTerranGenFire()
{
	Play_Sound(terran_gen_fire_snd_id);
}

void SoundManager::PlayTerranGenExplode()
{
	Play_Sound(terran_gen_explode_snd_id);
}

void SoundManager::PlayStingerThrust()
{
	if (stinger_thrust_timer <= 0)
		Play_Sound(stinger_thrust_snd_id,DSBPLAY_LOOPING);
	stinger_thrust_timer = THRUST_SND_TIME;
}

void SoundManager::StopStingerThrust()
{
	Stop_Sound(stinger_thrust_snd_id);
}

void SoundManager::PlayWarhawkThrust()
{
	if (warhawk_thrust_timer <= 0)
		Play_Sound(warhawk_thrust_snd_id,DSBPLAY_LOOPING);
	warhawk_thrust_timer = THRUST_SND_TIME;
}

void SoundManager::StopWarhawkThrust()
{
	Stop_Sound(warhawk_thrust_snd_id);
}

void SoundManager::PlayValiantThrust()
{
	if (valiant_thrust_timer <= 0)
		Play_Sound(valiant_thrust_snd_id,DSBPLAY_LOOPING);
	valiant_thrust_timer = THRUST_SND_TIME;
}

void SoundManager::StopValiantThrust()
{
	Stop_Sound(valiant_thrust_snd_id);
}

void SoundManager::PlayValiantFire()
{
	Play_Sound(valiant_fire_snd_id);
}

void SoundManager::PlayGrellGenThrust()
{	
	if (grell_gen_thrust_timer <= 0)
		Play_Sound(grell_gen_thrust_snd_id,DSBPLAY_LOOPING);
	grell_gen_thrust_timer = THRUST_SND_TIME;
}

void SoundManager::StopGrellGenThrust()
{
	Stop_Sound(grell_gen_thrust_snd_id);
}

void SoundManager::PlayGrellGenFire()
{
	Play_Sound(grell_gen_fire_snd_id);
}

void SoundManager::PlayGrellGenExplode()
{
	Play_Sound(grell_gen_explode_snd_id);
}

void SoundManager::PlayBuzzbombThrust()
{
	if (buzzbomb_thrust_timer <= 0)
		Play_Sound(buzzbomb_thrust_snd_id,DSBPLAY_LOOPING);
	buzzbomb_thrust_timer = THRUST_SND_TIME;
}

void SoundManager::StopBuzzbombThrust()
{
	Stop_Sound(buzzbomb_thrust_snd_id);
}

void SoundManager::PlayHellhornetSpawn()
{
	Play_Sound(hhornet_spawn_snd_id);
}

void SoundManager::PlayDreadnaughtFire()
{
	Play_Sound(dnaught_fire_snd_id);
}

void SoundManager::PlayDreadnaughtLtDamage()
{
	Play_Sound(dnaught_light_dam_snd_id);
}

void SoundManager::PlayDreadnaughtHvDamage()
{
	Play_Sound(dnaught_heavy_dam_snd_id);
}

void SoundManager::PlayDreadnaughtExplode()
{
	Play_Sound(dnaught_explode_snd_id);
}

void SoundManager::PlayNonCombatExplode()
{
	Play_Sound(non_com_explode_snd_id);
}

void SoundManager::PlayPowerupTripleFire()
{
	Play_Sound(powerup_triple_snd_id);
}

void SoundManager::PlayPowerupRapidFire()
{
	Play_Sound(powerup_rapid_snd_id);
}

void SoundManager::PlayPowerupRepair()
{
	Play_Sound(powerup_repair_snd_id);
}

void SoundManager::PlayLevelStart()
{
	Play_Sound(level_start_snd_id);
}

void SoundManager::StopAllSounds()
{
	Stop_All_Sounds();
}

void SoundManager::Update()
{
	BOOL reset_volumes = FALSE;

	// Process the thrusting sound timers.
	if (stinger_thrust_timer > 0)
		theGame->CountdownTickTimer(stinger_thrust_timer);
	if (stinger_thrust_timer <= 0)
		StopStingerThrust();

	if (warhawk_thrust_timer > 0)
		theGame->CountdownTickTimer(warhawk_thrust_timer);
	if (warhawk_thrust_timer <= 0)
		StopWarhawkThrust();

	if (valiant_thrust_timer > 0)
		theGame->CountdownTickTimer(valiant_thrust_timer);
	if (valiant_thrust_timer <= 0)
		StopValiantThrust();

	if (grell_gen_thrust_timer > 0)
		theGame->CountdownTickTimer(grell_gen_thrust_timer);
	if (grell_gen_thrust_timer <= 0)
		StopGrellGenThrust();

	if (buzzbomb_thrust_timer > 0)
		theGame->CountdownTickTimer(buzzbomb_thrust_timer);
	if (buzzbomb_thrust_timer <= 0)
		StopBuzzbombThrust();

	// Reset the volume and the track playing index
	// if a music track ends.
	if (music_trk_playing > 0)
	{
		switch(music_trk_playing)
		{
		case 1:
			if (!(Status_Sound(music_trk1_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 2:
			if (!(Status_Sound(music_trk2_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 3:
			if (!(Status_Sound(music_trk3_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 4:
			if (!(Status_Sound(music_trk4_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 5:
			if (!(Status_Sound(music_trk5_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 6:
			if (!(Status_Sound(music_trk6_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 7:
			if (!(Status_Sound(music_trk7_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 8:
			if (!(Status_Sound(music_trk8_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 9:
			if (!(Status_Sound(music_trk9_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 10:
			if (!(Status_Sound(music_trk10_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 11:
			if (!(Status_Sound(music_trk11_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 12:
			if (!(Status_Sound(music_trk12_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 13:
			if (!(Status_Sound(music_trk13_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 14:
			if (!(Status_Sound(music_trk14_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 15:
			if (!(Status_Sound(music_trk15_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 16:
			if (!(Status_Sound(music_trk16_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 17:
			if (!(Status_Sound(music_trk17_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 18:
			if (!(Status_Sound(music_trk18_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 19:
			if (!(Status_Sound(music_trk19_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 20:
			if (!(Status_Sound(music_trk20_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 21:
			if (!(Status_Sound(music_trk21_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		case 22:
			if (!(Status_Sound(music_trk22_id) & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)))
				reset_volumes = TRUE;
			break;
		}
	}
	if (reset_volumes)
	{
		music_trk_playing = 0;
		ResetSoundVolumes();
	} 
}


// PRIVATE FUNCTIONS

void SoundManager::ResetSoundVolumes()
{
	Set_Sound_Volume(terran_gen_fire_snd_id,100);
	Set_Sound_Volume(terran_gen_explode_snd_id,100);
	Set_Sound_Volume(stinger_thrust_snd_id,100);
	Set_Sound_Volume(warhawk_thrust_snd_id,100);
	Set_Sound_Volume(valiant_thrust_snd_id,100);
	Set_Sound_Volume(valiant_fire_snd_id,100);

	Set_Sound_Volume(grell_gen_thrust_snd_id,100);
	Set_Sound_Volume(grell_gen_fire_snd_id,100);
	Set_Sound_Volume(grell_gen_explode_snd_id,100);
	Set_Sound_Volume(buzzbomb_thrust_snd_id,100);
	Set_Sound_Volume(hhornet_spawn_snd_id,100);
	Set_Sound_Volume(dnaught_fire_snd_id,100);
	Set_Sound_Volume(dnaught_light_dam_snd_id,100);
	Set_Sound_Volume(dnaught_heavy_dam_snd_id,100);
	Set_Sound_Volume(dnaught_explode_snd_id,100);

	Set_Sound_Volume(non_com_explode_snd_id,100);
	Set_Sound_Volume(powerup_triple_snd_id,100);
	Set_Sound_Volume(powerup_rapid_snd_id,100);
	Set_Sound_Volume(powerup_repair_snd_id,100);
	Set_Sound_Volume(level_start_snd_id,100);
}
