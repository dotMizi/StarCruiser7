#include <3ds.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#include "vector.h"
#include "3dmodels.h"
#include "render.h"
#include "game.h"
#include "3drenderer.h"
#include "chars.h"
#include "colors.h"
#include "sound.h"

/*
* Render the screens
* Draw stars and debris, HUD of computer, computer displays
*
*
*
*
*/


int focal_distance;
int menu_item_selected = 0;
int tx, ty;
int target_marker=-1;
float theta;
float phi;
float rho = INFINITY;
bool shield_active= false;


void draw_menu (char* items[], int num)
{
	int height;
	int width;
	int i;
	int num_chars=0;

	u8* fbAdr = gfxGetFramebuffer(GFX_BOTTOM,GFX_LEFT,NULL,NULL);
	
	//draw_block(fbAdr, 0,0,WIDTH_BOTTOM, HEIGHT, C_BLACK, WIDTH_BOTTOM);
	
	for (i=0; i<num; i++)
		num_chars = (strlen(items[i]) > num_chars)?strlen(items[i]):num_chars;
	
	width = (num_chars+2)*18 + 20;
	height = num*18 +20;
	
	if ((width < WIDTH_BOTTOM)&&(height < HEIGHT))
	{
		int x,y;
		x = (WIDTH_BOTTOM - width)/2;
		y = (HEIGHT - height)/2;
		
		draw_block(fbAdr, x,y,width, height, C_COCKPIT_BLUE, WIDTH_BOTTOM);
		draw_box(fbAdr,x,y,width,height,2,C_YELLOW,WIDTH_BOTTOM); 

		for (i=0; i<num; i++)
		{
			draw_text_XXL (items[i], fbAdr, x+38, y+height - (i+1)*20, C_YELLOW, WIDTH_BOTTOM);
			if(i == menu_item_selected)
				draw_text_XXL ("l", fbAdr, x+10, y+height - (i+1)*20, C_YELLOW, WIDTH_BOTTOM);
		}
	}

}

int damage_color(enum subsystem_states state)
{
	switch (state)
	{
		case WORKING:
			return C_GREEN;
		case DAMAGED:
			return C_YELLOW;
		case DESTROYED:
			return C_RED;
		default:
			return C_BLACK;
	}
	return C_WHITE;
}

void draw_subsystem_state (u8* fbAdr, int x, int y, bool size)
{
	int color = C_WHITE;
	int charwidth = size?16:8;
	char text[81];
	int textposx = x; 
	
	sprintf(text, size?"DAMAGE CTRL: ":"DC: ");
	draw_text_SL (text, fbAdr, textposx, y, color, WIDTH_BOTTOM, size);
	textposx += charwidth * strlen(text);
	
	sprintf(text, "P");
	color = damage_color(get_photon_state());
	draw_text_SL (text, fbAdr, textposx, y, color, WIDTH_BOTTOM, size);
	textposx += charwidth * 1;
	
	sprintf(text, "E"); 
	color = damage_color(get_engines_state());
	draw_text_SL (text, fbAdr, textposx, y, color, WIDTH_BOTTOM, size);
	textposx += charwidth * 1;
	
	sprintf(text, "S"); 
	color = damage_color(get_shield_state());
	draw_text_SL (text, fbAdr, textposx, y, color, WIDTH_BOTTOM, size);
	textposx += charwidth * 1;
	
	sprintf(text, "C"); 
	color = damage_color(get_computer_state());
	draw_text_SL (text, fbAdr, textposx, y, color, WIDTH_BOTTOM, size);
	textposx += charwidth * 1;
	
	sprintf(text, "L"); 
	color = damage_color(get_sector_scan_state());
	draw_text_SL (text, fbAdr, textposx, y, color, WIDTH_BOTTOM, size);
	textposx += charwidth * 1;
	
	sprintf(text, "R"); 
	color = damage_color(get_subspace_radio_state());
	draw_text_SL (text, fbAdr, textposx, y, color, WIDTH_BOTTOM, size);
}

void draw_bottom_screen()
{
	shield_active = shield && shield_avail();
	u8* fbAdr = gfxGetFramebuffer(GFX_BOTTOM,GFX_LEFT,NULL,NULL);
	char text[81];
	char text1[81];
	char text2[81];
	char text3[81];
	int game_time=0;
	int c,i,j,x,y;
	
	game_time = t_act.tv_sec - t_game.tv_sec;
	if (target_marker >= 0)
	{
		rho = VectorLength(render_object[target_marker].mesh.Position);
		phi = asin (render_object[target_marker].mesh.Position.Y/rho)*2/M_PI*90;
		theta = atan2(render_object[target_marker].mesh.Position.X, render_object[target_marker].mesh.Position.Z)/M_PI*180;
	}	
	
	switch(bottomscreen_state) {
		case NO_SCREEN:
			draw_block(fbAdr, 0,0,WIDTH_BOTTOM, HEIGHT, C_BLACK, WIDTH_BOTTOM);
			break;
		case COCKPIT:
			for (x=0; x<WIDTH_BOTTOM; x++)
				for(y=0; y<HEIGHT; y++)
				{
					fbAdr[((x*HEIGHT)+y)*3+0] = color_b[C_COCKPIT_BLUE];
					fbAdr[((x*HEIGHT)+y)*3+1] = color_g[C_COCKPIT_BLUE];
					fbAdr[((x*HEIGHT)+y)*3+2] = color_r[C_COCKPIT_BLUE];
				}
			sprintf(text, "MISSION TIME: %02d.%02d", game_time/60, game_time%60);
			draw_text_XXL (text, fbAdr, 10, 30, C_WHITE, WIDTH_BOTTOM);
			
			if ((target_marker >= 0)&&((computer)&&(computer_avail()))) 
			{	
				sprintf(text, "a:%+03d b:%+02d", (int)theta, (int)phi);
				draw_text_XXL (text, fbAdr, (WIDTH_BOTTOM - strlen(text)*16)/2, 100, C_WHITE, WIDTH_BOTTOM);
				if (rho < 9999)
				{
					sprintf(text, "R:%+d", ((int)rho)*((render_object[target_marker].mesh.Position.Z >= 0)?1:(-1)));
				} else {
					sprintf(text, "R:+g");
				}
				draw_text_XXL (text, fbAdr, (WIDTH_BOTTOM - strlen(text)*16)/2, 65, C_WHITE, WIDTH_BOTTOM);
			} 
			
			draw_subsystem_state (fbAdr, 10, 135, true);
			
			sprintf(text, "ENERGY:%04d UNITS", (int)energy);
			draw_text_XXL (text, fbAdr, 10, 170, toggle_energy_color?C_RED:C_WHITE, WIDTH_BOTTOM);
			if (warp_state == NOT_ENGAGED)
			{
				sprintf(text, "VEL:%02d METRONS/S", velocity[speed]);
			} else {
				if (warp_speed < 100)
				{
					sprintf(text, "VEL:%02d METRONS/S", (int)warp_speed);
				} else {
					sprintf(text, "VEL:g METRONS/S");

				}
			}
			draw_text_XXL (text, fbAdr, 10, 205, C_WHITE, WIDTH_BOTTOM);
				
			break;
		case LONG_RANGE:
			draw_block(fbAdr, 0, 0, WIDTH_BOTTOM, HEIGHT, C_BLACK, WIDTH_BOTTOM);
			draw_text_XXL ("LONG RANGE SCAN", fbAdr, (WIDTH_BOTTOM - 15*16)/2, HEIGHT - 21, C_WHITE, WIDTH_BOTTOM);
			
			for (c=0; c < MAX_NUM_OF_ENEMIES; c++)
			{
				if ((render_object[c].active)&&((render_object[c].objecttype != PHOTONT)&&(render_object[c].objecttype != ENEMYPHOTONT)))
				{
					x=render_object[c].mesh.Position.X/50 + WIDTH_BOTTOM/2;
					y=render_object[c].mesh.Position.Z/50 + HEIGHT/2;
					
					if ((x<WIDTH_BOTTOM-LRS_WIDTH) && (x>0) && (y>0) && (y<HEIGHT-LRS_WIDTH))
					{
						for (i=0;i<LRS_WIDTH;i++)
							for (j=0;j<LRS_WIDTH;j++)
							{
								fbAdr[(((x+i)*HEIGHT)+y+j)*3+0] = color_b[sector_scan_avail()?C_LIGHTBLUE:(rand()%16)];
								fbAdr[(((x+i)*HEIGHT)+y+j)*3+1] = color_g[sector_scan_avail()?C_LIGHTBLUE:(rand()%16)];
								fbAdr[(((x+i)*HEIGHT)+y+j)*3+2] = color_r[sector_scan_avail()?C_LIGHTBLUE:(rand()%16)];
							}
					}
					
					if(!sector_scan_avail())
					{
						x=(x-WIDTH_BOTTOM/2)*(-1)+WIDTH_BOTTOM/2;
						y=(y-HEIGHT/2)*(-1)+HEIGHT/2;
					
						if ((x<WIDTH_BOTTOM-LRS_WIDTH) && (x>0) && (y>0) && (y<HEIGHT-LRS_WIDTH))
						{
							for (i=0;i<LRS_WIDTH;i++)
								for (j=0;j<LRS_WIDTH;j++)
								{
									fbAdr[(((x+i)*HEIGHT)+y+j)*3+0] = color_b[rand()%16];
									fbAdr[(((x+i)*HEIGHT)+y+j)*3+1] = color_g[rand()%16];
									fbAdr[(((x+i)*HEIGHT)+y+j)*3+2] = color_r[rand()%16];
								}
						}
					}
				}
			}

			draw_text ("m", fbAdr, (WIDTH_BOTTOM - 8)/2, (HEIGHT - 8)/2, C_WHITE, WIDTH_BOTTOM);
			break;
		case GALACTIC_MAP:
			draw_block(fbAdr, 0, 0, WIDTH_BOTTOM, HEIGHT - 25, C_COCKPIT_BLUE, WIDTH_BOTTOM);
			draw_block(fbAdr, 0, HEIGHT - 25, WIDTH_BOTTOM, HEIGHT, C_BLACK, WIDTH_BOTTOM);
			draw_text_XXL ("GALACTIC MAP", fbAdr, (WIDTH_BOTTOM - 192)/2, HEIGHT - 21, C_WHITE, WIDTH_BOTTOM);
			
			draw_box(
				fbAdr, 
				7, 
				25+18, 
				17*18-2, 
				19*8,
				2,
				C_WHITE, 
				WIDTH_TOP);

			for (x=1; x < GMAP_MAX_X; x++)	// set vertical grid lines
				draw_block(fbAdr, 7+x*19, 25+18, 2, 19*8+2, C_WHITE, WIDTH_BOTTOM);
			for (y=1; y < GMAP_MAX_Y; y++)	// set horizontal grid lines
				draw_block(fbAdr, 7, 25+18+y*19, 17*18, 2, C_WHITE, WIDTH_BOTTOM);

			// mark cruiser sector
			if ((hyperwarp_target_sector_x >= 0) && (hyperwarp_target_sector_y >= 0) && (hyperwarp_target_sector_x < GMAP_MAX_X) && (hyperwarp_target_sector_y < GMAP_MAX_Y))
			{
				if (blink_on)
					draw_block(fbAdr, 9+hyperwarp_target_sector_x*19, 25+20+hyperwarp_target_sector_y*19, 17, 17, C_STARBASE_ORANGE_SHIELD, WIDTH_BOTTOM);
			}
			draw_block(fbAdr, 9+cruiser_sector_x*19, 25+20+cruiser_sector_y*19, 17, 17, C_STARBASE_ORANGE_SHIELD, WIDTH_BOTTOM);
			hyperwarp_energy = hyperwarp_costs[
				(int)((
					sqrt(
						pow(abs(cruiser_sector_x-hyperwarp_target_sector_x), 2)+
						pow(abs(cruiser_sector_y-hyperwarp_target_sector_y), 2)
					)
				) / 18 *23)
			];
						
			sprintf(text, "MISSION TIME: %02d.%02d", game_time/60, game_time%60);
			draw_text (text, fbAdr, 10, 6, C_WHITE, WIDTH_BOTTOM);
			sprintf(text, "TARGETS: %d", gmap[hyperwarp_target_sector_x][hyperwarp_target_sector_y].layerB<5?gmap[hyperwarp_target_sector_x][hyperwarp_target_sector_y].layerB:0);
			draw_text (text, fbAdr, WIDTH_BOTTOM/2+50, 6, C_YELLOW, WIDTH_BOTTOM);
			sprintf(text, "WARP ENERGY: %04d UNITS", (int)hyperwarp_energy);
			draw_text (text, fbAdr, 10, 16, C_COMPUTER_SHIELD, WIDTH_BOTTOM);
			draw_subsystem_state (fbAdr, WIDTH_BOTTOM/2+50, 16, false);
			
			for (x=0; x < 16; x++)
				for (y=0; y<8; y++)
				{
					char *gm_icon;
					switch (gmap[x][y].layerB) {
						case 5:
							gm_icon = "c";
							c = C_STARBASE_ORANGE;
							break;
						case 4:
							gm_icon = "d";
							c = C_COMPUTER_SHIELD;
							break;
						case 3:
							gm_icon = "e";
							c = C_COMPUTER_SHIELD;
							break;
						case 2:
						case 1:
							gm_icon = "f";
							c = C_COMPUTER_SHIELD;
							break;
						default:
							gm_icon = " ";
							break;						
					}
					draw_text_XXL (gm_icon, fbAdr, 9+x*19, 25+20+y*19, c, WIDTH_BOTTOM);
				}
			break;
		case DEBUG_SCREEN:
			
			draw_block(fbAdr, 0, 0, WIDTH_BOTTOM, HEIGHT, C_BLACK, WIDTH_BOTTOM);
			draw_text_XXL ("DEBUG", fbAdr, (WIDTH_BOTTOM - 5*16)/2, HEIGHT - 21, C_WHITE, WIDTH_BOTTOM);
			int sb = starbase_surrounded();
			if (sb > 0)
			{
				x = sb/GMAP_MAX_X;
				y = sb%GMAP_MAX_X;
				sprintf(text2, "STARBASE SURROUNDED AT %d (%d %d)",sb, x, y);
				draw_text (text2, fbAdr, 15, 130, C_WHITE, WIDTH_BOTTOM);
				sprintf(text2, "TIME TO DESTRUCTION %d",countdown());
				draw_text (text2, fbAdr, 15, 115, C_WHITE, WIDTH_BOTTOM);
				
				sprintf(text2, "          (%d,%d):%d", x,y+1,gmap[x][y+1].layerA);
				draw_text (text2, fbAdr, 15, 100, C_WHITE, WIDTH_BOTTOM);
				sprintf(text2, "(%d,%d):%d          (%d,%d):%d", x-1,y,gmap[x-1][y].layerA, x+1,y,gmap[x+1][y].layerA);
				draw_text (text2, fbAdr, 15, 85, C_WHITE, WIDTH_BOTTOM);
				sprintf(text2, "          (%d,%d):%d", x,y-1,gmap[x][y-1].layerA);
				draw_text (text2, fbAdr, 15, 70, C_WHITE, WIDTH_BOTTOM);

			}
			break;
		default:
			bottomscreen_state = NO_SCREEN;
			break;
	}
}

void draw_top_screen()
{
    int x,y;
	float target_dist = INFINITY;
	shield_active = shield && shield_avail();
	
	// get Framebuffer addresses
	u8* fbAdr_L = gfxGetFramebuffer(GFX_TOP,GFX_LEFT,NULL,NULL);
    u8* fbAdr_R = gfxGetFramebuffer(GFX_TOP,GFX_RIGHT,NULL,NULL);
	
	checkSoundFX();

	// clear screen
	for (x=0; x<WIDTH_TOP; x++)
		for(y=0; y<HEIGHT; y++)
		{
					fbAdr_L[((x*HEIGHT)+y)*3+0] = color_b[shield_active?C_SPACE_SHIELD:C_SPACE];
					fbAdr_L[((x*HEIGHT)+y)*3+1] = color_g[shield_active?C_SPACE_SHIELD:C_SPACE];
					fbAdr_L[((x*HEIGHT)+y)*3+2] = color_r[shield_active?C_SPACE_SHIELD:C_SPACE];
					fbAdr_R[((x*HEIGHT)+y)*3+0] = color_b[shield_active?C_SPACE_SHIELD:C_SPACE];
					fbAdr_R[((x*HEIGHT)+y)*3+1] = color_g[shield_active?C_SPACE_SHIELD:C_SPACE];
					fbAdr_R[((x*HEIGHT)+y)*3+2] = color_r[shield_active?C_SPACE_SHIELD:C_SPACE];
		}
		
	// drawing mode normal flight
    if (warp_state != IN_HYPERSPACE) 
	{
		if (warp_state == ENGAGED|ABORTED) 
		{
			if (warp_speed > 65)
			{
				draw_trails(fbAdr_L, fbAdr_R);
			} else draw_stars(fbAdr_L, fbAdr_R);
		}
		if (gamestate == GAME_RUNNING)
		{
			int i;
			target_marker = -1;
			setFrameBuffer(fbAdr_L, fbAdr_R);
			for (i = 0; i < MAX_NUM_OF_ENEMIES; i++)
				if (render_object[i].active) 
				{
					if ((render_object[i].objecttype == TIE) ||
					(render_object[i].objecttype == RAIDER) ||
					(render_object[i].objecttype == ZYLONBASE) ||
					(render_object[i].objecttype == STARBASE))
					{
						if(VectorLength(render_object[i].mesh.Position) < target_dist) 
						{
							target_marker = i;
							target_dist = VectorLength(render_object[i].mesh.Position);
						}
					}

					if(VectorLength(render_object[i].mesh.Position) > 20)
					{
						if (warp_speed <= 65) Render(render_object[i].mesh, front_view, render_object[i].objecttype);
					}
				}
		}		
		draw_debris(fbAdr_L, fbAdr_R);
	}
	
	// draw hyperwarp marker when hyperwarp is engaged 
	if (warp_state == ENGAGED)
	{
		draw_hyperwarp_marker(fbAdr_L, fbAdr_R);
	}
	
	// draw the splash screen
	if ((gamestate == GAME_SELECT)||(gamestate == GAME_SPLASH))
	{
		int posY = HEIGHT -80;
		draw_text_XXL (message_text[19], fbAdr_L, (WIDTH_TOP-strlen(message_text[19])*16)/2, posY, C_WHITE, WIDTH_TOP);
		draw_text_XXL (message_text[19], fbAdr_R, (WIDTH_TOP-strlen(message_text[19])*16)/2, posY, C_WHITE, WIDTH_TOP);
		posY-=30;
		draw_text_XXL (message_text[5], fbAdr_L, (WIDTH_TOP-strlen(message_text[5])*16)/2, posY, C_WHITE, WIDTH_TOP);
		draw_text_XXL (message_text[5], fbAdr_R, (WIDTH_TOP-strlen(message_text[5])*16)/2, posY, C_WHITE, WIDTH_TOP);
		posY-=30;
		draw_text_XXL (message_text[6], fbAdr_L, (WIDTH_TOP-strlen(message_text[6])*16)/2, posY, C_WHITE, WIDTH_TOP);
		draw_text_XXL (message_text[6], fbAdr_R, (WIDTH_TOP-strlen(message_text[6])*16)/2, posY, C_WHITE, WIDTH_TOP);
		setFrameBuffer(fbAdr_L, fbAdr_R);
	}
	
	//draw game over screen
	if ((gamestate == GAME_END) && game_over_display_on())
	{
		int posY = HEIGHT -90;
		char message[81];
		setSoundFXPrefs(ENGINES, 0.0, 0);
		if (aborted == MISSION_COMPLETED)
		{ // CONGRATULATIONS
			if (gd_sound) 
			{
				setSoundFX(CONGRATULATIONS);
				gd_sound = false;
			}
			draw_text_XXL (message_text[29], fbAdr_L, (WIDTH_TOP-strlen(message_text[29])*16)/2, posY, C_WHITE, WIDTH_TOP); 
			draw_text_XXL (message_text[29], fbAdr_R, (WIDTH_TOP-strlen(message_text[29])*16)/2, posY, C_WHITE, WIDTH_TOP);
			posY-=30;
		}
		if ((aborted == ZYLON_FIRE) || (aborted == HIT_BY_ASTEROID))
		{ // POSTHUMOUS RANK IS
			if (gd_sound) 
			{
				setSoundFX(GAME_OVER);
				gd_sound = false;
			}
			draw_text_XXL (message_text[22], fbAdr_L, (WIDTH_TOP-strlen(message_text[22])*16)/2, posY, C_WHITE, WIDTH_TOP);
			draw_text_XXL (message_text[22], fbAdr_R, (WIDTH_TOP-strlen(message_text[22])*16)/2, posY, C_WHITE, WIDTH_TOP);
			posY-=30;
		} else { // NEW RANK IS
			if (gd_sound) 
			{
				setSoundFX(GAME_OVER);
				gd_sound = false;
			}
			draw_text_XXL (message_text[25], fbAdr_L, (WIDTH_TOP-strlen(message_text[25])*16)/2, posY, C_WHITE, WIDTH_TOP);
			draw_text_XXL (message_text[25], fbAdr_R, (WIDTH_TOP-strlen(message_text[25])*16)/2, posY, C_WHITE, WIDTH_TOP);
			posY-=30;
		}
		draw_text_XXL (rank_message[rank], fbAdr_L, (WIDTH_TOP-strlen(rank_message[rank])*16)/2, posY, C_WHITE, WIDTH_TOP);
		draw_text_XXL (rank_message[rank], fbAdr_R, (WIDTH_TOP-strlen(rank_message[rank])*16)/2, posY, C_WHITE, WIDTH_TOP);
		posY-=30;
		sprintf (message, "CLASS %d", class);
		draw_text_XXL (message, fbAdr_L, (WIDTH_TOP-strlen(message)*16)/2, posY, C_WHITE, WIDTH_TOP);
		draw_text_XXL (message, fbAdr_R, (WIDTH_TOP-strlen(message)*16)/2, posY, C_WHITE, WIDTH_TOP);
	}
	
	// draw the message
	if (strlen(act_message) > 0) 
	{
		int strpixellength = strlen(act_message)*16;
		draw_text_XXL (act_message, fbAdr_L, WIDTH_TOP/2-strpixellength/2, HEIGHT -30, C_WHITE, WIDTH_TOP);
		draw_text_XXL (act_message, fbAdr_R, WIDTH_TOP/2-strpixellength/2, HEIGHT -30, C_WHITE, WIDTH_TOP);
	}
	
	// draw attack computer
    if ((computer)&&(computer_avail())) 
	{
		
		int posX = (int)(focal_distance * 2 * render_object[target_marker].mesh.Position.X / render_object[target_marker].mesh.Position.Z * (render_object[target_marker].mesh.Position.Z/abs(render_object[target_marker].mesh.Position.Z)))+WIDTH_TOP/2;	
		int posY = (int)(focal_distance * 2 * render_object[target_marker].mesh.Position.Y / render_object[target_marker].mesh.Position.Z * (render_object[target_marker].mesh.Position.Z/abs(render_object[target_marker].mesh.Position.Z)))+HEIGHT/2;

		draw_computer(fbAdr_L, fbAdr_R, (target_marker >= 0)?true:false, (render_object[target_marker].mesh.Position.Z > 0)?true:false, posX, posY);
	}
}

int get_stereo_separation(int z)
{
	return STEREO_SEPARATION * abs(z) / MAX_DIST;
}

void draw_hyperwarp_marker(u8* fbAdr_L, u8* fbAdr_R)
{
	if (hyperwarp_location.x < 18+get_stereo_separation(170)/2) return;
	if (hyperwarp_location.x > WIDTH_TOP-18-get_stereo_separation(70)/2) return;
	if (hyperwarp_location.y < 18) return;
	if (hyperwarp_location.y > HEIGHT-18) return;
	draw_text_XXL ("j", fbAdr_L, ((int)hyperwarp_location.x)-18-get_stereo_separation(170)/2, ((int)hyperwarp_location.y)-18, C_WHITE, WIDTH_BOTTOM);
	draw_text_XXL ("k", fbAdr_L, ((int)hyperwarp_location.x)+2-get_stereo_separation(170)/2, ((int)hyperwarp_location.y)-18, C_WHITE, WIDTH_BOTTOM);
	draw_text_XXL ("h", fbAdr_L, ((int)hyperwarp_location.x)-18-get_stereo_separation(170)/2, ((int)hyperwarp_location.y)+2, C_WHITE, WIDTH_BOTTOM);
	draw_text_XXL ("i", fbAdr_L, ((int)hyperwarp_location.x)+2-get_stereo_separation(170)/2, ((int)hyperwarp_location.y)+2, C_WHITE, WIDTH_BOTTOM);
	draw_text_XXL ("j", fbAdr_R, ((int)hyperwarp_location.x)-18+get_stereo_separation(170)/2, ((int)hyperwarp_location.y)-18, C_WHITE, WIDTH_BOTTOM);
	draw_text_XXL ("k", fbAdr_R, ((int)hyperwarp_location.x)+2+get_stereo_separation(170)/2, ((int)hyperwarp_location.y)-18, C_WHITE, WIDTH_BOTTOM);
	draw_text_XXL ("h", fbAdr_R, ((int)hyperwarp_location.x)-18+get_stereo_separation(170)/2, ((int)hyperwarp_location.y)+2, C_WHITE, WIDTH_BOTTOM);
	draw_text_XXL ("i", fbAdr_R, ((int)hyperwarp_location.x)+2+get_stereo_separation(170)/2, ((int)hyperwarp_location.y)+2, C_WHITE, WIDTH_BOTTOM);

}

void draw_computer(u8* fbAdr_L, u8* fbAdr_R, bool target, bool lock, int target_x, int target_y)
{
	int x, y;
	int separation = get_stereo_separation(50);
	int vlength = WIDTH_TOP/3; //133
	int vstart = WIDTH_TOP/2-vlength/2; //134
	int vgap = vlength/4;	//33
	int vwidth = 2; //3
	int hlength = HEIGHT/3; //60
	int hstart = HEIGHT/2-hlength/2; //90
	int hgap = hlength/4; //15
	int hwidth = 2;
	int margin = 20;
	int t_start_x = WIDTH_TOP/5*4-margin-separation;
	int t_start_y = margin;
	int t_width = WIDTH_TOP/5;
	int t_height = HEIGHT/5;
	int idx;
	
	// vertical cross hair line
	draw_block(
		fbAdr_L, 
		vstart-separation/2, 
		(HEIGHT/2)-(vwidth/2), 
		(vlength-vgap)/2, 
		vwidth, 
		shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
		WIDTH_TOP);
		
	draw_block(
		fbAdr_R, 
		vstart+separation/2, 
		(HEIGHT/2)-(vwidth/2),
		(vlength-vgap)/2, 
		vwidth, 
		shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
		WIDTH_TOP);
	
	draw_block(
		fbAdr_L, 
		vstart+(vlength-vgap)/2+vgap-separation/2,
		(HEIGHT/2)-(vwidth/2), 
		(vlength-vgap)/2, 
		vwidth, 
		shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
		WIDTH_TOP);
		
	draw_block(
		fbAdr_R, 
		vstart+(vlength-vgap)/2+vgap+separation/2, 
		(HEIGHT/2)-(vwidth/2),
		(vlength-vgap)/2, 
		vwidth, 
		shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
		WIDTH_TOP);
		
	if (front_view) 
	{	
		// horizontal cross hair line
		draw_block(
			fbAdr_L, 
			(WIDTH_TOP/2)-(hwidth/2)-separation/2, 
			hstart, 
			hwidth, 
			(hlength-hgap)/2, 
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
			WIDTH_TOP);
			
		draw_block(
			fbAdr_R, 
			(WIDTH_TOP/2)-(hwidth/2)+separation/2, 
			hstart, 
			hwidth, 
			(hlength-hgap)/2, 
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
			WIDTH_TOP);
			
		draw_block(
			fbAdr_L, 
			(WIDTH_TOP/2)-(hwidth/2) - separation/2, 
			hstart+((hlength-hgap)/2)+hgap, 
			hwidth, 
			(hlength-hgap)/2, 
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
			WIDTH_TOP);
			
		draw_block(
			fbAdr_R, 
			(WIDTH_TOP/2)-(hwidth/2)+separation/2, 
			hstart+((hlength-hgap)/2)+hgap, 
			hwidth, 
			(hlength-hgap)/2, 
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
			WIDTH_TOP);	
		
		// target display
		if (target && (rho <= 9999))
		{
			int x = target_x;
			int y = target_y;
			if (x < 8)
				x = 8;
			if (x > WIDTH_TOP+8)
				x = WIDTH_TOP+8;
			if (y < 8)
				y = 8;
			if (y > HEIGHT+8)
				y = HEIGHT + 8;

			tx = x*(t_width - 16)/WIDTH_TOP+t_start_x;
			ty = y*(t_height - 16)/HEIGHT+t_start_y;
			draw_text_XXL ("n", fbAdr_L, x*(t_width - 16)/WIDTH_TOP+t_start_x-separation/2 -1, y*(t_height-16)/HEIGHT+t_start_y, shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT, WIDTH_TOP);
			draw_text_XXL ("n", fbAdr_R, x*(t_width - 16)/WIDTH_TOP+t_start_x+separation/2 +1, y*(t_height-16)/HEIGHT+t_start_y, shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT, WIDTH_TOP);
				
			if ((rho < 800) && lock && (abs(target_y - HEIGHT/2) < 10))
			{
				draw_block(
					fbAdr_L,
					hwidth + t_start_x-separation/2 -1,
					hwidth*2 + t_start_y + t_height/2,
					t_width/4 - 2*hwidth,
					hwidth*2,
					shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
					WIDTH_TOP);
					
				draw_block(
					fbAdr_R,
					hwidth + t_start_x+separation/2 +1,
					hwidth*2 + t_start_y + t_height/2,
					t_width/4 - 2*hwidth,
					hwidth*2,
					shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
					WIDTH_TOP);
						
				draw_block(
					fbAdr_L,
					2*hwidth + t_start_x + 3*t_width/4-separation/2 -1,
					hwidth*2 + t_start_y + t_height/2,
					t_width/4 - 2*hwidth,
					hwidth*2,
					shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
					WIDTH_TOP);
					
				draw_block(
					fbAdr_R,
					2*hwidth + t_start_x + 3*t_width/4+separation/2 +1,
					hwidth*2 + t_start_y + t_height/2,
					t_width/4 - 2*hwidth,
					hwidth*2,
					shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
					WIDTH_TOP);
						
				if(abs(target_x - WIDTH_TOP/2) < 10)
				{
					draw_block(
						fbAdr_L,
						hwidth + t_start_x-separation/2 -1,
						hwidth*(-3) + t_start_y + t_height/2,
						t_width/4 - 2*hwidth,
						hwidth*2,
						shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
						WIDTH_TOP);
					
					draw_block(
						fbAdr_R,
						hwidth + t_start_x+separation/2 +1,
						hwidth*(-3) + t_start_y + t_height/2,
						t_width/4 - 2*hwidth,
						hwidth*2,
						shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
						WIDTH_TOP);
						
					draw_block(
						fbAdr_L,
						2*hwidth + t_start_x + 3*t_width/4-separation/2 -1,
						hwidth*(-3) + t_start_y + t_height/2,
						t_width/4 - 2*hwidth,
						hwidth*2,
						shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
						WIDTH_TOP);
					
					draw_block(
						fbAdr_R,
						2*hwidth + t_start_x + 3*t_width/4+separation/2 +1,
						hwidth*(-3) + t_start_y + t_height/2,
						t_width/4 - 2*hwidth,
						hwidth*2,
						shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
						WIDTH_TOP);
						
					if (rho < 400)
					{
						draw_block(
							fbAdr_L,
							(-3)*hwidth + t_start_x + t_width/2 - separation/2,
							t_start_y + hwidth,
							vwidth*2,
							t_height/4 - hwidth,
							shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
							WIDTH_TOP);

						draw_block(
							fbAdr_R,
							(-3)*hwidth + t_start_x + separation/2 + t_width/2,
							t_start_y,
							vwidth*2,
							t_height/4 -hwidth,
							shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
							WIDTH_TOP);
							
						draw_block(
							fbAdr_L,
							2*hwidth + t_start_x + t_width/2 - separation/2,
							t_start_y + hwidth,
							vwidth*2,
							t_height/4 - hwidth,
							shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
							WIDTH_TOP);

						draw_block(
							fbAdr_R,
							2*hwidth + t_start_x + separation/2 + t_width/2,
							t_start_y,
							vwidth*2,
							t_height/4 -hwidth,
							shield_active?C_COMPUTER_LIGHT_SHIELD:C_COMPUTER_LIGHT,
							WIDTH_TOP);
					}
				}
			}
		}
		
		draw_box(
			fbAdr_L, 
			t_start_x-separation/2, 
			t_start_y, 
			t_width, 
			t_height+1,
			hwidth,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
			WIDTH_TOP);

		draw_box(
			fbAdr_R, 
			t_start_x + separation/2, 
			t_start_y, 
			t_width, 
			t_height,
			hwidth,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
			WIDTH_TOP);
				
		draw_box(
			fbAdr_L, 
			t_start_x + t_width/4-separation/2, 
			t_start_y + t_height/4, 
			t_width/2, 
			t_height/2,
			hwidth,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
			WIDTH_TOP);

		draw_box(
			fbAdr_R, 
			t_start_x + t_width/4 + separation/2, 
			t_start_y + t_height/4, 
			t_width/2, 
			t_height/2 + 1,
			hwidth,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER, 
			WIDTH_TOP);	
		
		draw_block(
			fbAdr_L,
			t_start_x-separation/2,
			t_start_y + t_height/2,
			t_width/4,
			hwidth,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER,
			WIDTH_TOP);

		draw_block(
			fbAdr_R,
			t_start_x + separation/2,
			t_start_y + t_height/2,
			t_width/4,
			hwidth,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER,
			WIDTH_TOP);

		draw_block(
			fbAdr_L,
			t_start_x + 3*t_width/4-separation/2,
			t_start_y + t_height/2,
			t_width/4,
			hwidth,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER,
			WIDTH_TOP);

		draw_block(
			fbAdr_R,
			t_start_x + 3*t_width/4 + separation/2,
			t_start_y + t_height/2,
			t_width/4,
			hwidth,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER,
			WIDTH_TOP);

		draw_block(
			fbAdr_L,
			t_start_x + t_width/2 - separation/2,
			t_start_y,
			vwidth,
			t_height/4,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER,
			WIDTH_TOP);

		draw_block(
			fbAdr_R,
			t_start_x + separation/2 + t_width/2,
			t_start_y,
			vwidth,
			t_height/4,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER,
			WIDTH_TOP);

		draw_block(
			fbAdr_L,
			t_start_x + t_width/2 -separation/2,
			t_start_y + 3*t_height/4,
			vwidth,
			t_height/4,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER,
			WIDTH_TOP);

		draw_block(
			fbAdr_R,
			t_start_x + separation/2 + t_width/2,
			t_start_y + 3*t_height/4,
			vwidth,
			t_height/4,
			shield_active?C_COMPUTER_SHIELD:C_COMPUTER,
			WIDTH_TOP);
	}
}

void draw_box(u8* fbAdr, int posx, int posy, int length, int height, int lwidth, int color, int max_width)
{
		draw_block(
			fbAdr, 
			posx, 
			posy, 
			length+1, 
			lwidth, 
			color, 
			max_width); 
		
		draw_block(
			fbAdr, 
			posx, 
			posy + height, 
			length+1, 
			lwidth, 
			color, 
			max_width); 
			
		draw_block(
			fbAdr, 
			posx, 
			posy, 
			lwidth, 
			height, 
			color, 
			max_width);
			
		draw_block(
			fbAdr, 
			posx + length, 
			posy, 
			lwidth, 
			height, 
			color, 
			max_width);
}

void draw_block (u8* fbAdr, int posx, int posy, int length, int width, int color, int max_width)
{
	int x,y;
	
	for (x=posx; (x < posx+length) && (x < max_width); x++)
		for (y=posy; (y < posy+width) && (y < HEIGHT);y++)
		{
			int idx;
			idx = ((x*HEIGHT)+y)*3;
			fbAdr[idx+0] = color_b[color];
			fbAdr[idx+1] = color_g[color];
			fbAdr[idx+2] = color_r[color];
		}
}

void draw_asteroid(u8* fbAdr_L, u8* fbAdr_R)
{
	if (render_object[ASlot].active)
	{
		if(VectorLength(render_object[ASlot].mesh.Position) > 20)
		{
						Render(render_object[ASlot].mesh, front_view, ASTEROID);
		} 
	}
}
	
void draw_trails(u8* fbAdr_L, u8* fbAdr_R)
{
	int i;
	for (i=0; i<NUM_STARS; i++)
	{
		int j;
		Vector3 start = (Vector3) {trail_stars[i].x, trail_stars[i].y, trail_stars[i].z, 0};
		Vector3 trail = VectorSub(start, (Vector3){stars[i].x, stars[i].y, stars[i].z, 0});
		Vector3 step = VectorNormal(trail);
		int dist = abs((int)VectorLength(trail));
		
		checkSoundFX();
		
		for (j=0; j<dist; j++)
		{
			int posX, posY;
			if ((int)start.Z  > focal_distance)
			{
			
				posX = (int)(focal_distance * start.X / start.Z)+WIDTH_TOP/2;	
				posY = (int)(focal_distance * start.Y / start.Z)+HEIGHT/2;

				if ((posX < 0) || (posX > WIDTH_TOP))
				{
					create_new_star(i);
					continue;
				}		
				if ((posY < 0) || (posY > HEIGHT))
				{
					create_new_star(i);
					continue;
				}
				if ((posX > 0) && (posX < WIDTH_TOP) && (posY > 0) && (posY < HEIGHT))
				{
					int idx;
					int x, y;
					int separation;
				
					separation = get_stereo_separation(start.Z);
				
					if ((posX + separation/2 + STAR_WIDTH > WIDTH_TOP) ||(posX - separation/2 < 0))
					{
						create_new_star(i);
						continue;
					}

					for (x=0; x<STAR_WIDTH; x++)
						for (y=0; y<STAR_WIDTH; y++)
						{
							int r, g, b;
							r = g = b = (int)(color_b[C_STARS]*(1-start.Z/MAX_DIST)+55)-j;
							if (shield_active)
							{
								if(g<225)
								{
									g += 30;
								} else g = 255;
								r = r/2;
							}
						
							idx=(((posX+x-separation/2)*HEIGHT)+posY+y)*3;
							fbAdr_L[idx+0] = b;
							fbAdr_L[idx+1] = g;
							fbAdr_L[idx+2] = r;
						
							idx=(((posX+x+separation/2)*HEIGHT)+posY+y)*3;
							fbAdr_R[idx+0] = b;
							fbAdr_R[idx+1] = g;
							fbAdr_R[idx+2] = r;
						}
				} 
			} else 
			{
				create_new_star(i);
				continue;
			}
			start = VectorSub(start, step);
		}
	}
}

void draw_stars(u8* fbAdr_L, u8* fbAdr_R)
{
	int i;
	int dir = 1;
	
	if (!front_view) dir = dir * (-1); 
	
	for (i=0; i<NUM_STARS; i++)
	{
		int posX, posY;
		
		checkSoundFX();
		
		if ((int)stars[i].z*dir  > focal_distance)
		{
			posX = (int)(focal_distance * dir * stars[i].x / stars[i].z)+WIDTH_TOP/2;	
			posY = (int)(focal_distance * dir * stars[i].y / stars[i].z)+HEIGHT/2;

			if ((posX < 0) || (posX > WIDTH_TOP))
			{
				create_new_star(i);
				continue;
			}		
			if ((posY < 0) || (posY > HEIGHT))
			{
				create_new_star(i);
				continue;
			}
			if ((posX > 0) && (posX < WIDTH_TOP) && (posY > 0) && (posY < HEIGHT))
			{
				int idx;
				int x, y;
				int separation;
				
				separation = get_stereo_separation(stars[i].z);
				
				if ((posX + separation/2 + STAR_WIDTH > WIDTH_TOP) ||(posX - separation/2 < 0))
				{
					create_new_star(i);
					continue;
				}

				for (x=0; x<STAR_WIDTH; x++)
					for (y=0; y<STAR_WIDTH; y++)
					{
						int r, g, b;
						r = g = b = (int)(color_b[C_STARS]*(1-stars[i].z/MAX_DIST*dir)+55);
						if (shield_active)
						{
							if(g<225)
							{
								g += 30;
							} else g = 255;
							r = r/2;
						}
						
						idx=(((posX+x-separation/2)*HEIGHT)+posY+y)*3;
						fbAdr_L[idx+0] = b;
						fbAdr_L[idx+1] = g;
						fbAdr_L[idx+2] = r;
						
						idx=(((posX+x+separation/2)*HEIGHT)+posY+y)*3;
						fbAdr_R[idx+0] = b;
						fbAdr_R[idx+1] = g;
						fbAdr_R[idx+2] = r;
					}
			} 
		} else {
			create_new_star(i);
		}	
	}
}

void draw_debris(u8* fbAdr_L, u8* fbAdr_R)
{
	int i;
	int dir = 1;
	
	if (!front_view) dir = dir * (-1); 
	
	for (i=0; i<MAX_NUM_DEBRIS; i++)
	{
		int posX, posY;
		if (debris[i].active && ((int)debris[i].pos.z*dir  > 0))
		{
			posX = (int)(focal_distance * dir * debris[i].pos.x / debris[i].pos.z)+WIDTH_TOP/2;	
			posY = (int)(focal_distance * dir * debris[i].pos.y / debris[i].pos.z)+HEIGHT/2;

			if ((posX < 0) || (posX > WIDTH_TOP))
			{
				debris[i].active = false;
				continue;
			}		
			if ((posY < 0) || (posY > HEIGHT))
			{
				debris[i].active = false;
				continue;
			}
			if ((posX > 0) && (posX < WIDTH_TOP) && (posY > 0) && (posY < HEIGHT))
			{
				int idx;
				int x, y;
				int separation;
				
				separation = STEREO_SEPARATION;
				if (debris[i].pos.z*dir > MAX_DIST/10) separation = 3*STEREO_SEPARATION/4;
				if (debris[i].pos.z*dir > MAX_DIST/7) separation = STEREO_SEPARATION/2;
				if (debris[i].pos.z*dir > MAX_DIST/2) separation = STEREO_SEPARATION/4;
				if (debris[i].pos.z*dir > 2*MAX_DIST/3) separation = 0;
				
				if (posX + separation + DEBRIS_WIDTH > WIDTH_TOP)
				{
					debris[i].active = false;
					continue;
				}

				for (x=0; x<DEBRIS_WIDTH; x++)
					for (y=0; y<DEBRIS_WIDTH; y++)
					{
						idx=(((posX+x)*HEIGHT)+posY+y)*3;
						int r, g, b;
						r = g = b = (int)(color_b[C_STARS]*(1-stars[i].z/MAX_DIST*dir)+55);
						if (shield_active)
						{
							if(g<225)
							{
								g += 30;
							} else g = 255;
							r = r/2;
						}
						
						
						fbAdr_L[idx+0] = b;
						fbAdr_L[idx+1] = g;
						fbAdr_L[idx+2] = r;
						
						idx=(((posX+x+separation)*HEIGHT)+posY+y)*3;
						fbAdr_R[idx+0] = b;
						fbAdr_R[idx+1] = g;
						fbAdr_R[idx+2] = r;
					}
			}
		} else {
			debris[i].active = false;
		}	
	}
}

void draw_text_SL (char* text, u8* fbAdr, int posX, int posY, int color, int max_width, bool size)
{
	if (size)
	{
		draw_text_XXL (text, fbAdr, posX, posY, color, max_width);
	} else {
		draw_text (text, fbAdr, posX, posY, color, max_width);
	}
}

void draw_text (char* text, u8* fbAdr, int posX, int posY, int color, int max_width)
{
	int i;
	int x, y;
	
	x = posX;
	y = posY;
	
	for (i=0; i < strlen(text); i++)
	{
		if ((x+8 < max_width)&&(y+8 < HEIGHT))
		{
			int idx = (int)text[i] - 32;
			int j, k;
			for (j=0; j<8; j++)
				for (k=0; k<8; k++)
				{
					u8 pixel = character[idx][7-j][k];
					if (pixel == 1)
					{
						fbAdr[3*((x+k)*HEIGHT + y +j)+0] = color_b[color]; 
						fbAdr[3*((x+k)*HEIGHT + y +j)+1] = color_g[color]; 
						fbAdr[3*((x+k)*HEIGHT + y +j)+2] = color_r[color]; 
					}
				}
		}
		x+=8;
	}
}

void draw_text_XXL (char* text, u8* fbAdr, int posX, int posY, int color, int max_width)
{
	int i;
	int x, y;
	
	x = posX;
	y = posY;
	
	for (i=0; i < strlen(text); i++)
	{
		if ((x+16 < max_width)&&(y+16 < HEIGHT))
		{
			int idx = (int)text[i] - 32;
			int j, k;
			for (j=0; j<16; j++)
				for (k=0; k<16; k++)
				{
					u8 pixel = character[idx][(15-j)/2][k/2];
					if (pixel == 1)
					{
						fbAdr[3*((x+k)*HEIGHT + y +j)+0] = color_b[color]; 
						fbAdr[3*((x+k)*HEIGHT + y +j)+1] = color_g[color]; 
						fbAdr[3*((x+k)*HEIGHT + y +j)+2] = color_r[color]; 
					}
				}
		}
		x+=16;
	}
}

