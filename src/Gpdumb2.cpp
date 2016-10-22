// GPDUMB2.CPP - Game Engine Part II
 
// INCLUDES ///////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN  
#define INITGUID

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <objbase.h>
#include <iostream>    // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <tchar.h>

#include <ddraw.h>  // directX includes
#include <dsound.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
// #define DIRECTINPUT_VERSION 0x0700
// #include <dinput.h>
#include "gpdumb1.h"
#include "gpdumb2.h"

// DEFINES ////////////////////////////////////////////////

// TYPES //////////////////////////////////////////////////

// PROTOTYPES /////////////////////////////////////////////

// EXTERNALS /////////////////////////////////////////////

extern HWND main_window_handle;     // access to main window handle in main module

// GLOBALS ////////////////////////////////////////////////

// directsound stuff
LPDIRECTSOUND		lpds;           // directsound interface pointer
DSBUFFERDESC		dsbd;           // directsound description
DSCAPS				dscaps;         // directsound caps
HRESULT				dsresult;       // general directsound result
DSBCAPS				dsbcaps;        // directsound buffer caps
LPDIRECTSOUNDBUFFER	lpdsbprimary;   // the primary mixing buffer
pcm_sound			sound_fx[MAX_SOUNDS];    // the array of secondary sound buffers

WAVEFORMATEX		pcmwf;          // generic waveformat structure

// directinput globals
LPDIRECTINPUT8       lpdi      = NULL;    // dinput object
LPDIRECTINPUTDEVICE8 lpdikey   = NULL;    // dinput keyboard
LPDIRECTINPUTDEVICE8 lpdimouse = NULL;    // dinput mouse
LPDIRECTINPUTDEVICE8 lpdijoy   = NULL;    // dinput joystick 
LPDIRECTINPUTDEVICE2 lpdijoy2  = NULL;    // dinput joystick
GUID                 joystickGUID;        // guid for main joystick
char                 joyname[80];         // name of joystick

// these contain the target records for all di input packets
UCHAR keyboard_state[256]; // contains keyboard state table
DIMOUSESTATE mouse_state;  // contains state of mouse
DIJOYSTATE joy_state;      // contains state of joystick
int joystick_found = 0;    // tracks if joystick was found and inited

// FUNCTIONS //////////////////////////////////////////////

extern LPCWSTR MultiCharToUniChar(char* mbString);

int Load_VOC(char *filename)
{
// this function loads a .voc file, sets up the directsound buffer and loads the data
// into memory, the function returns the id number of the sound

int	sound_id = -1,       // id of sound to be loaded
	index,               // looping variable
	data_offset,         // offset to data part of file
	playback_rate,       // playback rate as encoded in file
	data_length;         // length of data

ULONG  bytesread = 0,    // actual number of bytes read during file read
       filelength;       // length of file

int    file_handle;      // general file handle

UCHAR *snd_buffer,       // temporary sound buffer to hold voc data
      *audio_ptr_1=NULL, // data ptr to first write buffer 
	  *audio_ptr_2=NULL; // data ptr to second write buffer

DWORD audio_length_1=0,  // length of first write buffer
	  audio_length_2=0;  // length of second write buffer
			
// step one: are there any open id's ?
for (index=0; index < MAX_SOUNDS; index++)
	{	
    // make sure this sound is unused
	if (sound_fx[index].state==SOUND_NULL)
	   {
	   sound_id = index;
	   break;
	   } // end if

	} // end for index

// did we get a free id?
if (sound_id==-1)
	return(-1);

// step two: load the voc file off disk
if ((file_handle = _open(filename,_O_BINARY | _O_RDONLY))==-1)
	return(0);

// get size of file so we can allocate temporary read buffer
filelength = _filelength(file_handle);

// allocate a large enough temporary buffer
snd_buffer = (UCHAR *)malloc(filelength);

// now read in the data
bytesread = _read(file_handle, snd_buffer, filelength);

// access all values and decode VOC encoding for data fields
data_offset	  = snd_buffer[20];
playback_rate = (-1000000/(snd_buffer[data_offset+4]-256));
data_length   = ((*(int *)(snd_buffer+data_offset)) >> 8);

// set rate and size in data structure
sound_fx[sound_id].rate  = playback_rate;
sound_fx[sound_id].size  = data_length;
sound_fx[sound_id].state = SOUND_LOADED;

// close the file
_close(file_handle);

// step three: create the sound buffer and copy voc data into buffer

// set up the format data structure
memset(&pcmwf, 0, sizeof(WAVEFORMATEX));

pcmwf.wFormatTag	  = WAVE_FORMAT_PCM;  // pulse code modulation
pcmwf.nChannels		  = 1;                // mono 
pcmwf.nSamplesPerSec  = 11025;            // always this rate
pcmwf.nBlockAlign	  = 1;                
pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
pcmwf.wBitsPerSample  = 8;
pcmwf.cbSize		  = 0;

// prepare to create sounds buffer
dsbd.dwSize			= sizeof(DSBUFFERDESC);
dsbd.dwFlags		= DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
dsbd.dwBufferBytes	= data_length-NVB_SIZE;
dsbd.lpwfxFormat	= &pcmwf;

// create the sound buffer
if (lpds->CreateSoundBuffer(&dsbd,&sound_fx[sound_id].dsbuffer,NULL)!=DS_OK)
   {
   // release memory
   free(snd_buffer);

   // return error
   return(-1);
   } // end if

// copy data into sound buffer
if (sound_fx[sound_id].dsbuffer->Lock(0,					 
								      data_length-NVB_SIZE,			
								      (void **) &audio_ptr_1, 
								      &audio_length_1,
								      (void **)&audio_ptr_2, 
								      &audio_length_2,
								      DSBLOCK_FROMWRITECURSOR)!=DS_OK)
								 return(0);

// copy first section of circular buffer
memcpy(audio_ptr_1, snd_buffer+data_offset+NVB_SIZE, audio_length_1);

// copy last section of circular buffer
memcpy(audio_ptr_2, (snd_buffer+data_offset+NVB_SIZE+audio_length_1),audio_length_2);

// unlock the buffer
if (sound_fx[sound_id].dsbuffer->Unlock(audio_ptr_1, 
									    audio_length_1, 
									    audio_ptr_2, 
									    audio_length_2)!=DS_OK)
 							     return(0);

// release the temp buffer
free(snd_buffer);

// return id
return(sound_id);

} // end Load_Voc

///////////////////////////////////////////////////////////

int Load_WAV(char *filename, int control_flags)
{
// this function loads a .wav file, sets up the directsound 
// buffer and loads the data into memory, the function returns 
// the id number of the sound


HMMIO 			hwav;    // handle to wave file
MMCKINFO		parent,  // parent chunk
                child;   // child chunk
WAVEFORMATEX    wfmtx;   // wave format structure

int	sound_id = -1,       // id of sound to be loaded
	index;               // looping variable

UCHAR *snd_buffer,       // temporary sound buffer to hold voc data
      *audio_ptr_1=NULL, // data ptr to first write buffer 
	  *audio_ptr_2=NULL; // data ptr to second write buffer

DWORD audio_length_1=0,  // length of first write buffer
	  audio_length_2=0;  // length of second write buffer
			
// step one: are there any open id's ?
for (index=0; index < MAX_SOUNDS; index++)
	{	
    // make sure this sound is unused
	if (sound_fx[index].state==SOUND_NULL)
	   {
	   sound_id = index;
	   break;
	   } // end if

	} // end for index

// did we get a free id?
if (sound_id==-1)
	return(-1);

// set up chunk info structure
parent.ckid 	    = (FOURCC)0;
parent.cksize 	    = 0;
parent.fccType	    = (FOURCC)0;
parent.dwDataOffset = 0;
parent.dwFlags		= 0;

// copy data
child = parent;

// open the WAV file
if ((hwav = mmioOpen((LPWSTR) MultiCharToUniChar(filename), NULL, 
	MMIO_READ | MMIO_ALLOCBUF))==NULL)
    return(-1);

// descend into the RIFF 
parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

if (mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF))
    {
    // close the file
    mmioClose(hwav, 0);

    // return error, no wave section
    return(-1); 	
    } // end if

// descend to the WAVEfmt 
child.ckid = mmioFOURCC('f', 'm', 't', ' ');

if (mmioDescend(hwav, &child, &parent, 0))
    {
    // close the file
    mmioClose(hwav, 0);

    // return error, no format section
    return(-1); 	
    } // end if

// now read the wave format information from file
if (mmioRead(hwav, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx))
    {
    // close file
    mmioClose(hwav, 0);

    // return error, no wave format data
    return(-1);
    } // end if

// make sure that the data format is PCM
if (wfmtx.wFormatTag != WAVE_FORMAT_PCM)
    {
    // close the file
    mmioClose(hwav, 0);

    // return error, not the right data format
    return(-1); 
    } // end if

// now ascend up one level, so we can access data chunk
if (mmioAscend(hwav, &child, 0))
   {
   // close file
   mmioClose(hwav, 0);

   // return error, couldn't ascend
   return(-1); 	
   } // end if

// descend to the data chunk 
child.ckid = mmioFOURCC('d', 'a', 't', 'a');

if (mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK))
    {
    // close file
    mmioClose(hwav, 0);

    // return error, no data
    return(-1); 	
    } // end if

// finally!!!! now all we have to do is read the data in and
// set up the directsound buffer

// allocate the memory to load sound data
snd_buffer = (UCHAR *)malloc(child.cksize);

// read the wave data 
mmioRead(hwav, (char *)snd_buffer, child.cksize);

// close the file
mmioClose(hwav, 0);

// set rate and size in data structure
sound_fx[sound_id].rate  = wfmtx.nSamplesPerSec;
sound_fx[sound_id].size  = child.cksize;
sound_fx[sound_id].state = SOUND_LOADED;

// set up the format data structure
memset(&pcmwf, 0, sizeof(WAVEFORMATEX));

pcmwf.wFormatTag	  = WAVE_FORMAT_PCM;  // pulse code modulation
pcmwf.nChannels		  = 1;                // mono 
pcmwf.nSamplesPerSec  = 11025;            // always this rate
pcmwf.nBlockAlign	  = 1;                
pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
pcmwf.wBitsPerSample  = 8;
pcmwf.cbSize		  = 0;

// prepare to create sounds buffer
dsbd.dwSize			= sizeof(DSBUFFERDESC);
dsbd.dwFlags		= control_flags | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
dsbd.dwBufferBytes	= child.cksize;
dsbd.lpwfxFormat	= &pcmwf;

// create the sound buffer
if (lpds->CreateSoundBuffer(&dsbd,&sound_fx[sound_id].dsbuffer,NULL)!=DS_OK)
   {
   // release memory
   free(snd_buffer);

   // return error
   return(-1);
   } // end if

// copy data into sound buffer
if (sound_fx[sound_id].dsbuffer->Lock(0,					 
								      child.cksize,			
								      (void **) &audio_ptr_1, 
								      &audio_length_1,
								      (void **)&audio_ptr_2, 
								      &audio_length_2,
								      DSBLOCK_FROMWRITECURSOR)!=DS_OK)
								 return(0);

// copy first section of circular buffer
memcpy(audio_ptr_1, snd_buffer, audio_length_1);

// copy last section of circular buffer
memcpy(audio_ptr_2, (snd_buffer+audio_length_1),audio_length_2);

// unlock the buffer
if (sound_fx[sound_id].dsbuffer->Unlock(audio_ptr_1, 
									    audio_length_1, 
									    audio_ptr_2, 
									    audio_length_2)!=DS_OK)
 							     return(0);

// release the temp buffer
free(snd_buffer);

// return id
return(sound_id);

} // end Load_WAV

///////////////////////////////////////////////////////////

int Replicate_Sound(int source_id)
{
// this function replicates the sent sound and sends back the
// id of the replicated sound, you would use this function
// to make multiple copies of a gunshot or something that
// you want to play multiple times simulataneously, but you
// only want to load once

if (source_id!=-1)
    {
    // duplicate the sound buffer
    // first hunt for an open id

    for (int id=0; id < MAX_SOUNDS; id++)
        {
        // is this sound open?
        if (sound_fx[id].state==SOUND_NULL)
            {
            // first make an identical copy
            sound_fx[id] = sound_fx[source_id];

            // now actually replicate the directsound buffer
            if (lpds->DuplicateSoundBuffer(sound_fx[source_id].dsbuffer,
                                           &sound_fx[id].dsbuffer)!=DS_OK)
                {
                // reset sound to NULL
                sound_fx[id].dsbuffer = NULL;
                sound_fx[id].state    = SOUND_NULL;

                // return error
                return(-1);
                } // end if

            // now fix up id
            sound_fx[id].id = id;
            
            // return replicated sound
            return(id);

            } // end if found
  
        } // end for id

    } // end if
else
   return(-1);
    
// else failure
return(-1);

} // end 







///////////////////////////////////////////////////////////

int DSound_Init(void)
{
// this function initializes the sound system
static int first_time = 1; // used to track the first time the function
                           // is entered

// test for very first time
if (first_time)
	{		
	// clear everything out
	memset(sound_fx,0,sizeof(pcm_sound)*MAX_SOUNDS);
	
	// reset first time
	first_time = 0;

	// create a directsound object
	if (DirectSoundCreate(NULL, &lpds, NULL)!=DS_OK )
		return(0);

	// set cooperation level
	if (lpds->SetCooperativeLevel((HWND)main_window_handle,DSSCL_NORMAL)!=DS_OK)
		return(0);

	} // end if

// initialize the sound fx array
for (int index=0; index<MAX_SOUNDS; index++)
	{
	// test if this sound has been loaded
	if (sound_fx[index].dsbuffer)
		{
		// stop the sound
		sound_fx[index].dsbuffer->Stop();

		// release the buffer
		sound_fx[index].dsbuffer->Release();
	
		} // end if

	// clear the record out
	memset(&sound_fx[index],0,sizeof(pcm_sound));

	// now set up the fields
	sound_fx[index].state = SOUND_NULL;
	sound_fx[index].id    = index;

	} // end for index

// return sucess
return(1);

} // end DSound_Init

///////////////////////////////////////////////////////////

int DSound_Shutdown(void)
{
// this function releases all the memory allocated and the directsound object
// itself

// first turn all sounds off
Stop_All_Sounds();

// now release all sound buffers
for (int index=0; index<MAX_SOUNDS; index++)
	if (sound_fx[index].dsbuffer)
		sound_fx[index].dsbuffer->Release();

// now release the directsound interface itself
lpds->Release();

// return success
return(1);

} // end DSound_Shutdown

///////////////////////////////////////////////////////////

int Play_Sound(int id, int flags, int volume, int rate, int pan)
{
// this function plays a sound, the only parameter that 
// works is the flags which can be 0 to play once or
// DSBPLAY_LOOPING

if (sound_fx[id].dsbuffer)
	{
	// reset position to start
	if (sound_fx[id].dsbuffer->SetCurrentPosition(0)!=DS_OK)
		return(0);
	
	// play sound
	if (sound_fx[id].dsbuffer->Play(0,0,flags)!=DS_OK)
		return(0);
	} // end if

// return success
return(1);

} // end Play_Sound

///////////////////////////////////////////////////////////

int Set_Sound_Volume(int id,int vol)
{
// this function sets the volume on a sound 0-100

if (sound_fx[id].dsbuffer->SetVolume(DSVOLUME_TO_DB(vol))!=DS_OK)
	return(0);

// return success
return(1);

} // end Set_Sound_Volume

///////////////////////////////////////////////////////////

int Set_Sound_Freq(int id,int freq)
{
// this function sets the playback rate

if (sound_fx[id].dsbuffer->SetFrequency(freq)!=DS_OK)
	return(0);

// return success
return(1);

} // end Set_Sound_Freq

///////////////////////////////////////////////////////////

int Set_Sound_Pan(int id,int pan)
{
// this function sets the pan, -10,000 to 10,0000

if (sound_fx[id].dsbuffer->SetPan(pan)!=DS_OK)
	return(0);

// return success
return(1);

} // end Set_Sound_Pan

////////////////////////////////////////////////////////////

int Stop_Sound(int id)
{
// this function stops a sound from playing
if (sound_fx[id].dsbuffer)
   {
   sound_fx[id].dsbuffer->Stop();
   sound_fx[id].dsbuffer->SetCurrentPosition(0);
   } // end if

// return success
return(1);

} // end Stop_Sound

///////////////////////////////////////////////////////////

int Delete_All_Sounds(void)
{
// this function deletes all the sounds

for (int index=0; index < MAX_SOUNDS; index++)
    Delete_Sound(index);

// return success always
return(1);

} // end Delete_All_Sounds

///////////////////////////////////////////////////////////

int Delete_Sound(int id)
{
// this function deletes a single sound and puts it back onto the available list

// first stop it
if (!Stop_Sound(id))
   return(0);

// now delete it
if (sound_fx[id].dsbuffer)
   {
   // release the com object
   sound_fx[id].dsbuffer->Release();
   sound_fx[id].dsbuffer = NULL;
   
   // return success
   return(1);
   } // end if

// return success
return(1);

} // end Delete_Sound

///////////////////////////////////////////////////////////

int Stop_All_Sounds(void)
{
// this function stops all sounds

for (int index=0; index<MAX_SOUNDS; index++)
	Stop_Sound(index);	

// return success
return(1);

} // end Stop_All_Sounds

///////////////////////////////////////////////////////////

int Status_Sound(int id)
{
// this function returns the status of a sound
if (sound_fx[id].dsbuffer)
	{
	ULONG status; 

	// get the status
	sound_fx[id].dsbuffer->GetStatus(&status);

	// return the status
	return(status);

	} // end if
else // total failure
	return(-1);

} // end Status_Sound

///////////////////////////////////////////////////////////

void HLine(int x1,int x2,int y,int color, UCHAR *vbuffer, int lpitch)
{
// draw a horizontal line using the memset function

int temp; // used for temporary storage during endpoint swap

// perform trivial rejections
if (y > max_clip_y || y < min_clip_y)
   return;

// sort x1 and x2, so that x2 > x1
if (x1>x2)
   {
   temp = x1;
   x1   = x2;
   x2   = temp;
   } // end swap

// perform trivial rejections
if (x1 > max_clip_x || x2 < min_clip_x)
   return;

// now clip
x1 = ((x1 < min_clip_x) ? min_clip_x : x1);
x2 = ((x2 > max_clip_x) ? max_clip_x : x2);

// draw the row of pixels
memset((UCHAR *)(vbuffer+(y*lpitch)+x1),
       (UCHAR)color,x2-x1+1);

} // end HLine

//////////////////////////////////////////////////////////////////////////////

void VLine(int y1,int y2,int x,int color,UCHAR *vbuffer, int lpitch)
{
// draw a vertical line, note that a memset function can no longer be
// used since the pixel addresses are no longer contiguous in memory
// note that the end points of the line must be on the screen

UCHAR *start_offset; // starting memory offset of line

int index, // loop index
    temp;  // used for temporary storage during swap


// perform trivial rejections
if (x > max_clip_x || x < min_clip_x)
   return;

// make sure y2 > y1
if (y1>y2)
   {
   temp = y1;
   y1   = y2;
   y2   = temp;
   } // end swap

// perform trivial rejections
if (y1 > max_clip_y || y2 < min_clip_y)
   return;

// now clip
y1 = ((y1 < min_clip_y) ? min_clip_y : y1);
y2 = ((y2 > max_clip_y) ? max_clip_y : y2);

// compute starting position
start_offset = vbuffer + (y1*lpitch) + x;

// draw line one pixel at a time
for (index=0; index<=y2-y1; index++)
    {
    // set the pixel
    *start_offset = (UCHAR)color;

    // move downward to next line
    start_offset+=lpitch;

    } // end for index

} // end VLine

///////////////////////////////////////////////////////////

void Screen_Transitions(int effect, UCHAR *vbuffer, int lpitch)
{
// this function can be called to perform a myraid of screen transitions
// to the destination buffer, make sure to save and restore the palette
// when performing color transitions

int pal_reg;         // used as loop counter
int index;           // used as loop counter
int red,green,blue;           // used in fad algorithm

PALETTEENTRY color;              // temporary color
PALETTEENTRY work_palette[256];  // used as a working palette
PALETTEENTRY work_color;         // used in color algorithms

// test which screen effect is being selected
switch(effect)
      {
      case SCREEN_DARKNESS:
           {
           // fade to black

           for (index=0; index<80; index++)
               {
               // get the palette 
               Save_Palette(work_palette);

               // process each color
               for (pal_reg=1; pal_reg<256; pal_reg++)
                   {
                   // get the entry data
                   color = work_palette[pal_reg];

                   // test if this color register is already black
                   if (color.peRed > 4) color.peRed-=3;
                   else
                      color.peRed = 0;

                   if (color.peGreen > 4) color.peGreen-=3;
                   else
                      color.peGreen = 0;

                   if (color.peBlue  > 4) color.peBlue-=3;
                   else
                      color.peBlue = 0;

                   // set the color to a diminished intensity
                   work_palette[pal_reg] = color;

                   } // end for pal_reg

               // write the palette back out
               Set_Palette(work_palette);

               // wait a bit
               //DD_Wait_For_Vsync();
               Start_Clock(); Wait_Clock(12);
               
               } // end for index

           } break;

      case SCREEN_WHITENESS:
           {
           // fade to white
           for (index=0; index<64; index++)
               {
               // get the palette 
               Save_Palette(work_palette);

               // loop thru all palette registers
               for (pal_reg=0; pal_reg < 256; pal_reg++)
                   {
                   // get the entry data
                   color = work_palette[pal_reg];

                   // make 32 bit copy of color
                   red   = color.peRed;
                   green = color.peGreen;
                   blue  = color.peBlue; 

                   if ((red+=4) >=255)
                      red=255;

                   if ((green+=4) >=255)
                      green=255;

                   if ((blue+=4) >=255)
                      blue=255;
                          
                   // store colors back
                   color.peRed   = red;
                   color.peGreen = green;
                   color.peBlue  = blue;

                   // set the color to a diminished intensity
                   work_palette[pal_reg] = color;
                   
                   } // end for pal_reg

               // write the palette back out
               Set_Palette(work_palette);

               // wait a bit
               //DD_Wait_For_Vsync();
               Start_Clock(); Wait_Clock(12);

               } // end for index

           } break;

      case SCREEN_REDNESS:
           {
           // fade to red

           for (index=0; index<64; index++)
               {
               // get the palette 
               Save_Palette(work_palette);
               
               // loop thru all palette registers
               for (pal_reg=0; pal_reg < 256; pal_reg++)
                   {
                   // get the entry data
                   color = work_palette[pal_reg];

                   // make 32 bit copy of color
                   red   = color.peRed;
                   green = color.peGreen;
                   blue  = color.peBlue; 

                   if ((red+=6) >=255)
                      red=255; 

                   if ((green-=4) < 0)
                      green=0;

                   if ((blue-=4) < 0)
                      blue=0;
                          
                   // store colors back
                   color.peRed   = red;
                   color.peGreen = green;
                   color.peBlue  = blue;
                  
                   // set the color to a diminished intensity
                   work_palette[pal_reg] = color;

                   } // end for pal_reg

               // write the palette back out
               Set_Palette(work_palette);

               // wait a bit
               //DD_Wait_For_Vsync();
               Start_Clock(); Wait_Clock(12);

               } // end for index

           } break;

      case SCREEN_BLUENESS:
           {
           // fade to blue

           for (index=0; index<64; index++)
               {
               // get the palette 
               Save_Palette(work_palette);
               
               // loop thru all palette registers
               for (pal_reg=0; pal_reg < 256; pal_reg++)
                   {
                   // get the entry data
                   color = work_palette[pal_reg];

                   // make 32 bit copy of color
                   red   = color.peRed;
                   green = color.peGreen;
                   blue  = color.peBlue; 

                   if ((red-=4) < 0)
                      red=0;

                   if ((green-=4) < 0)
                      green=0;

                   if ((blue+=6) >=255)
                      blue=255;
                          
                   // store colors back
                   color.peRed   = red;
                   color.peGreen = green;
                   color.peBlue  = blue;
                  
                   // set the color to a diminished intensity
                   work_palette[pal_reg] = color;

                   } // end for pal_reg

               // write the palette back out
               Set_Palette(work_palette);

               // wait a bit
               //DD_Wait_For_Vsync();
               Start_Clock(); Wait_Clock(12);

               } // end for index

           } break;

      case SCREEN_GREENNESS:
           {
           // fade to green
           for (index=0; index<64; index++)
               {
               // get the palette 
               Save_Palette(work_palette);

               // loop thru all palette registers
               for (pal_reg=0; pal_reg < 256; pal_reg++)
                   {
                   // get the entry data
                   color = work_palette[pal_reg];                  

                   // make 32 bit copy of color
                   red   = color.peRed;
                   green = color.peGreen;
                   blue  = color.peBlue; 

                   if ((red-=4) < 0)
                      red=0;

                   if ((green+=6) >=255)
                      green=255;

                   if ((blue-=4) < 0)
                      blue=0;
                          
                   // store colors back
                   color.peRed   = red;
                   color.peGreen = green;
                   color.peBlue  = blue;

                   // set the color to a diminished intensity
                   work_palette[pal_reg] = color; 

                   } // end for pal_reg

               // write the palette back out
               Set_Palette(work_palette);

               // wait a bit
               //DD_Wait_For_Vsync();
               Start_Clock(); Wait_Clock(12);


               } // end for index

           } break;

      case SCREEN_SWIPE_X:
           {
           // do a screen wipe from right to left, left to right
           for (index=0; index < (screen_width/2); index+=2)
               {
               // use this as a 1/70th of second time delay
               //DD_Wait_For_Vsync();
               Start_Clock(); Wait_Clock(12);

               // draw two vertical lines at opposite ends of the screen
               VLine(0,(screen_height-1),(screen_width-1)-index,0,vbuffer,lpitch);
               VLine(0,(screen_height-1),index,0,vbuffer,lpitch);
               VLine(0,(screen_height-1),(screen_width-1)-(index+1),0,vbuffer,lpitch);
               VLine(0,(screen_height-1),index+1,0,vbuffer,lpitch);

               } // end for index

           } break;

      case SCREEN_SWIPE_Y:
           {
           // do a screen wipe from top to bottom, bottom to top
           for (index=0; index < (screen_height/2); index+=2)
               {
               // use this as a 1/70th of second time delay
               //DD_Wait_For_Vsync();
               Start_Clock(); Wait_Clock(12);

               // draw two horizontal lines at opposite ends of the screen
               HLine(0,(screen_width-1),(screen_height-1)-index,0,vbuffer,lpitch);
               HLine(0,(screen_width-1),index,0,vbuffer,lpitch);
               HLine(0,(screen_width-1),(screen_height-1)-(index+1),0,vbuffer,lpitch);
               HLine(0,(screen_width-1),index+1,0,vbuffer,lpitch);

               } // end for index


            } break;

      case SCREEN_SCRUNCH:
           {
           // do a screen wipe from top to bottom, bottom to top
           for (index=0; index < (screen_width/2); index+=2)
               {
               // use this as a 1/70th of second time delay
               //DD_Wait_For_Vsync();
               Start_Clock(); Wait_Clock(12);

               // draw two horizontal lines at opposite ends of the screen
               HLine(0,(screen_width-1),(screen_height-1)-index%(screen_height/2),0,vbuffer,lpitch);
               HLine(0,(screen_width-1),index%(screen_height/2),0,vbuffer,lpitch);
               HLine(0,(screen_width-1),(screen_height-1)-(index%(screen_height/2)+1),0,vbuffer,lpitch);
               HLine(0,(screen_width-1),index%(screen_height/2)+1,0,vbuffer,lpitch);

               // draw two vertical lines at opposite ends of the screen
               VLine(0,(screen_height-1),(screen_width-1)-index,0,vbuffer,lpitch);
               VLine(0,(screen_height-1),index,0,vbuffer,lpitch);
               VLine(0,(screen_height-1),(screen_width-1)-(index+1),0,vbuffer,lpitch);
               VLine(0,(screen_height-1),index+1,0,vbuffer,lpitch);

               } // end for index

           } break;


      case SCREEN_DISOLVE:
           {
           // disolve the screen by plotting zillions of little black dots

           for (index=0; index<=screen_width*screen_height*4; index++)
               Draw_Pixel(rand()%screen_width,rand()%screen_height,0,vbuffer,lpitch);
       
           } break;

       default:break;

      } // end switch

} // end Screen_Transitions

//////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK DI_Enum_Joysticks(LPCDIDEVICEINSTANCE lpddi,
								LPVOID guid_ptr) 
{
// this function enumerates the joysticks, but
// stops at the first one and returns the
// instance guid of it, so we can create it

*(GUID*)guid_ptr = lpddi->guidInstance; 

// copy name into global
strcpy(joyname, (char *)lpddi->tszProductName);

// stop enumeration after one iteration
return(DIENUM_STOP);

} // end DI_Enum_Joysticks

//////////////////////////////////////////////////////////////////////////////

int DInput_Init(void)
{
// this function initializes directinput

if (DirectInput8Create(main_instance,DIRECTINPUT_VERSION,IID_IDirectInput8A,(void **)&lpdi,NULL)!=DI_OK)
   return(0);

// return success
return(1);

} // end DInput_Init

///////////////////////////////////////////////////////////

void DInput_Shutdown(void)
{
// this function shuts down directinput

if (lpdi)
   lpdi->Release();

} // end DInput_Shutdown

///////////////////////////////////////////////////////////

int DI_Init_Joystick(int min_x, int max_x, int min_y, int max_y)
{
// this function initializes the joystick, it allows you to set
// the minimum and maximum x-y ranges 
/*
// first find the fucking GUID of your particular joystick
lpdi->EnumDevices(DIDEVTYPE_JOYSTICK, 
                  DI_Enum_Joysticks, 
                  &joystickGUID, 
                  DIEDFL_ATTACHEDONLY); 

if (lpdi->CreateDevice(joystickGUID, &lpdijoy, NULL)!=DI_OK)
   return(0);

// now that we have created the joystick device we need to
// obtain a new more advanced interface called 
// IDIRECTINPUTDEVICE2, this needs to be done using the low level
// COM method queryinterface(), unfortunetely createdevice
// still gives a 1.0 version interface, so we to do this manually

lpdijoy->QueryInterface(IID_IDirectInputDevice2, 
                        (void **) &lpdijoy2); 

// release the version 1.0 interface since we have converted
// it to 2.0
lpdijoy->Release();

// set cooperation level
if (lpdijoy2->SetCooperativeLevel(main_window_handle, 
	                 DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
   return(0);

// set data format
if (lpdijoy2->SetDataFormat(&c_dfDIJoystick)!=DI_OK)
   return(0);

// set the range of the joystick
DIPROPRANGE joy_axis_range;

// first x axis
joy_axis_range.lMin = min_x;
joy_axis_range.lMax = max_x;

joy_axis_range.diph.dwSize       = sizeof(DIPROPRANGE); 
joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
joy_axis_range.diph.dwObj        = DIJOFS_X;
joy_axis_range.diph.dwHow        = DIPH_BYOFFSET;

lpdijoy2->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);

// now y-axis
joy_axis_range.lMin = min_y;
joy_axis_range.lMax = max_y;

joy_axis_range.diph.dwSize       = sizeof(DIPROPRANGE); 
joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
joy_axis_range.diph.dwObj        = DIJOFS_Y;
joy_axis_range.diph.dwHow        = DIPH_BYOFFSET;

lpdijoy2->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);

// acquire the joystick
if (lpdijoy2->Acquire()!=DI_OK)
   return(0);

// set found flag
joystick_found = 1;

// return success
return(1); */
return(0);
} // end DI_Init_Joystick

///////////////////////////////////////////////////////////

int DI_Init_Mouse(void)
{
// this function intializes the mouse

// create a mouse device 
if (lpdi->CreateDevice(GUID_SysMouse, &lpdimouse, NULL)!=DI_OK)
   return(0);

// set cooperation level
if (lpdimouse->SetCooperativeLevel(main_window_handle, 
                       DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
   return(0);

// set data format
if (lpdimouse->SetDataFormat(&c_dfDIMouse)!=DI_OK)
   return(0);

// acquire the mouse
if (lpdimouse->Acquire()!=DI_OK)
   return(0);

// return success
return(1);

} // end DI_Init_Mouse

///////////////////////////////////////////////////////////

int DI_Init_Keyboard(void)
{
// this function initializes the keyboard device

// create the keyboard device  
if (lpdi->CreateDevice(GUID_SysKeyboard, &lpdikey, NULL)!=DI_OK)
   return(0);

// set cooperation level
if (lpdikey->SetCooperativeLevel(main_window_handle, 
                 DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
    return(0);

// set data format
if (lpdikey->SetDataFormat(&c_dfDIKeyboard)!=DI_OK)
   return(0);

// acquire the keyboard
if (lpdikey->Acquire()!=DI_OK)
   return(0);

// return success
return(1);

} // end DI_Init_Keyboard

///////////////////////////////////////////////////////////

int DI_Read_Joystick(void)
{
// this function reads the joystick state

// make sure the joystick was initialized
if (!joystick_found)
   return(0);

if (lpdijoy2)
    {
    // this is needed for joysticks only    
    if (lpdijoy2->Poll()!=DI_OK)
        return(0);

    if (lpdijoy2->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joy_state)!=DI_OK)
        return(0);
    }
else
    {
    // joystick isn't plugged in, zero out state
    memset(&joy_state,0,sizeof(joy_state));

    // return error
    return(0);
    } // end else

// return sucess
return(1);

} // end DI_Read_Joystick

///////////////////////////////////////////////////////////

int DI_Read_Mouse(void)
{
// this function reads  the mouse state

if (lpdimouse)    
    {
    if (lpdimouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state)!=DI_OK)
        return(0);
    }
else
    {
    // mouse isn't plugged in, zero out state
    memset(&mouse_state,0,sizeof(mouse_state));

    // return error
    return(0);
    } // end else

// return sucess
return(1);

} // end DI_Read_Mouse

///////////////////////////////////////////////////////////

int DI_Read_Keyboard(void)
{
// this function reads the state of the keyboard

if (lpdikey)
    {
    if (lpdikey->GetDeviceState(256, (LPVOID)keyboard_state)!=DI_OK)
       return(0);
    }
else
    {
    // keyboard isn't plugged in, zero out state
    memset(keyboard_state,0,sizeof(keyboard_state));

    // return error
    return(0);
    } // end else

// return sucess
return(1);

} // end DI_Read_Keyboard

///////////////////////////////////////////////////////////

void DI_Release_Joystick(void)
{
// this function unacquires and releases the joystick

if (lpdijoy2)
    {    
    lpdijoy2->Unacquire();
    lpdijoy2->Release();
    } // end if

} // end DI_Release_Joystick

///////////////////////////////////////////////////////////

void DI_Release_Mouse(void)
{
// this function unacquires and releases the mouse

if (lpdimouse)
    {    
    lpdimouse->Unacquire();
    lpdimouse->Release();
    } // end if

} // end DI_Release_Mouse

///////////////////////////////////////////////////////////

void DI_Release_Keyboard(void)
{
// this function unacquires and releases the keyboard

if (lpdikey)
    {
    lpdikey->Unacquire();
    lpdikey->Release();
    } // end if

} // end DI_Release_Keyboard

///////////////////////////////////////////////////////////

int Clone_BOB(BOB_PTR source, BOB_PTR dest)
{
// this function clones a BOB and updates the attr var to reflect that
// the BOB is a clone and not real, this is used later in the destroy
// function so a clone doesn't destroy the memory of a real bob

if (source && dest)
   {
   // copy the bob data
   memcpy(dest,source, sizeof(BOB));

   // set the clone attribute
   dest->attr |= BOB_ATTR_CLONE;

   } // end if

else
    return(0);

// return success
return(1);

} // end Clone_BOB

///////////////////////////////////////////////////////////

int Destroy_BOB2(BOB_PTR bob)
{
// destroy the BOB, tests if this is a real bob or a clone
// if real then release all the memory, otherwise, just resets
// the pointers to null

int index; // looping var

// is this bob valid
if (!bob)
    return(0);

// test if this is a clone
if (bob->attr && BOB_ATTR_CLONE)
    {
    // null link all surfaces
    for (index=0; index<MAX_BOB_FRAMES; index++)
        if (bob->images[index])
            bob->images[index]=NULL;

    // release memory for animation sequences 
    for (index=0; index<MAX_BOB_ANIMATIONS; index++)
        if (bob->animations[index])
            bob->animations[index]==NULL;

    } // end if
else
    {
    // destroy each bitmap surface
    for (index=0; index<MAX_BOB_FRAMES; index++)
        if (bob->images[index])
            (bob->images[index])->Release();

    // release memory for animation sequences 
    for (index=0; index<MAX_BOB_ANIMATIONS; index++)
        if (bob->animations[index])
            free(bob->animations[index]);

    } // end else not clone

// return success
return(1);

} // end Destroy_BOB2

///////////////////////////////////////////////////////////

int Collision_Test(int x1, int y1, int w1, int h1, 
                   int x2, int y2, int w2, int h2) 
{
// this function tests if the two rects overlap

// get the radi of each rect
int width1  = (w1>>1) - (w1>>3);
int height1 = (h1>>1) - (h1>>3);

int width2  = (w2>>1) - (w2>>3);
int height2 = (h2>>1) - (h2>>3);

// compute center of each rect
int cx1 = x1 + width1;
int cy1 = y1 + height1;

int cx2 = x2 + width2;
int cy2 = y2 + height2;

// compute deltas
int dx = abs(cx2 - cx1);
int dy = abs(cy2 - cy1);

// test if rects overlap
if (dx < (width1+width2) && dy < (height1+height2))
   return(1);
else
// else no collision
return(0);

} // end Collision_Test

///////////////////////////////////////////////////////////

int Color_Scan(int x1, int y1, int x2, int y2, 
               UCHAR scan_start, UCHAR scan_end,
               UCHAR *scan_buffer, int scan_lpitch)
{
// this function implements a crude collision technique
// based on scanning for a range of colors within a rectangle

// clip rectangle

// x coords first    
if (x1 >= screen_width)
   x1=screen_width-1;
else
if (x1 < 0)
   x1=0;

if (x2 >= screen_width)
   x2=screen_width-1;
else
if (x2 < 0)
   x2=0;

// now y-coords
if (y1 >= screen_height)
   y1=screen_height-1;
else
if (y1 < 0)
   y1=0;

if (y2 >= screen_height)
   y2=screen_height-1;
else
if (y2 < 0)
   y2=0;

// scan the region
scan_buffer +=y1*scan_lpitch;

for (int scan_y=y1; scan_y<=y2; scan_y++)
    {
    for (int scan_x=x1; scan_x<=x2; scan_x++)
        {
        if (scan_buffer[scan_x] >= scan_start && scan_buffer[scan_x] <= scan_end )
            return(1);
        } // end for x

    // move down a line
    scan_buffer+=scan_lpitch;

    } // end for y

// return failure
return(0);

} // end Color_Scan
