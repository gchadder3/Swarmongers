// GPDUMB2.H - Header file for GPDUMB2.CPP game engine library

// watch for multiple inclusions
#ifndef GPDUMB2
#define GPDUMB2

// DEFINES ////////////////////////////////////////////////

#define MAX_SOUNDS     64 // max number of sounds in system at once 

#define SOUND_NULL     0
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3

// voc file defines
#define NVB_SIZE       6  // size of new voice block in bytes

// screen transition commands
#define SCREEN_DARKNESS  0         // fade to black
#define SCREEN_WHITENESS 1         // fade to white
#define SCREEN_SWIPE_X   2         // do a horizontal swipe
#define SCREEN_SWIPE_Y   3         // do a vertical swipe
#define SCREEN_DISOLVE   4         // a pixel disolve
#define SCREEN_SCRUNCH   5         // a square compression
#define SCREEN_BLUENESS  6         // fade to blue
#define SCREEN_REDNESS   7         // fade to red
#define SCREEN_GREENNESS 8         // fade to green

// BOB defines
#define BOB_ATTR_CLONE   256 // the bob is a clone

// MACROS /////////////////////////////////////////////////

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100 - volume)))

// TYPES //////////////////////////////////////////////////

// this holds a single sound
typedef struct pcm_sound_typ
	{
	LPDIRECTSOUNDBUFFER dsbuffer;   // the ds buffer containing the sound
	int state;                      // state of the sound
	int rate;                       // playback rate
	int size;                       // size of sound
	int id;                         // id number of the sound
	} pcm_sound, *pcm_sound_ptr;

// PROTOTYPES /////////////////////////////////////////////

// sound
int Load_VOC(char *filename);
int Load_WAV(char *filename, int control_flags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY);
int Replicate_Sound(int source_id);
int Play_Sound(int id, int flags=0,int volume=0, int rate=0, int pan=0);
int Stop_Sound(int id);
int Stop_All_Sounds(void);
int DSound_Init(void);
int DSound_Shutdown(void);
int Delete_Sound(int id);
int Delete_All_Sounds(void);
int Status_Sound(int id);
int Set_Sound_Volume(int id,int vol);
int Set_Sound_Freq(int id,int freq);
int Set_Sound_Pan(int id,int pan);

// graphics
void HLine(int x1,int x2,int y,int color, UCHAR *vbuffer, int lpitch);
void VLine(int y1,int y2,int x,int color, UCHAR *vbuffer, int lpitch);
void Screen_Transitions(int effect, UCHAR *vbuffer, int lpitch);

// input
int DInput_Init(void);
void DInput_Shutdown(void);
int DI_Init_Joystick(int min_x=-256, int max_x=256, int min_y=-256, int max_y=256);
int DI_Init_Mouse(void);
int DI_Init_Keyboard(void);
int DI_Read_Joystick(void);
int DI_Read_Mouse(void);
int DI_Read_Keyboard(void);
void DI_Release_Joystick(void);
void DI_Release_Mouse(void);
void DI_Release_Keyboard(void);

// bobs
int Destroy_BOB2(BOB_PTR bob);
int Clone_BOB(BOB_PTR source, BOB_PTR dest);

// general util
int Collision_Test(int x1, int y1, int w1, int h1, 
                   int x2, int y2, int w2, int h2); 

int Color_Scan(int x1, int y1, int x2, int y2, 
      UCHAR scan_start, UCHAR scan_end, 
      UCHAR *scan_buffer, int scan_lpitch);

// GLOBALS ////////////////////////////////////////////////


// EXTERNALS //////////////////////////////////////////////

extern HWND main_window_handle; // save the window handle
extern HINSTANCE main_instance; // save the instance

extern LPDIRECTSOUND		lpds;           // directsound interface pointer
extern DSBUFFERDESC			dsbd;           // directsound description
extern DSCAPS				dscaps;         // directsound caps
extern HRESULT				dsresult;       // general directsound result
extern DSBCAPS				dsbcaps;        // directsound buffer caps

extern LPDIRECTSOUNDBUFFER	lpdsbprimary;   // the primary mixing buffer
extern pcm_sound			sound_fx[MAX_SOUNDS];    // the array of secondary sound buffers

extern WAVEFORMATEX			pcmwf;          // generic waveformat structure

// directinput globals
extern LPDIRECTINPUT8       lpdi;       // dinput object
extern LPDIRECTINPUTDEVICE8 lpdikey;    // dinput keyboard
extern LPDIRECTINPUTDEVICE8 lpdimouse;  // dinput mouse
extern LPDIRECTINPUTDEVICE8 lpdijoy;    // dinput joystick 
extern LPDIRECTINPUTDEVICE2 lpdijoy2;   // dinput joystick
extern GUID                 joystickGUID; // guid for main joystick
extern char                 joyname[80];  // name of joystick

// these contain the target records for all di input packets
extern UCHAR keyboard_state[256]; // contains keyboard state table
extern DIMOUSESTATE mouse_state;  // contains state of mouse
extern DIJOYSTATE joy_state;      // contains state of joystick
extern int joystick_found;        // tracks if stick is plugged in
#endif


