// GPDUMB1.CPP - Game Engine Part I (DirectDraw stuff)
 
// INCLUDES ///////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
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
#include "gpdumb1.h"

// DEFINES ////////////////////////////////////////////////

// TYPES //////////////////////////////////////////////////

// PROTOTYPES /////////////////////////////////////////////

// EXTERNALS /////////////////////////////////////////////

extern HWND main_window_handle; // save the window handle
extern HINSTANCE main_instance; // save the instance
extern BOOL full_screen;

// GLOBALS ////////////////////////////////////////////////

FILE *fp_error                    = NULL; // general error file
LPDIRECTDRAW         lpdd         = NULL;  // dd object
LPDIRECTDRAWSURFACE  lpddsprimary = NULL;  // dd primary surface
LPDIRECTDRAWSURFACE  lpddsback    = NULL;  // dd back surface
LPDIRECTDRAWPALETTE  lpddpal      = NULL;  // a pointer to the created dd palette
LPDIRECTDRAWCLIPPER  lpddclipper  = NULL;  // dd clipper
PALETTEENTRY         palette[256];         // color palette
PALETTEENTRY         save_palette[256];    // used to save palettes
DDSURFACEDESC        ddsd;                 // a direct draw surface description struct
DDBLTFX              ddbltfx;              // used to fill
DDSCAPS              ddscaps;              // a direct draw surface capabilities struct
HRESULT              ddrval;               // result back from dd calls
UCHAR                *primary_buffer = NULL; // primary video buffer
UCHAR                *back_buffer    = NULL; // secondary back buffer
int                  primary_lpitch  = 0;    // memory line pitch
int                  back_lpitch     = 0;    // memory line pitch
BITMAP_FILE          bitmap16bit;            // a 16 bit bitmap file
BITMAP_FILE          bitmap8bit;             // a 8 bit bitmap file

DWORD                start_clock_count = 0; // used for timing

// these defined the general clipping rectangle
int min_clip_x = 0,                          // clipping rectangle 
    max_clip_x = SCREEN_WIDTH-1,
    min_clip_y = 0,
    max_clip_y = SCREEN_HEIGHT-1;

// these are overwritten globally by DD_Init()
int screen_width  = SCREEN_WIDTH,            // width of screen
    screen_height = SCREEN_HEIGHT,           // height of screen
    screen_bpp    = SCREEN_BPP;              // bits per pixel



// FUNCTIONS //////////////////////////////////////////////

LPCWSTR MultiCharToUniChar(char* mbString)
{
	int len = strlen(mbString) + 1;
	wchar_t *ucString = new wchar_t[len];
	mbstowcs(ucString, mbString, len);
	return (LPCWSTR)ucString;
}

int Create_BOB(BOB_PTR bob,           // the bob to create
               int x, int y,          // initial posiiton
               int width, int height, // size of bob
               int num_frames,        // number of frames
               int attr,              // attrs
               int mem_flags)         // memory flag
{
// Create the BOB object, note that all BOBs 
// are created as offscreen surfaces in VRAM as the
// default, if you want to use system memory then
// set flags equal to DDSCAPS_SYSTEMMEMORY

DDSURFACEDESC ddsd; // used to create surface
int index;          // looping var

// set state and attributes of BOB
bob->state          = BOB_STATE_ALIVE;
bob->attr           = attr;
bob->anim_state     = 0;
bob->counter_1      = 0;     
bob->counter_2      = 0;
bob->max_count_1    = 0;
bob->max_count_2    = 0;

bob->curr_frame     = 0;
bob->num_frames     = num_frames;
bob->curr_animation = 0;
bob->anim_counter   = 0;
bob->anim_index     = 0;
bob->anim_count_max = 0; 
bob->x              = x;
bob->y              = y;
bob->xv             = 0;
bob->yv             = 0;

// set dimensions of the new bitmap surface
bob->width  = width;
bob->height = height;

// set all images to null
for (index=0; index<MAX_BOB_FRAMES; index++)
    bob->images[index] = NULL;

// set all animations to null
for (index=0; index<MAX_BOB_ANIMATIONS; index++)
    bob->animations[index] = NULL;

// make sure width is a multiple of 8
bob->width_fill = ((width%8!=0) ? (8-width%8) : 0);
Write_Error("\nCreate BOB %d",bob->width_fill);

// now create each surface
for (index=0; index<bob->num_frames; index++)
    {
    // set to access caps, width, and height
    memset(&ddsd,0,sizeof(ddsd));
    ddsd.dwSize  = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

    ddsd.dwWidth  = bob->width + bob->width_fill;
    ddsd.dwHeight = bob->height;

    // set surface to offscreen plain
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;

    // create the surfaces
    if (lpdd->CreateSurface(&ddsd,&(bob->images[index]),NULL)!=DD_OK)
        return(0);

    // set color key to color 0
    DDCOLORKEY color_key; // used to set color key
    color_key.dwColorSpaceLowValue  = 0;
    color_key.dwColorSpaceHighValue = 0;

    // now set the color key for source blitting
    (bob->images[index])->SetColorKey(DDCKEY_SRCBLT, &color_key);
    } // end for index

// return success
return(1);

} // end Create_BOB

///////////////////////////////////////////////////////////

int Destroy_BOB(BOB_PTR bob)
{
// destroy the BOB, simply release the surface

int index; // looping var

// is this bob valid
if (!bob)
    return(0);
   
// destroy each bitmap surface
for (index=0; index<MAX_BOB_FRAMES; index++)
    if (bob->images[index])
       (bob->images[index])->Release();

// release memory for animation sequences 
for (index=0; index<MAX_BOB_ANIMATIONS; index++)
    if (bob->animations[index])
       free(bob->animations[index]);

// return success
return(1);

} // end Destroy_BOB

///////////////////////////////////////////////////////////

int Draw_BOB(BOB_PTR bob,              // bob to draw
             LPDIRECTDRAWSURFACE dest) // surface to draw the bob on
{
// draw a bob at the x,y defined in the BOB
// on the destination surface defined in dest

RECT dest_rect,   // the destination rectangle
     source_rect; // the source rectangle                             

// is this a valid bob
if (!bob)
    return(0);

// is bob visible
if (!(bob->attr & BOB_ATTR_VISIBLE))
   return(1);

// fill in the destination rect
dest_rect.left   = bob->x;
dest_rect.top    = bob->y;
dest_rect.right  = bob->x+bob->width-1;
dest_rect.bottom = bob->y+bob->height-1;

// fill in the source rect
source_rect.left    = 0;
source_rect.top     = 0;
source_rect.right   = bob->width-1;
source_rect.bottom  = bob->height-1;

// blt to destination surface
if (dest->Blt(&dest_rect, bob->images[bob->curr_frame],
          &source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
          NULL)!=DD_OK)
    return(0);

// return success
return(1);
} // end Draw_BOB

///////////////////////////////////////////////////////////

int Draw_Scaled_BOB(BOB_PTR bob, int swidth, int sheight,  // bob and new dimensions
                    LPDIRECTDRAWSURFACE dest) // surface to draw the bob on)
{
// this function draws a scaled bob to the size swidth, sheight

RECT dest_rect,   // the destination rectangle
     source_rect; // the source rectangle                             

// is this a valid bob
if (!bob)
    return(0);

// is bob visible
if (!(bob->attr & BOB_ATTR_VISIBLE))
   return(1);

// fill in the destination rect
dest_rect.left   = bob->x;
dest_rect.top    = bob->y;
dest_rect.right  = bob->x+swidth-1;
dest_rect.bottom = bob->y+sheight-1;

// fill in the source rect
source_rect.left    = 0;
source_rect.top     = 0;
source_rect.right   = bob->width-1;
source_rect.bottom  = bob->height-1;

// blt to destination surface
if (dest->Blt(&dest_rect, bob->images[bob->curr_frame],
          &source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
          NULL)!=DD_OK)
    return(0);

// return success
return(1);
} // end Draw_Scaled_BOB

///////////////////////////////////////////////////////////

int Load_Frame_BOB(BOB_PTR bob, // bob to load with data
                   BITMAP_FILE_PTR bitmap, // bitmap to scan image data from
                   int frame,       // frame to load
                   int cx,int cy,   // cell or absolute pos. to scan image from
                   int mode)        // if 0 then cx,cy is cell position, else 
                                    // cx,cy are absolute coords
{
// this function extracts a bitmap out of a bitmap file

UCHAR *source_ptr,   // working pointers
      *dest_ptr;

DDSURFACEDESC ddsd;  //  direct draw surface description 

// is this a valid bob
if (!bob)
   return(0);

// test the mode of extraction, cell based or absolute
if (mode==BITMAP_EXTRACT_MODE_CELL)
   {
   // re-compute x,y
   cx = cx*(bob->width+1) + 1;
   cy = cy*(bob->height+1) + 1;
   } // end if

// extract bitmap data
source_ptr = bitmap->buffer + cy*bitmap->bitmapinfoheader.biWidth+cx;

// get the addr to destination surface memory

// set size of the structure
ddsd.dwSize = sizeof(ddsd);

// lock the display surface
(bob->images[frame])->Lock(NULL,
                           &ddsd,
                           DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
                           NULL);

// assign a pointer to the memory surface for manipulation
dest_ptr = (UCHAR *)ddsd.lpSurface;

// iterate thru each scanline and copy bitmap
for (int index_y=0; index_y<bob->height; index_y++)
    {
    // copy next line of data to destination
    memcpy(dest_ptr, source_ptr,bob->width);

    // advance pointers
    dest_ptr   += (bob->width+bob->width_fill);
    source_ptr += bitmap->bitmapinfoheader.biWidth;
    } // end for index_y

// unlock the surface 
(bob->images[frame])->Unlock(ddsd.lpSurface);

// set state to loaded
bob->attr |= BOB_ATTR_LOADED;

// return success
return(1);

} // end Load_Frame_BOB

///////////////////////////////////////////////////////////

int Animate_BOB(BOB_PTR bob)
{
// this function animates a bob, basically it takes a look at
// the attributes of the bob and determines if the bob is 
// a single frame, multiframe, or multi animation, updates
// the counters and frames appropriately

#if 0
        int curr_frame;     // current animation frame
        int num_frames;     // total number of animation frames
        int curr_animation; // index of current animation
        int anim_counter;   // used to time animation transitions
        int anim_index;     // animation element index
        int anim_count_max; // number of cycles before animation
        int *animations[MAX_BOB_ANIMATIONS]; // animation sequences

        LPDIRECTDRAWSURFACE images[MAX_BOB_FRAMES]; // the bitmap images DD surfaces
#endif


// is this a valid bob
if (!bob)
   return(0);

// test the level of animation
if (bob->attr & BOB_ATTR_SINGLE_FRAME)
    {
    // current frame always = 0
    bob->curr_frame = 0;
    return(1);
    } // end if
else
if (bob->attr & BOB_ATTR_MULTI_FRAME)
   {
   // update the counter and test if its time to increment frame
   if (++bob->anim_counter >= bob->anim_count_max)
      {
      // reset counter
      bob->anim_counter = 0;

      // move to next frame
      if (++bob->curr_frame >= bob->num_frames)
         bob->curr_frame = 0;

      } // end if
  
   } // end elseif
else
if (bob->attr & BOB_ATTR_MULTI_ANIM)
   {
   // this is the most complex of the animations it must look up the
   // next frame in the animation sequence

   // first test if its time to animate
   if (++bob->anim_counter >= bob->anim_count_max)
      {
      // reset counter
      bob->anim_counter = 0;

      // increment the animation frame index
      bob->anim_index++;
      
      // extract the next frame from animation list 
      bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];
     
      // is this and end sequence flag -1
      if (bob->curr_frame == -1)
         {
         // test if this is a single shot animation
         if (bob->attr & BOB_ATTR_ANIM_ONE_SHOT)
            {
            // set animation state message to done
            bob->anim_state = BOB_STATE_ANIM_DONE;
            
            // reset frame back one
            bob->anim_index--;

            // extract animation frame
            bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];    

            } // end if
        else
           {
           // reset animation index
           bob->anim_index = 0;

           // extract first animation frame
           bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];
           } // end else

         }  // end if
      
      } // end if

   } // end elseif

// return success
return(1);

} // end Amimate_BOB

///////////////////////////////////////////////////////////

int Scroll_BOB(void)
{
// this function scrolls a bob 
// not implemented

// return success
return(1);
} // end Scroll_BOB

///////////////////////////////////////////////////////////

int Move_BOB(BOB_PTR bob)
{
// this function moves the bob based on its current velocity
// also, the function test for various motion attributes of the'
// bob and takes the appropriate actions
   

// is this a valid bob
if (!bob)
   return(0);

// translate the bob
bob->x+=bob->xv;
bob->y+=bob->yv;

// test for wrap around
if (bob->attr & BOB_ATTR_WRAPAROUND)
   {
   // test x extents first
   if (bob->x > max_clip_x)
       bob->x = min_clip_x - bob->width;
   else
   if (bob->x < min_clip_x-bob->width)
       bob->x = max_clip_x;
   
   // now y extents
   if (bob->x > max_clip_x)
       bob->x = min_clip_x - bob->width;
   else
   if (bob->x < min_clip_x-bob->width)
       bob->x = max_clip_x;

   } // end if
else
// test for bounce
if (bob->attr & BOB_ATTR_BOUNCE)
   {
   // test x extents first
   if ((bob->x > max_clip_x - bob->width) || (bob->x < min_clip_x) )
       bob->xv = -bob->xv;
    
   // now y extents 
   if ((bob->y > max_clip_y - bob->height) || (bob->y < min_clip_y) )
       bob->yv = -bob->yv;

   } // end if

// return success
return(1);
} // end Move_BOB

///////////////////////////////////////////////////////////

int Load_Animation_BOB(BOB_PTR bob, 
                       int anim_index, 
                       int num_frames, 
                       int *sequence)
{
	int index;

// this function load an animation sequence for a bob
// the sequence consists of frame indices, the function
// will append a -1 to the end of the list so the display
// software knows when to restart the animation sequence

// is this bob valid
if (!bob)
   return(0);

// allocate memory for bob animation
if (!(bob->animations[anim_index] = (int *)malloc((num_frames+1)*sizeof(int))))
   return(0);

// load data into 
for (index=0; index<num_frames; index++)
    bob->animations[anim_index][index] = sequence[index];

// set the end of the list to a -1
bob->animations[anim_index][index] = -1;

// return success
return(1);

} // end Load_Animation_BOB

///////////////////////////////////////////////////////////

int Set_Pos_BOB(BOB_PTR bob, int x, int y)
{
// this functions sets the postion of a bob

// is this a valid bob
if (!bob)
   return(0);

// set positin
bob->x = x;
bob->y = y;

// return success
return(1);
} // end Set_Pos_BOB

///////////////////////////////////////////////////////////

int Set_Anim_Speed_BOB(BOB_PTR bob,int speed)
{
// this function simply sets the animation speed of a bob
    
// is this a valid bob
if (!bob)
   return(0);

// set speed
bob->anim_count_max = speed;

// return success
return(1);

} // end Set_Anim_Speed

///////////////////////////////////////////////////////////

int Set_Animation_BOB(BOB_PTR bob, int anim_index)
{
// this function sets the animation to play

// is this a valid bob
if (!bob)
   return(0);

// set the animation index
bob->curr_animation = anim_index;

// reset animation 
bob->anim_index = 0;

// return success
return(1);

} // end Set_Animation_BOB

///////////////////////////////////////////////////////////

int Set_Vel_BOB(BOB_PTR bob,int xv, int yv)
{
// this function sets the velocity of a bob

// is this a valid bob
if (!bob)
   return(0);

// set velocity
bob->xv = xv;
bob->yv = yv;

// return success
return(1);
} // end Set_Vel_BOB

///////////////////////////////////////////////////////////

int Hide_BOB(BOB_PTR bob)
{
// this functions hides bob 

// is this a valid bob
if (!bob)
   return(0);

// reset the visibility bit
RESET_BIT(bob->attr, BOB_ATTR_VISIBLE);

// return success
return(1);
} // end Hide_BOB

///////////////////////////////////////////////////////////

int Show_BOB(BOB_PTR bob)
{
// this function shows a bob

// is this a valid bob
if (!bob)
   return(0);

// set the visibility bit
SET_BIT(bob->attr, BOB_ATTR_VISIBLE);

// return success
return(1);
} // end Show_BOB

///////////////////////////////////////////////////////////

int Collision_BOBS(BOB_PTR bob1, BOB_PTR bob2)
{
// are these a valid bobs
if (!bob1 || !bob2)
   return(0);

// get the radi of each rect
int width1  = (bob1->width>>1) - (bob1->width>>3);
int height1 = (bob1->height>>1) - (bob1->height>>3);

int width2  = (bob2->width>>1) - (bob2->width>>3);
int height2 = (bob2->height>>1) - (bob2->height>>3);

// compute center of each rect
int cx1 = bob1->x + width1;
int cy1 = bob1->y + height1;

int cx2 = bob2->x + width2;
int cy2 = bob2->y + height2;

// compute deltas
int dx = abs(cx2 - cx1);
int dy = abs(cy2 - cy1);

// test if rects overlap
if (dx < (width1+width2) && dy < (height1+height2))
   return(1);
else
// else no collision
return(0);

} // end Collision_BOBS

///////////////////////////////////////////////////////////

int DD_Init(int width, int height, int bpp)
{
// this function initializes directdraw
int index; // looping variable
HRESULT hr;

// create object and test for error
if (DirectDrawCreate(NULL,&lpdd,NULL)!=DD_OK)
   return(0);

if (full_screen)
{
	// set cooperation level to full-screen mode
	if (lpdd->SetCooperativeLevel(main_window_handle,
           DDSCL_ALLOWMODEX | DDSCL_FULLSCREEN | 
           DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)!=DD_OK)
		return(0); 
}
else
{
	if (lpdd->SetCooperativeLevel(main_window_handle,
           DDSCL_NORMAL)!=DD_OK)
		return(0); 
}

if (full_screen)
{
	// set the display mode
	if (lpdd->SetDisplayMode(width,height,bpp)!=DD_OK)
		return(0);
}

// set globals
screen_height = height;
screen_width = width;
screen_bpp = bpp;

if (full_screen)
{
	// Create the primary surface
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;

	// we need to let dd know that we want a complex 
	// flippable surface structure, set flags for that
	ddsd.ddsCaps.dwCaps = 
		DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;

	// set the backbuffer count to 1
	ddsd.dwBackBufferCount = 1;

	// create the primary surface
	lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL);

	// query for the backbuffer i.e the secondary surface
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	lpddsprimary->GetAttachedSurface(&ddscaps,&lpddsback);
}
else
{
	// Create the primary surface
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL);

	// Create a pseudo back buffer
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth = SCREEN_WIDTH;
	ddsd.dwHeight = SCREEN_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	lpdd->CreateSurface(&ddsd,&lpddsback,NULL);

	// Make a clipper
	lpdd->CreateClipper(0,&lpddclipper,NULL);

	// Attach clipper
	lpddclipper->SetHWnd(0,main_window_handle);
	lpddsprimary->SetClipper(lpddclipper);
//	lpddclipper->Release();
}

// create and attach palette

// create palette data
// clear all entries defensive programming
memset(palette,0,256*sizeof(PALETTEENTRY));

// create a R,G,B,GR gradient palette
for (index=0; index < 256; index++)
    {
    // set each entry
    if (index < 64) 
        palette[index].peRed = index*4; 
    else           // shades of green
    if (index >= 64 && index < 128) 
        palette[index].peGreen = (index-64)*4;
    else           // shades of blue
    if (index >= 128 && index < 192) 
       palette[index].peBlue = (index-128)*4;
    else           // shades of grey
    if (index >= 192 && index < 256) 
        palette[index].peRed = palette[index].peGreen = 
        palette[index].peBlue = (index-192)*4;
    
    // set flags
    palette[index].peFlags = PC_NOCOLLAPSE;
    } // end for index

// now create the palette object
if (lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE | DDPCAPS_ALLOW256,
                         palette,&lpddpal,NULL)!=DD_OK)
   return(0);

// attach the palette to the primary
if (lpddsprimary->SetPalette(lpddpal)!=DD_OK)
   return(0);

// clear out both primary and secondary surfaces
DD_Fill_Surface(lpddsprimary,0);
DD_Fill_Surface(lpddsback,0);

// return success
return(1);
} // end DD_Init

///////////////////////////////////////////////////////////

int DD_Shutdown(void)
{
// this function release all the resources directdraw
// allocated, mainly to com objects

// release the clipper first
if (lpddclipper)
    lpddclipper->Release();

// release the palette
if (lpddpal)
   lpddpal->Release();

// release the secondary surface
if (lpddsback)
    lpddsback->Release();

// release the primary surface
if (lpddsprimary)
   lpddsprimary->Release();

// finally, the main dd object
if (lpdd)
    lpdd->Release();

// return success
return(1);
} // end DD_Shutdown

///////////////////////////////////////////////////////////   

LPDIRECTDRAWCLIPPER DD_Attach_Clipper(LPDIRECTDRAWSURFACE lpdds,
                                      int num_rects,
                                      LPRECT clip_list)

{
// this function creates a clipper from the sent clip list and attaches
// it to the sent surface

int index;                         // looping var
LPDIRECTDRAWCLIPPER lpddclipper;   // pointer to the newly created dd clipper
LPRGNDATA region_data;             // pointer to the region data that contains
                                   // the header and clip list

// first create the direct draw clipper
if ((lpdd->CreateClipper(0,&lpddclipper,NULL))!=DD_OK)
   return(NULL);

// now create the clip list from the sent data

// first allocate memory for region data
region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+num_rects*sizeof(RECT));

// now copy the rects into region data
memcpy(region_data->Buffer, clip_list, sizeof(RECT)*num_rects);

// set up fields of header
region_data->rdh.dwSize          = sizeof(RGNDATAHEADER);
region_data->rdh.iType           = RDH_RECTANGLES;
region_data->rdh.nCount          = num_rects;
region_data->rdh.nRgnSize        = num_rects*sizeof(RECT);

region_data->rdh.rcBound.left    =  64000;
region_data->rdh.rcBound.top     =  64000;
region_data->rdh.rcBound.right   = -64000;
region_data->rdh.rcBound.bottom  = -64000;

// find bounds of all clipping regions
for (index=0; index<num_rects; index++)
    {
    // test if the next rectangle unioned with the current bound is larger
    if (clip_list[index].left < region_data->rdh.rcBound.left)
       region_data->rdh.rcBound.left = clip_list[index].left;

    if (clip_list[index].right > region_data->rdh.rcBound.right)
       region_data->rdh.rcBound.right = clip_list[index].right;

    if (clip_list[index].top < region_data->rdh.rcBound.top)
       region_data->rdh.rcBound.top = clip_list[index].top;

    if (clip_list[index].bottom > region_data->rdh.rcBound.bottom)
       region_data->rdh.rcBound.bottom = clip_list[index].bottom;

    } // end for index

// now we have computed the bounding rectangle region and set up the data
// now let's set the clipping list

if ((lpddclipper->SetClipList(region_data, 0))!=DD_OK)
   {
   // release memory and return error
   free(region_data);
   return(NULL);
   } // end if

// now attach the clipper to the surface
if ((lpdds->SetClipper(lpddclipper))!=DD_OK)
   {
   // release memory and return error
   free(region_data);
   return(NULL);
   } // end if

// all is well, so release memory and send back the pointer to the new clipper
free(region_data);
return(lpddclipper);

} // end DD_Attach_Clipper

///////////////////////////////////////////////////////////   
   
LPDIRECTDRAWSURFACE DD_Create_Surface(int width, int height, int mem_flags)
{
// this function creates an offscreen plain surface

DDSURFACEDESC ddsd;         // working description
LPDIRECTDRAWSURFACE lpdds;  // temporary surface
    
// set to access caps, width, and height
memset(&ddsd,0,sizeof(ddsd));
ddsd.dwSize  = sizeof(ddsd);
ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

// set dimensions of the new bitmap surface
ddsd.dwWidth  =  width;
ddsd.dwHeight =  height;

// set surface to offscreen plain
ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;

// create the surface
if (lpdd->CreateSurface(&ddsd,&lpdds,NULL)!=DD_OK)
   return(NULL);

// set color key to color 0
DDCOLORKEY color_key; // used to set color key
color_key.dwColorSpaceLowValue  = 0;
color_key.dwColorSpaceHighValue = 0;

// now set the color key for source blitting
lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);

// return surface
return(lpdds);
} // end DD_Create_Surface

///////////////////////////////////////////////////////////   
   
int DD_Flip(void)
{
	RECT dest_rect;   // the destination rectangle 
	POINT p1, p2;
    
// this function flip the primary surface with the secondary surface

// test if either of the buffers are locked
if (primary_buffer || back_buffer)
   return(0);

// flip pages
if (full_screen)
	while(lpddsprimary->Flip(NULL, DDFLIP_WAIT)!=DD_OK);
else
{
	// Set the destination rect to the client area.
	dest_rect.left = 0;
	dest_rect.top = 0;
	dest_rect.right = 0;
	dest_rect.bottom = 0;
	GetClientRect(main_window_handle,&dest_rect);

	// Set two points for a conversion.
	p1.x = dest_rect.left;
    p1.y = dest_rect.top;
    p2.x = dest_rect.right;
    p2.y = dest_rect.bottom;

    // Convert them to screen coordinates (like DirectDraw uses)
    ClientToScreen(main_window_handle,&p1);
    ClientToScreen(main_window_handle,&p2);

    // Copy the two points' data back into the rect
    dest_rect.left   = p1.x;
    dest_rect.top    = p1.y;
    dest_rect.right  = p2.x;
    dest_rect.bottom = p2.y;

	// Blit the pseudo back-buffer to the primary surface.
	while(lpddsprimary->Blt(&dest_rect,lpddsback,NULL,
			DDBLT_WAIT,NULL)!=DD_OK);
}

// flip the surface

// return success
return(1);

} // end DD_Flip

///////////////////////////////////////////////////////////   
   
int DD_Wait_For_Vsync(void)
{
// this function waits for a vertical blank to begin
    
lpdd->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN,0);

// return success
return(1);
} // end DD_Wait_For_Vsync

///////////////////////////////////////////////////////////   
   
int DD_Fill_Surface(LPDIRECTDRAWSURFACE lpdds,int color)
{
DDBLTFX ddbltfx; // this contains the DDBLTFX structure

// clear out the structure and set the size field 
DD_INIT_STRUCT(ddbltfx);

// set the dwfillcolor field to the desired color
ddbltfx.dwFillColor = color; 

// ready to blt to surface
lpdds->Blt(NULL,       // ptr to dest rectangle
           NULL,       // ptr to source surface, NA            
           NULL,       // ptr to source rectangle, NA
           DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
           &ddbltfx);  // ptr to DDBLTFX structure

// return success
return(1);
} // end DD_Fill_Surface

///////////////////////////////////////////////////////////   
   
UCHAR *DD_Lock_Surface(LPDIRECTDRAWSURFACE lpdds,int *lpitch)
{
// this function locks the sent surface and returns a pointer to it

// is this surface valid
if (!lpdds)
   return(NULL);

// lock the surface
DD_INIT_STRUCT(ddsd);
lpdds->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,NULL); 

// set the memory pitch
if (lpitch)
   *lpitch = ddsd.lPitch;

// return pointer to surface
return((UCHAR *)ddsd.lpSurface);

} // end DD_Lock_Surface

///////////////////////////////////////////////////////////   
   
int DD_Unlock_Surface(LPDIRECTDRAWSURFACE lpdds, UCHAR *surface_buffer)
{
// this unlocks a general surface

// is this surface valid
if (!lpdds)
   return(0);

// unlock the surface memory
lpdds->Unlock(surface_buffer);

// return success
return(1);
} // end DD_Unlock_Surface

///////////////////////////////////////////////////////////

UCHAR *DD_Lock_Primary_Surface(void)
{
// this function locks the priamary surface and returns a pointer to it
// and updates the global variables primary_buffer, and primary_lpitch

// is this surface already locked
if (primary_buffer)
   {
   // return to current lock
   return(primary_buffer);
   } // end if

// lock the primary surface
DD_INIT_STRUCT(ddsd);
lpddsprimary->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,NULL); 

// set globals
primary_buffer = (UCHAR *)ddsd.lpSurface;
primary_lpitch = ddsd.lPitch;

// return pointer to surface
return(primary_buffer);

} // end DD_Lock_Primary_Surface

///////////////////////////////////////////////////////////   
   
int DD_Unlock_Primary_Surface(void)
{
// this unlocks the primary

// is this surface valid
if (!primary_buffer)
   return(0);

// unlock the primary surface
lpddsprimary->Unlock(primary_buffer);

// reset the primary surface
primary_buffer = NULL;
primary_lpitch = 0;

// return success
return(1);
} // end DD_Unlock_Primary_Surface

//////////////////////////////////////////////////////////

UCHAR *DD_Lock_Back_Surface(void)
{
// this function locks the secondary back surface and returns a pointer to it
// and updates the global variables secondary buffer, and back_lpitch

// is this surface already locked
if (back_buffer)
   {
   // return to current lock
   return(back_buffer);
   } // end if

// lock the primary surface
DD_INIT_STRUCT(ddsd);
lpddsback->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,NULL); 

// set globals
back_buffer = (UCHAR *)ddsd.lpSurface;
back_lpitch = ddsd.lPitch;

// return pointer to surface
return(back_buffer);

} // end DD_Lock_Back_Surface

///////////////////////////////////////////////////////////   
   
int DD_Unlock_Back_Surface(void)
{
// this unlocks the secondary

// is this surface valid
if (!back_buffer)
   return(0);

// unlock the secondary surface
lpddsback->Unlock(back_buffer);

// reset the secondary surface
back_buffer = NULL;
back_lpitch = 0;

// return success
return(1);
} // end DD_Unlock_Back_Surface

///////////////////////////////////////////////////////////

DWORD Get_Clock(void)
{
// this function returns the current tick count

// return time
return(GetTickCount());

} // end Get_Clock

///////////////////////////////////////////////////////////

DWORD Start_Clock(void)
{
// this function starts the clock, that is, saves the current
// count, use in conjunction with Wait_Clock()

return(start_clock_count = Get_Clock());

} // end Start_Clock

////////////////////////////////////////////////////////////

DWORD Wait_Clock(DWORD count)
{
// this function is used to wait for a specific number of clicks
// since the call to Start_Clock

while((Get_Clock() - start_clock_count) < count);
return(Get_Clock());

} // end Wait_Clock

///////////////////////////////////////////////////////////

int Draw_Clip_Line(int x0,int y0, int x1, int y1,UCHAR color, 
                    UCHAR *dest_buffer, int lpitch)
{
// this function draws a wireframe triangle

int cxs, cys,
	cxe, cye;

// clip and draw each line
cxs = x0;
cys = y0;
cxe = x1;
cye = y1;

// clip the line
if (Clip_Line(cxs,cys,cxe,cye))
	Draw_Line(cxs, cys, cxe,cye,color,dest_buffer,lpitch);

// return success
return(1);

} // end Draw_Clip_Line

///////////////////////////////////////////////////////////

int Clip_Line(int &x1,int &y1,int &x2, int &y2)
{
// this function clips the sent line using the globally defined clipping
// region

// internal clipping codes
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008
#define CLIP_CODE_S  0x0004
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001

#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009 
#define CLIP_CODE_SW 0x0005

int xc1=x1, 
    yc1=y1, 
	xc2=x2, 
	yc2=y2;

int p1_code=0, 
    p2_code=0;

// determine codes for p1 and p2
if (y1 < min_clip_y)
	p1_code|=CLIP_CODE_N;
else
if (y1 > max_clip_y)
	p1_code|=CLIP_CODE_S;

if (x1 < min_clip_x)
	p1_code|=CLIP_CODE_W;
else
if (x1 > max_clip_x)
	p1_code|=CLIP_CODE_E;

if (y2 < min_clip_y)
	p2_code|=CLIP_CODE_N;
else
if (y2 > max_clip_y)
	p2_code|=CLIP_CODE_S;

if (x2 < min_clip_x)
	p2_code|=CLIP_CODE_W;
else
if (x2 > max_clip_x)
	p2_code|=CLIP_CODE_E;

// try and trivially reject
if ((p1_code & p2_code)) 
	return(0);

// test for totally visible, if so leave points untouched
if (p1_code==0 && p2_code==0)
	return(1);

// determine end clip point for p1
switch(p1_code)
	  {
	  case CLIP_CODE_C: break;

	  case CLIP_CODE_N:
		   {
		   yc1 = min_clip_y;
		   xc1 = x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1);
		   } break;
	  case CLIP_CODE_S:
		   {
		   yc1 = max_clip_y;
		   xc1 = x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1);
		   } break;

	  case CLIP_CODE_W:
		   {
		   xc1 = min_clip_x;
		   yc1 = y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1);
		   } break;
		
	  case CLIP_CODE_E:
		   {
		   xc1 = max_clip_x;
		   yc1 = y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1);
		   } break;

	// these cases are more complex, must compute 2 intersections
	  case CLIP_CODE_NE:
		   {
		   // north hline intersection
		   yc1 = min_clip_y;
		   xc1 = x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1);

		   // test if intersection is valid, of so then done, else compute next
			if (xc1 < min_clip_x || xc1 > max_clip_x)
				{
				// east vline intersection
				xc1 = max_clip_x;
				yc1 = y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1);
				} // end if

		   } break;
	  
	  case CLIP_CODE_SE:
      	   {
		   // south hline intersection
		   yc1 = max_clip_y;
		   xc1 = x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1);	

		   // test if intersection is valid, of so then done, else compute next
		   if (xc1 < min_clip_x || xc1 > max_clip_x)
		      {
			  // east vline intersection
			  xc1 = max_clip_x;
			  yc1 = y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1);
			  } // end if

		   } break;
	    
	  case CLIP_CODE_NW: 
      	   {
		   // north hline intersection
		   yc1 = min_clip_y;
		   xc1 = x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1);
		   
		   // test if intersection is valid, of so then done, else compute next
		   if (xc1 < min_clip_x || xc1 > max_clip_x)
		      {
			  xc1 = min_clip_x;
		      yc1 = y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1);	
			  } // end if

		   } break;
	  	  
	  case CLIP_CODE_SW:
		   {
		   // south hline intersection
		   yc1 = max_clip_y;
		   xc1 = x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1);	
		   
		   // test if intersection is valid, of so then done, else compute next
		   if (xc1 < min_clip_x || xc1 > max_clip_x)
		      {
			  xc1 = min_clip_x;
		      yc1 = y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1);	
			  } // end if

		   } break;

	  default:break;

	  } // end switch

// determine clip point for p2
switch(p2_code)
	  {
	  case CLIP_CODE_C: break;

	  case CLIP_CODE_N:
		   {
		   yc2 = min_clip_y;
		   xc2 = x2 + (min_clip_y-y2)*(x1-x2)/(y1-y2);
		   } break;

	  case CLIP_CODE_S:
		   {
		   yc2 = max_clip_y;
		   xc2 = x2 + (max_clip_y-y2)*(x1-x2)/(y1-y2);
		   } break;

	  case CLIP_CODE_W:
		   {
		   xc2 = min_clip_x;
		   yc2 = y2 + (min_clip_x-x2)*(y1-y2)/(x1-x2);
		   } break;
		
	  case CLIP_CODE_E:
		   {
		   xc2 = max_clip_x;
		   yc2 = y2 + (max_clip_x-x2)*(y1-y2)/(x1-x2);
		   } break;

		// these cases are more complex, must compute 2 intersections
	  case CLIP_CODE_NE:
		   {
		   // north hline intersection
		   yc2 = min_clip_y;
		   xc2 = x2 + 0.5+(min_clip_y-y2)*(x1-x2)/(y1-y2);

		   // test if intersection is valid, of so then done, else compute next
			if (xc2 < min_clip_x || xc2 > max_clip_x)
				{
				// east vline intersection
				xc2 = max_clip_x;
				yc2 = y2 + 0.5+(max_clip_x-x2)*(y1-y2)/(x1-x2);
				} // end if

		   } break;
	  
	  case CLIP_CODE_SE:
      	   {
		   // south hline intersection
		   yc2 = max_clip_y;
		   xc2 = x2 + 0.5+(max_clip_y-y2)*(x1-x2)/(y1-y2);	

		   // test if intersection is valid, of so then done, else compute next
		   if (xc2 < min_clip_x || xc2 > max_clip_x)
		      {
			  // east vline intersection
			  xc2 = max_clip_x;
			  yc2 = y2 + 0.5+(max_clip_x-x2)*(y1-y2)/(x1-x2);
			  } // end if

		   } break;
	    
	  case CLIP_CODE_NW: 
      	   {
		   // north hline intersection
		   yc2 = min_clip_y;
		   xc2 = x2 + 0.5+(min_clip_y-y2)*(x1-x2)/(y1-y2);
		   
		   // test if intersection is valid, of so then done, else compute next
		   if (xc2 < min_clip_x || xc2 > max_clip_x)
		      {
			  xc2 = min_clip_x;
		      yc2 = y2 + 0.5+(min_clip_x-x2)*(y1-y2)/(x1-x2);	
			  } // end if

		   } break;
	  	  
	  case CLIP_CODE_SW:
		   {
		   // south hline intersection
		   yc2 = max_clip_y;
		   xc2 = x2 + 0.5+(max_clip_y-y2)*(x1-x2)/(y1-y2);	
		   
		   // test if intersection is valid, of so then done, else compute next
		   if (xc2 < min_clip_x || xc2 > max_clip_x)
		      {
			  xc2 = min_clip_x;
		      yc2 = y2 + 0.5+(min_clip_x-x2)*(y1-y2)/(x1-x2);	
			  } // end if

		   } break;
	
	  default:break;

	  } // end switch

// do bounds check
if ((xc1 < min_clip_x) || (xc1 > max_clip_x) ||
	(yc1 < min_clip_y) || (yc1 > max_clip_y) ||
	(xc2 < min_clip_x) || (xc2 > max_clip_x) ||
	(yc2 < min_clip_y) || (yc2 > max_clip_y) )
	{
	return(0);
	} // end if

// store vars back
x1 = xc1;
y1 = yc1;
x2 = xc2;
y2 = yc2;

return(1);

} // end Clip_Line

///////////////////////////////////////////////////////////

int Draw_Line(int xo, int yo, int x1,int y1, UCHAR color,
               UCHAR *vb_start,int lpitch)
{
// this function draws a line from xo,yo to x1,y1 using differential error
// terms (based on Bresenahams work)

int dx,             // difference in x's
    dy,             // difference in y's
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error=0,        // the discriminant i.e. error i.e. decision variable
    index;          // used for looping

int swap;           // GLC-added variable

// Rearrange the points (if necessary) if the second point
// is above the first (in the y-coordinate).
// GLC addition to improve vector display.
if (yo > y1)
{
	swap = xo;
	xo = x1;
	x1 = swap;
	
	swap = yo;
	yo = y1;
	y1 = swap;
}

// pre-compute first pixel address in video buffer
vb_start = vb_start + xo + yo*lpitch;

// compute horizontal and vertical deltas
dx = x1-xo;
dy = y1-yo;

// test which direction the line is going in i.e. slope angle
if (dx>=0)
   {
   x_inc = 1;

   } // end if line is moving right
else
   {
   x_inc = -1;
   dx    = -dx;  // need absolute value

   } // end else moving left

// test y component of slope

if (dy>=0)
   {
   y_inc = lpitch;
   } // end if line is moving down
else
   {
   y_inc = -lpitch;
   dy    = -dy;  // need absolute value

   } // end else moving up

// now based on which delta is greater we can draw the line
if (dx>dy)
   {
   // draw the line
   for (index=0; index<=dx; index++)
       {
       // set the pixel
       *vb_start = color;

       // adjust the error term
       error+=dy;

       // test if error has overflowed
       if (error>dx)
          {
          error-=dx;

          // move to next line
          vb_start+=y_inc;


	   } // end if error overflowed

       // move to the next pixel
       vb_start+=x_inc;

       } // end for

   } // end if |slope| <= 1
else
   {
   // draw the line
   for (index=0; index<=dy; index++)
       {
       // set the pixel
       *vb_start = color;

       // adjust the error term
       error+=dx;

       // test if error overflowed
       if (error>0)
          {
          error-=dy;

          // move to next line
          vb_start+=x_inc;

          } // end if error overflowed

       // move to the next pixel
       vb_start+=y_inc;

       } // end for

   } // end else |slope| > 1

// return success
return(1);

} // end Draw_Line

///////////////////////////////////////////////////////////   
   
int Draw_Pixel(int x, int y,int color,
               UCHAR *video_buffer, int lpitch)
{
// this function plots a single pixel at x,y with color

video_buffer[x + y*lpitch] = color;

// return success
return(1);

} // end Draw_Pixel

///////////////////////////////////////////////////////////   
   
int Draw_Rectangle(int x1, int y1, int x2, int y2, int color,
                   LPDIRECTDRAWSURFACE lpdds)
{
// this function uses directdraw to draw a filled rectangle

DDBLTFX ddbltfx; // this contains the DDBLTFX structure
RECT fill_area;  // this contains the destination rectangle

// clear out the structure and set the size field 
DD_INIT_STRUCT(ddbltfx);

// set the dwfillcolor field to the desired color
ddbltfx.dwFillColor = color; 

// fill in the destination rectangle data (your data)
fill_area.top    = y1;
fill_area.left   = x1;
fill_area.bottom = y2;
fill_area.right  = x2;

// ready to blt to surface, in this case blt to primary
lpdds->Blt(&fill_area, // ptr to dest rectangle
           NULL,       // ptr to source surface, NA            
           NULL,       // ptr to source rectangle, NA
           DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
           &ddbltfx);  // ptr to DDBLTFX structure

// return success
return(1);

} // end Draw_Rectangle

///////////////////////////////////////////////////////////
   
int Set_Palette_Entry(int color_index, LPPALETTEENTRY color)
{
// this function sets a palette color in the palette
lpddpal->SetEntries(0,color_index,1,color);

// set data in shadow palette
memcpy(&palette[color_index],color,sizeof(PALETTEENTRY));

// return success
return(1);
} // end Set_Palette_Entry

///////////////////////////////////////////////////////////   
   
int Get_Palette_Entry(int color_index,LPPALETTEENTRY color)
{
// this function retrieves a palette entry from the color
// palette

// copy data out from shadow palette
memcpy(color, &palette[color_index],sizeof(PALETTEENTRY));

// return success
return(1);
} // end Get_Palette_Entry

///////////////////////////////////////////////////////////
   
int Load_Palette_From_File(char *filename, LPPALETTEENTRY palette)
{
// this function loads a palette from disk into a palette
// structure, but does not set the pallette

FILE *fp_file; // working file

// try and open file
if ((fp_file = fopen(filename,"r"))==NULL)
   return(0);

// read in all 256 colors RGBF
for (int index=0; index<256; index++)
    {
    // read the next entry in
    fscanf(fp_file,"%d %d %d %d",&palette[index].peRed,
                                 &palette[index].peGreen,
                                 &palette[index].peBlue,                                
                                 &palette[index].peFlags);
    } // end for index

// close the file
fclose(fp_file);

// return success
return(1);
} // end Load_Palette_From_Disk

///////////////////////////////////////////////////////////   
   
int Save_Palette_To_File(char *filename, LPPALETTEENTRY palette)
{
// this function saves a palette to disk

FILE *fp_file; // working file

// try and open file
if ((fp_file = fopen(filename,"w"))==NULL)
   return(0);

// write in all 256 colors RGBF
for (int index=0; index<256; index++)
    {
    // read the next entry in
    fprintf(fp_file,"\n%d %d %d %d",palette[index].peRed,
                                    palette[index].peGreen,
                                    palette[index].peBlue,                                
                                    palette[index].peFlags);
    } // end for index

// close the file
fclose(fp_file);

// return success
return(1);

} // end Save_Palette_To_Disk

///////////////////////////////////////////////////////////

int Save_Palette(LPPALETTEENTRY sav_palette)
{
// this function saves the current palette 

memcpy(sav_palette, palette,256*sizeof(PALETTEENTRY));

// return success
return(1);
} // end Save_Palette

///////////////////////////////////////////////////////////   
   
int Set_Palette(LPPALETTEENTRY set_palette)
{
// this function writes the sent palette

// first save the new palette in shadow
memcpy(palette, set_palette,256*sizeof(PALETTEENTRY));

// now set the new palette
lpddpal->SetEntries(0,0,256,palette);

// return success
return(1);
} // end Set_Palette

///////////////////////////////////////////////////////////

int Rotate_Colors(int start_index, int colors)
{
// this function rotates the color between start and end

PALETTEENTRY work_pal[256]; // working palette

// get the color palette
lpddpal->GetEntries(0,start_index,colors,work_pal);

// shift the colors
lpddpal->SetEntries(0,start_index+1,colors-1,work_pal);

// fix up the last color
lpddpal->SetEntries(0,start_index,1,&work_pal[colors - 1]);

// update shadow palette
lpddpal->GetEntries(0,0,256,palette);

// return sucess
return(1);

} // end Rotate_Colors

///////////////////////////////////////////////////////////   
   
int Blink_Colors(void)
{
// this funciton blinks a set of lights
// not implemented

// return success
return(1);
} // end Blink_Colors

///////////////////////////////////////////////////////////   
   
int Screen_Transition(void)
{
// this function performs a screen transition on the sent
// video buffer
// not implemented


// return success
return(1);
} // end Screen_Transition

///////////////////////////////////////////////////////////

int Draw_Text_GDI(char *text, int x,int y,COLORREF color, LPDIRECTDRAWSURFACE lpdds)
{
// this function draws the sent text on the sent surface 
// using color index as the color in the palette

HDC xdc; // the working dc

// get the dc from surface
if (lpdds->GetDC(&xdc)!=DD_OK)
   return(0);

// set the colors for the text up
SetTextColor(xdc,color);

// set background mode to transparent so black isn't copied
SetBkMode(xdc, TRANSPARENT);

// draw the text a
TextOut(xdc,x,y,MultiCharToUniChar(text),strlen(text));

// release the dc
lpdds->ReleaseDC(xdc);

// return success
return(1);
} // end Draw_Text_GDI

///////////////////////////////////////////////////////////

int Draw_Text_GDI(char *text, int x,int y,int color, LPDIRECTDRAWSURFACE lpdds)
{
// this function draws the sent text on the sent surface 
// using color index as the color in the palette

HDC xdc; // the working dc

// get the dc from surface
if (lpdds->GetDC(&xdc)!=DD_OK)
   return(0);

// set the colors for the text up
SetTextColor(xdc,RGB(palette[color].peRed,palette[color].peGreen,palette[color].peBlue) );

// set background mode to transparent so black isn't copied
SetBkMode(xdc, TRANSPARENT);

// draw the text a
TextOut(xdc,x,y,MultiCharToUniChar(text),strlen(text));

// release the dc
lpdds->ReleaseDC(xdc);

// return success
return(1);
} // end Draw_Text_GDI

///////////////////////////////////////////////////////////

int Open_Error_File(char *filename)
{
// this function opens the output error file

FILE *fp_temp; // temporary file

// test if this file is valid
if ((fp_temp = fopen(filename,"w"))==NULL)
   return(0);

// close old file if there was one
if (fp_error)
   Close_Error_File();

// assign new file

fp_error = fp_temp;

// write out header
Write_Error("\nOpening Error Output File (%s):\n",filename);

// return success
return(1);

} // end Open_Error_File

///////////////////////////////////////////////////////////

int Close_Error_File(void)
{
// this function closes the error file

if (fp_error)
    {
    // write close file string
    Write_Error("\nClosing Error Output File.");

    // close the file handle
    fclose(fp_error);
    fp_error = NULL;

    // return success
    return(1);
    } // end if
else
   return(0);

} // end Close_Error_File

///////////////////////////////////////////////////////////

int Write_Error(char *string, ...)
{
// this function prints out the error string to the error file

char buffer[80]; // working buffer

va_list arglist; // variable argument list

// make sure both the error file and string are valid
if (!string || !fp_error)
   return(0);

// print out the string using the variable number of arguments on stack
va_start(arglist,string);
vsprintf(buffer,string,arglist);
va_end(arglist);

// write string to file
fprintf(fp_error,buffer);

// return success
return(1);
} // end Write_Error

///////////////////////////////////////////////////////////////////////////////

int Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height)
{
// this function is used to intialize a bitmap

// allocate the memory
if (!(image->buffer = (UCHAR *)malloc(width*height)))
   return(0);

// initialize variables
image->state     = BITMAP_STATE_ALIVE;
image->attr      = 0;
image->width     = width;
image->height    = height;
image->x         = x;
image->y         = y;
image->num_bytes = width*height;

// clear memory out
memset(image->buffer,0,width*height);

// return success
return(1);

} // end Create_Bitmap

///////////////////////////////////////////////////////////////////////////////

int Destroy_Bitmap(BITMAP_IMAGE_PTR image)
{
// this function releases the memory associated with a bitmap

if (image && image->buffer)
   {
   // free memory and reset vars
   free(image->buffer);

   // set all vars in structure to 0
   memset(image,0,sizeof(BITMAP_IMAGE));

   // return success
   return(1);

   } // end if
else  // invalid entry pointer of the object wasn't initialized
   return(0);

} // end Destroy_Bitmap

///////////////////////////////////////////////////////////

int Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap,UCHAR *dest_buffer, int lpitch, int transparent)
{
// this function draws the bitmap onto the destination memory surface
// if transparent is 1 then color 0 will be transparent
// note this function does NOT clip, so be carefull!!!

UCHAR *dest_addr,   // starting address of bitmap in destination
      *source_addr; // starting adddress of bitmap data in source

UCHAR pixel;        // used to hold pixel value

int index,          // looping vars
    pixel_x;

// test if this bitmap is loaded

if (source_bitmap->attr & BITMAP_ATTR_LOADED)
   {
   // compute starting destination address
   dest_addr = dest_buffer + source_bitmap->y*lpitch + source_bitmap->x;

   // compute the starting source address
   source_addr = source_bitmap->buffer;

   // is this bitmap transparent
   if (transparent)
   {
   // copy each line of bitmap into destination with transparency
   for (index=0; index<source_bitmap->height; index++)
       {
       // copy the memory
       for (pixel_x=0; pixel_x<source_bitmap->width; pixel_x++)
           {
           if ((pixel = source_addr[pixel_x])!=0)
               dest_addr[pixel_x] = pixel;

           } // end if

       // advance all the pointers
       dest_addr   += lpitch;
       source_addr += source_bitmap->width;

       } // end for index
   } // end if
   else
   {
   // non-transparent version
   // copy each line of bitmap into destination
   for (index=0; index<source_bitmap->height; index++)
       {
       // copy the memory
       memcpy(dest_addr, source_addr, source_bitmap->width);

       // advance all the pointers
       dest_addr   += lpitch;
       source_addr += source_bitmap->width;

       } // end for index

   } // end else

   // return success
   return(1);

   } // end if
else
   return(0);

} // end Draw_Bitmap

///////////////////////////////////////////////////////////////////////////////

int Load_Image_Bitmap(BITMAP_IMAGE_PTR image, // bitmap image to load with data
                      BITMAP_FILE_PTR bitmap,    // bitmap to scan image data from
                      int cx,int cy,   // cell or absolute pos. to scan image from
                      int mode)        // if 0 then cx,cy is cell position, else 
                                    // cx,cy are absolute coords
{
// this function extracts a bitmap out of a bitmap file

UCHAR *source_ptr,   // working pointers
      *dest_ptr;

// is this a valid bob
if (!image)
   return(0);

// test the mode of extraction, cell based or absolute
if (mode==BITMAP_EXTRACT_MODE_CELL)
   {
   // re-compute x,y
   cx = cx*(image->width+1) + 1;
   cy = cy*(image->height+1) + 1;
   } // end if

// extract bitmap data
source_ptr = bitmap->buffer +
      cy*bitmap->bitmapinfoheader.biWidth+cx;

// assign a pointer to the bimap image
dest_ptr = (UCHAR *)image->buffer;

// iterate thru each scanline and copy bitmap
for (int index_y=0; index_y<image->height; index_y++)
    {
    // copy next line of data to destination
    memcpy(dest_ptr, source_ptr,image->width);

    // advance pointers
    dest_ptr   += image->width;
    source_ptr += bitmap->bitmapinfoheader.biWidth;
    } // end for index_y

// set state to loaded
image->attr |= BITMAP_ATTR_LOADED;

// return success
return(1);

} // end Load_Image_Bitmap

///////////////////////////////////////////////////////////

int Scroll_Bitmap(void)
{
// this function scrolls a bitmap
// not implemented

// return success
return(1);
} // end Scroll_Bitmap
///////////////////////////////////////////////////////////

int Copy_Bitmap(void)
{
// this function copies a bitmap from one source to another
// not implemented


// return success
return(1);
} // end Copy_Bitmap

///////////////////////////////////////////////////////////

int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename)
{
// this function opens a bitmap file and loads the data into bitmap

int file_handle,  // the file handle
    index;        // looping index

UCHAR *temp_buffer = NULL; // used to convert 24 bit images to 16 bit
OFSTRUCT file_data;        // the file data information

// open the file if it exists
if ((file_handle = OpenFile(filename,&file_data,OF_READ))==-1)
   return(0);

// now load the bitmap file header
_lread(file_handle, &bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));

// test if this is a bitmap file
if (bitmap->bitmapfileheader.bfType!=BITMAP_ID)
   {
   // close the file
   _lclose(file_handle);

   // return error
   return(0);
   } // end if

// now we know this is a bitmap, so read in all the sections

// first the bitmap infoheader

// now load the bitmap file header
_lread(file_handle, &bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));

// now load the color palette if there is one
if (bitmap->bitmapinfoheader.biBitCount == 8)
   {
   _lread(file_handle, &bitmap->palette,256*sizeof(PALETTEENTRY));

   // now set all the flags in the palette correctly and fix the reversed 
   // BGR RGBQUAD data format
   for (index=0; index < 256; index++)
       {
       // reverse the red and green fields
       int temp_color = bitmap->palette[index].peRed;
       bitmap->palette[index].peRed  = bitmap->palette[index].peBlue;
       bitmap->palette[index].peBlue = temp_color;
       
       // always set the flags word to this
       bitmap->palette[index].peFlags = PC_NOCOLLAPSE;
       } // end for index

    } // end if

// finally the image data itself
_llseek(file_handle,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);

// now read in the image, if the image is 8 or 16 bit then simply read it
// but if its 24 bit then read it into a temporary area and then convert
// it to a 16 bit image

if (bitmap->bitmapinfoheader.biBitCount==8 || bitmap->bitmapinfoheader.biBitCount==16)
   {
   // delete the last image if there was one
   if (bitmap->buffer)
       free(bitmap->buffer);

   // allocate the memory for the image
   if (!(bitmap->buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
      {
      // close the file
      _lclose(file_handle);

      // return error
      return(0);
      } // end if

   // now read it in
   _lread(file_handle,bitmap->buffer,bitmap->bitmapinfoheader.biSizeImage);

   } // end if
else
   {
   // this must be a 24 bit image, load it in and convert it to 16 bit
//   printf("\nconverting 24 bit image...");

   // allocate temporary buffer
   if (!(temp_buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
      {
      // close the file
      _lclose(file_handle);

      // return error
      return(0);
      } // end if
   
   // allocate final 16 bit storage buffer
   if (!(bitmap->buffer=(UCHAR *)malloc(2*bitmap->bitmapinfoheader.biWidth*bitmap->bitmapinfoheader.biHeight)))
      {
      // close the file
      _lclose(file_handle);

      // release working buffer
      free(temp_buffer);

      // return error
      return(0);
      } // end if

   // now read it in
   _lread(file_handle,temp_buffer,bitmap->bitmapinfoheader.biSizeImage);

   // now convert each 24 bit RGB value into a 16 bit value
   for (index=0; index<bitmap->bitmapinfoheader.biWidth*bitmap->bitmapinfoheader.biHeight; index++)
       {
       // extract RGB components (in BGR order), note the scaling
       UCHAR blue  = (temp_buffer[index*3 + 0] >> 3),
             green = (temp_buffer[index*3 + 1] >> 3),
             red   = (temp_buffer[index*3 + 2] >> 3);

       // build up 16 bit color word
       USHORT color = _RGB16BIT(red,green,blue);

       // write color to buffer
       ((USHORT *)bitmap->buffer)[index] = color;

       } // end for index

   // finally write out the correct number of bits
   bitmap->bitmapinfoheader.biBitCount=16;

   } // end if

#if 0
// write the file info out 
printf("\nfilename:%s \nsize=%d \nwidth=%d \nheight=%d \nbitsperpixel=%d \ncolors=%d \nimpcolors=%d",
        filename,
        bitmap->bitmapinfoheader.biSizeImage,
        bitmap->bitmapinfoheader.biWidth,
        bitmap->bitmapinfoheader.biHeight,
		bitmap->bitmapinfoheader.biBitCount,
        bitmap->bitmapinfoheader.biClrUsed,
        bitmap->bitmapinfoheader.biClrImportant);
#endif

// close the file
_lclose(file_handle);

// flip the bitmap
Flip_Bitmap(bitmap->buffer, 
            bitmap->bitmapinfoheader.biWidth*(bitmap->bitmapinfoheader.biBitCount/8), 
            bitmap->bitmapinfoheader.biHeight);

// return success
return(1);

} // end Load_Bitmap_File

///////////////////////////////////////////////////////////

int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap)
{
// this function releases all memory associated with "bitmap"
if (bitmap->buffer)
   {
   // release memory
   free(bitmap->buffer);

   // reset pointer
   bitmap->buffer = NULL;

   } // end if

// return success
return(1);

} // end Unload_Bitmap_File

///////////////////////////////////////////////////////////

int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height)
{
// this function is used to flip upside down .BMP images

UCHAR *buffer; // used to perform the image processing
int index;     // looping index

// allocate the temporary buffer
if (!(buffer = (UCHAR *)malloc(bytes_per_line*height)))
   return(0);

// copy image to work area
memcpy(buffer,image,bytes_per_line*height);

// flip vertically
for (index=0; index < height; index++)
    memcpy(&image[((height-1) - index)*bytes_per_line],
           &buffer[index*bytes_per_line], bytes_per_line);

// release the memory
free(buffer);

// return success
return(1);

} // end Flip_Bitmap

///////////////////////////////////////////////////////////

