#include <3ds.h>

u8 color_r[] = 
	{
		255, 0, 0, 128, 200, 0, 255, 200, 128, 110, 255, 100, 235, 220, 160, 235, 0
	};
u8 color_g[] = 
	{
		255, 0, 0, 128, 255, 32, 0, 200, 128, 140, 255, 100, 152, 172, 215, 235, 255
	};
u8 color_b[] = 
	{
		255, 0, 255, 128, 200, 0, 0, 200, 128, 110, 0, 255, 52, 52, 160, 235, 0
	};

u8 RGBAToRed(u32 c)
{
        u32 rgb = c;
        
        int red = (rgb >> 24) & 0xFF;

        return red;
}

u8 RGBAToGreen(u32 c)
{
        u32 rgb = c;
        
        int green = (rgb >> 16) & 0xFF;

        return green;
}

u8 RGBAToBlue(u32 c)
{
        u32 rgb = c;
		
        int blue = (rgb >> 8) & 0xFF;

        return blue;
}

float RGBAToAlpha(u32 c)
{
        u32 rgb = c;
		
        int alpha = rgb & 0xFF;

        return ((float)alpha)/256;
}

u32 ColorsToRGBA (u8 r, u8 g, u8 b, float a)
{
	// 0x00000000 00000000000000000000000000000000
	// 0x000000FF 00000000000000000000000011111111
	// << 8       00000000000000001111111100000000
	//
	u32 rgba = 0;
	u8 t=r;
	//printf ("%X %X %X ", r, g, b);

	rgba = 0;
	rgba = rgba | (((u32)r) << 24);
	rgba = rgba | (((u32)g) << 16);
	rgba = rgba | (((u32)b) << 8);
	rgba = rgba | 255; (u32)(a*255);
	
	//printf("%X\n", rgba);
	
	return rgba;
}

u32 RGBAToRGB(u32 RGB_background, u32 RGBA_color)
{
    float alpha = RGBAToAlpha(RGBA_color);

    return ColorsToRGBA (
        (u8)((1 - alpha) * (float)RGBAToRed(RGB_background) + alpha * (float)RGBAToRed(RGBA_color)),
        (u8)((1 - alpha) * (float)RGBAToGreen(RGB_background) + alpha * (float)RGBAToGreen(RGBA_color)),
        (u8)((1 - alpha) * (float)RGBAToBlue(RGB_background) + alpha * (float)RGBAToBlue(RGBA_color)),
		1.0
    );
}

u32 GetColor(int idx)
{
	   return ColorsToRGBA (
        color_r[idx],
        color_g[idx],
        color_b[idx],
		1.0
    );
}