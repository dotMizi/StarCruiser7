#ifndef _RENDER_H_
#define _RENDER_H_ 

#define FOCAL_DISTANCE 200
#define STAR_WIDTH 2
#define DEBRIS_WIDTH 4
#define LRS_WIDTH 3


#define WIDTH_TOP 400
#define WIDTH_BOTTOM 320
#define HEIGHT 240
#define ARRAY_LENGTH (u32)WIDTH_TOP*HEIGHT

#define CONFIG_3D_SLIDERSTATE (*(float*)0x1FF81080) //thanks shinyquagsire23
#define MAX_STEREO_SEPARATION 42
#define STEREO_SEPARATION (MAX_STEREO_SEPARATION * CONFIG_3D_SLIDERSTATE)

extern int focal_distance;
extern int menu_item_selected;
extern bool shield_active;
#endif