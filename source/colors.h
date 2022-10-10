#define C_WHITE 0
#define C_BLACK 1
#define C_COCKPIT_BLUE 2
#define C_COMPUTER 3
#define C_COMPUTER_SHIELD 4
#define C_SPACE_SHIELD 5
#define C_SPACE 1
#define C_STARS_SHIELD 4
#define C_STARS 0
#define C_RED 6
#define C_GRAY 7
#define C_ASTEROID 8
#define C_ASTEROID_SHIELD 9
#define C_YELLOW 10
#define C_LIGHTBLUE 11
#define C_STARBASE_ORANGE 12
#define C_STARBASE_ORANGE_SHIELD 13
#define C_COMPUTER_LIGHT_SHIELD 14
#define C_COMPUTER_LIGHT 15
#define C_GREEN 16

extern u8 color_r[];
extern u8 color_g[];
extern u8 color_b[];

extern u8 RGBAToRed(u32 c);
extern u8 RGBAToGreen(u32 c);
extern u8 RGBAToBlue(u32 c);
extern float RGBAToAlpha(u32 c);
extern u32 ColorsToRGBA(u8 r, u8 g, u8 b, float a);
extern u32 RGBAToRGB(u32 RGB_background, u32 RGBA_color);
extern u32 GetColor(int idx);
