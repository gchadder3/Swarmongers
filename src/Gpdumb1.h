// GPDUMB1.H - Header file for GPDUMB1.CPP game engine library

// watch for multiple inclusions
#ifndef GPDUMB1
#define GPDUMB1

// DEFINES ////////////////////////////////////////////////

// default screen size
//#define SCREEN_WIDTH    640  // size of screen
//#define SCREEN_HEIGHT   480
#define SCREEN_WIDTH    1024  // size of screen
#define SCREEN_HEIGHT   768
#define SCREEN_BPP      8    // bits per pixel

// bitmap defines
#define BITMAP_ID            0x4D42 // universal id for a bitmap
#define BITMAP_STATE_DEAD    0
#define BITMAP_STATE_ALIVE   1
#define BITMAP_STATE_DYING   2 
#define BITMAP_ATTR_LOADED   128

#define BITMAP_EXTRACT_MODE_CELL  0
#define BITMAP_EXTRACT_MODE_ABS   1

// defines for BOBs
#define BOB_STATE_DEAD         0    // this is a dead bob
#define BOB_STATE_ALIVE        1    // this is a live bob
#define BOB_STATE_DYING        2    // this bob is dying
#define BOB_STATE_ANIM_DONE    1    // done animation state
#define MAX_BOB_FRAMES         64   // maximum number of bob frames
#define MAX_BOB_ANIMATIONS     16   // maximum number of animation sequeces

#define BOB_ATTR_SINGLE_FRAME   1   // bob has single frame
#define BOB_ATTR_MULTI_FRAME    2   // bob has multiple frames
#define BOB_ATTR_MULTI_ANIM     4   // bob has multiple animations
#define BOB_ATTR_ANIM_ONE_SHOT  8   // bob will perform the animation once
#define BOB_ATTR_VISIBLE        16  // bob is visible
#define BOB_ATTR_BOUNCE         32  // bob bounces off edges
#define BOB_ATTR_WRAPAROUND     64  // bob wraps around edges
#define BOB_ATTR_LOADED         128 // the bob has been loaded

// MACROS /////////////////////////////////////////////////

// these read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// this builds a 16 bit color value
#define _RGB16BIT(r,g,b) ((b%32) + ((g%32) << 5) + ((r%32) << 10))

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

// initializes a direct draw struct
#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

// TYPES //////////////////////////////////////////////////

// basic unsigned types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// container structure for bitmaps .BMP file
typedef struct BITMAP_FILE_TAG
        {
        BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
        BITMAPINFOHEADER bitmapinfoheader;  // this is all the info including the palette
        PALETTEENTRY     palette[256];      // we will store the palette here
        UCHAR            *buffer;           // this is a pointer to the data

        } BITMAP_FILE, *BITMAP_FILE_PTR;

// the blitter object structure BOB
typedef struct BOB_TYP
        {
        int state;          // the state of the object (general)
        int anim_state;     // an animation state variable, up to you
        int attr;           // attributes pertaining to the object (general)
        int x,y;            // position bitmap will be displayed at
        int xv,yv;          // velocity of object
        int width, height;  // the width and height of the bob
        int width_fill;     // internal, used to force 8*x wide surfaces
        int counter_1;      // general counters
        int counter_2;
        int max_count_1;    // general threshold values;
        int max_count_2;
        int varsI[16];      // stack of 16 integers
        float varsF[16];    // stack of 16 floats
        int curr_frame;     // current animation frame
        int num_frames;     // total number of animation frames
        int curr_animation; // index of current animation
        int anim_counter;   // used to time animation transitions
        int anim_index;     // animation element index
        int anim_count_max; // number of cycles before animation
        int *animations[MAX_BOB_ANIMATIONS]; // animation sequences

        LPDIRECTDRAWSURFACE images[MAX_BOB_FRAMES]; // the bitmap images DD surfaces
 
        } BOB, *BOB_PTR;

// the simple bitmap image
typedef struct BITMAP_IMAGE_TYP
        {
        int state;          // state of bitmap
        int attr;           // attributes of bitmap
        int x,y;            // position of bitmap
        int width, height;  // size of bitmap
        int num_bytes;      // total bytes of bitmap
        UCHAR *buffer;      // pixels of bitmap

        } BITMAP_IMAGE, *BITMAP_IMAGE_PTR;

// PROTOTYPES /////////////////////////////////////////////

// DirectDraw functions
int DD_Init(int width, int height, int bpp);
int DD_Shutdown(void);
LPDIRECTDRAWCLIPPER DD_Attach_Clipper(LPDIRECTDRAWSURFACE lpdds, int num_rects, LPRECT clip_list);
LPDIRECTDRAWSURFACE DD_Create_Surface(int width, int height, int mem_flags);
int DD_Flip(void);
int DD_Wait_For_Vsync(void);
int DD_Fill_Surface(LPDIRECTDRAWSURFACE lpdds,int color);
UCHAR *DD_Lock_Surface(LPDIRECTDRAWSURFACE lpdds,int *lpitch);
int DD_Unlock_Surface(LPDIRECTDRAWSURFACE lpdds, UCHAR *surface_buffer);
UCHAR *DD_Lock_Primary_Surface(void);
int DD_Unlock_Primary_Surface(void);
UCHAR *DD_Lock_Back_Surface(void);
int DD_Unlock_Back_Surface(void);

// BOB functions
int Create_BOB(BOB_PTR bob,int x, int y,int width, int height,int num_frames,int attr,int mem_flags);              
int Destroy_BOB(BOB_PTR bob);
int Draw_BOB(BOB_PTR bob, LPDIRECTDRAWSURFACE dest);
int Draw_Scaled_BOB(BOB_PTR bob, int swidth, int sheight,LPDIRECTDRAWSURFACE dest);
int Load_Frame_BOB(BOB_PTR bob, BITMAP_FILE_PTR bitmap, int frame, int cx,int cy,int mode);                   // cx,cy are absolute coords
int Animate_BOB(BOB_PTR bob);
int Scroll_BOB(void); // ni
int Move_BOB(BOB_PTR bob);
int Load_Animation_BOB(BOB_PTR bob, int anim_index, int num_frames, int *sequence);
int Set_Pos_BOB(BOB_PTR bob, int x, int y);
int Set_Vel_BOB(BOB_PTR bob,int xv, int yv);
int Set_Anim_Speed_BOB(BOB_PTR bob,int speed);
int Set_Animation_BOB(BOB_PTR bob, int anim_index);
int Hide_BOB(BOB_PTR bob);
int Show_BOB(BOB_PTR bob);
int Collision_BOBS(BOB_PTR bob1, BOB_PTR bob2);

// general utility functions
DWORD Get_Clock(void);
DWORD Start_Clock(void);
DWORD Wait_Clock(DWORD count);

// graphics functions
int Draw_Clip_Line(int x0,int y0, int x1, int y1,UCHAR color,UCHAR *dest_buffer, int lpitch);
int Clip_Line(int &x1,int &y1,int &x2, int &y2);
int Draw_Line(int xo, int yo, int x1,int y1, UCHAR color,UCHAR *vb_start,int lpitch);
int Draw_Pixel(int x, int y,int color,UCHAR *video_buffer, int lpitch);
int Draw_Rectangle(int x1, int y1, int x2, int y2, int color,LPDIRECTDRAWSURFACE lpdds);
int Screen_Transition(void); // ni

// palette functions
int Set_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Get_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Load_Palette_From_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette_To_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette(LPPALETTEENTRY sav_palette);
int Set_Palette(LPPALETTEENTRY set_palette);
int Rotate_Colors(int start_index, int colors);
int Blink_Colors(void); // ni

// simple bitmap image functions
int Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height);
int Destroy_Bitmap(BITMAP_IMAGE_PTR image);
int Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap,UCHAR *dest_buffer, int lpitch, int transparent);
int Load_Image_Bitmap(BITMAP_IMAGE_PTR image,BITMAP_FILE_PTR bitmap,int cx,int cy,int mode);               
int Scroll_Bitmap(void); // ni
int Copy_Bitmap(void); // ni
int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);

// bitmap file functions
int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename);
int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);

// gdi functions
int Draw_Text_GDI(char *text, int x,int y,COLORREF color, LPDIRECTDRAWSURFACE lpdds);
int Draw_Text_GDI(char *text, int x,int y,int color, LPDIRECTDRAWSURFACE lpdds);

// error functions
int Open_Error_File(char *filename);
int Close_Error_File(void);
int Write_Error(char *string, ...);

// GLOBALS ////////////////////////////////////////////////

extern FILE *fp_error;                           // general error file
extern LPDIRECTDRAW         lpdd;                 // dd object
extern LPDIRECTDRAWSURFACE  lpddsprimary;         // dd primary surface
extern LPDIRECTDRAWSURFACE  lpddsback;            // dd back surface
extern LPDIRECTDRAWPALETTE  lpddpal;              // a pointer to the created dd palette
extern LPDIRECTDRAWCLIPPER  lpddclipper;          // dd clipper
extern PALETTEENTRY         palette[256];         // color palette
extern PALETTEENTRY         save_palette[256];    // used to save palettes
extern DDSURFACEDESC        ddsd;                 // a direct draw surface description struct
extern DDBLTFX              ddbltfx;              // used to fill
extern DDSCAPS              ddscaps;              // a direct draw surface capabilities struct
extern HRESULT              ddrval;               // result back from dd calls
extern UCHAR                *primary_buffer;      // primary video buffer
extern UCHAR                *back_buffer;         // secondary back buffer
extern int                  primary_lpitch;       // memory line pitch
extern int                  back_lpitch;          // memory line pitch
extern BITMAP_FILE          bitmap16bit;          // a 16 bit bitmap file
extern BITMAP_FILE          bitmap8bit;           // a 8 bit bitmap file

extern DWORD                start_clock_count;    // used for timing

// these defined the general clipping rectangle
extern int min_clip_x,                             // clipping rectangle 
           max_clip_x,                  
           min_clip_y,     
           max_clip_y;                  

// these are overwritten globally by DD_Init()
extern int screen_width,                            // width of screen
           screen_height,                           // height of screen
           screen_bpp;                              // bits per pixel 

#endif


