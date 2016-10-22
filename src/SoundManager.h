// SoundManager.h
//
// The header file for the SoundManager object.

#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

// INCLUDES

#include "GPDUMB_includes.h"


const int THRUST_SND_TIME = 250;  // in milliseconds


class SoundManager
{
public:
	SoundManager();

	BOOL LoadSounds();
	int GetTrackPlaying();
	void PlayMusic(int track);
	void StopMusic();
	void PlayTerranGenFire();
	void PlayTerranGenExplode();
	void PlayStingerThrust();
	void StopStingerThrust();
	void PlayWarhawkThrust();
	void StopWarhawkThrust();
	void PlayValiantThrust();
	void StopValiantThrust();
	void PlayValiantFire();
	void PlayGrellGenThrust();
	void StopGrellGenThrust();
	void PlayGrellGenFire();
	void PlayGrellGenExplode();
	void PlayBuzzbombThrust();
	void StopBuzzbombThrust();
	void PlayHellhornetSpawn();
	void PlayDreadnaughtFire();
	void PlayDreadnaughtLtDamage();
	void PlayDreadnaughtHvDamage();
	void PlayDreadnaughtExplode();
	void PlayNonCombatExplode();
	void PlayPowerupTripleFire();
	void PlayPowerupRapidFire();
	void PlayPowerupRepair();
	void PlayLevelStart();
	void StopAllSounds();

	// Update any timers.
	void Update();

private:
	void ResetSoundVolumes();

	// Sound IDs
	int terran_gen_fire_snd_id;
	int terran_gen_explode_snd_id;
	int stinger_thrust_snd_id;
	int warhawk_thrust_snd_id;
	int valiant_thrust_snd_id;
	int valiant_fire_snd_id;

	int grell_gen_thrust_snd_id;
	int grell_gen_fire_snd_id;
	int grell_gen_explode_snd_id;
	int buzzbomb_thrust_snd_id;
	int hhornet_spawn_snd_id;
	int dnaught_fire_snd_id;
	int dnaught_light_dam_snd_id;
	int dnaught_heavy_dam_snd_id;
	int dnaught_explode_snd_id;

	int non_com_explode_snd_id;
	int powerup_triple_snd_id;
	int powerup_rapid_snd_id;
	int powerup_repair_snd_id;
	int level_start_snd_id;

	int music_trk1_id;
	int music_trk2_id;
	int music_trk3_id;
	int music_trk4_id;
	int music_trk5_id;
	int music_trk6_id;
	int music_trk7_id;
	int music_trk8_id;
	int music_trk9_id;
	int music_trk10_id;
	int music_trk11_id;
	int music_trk12_id;
	int music_trk13_id;
	int music_trk14_id;
	int music_trk15_id;
	int music_trk16_id;
	int music_trk17_id;
	int music_trk18_id;
	int music_trk19_id;
	int music_trk20_id;
	int music_trk21_id;
	int music_trk22_id;

	int music_trk_playing;

	int stinger_thrust_timer;  // in milliseconds
	int warhawk_thrust_timer;  // in milliseconds
	int valiant_thrust_timer;  // in milliseconds
	int grell_gen_thrust_timer;   // in milliseconds
	int buzzbomb_thrust_timer;   // in milliseconds
};

#endif
