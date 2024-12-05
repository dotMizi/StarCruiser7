#include <3ds.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <sys/param.h>

#include "vector.h"
#include "3dmodels.h"
#include "chars.h"
#include "game.h"
#include "colors.h"
#include "render.h"
#include "3drenderer.h"
#include "sound.h"

/*
* MAIN contains all the game logic and object moving
* 
*/

int speed;
float energy=9999.0;
float overall_energy = 0.0;
int overall_enemies_destroyed = 0;
int overall_starbases_destroyed =0;
int overall_damage_count = 0;
enum abort_reason aborted = RUNNING;

int rank = -1;
int class = -1;

star joystick;

bool front_view = true;
bool computer = false;
bool shield = false;
bool photon_right_avail = true;
bool photon_left_avail = true;
bool photon_left_ready = true;
bool toggle_energy_color = false;
bool blink_on = false;
bool blink_3s_on = false;
int maxspeed = 9;

enum game_states gamestate;
enum bottom_screen_states bottomscreen_state;
bool touch_down=false;

struct gmapentry gmap[GMAP_MAX_X][GMAP_MAX_Y];
int cruiser_sector_x;
int cruiser_sector_y;
int hyperwarp_target_sector_x;
int hyperwarp_target_sector_y;

struct timeval t_game, t_start, t_last, t_act;
time_t last_secs = 0;
time_t last_secs2 = 0;
time_t last_secs9 = 0;
time_t last_secs_min = 0;
time_t rand_secs = 0;
time_t secs = 0;

int fps_counter = 0;
int fps = 0;

int ASlot;
int counter;

int velocity[10] = {0,1,2,3,4,6,12,25,37,43};
float speed_costs[10]  = {0.0,1.0,1.5,2.0,2.5,3.0,3.5,7.5,11.25,15};
float hyperwarp_energy;
enum warp_states warp_state;
enum docking_states docking_state;
float warp_speed;
int warp_sound = 0;
star hyperwarp_location;
star hyperwarp_direction;
int hyperspace_ttr;
float dx, dy;

bool red_alert = false;

int target_lock;
bool display_diagnostic = false;

floater asteroid;

int game_level;

char vc_text[21];

int messages[MAX_MESSAGES];
int message_index;
char act_message[81];
char message1[81];
char message2[81];
const char* message_text[] = 
{ 
	" ",						// 0
	"BY ASTEROID SMASH",		// 1
	"ATTACK COMPUTER OFF",		// 2
	"SHIELDS ON",				// 3
	"SHIELDS OFF",				// 4
	"SELECT MISSION",			// 5
	"& PRESS START",			// 6
	"HYPERWARP ENGAGED",		// 7
	"STARBASE SURROUNDED",		// 8
	"STARBASE DESTROYED",		// 9
	"HYPERWARP ABORTED",		// 10
	"HYPERWARP COMPLETE",		// 11
	"HYPERSPACE",				// 12
	"DOCKING ESTABLISHED",		// 13
	"STANDBY",					// 14
	"DOCKING ABORTED",			// 15
	"TRANSFER COMPLETE",		// 16
	"STAR FLEET TO",			// 17
	"ALL UNITS",				// 18
	"STAR CRUISER 7",			// 19
	"DESTROYED",				// 20
	"BY ZYLON FIRE",			// 21
	"POSTHUMOUS RANK IS",		// 22
	"MISSION ABORTED",			// 23
	"ZERO ENERGY",				// 24
	"NEW RANK IS",				// 25
	"REPORT TO BASE",			// 26	
	"FOR TRAINING",				// 27
	"MISSION COMPLETE",			// 28
	"CONGRATULATIONS",			// 29
	"DAMAGE CONTROL",			// 30
	"PHOTONS DAMAGED",			// 31
	"PHOTONS DESTROYED",		// 32
	"ENGINES DAMAGED",			// 33
	"ENGINES DESTROYED",		// 34
	"SHIELDS DAMAGED",			// 35
	"SHIELDS DESTROYED",		// 36
	"COMPUTER DAMAGED",			// 37
	"COMPUTER DESTROYED",		// 38
	"SECTOR SCAN DAMAGED",		// 39
	"SECTOR SCAN DESTROYED",	// 40
	"SUBSPACE RADIO DAMAGED",	// 41
	"SUBSPACE RADIO DESTROYED",	// 42
	"RED ALERT", 				// 43
	"AFT VIEW"					// 44	
};

const char* rank_message[] = 
{ 
	"GALACTIC COOK",		// 0
	"GARBAGE SCOW CAPTAIN",	// 1
	"ROOKIE",				// 2
	"NOVICE",				// 3
	"ENSIGN",				// 4
	"PILOT",				// 5
	"ACE",					// 6
	"LIEUTENANT",			// 7
	"WARRIOR",				// 8
	"CAPTAIN",				// 9
	"COMMANDER",			// 10
	"STAR COMMANDER"		// 12
};

// Konstante für festen Zeitschritt (60 Simulationen pro Sekunde)
const double FIXED_DELTA_TIME = 1.0 / 60.0;

// Funktionen zur Zeitmessung
double get_current_time()
{
    struct timeval ttv;
    gettimeofday(&ttv, NULL);

    return ttv.tv_sec + ttv.tv_usec * 1e-6;
}

char debug_string[81];

struct floater debris[MAX_NUM_DEBRIS];
struct floater render_object[MAX_NUM_OF_ENEMIES];
star stars[NUM_STARS];
star trail_stars[NUM_STARS];

float rand_range(int max, int min)
{
	float range = 0;
	while ((abs(range) < min) || (abs(range) > max))
	{	
		range = (int)(((((float)(rand()%100))/100)*(max-min)+min)*((rand()%3)-1));
	}
	return range;
}

enum subsystem_states computer_state;
bool computer_avail()
{
	if (computer_state == WORKING)
		return true;
	return false;
}

void set_computer_state (enum subsystem_states state)
{
	computer_state = state;
}

enum subsystem_states get_computer_state()
{
	return computer_state;
}

enum subsystem_states shield_state;

bool shield_avail()
{
	if (shield_state == WORKING)
		return true;
	if (shield_state == DAMAGED)
		return (((rand() % 3) < 2)?true:false);
	return false;
}

void set_shield_state (enum subsystem_states state)
{
	shield_state = state;
}

enum subsystem_states get_shield_state()
{
	return shield_state;
}

enum subsystem_states get_photon_state()
{
	if (photon_left_avail&&photon_right_avail) return (WORKING);
	if (!photon_left_avail&&!photon_right_avail) return (DESTROYED);
	return (DAMAGED);
}

enum subsystem_states engines_state;

enum subsystem_states get_engines_state()
{
	return engines_state;
}

void set_engines_state (enum subsystem_states state)
{
	engines_state = state;
}

bool engines_avail()
{
	if (engines_state != WORKING)
		return false;
	return true;
}

enum subsystem_states sector_scan_state;

bool sector_scan_avail()
{
	if (sector_scan_state == WORKING)
		return true;
	return false;
}

void set_sector_scan_state(enum subsystem_states state)
{
	sector_scan_state = state;
}

enum subsystem_states get_sector_scan_state()
{
	return sector_scan_state;
}

enum subsystem_states subspace_radio_state;

bool subspace_radio_avail()
{
	if (subspace_radio_state == WORKING)
		return true;
	return false;
}

void set_subspace_radio_state(enum subsystem_states state)
{
	subspace_radio_state = state;
}

enum subsystem_states get_subspace_radio_state()
{
	return subspace_radio_state;
}

void lock_next_target(bool succ)
{
	if (target_lock < 0)
	{
		target_lock = locked_target();
	} else {
		int i = target_lock;
		
		do {
			succ?i++:i--;
			if (i < 0) i = MAX_NUM_OF_ENEMIES-1;
			if (i >= MAX_NUM_OF_ENEMIES) i = 0;
		} while (((render_object[i].objecttype != FIGHTER) ||
			(render_object[i].objecttype != RAIDER) ||
			(render_object[i].objecttype != ZYLONBASE) ||
			(render_object[i].objecttype != STARBASE)) && 
			(!render_object[i].active));
		target_lock = i;
	}
}

int locked_target()
{
	float target_dist = INFINITY;
	int i, target_marker = -1;
	
	if (target_lock >= 0)
		if (render_object[i].active)
		{
			if ((render_object[i].objecttype == FIGHTER) ||
			(render_object[i].objecttype == RAIDER) ||
			(render_object[i].objecttype == ZYLONBASE) ||
			(render_object[i].objecttype == STARBASE))
			{
				return target_lock;
			} else target_lock = -1;
		} else target_lock = -1;
	
	for (i = 0; i < MAX_NUM_OF_ENEMIES; i++)
		if (render_object[i].active) 
		{
			if ((render_object[i].objecttype == FIGHTER) ||
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
		}
	return target_marker;
}

int get_enemie_slot()
{
	int i;

	for (i=0; i<MAX_NUM_OF_ENEMIES; i++)
	{
		if (!render_object[i].active) return i;
	}
	return -1;
}

int get_active_slot(enum object_type object)
{
	int i;

	for (i=0; i<MAX_NUM_OF_ENEMIES; i++)
	{
		if ((render_object[i].active) && (render_object[i].objecttype == object)) 
			return i;
	}
	return -1;
}

bool is_starbase_sector()
{
	int i;

	for (i=0; i<MAX_NUM_OF_ENEMIES; i++)
	{
		if ((render_object[i].active) && (render_object[i].objecttype == STARBASE)) 
			return true;
	}
	return false;
}

bool has_no_neighbour(int x, int y)
{
	int i, j;
	for (i=x-1;i<=x+1;i++)
		for (j=y-1;j<=y+1;j++)
			if (gmap[i][j].layerA != 0) return false;
	return true;
}

bool has_no_starbase_neighbour(int x, int y)
{
	if (x-1>=0) if (gmap[x-1][y].layerA == 5) return false;
	if (x+1<GMAP_MAX_X) if (gmap[x+1][y].layerA == 5) return false;
	if (y-1>=0) if (gmap[x][y-1].layerA == 5) return false;
	if (y+1<GMAP_MAX_Y) if (gmap[x][y+1].layerA == 5) return false;
	return true;
}

bool is_neighbour_of (int x, int y, int tx, int ty)
{
	if (x=tx)
		if ((y==ty+1)||(y==ty-1)) return true;
	if (y=ty)
		if ((x==tx+1)||(x==tx-1)) return true;
	return false;
}

int starbase_surrounded()
{
	int x,y;
	for (x=1; x<15; x++)
		for(y=1; y<7; y++)
			if(gmap[x][y].layerA == 5)
				if(
					(gmap[x-1][y].layerA > 0)&&
					(gmap[x+1][y].layerA > 0)&&
					(gmap[x][y-1].layerA > 0)&&
					(gmap[x][y+1].layerA > 0)
				)	return (x*GMAP_MAX_X+y);
	return (0);
}

bool is_starbase_surrounded(int x, int y)
{
	if(
		(gmap[x-1][y].layerA > 0)&&
		(gmap[x+1][y].layerA > 0)&&
		(gmap[x][y-1].layerA > 0)&&
		(gmap[x][y+1].layerA > 0)
	)	return true;
	return false;
}

int target_starbase_x;
int target_starbase_y;
bool target_starbase_valid = false;
bool target_timer_running = false;
int starbase_destruction_timer;

int num_of_starbases()
{
    int x,y,r;
    r = 0;
    for (x=1; x<15; x++)
        for(y=1; y<7; y++)
            if(gmap[x][y].layerA == 5) r++;
    return r;
}

bool set_target_starbase()
{
	if (num_of_starbases() > 0)
	{
		int starbasesectors[num_of_starbases()][2];
		int i,j, sectorindex = 0;
		
		for (i=0; i<GMAP_MAX_X; i++)
			for (j=0; j<GMAP_MAX_Y; j++)
				if (gmap[i][j].layerA == 5)
				{
					starbasesectors[sectorindex][0] = i;
					starbasesectors[sectorindex][1] = j;
					sectorindex++;
				}
		
		if (sectorindex <= 1)
		{
			sectorindex = 0;
		} else {
			sectorindex = rand()%sectorindex;
		}
		target_starbase_x = starbasesectors[sectorindex][0];
		target_starbase_y = starbasesectors[sectorindex][1];
		target_starbase_valid = true;
	} else target_starbase_valid = false;
	return target_starbase_valid;	
}

void copygmap()
{
	int x,y;
	
	for (x=0; x<GMAP_MAX_X; x++)
		for(y=0; y<GMAP_MAX_Y; y++)
		{
			if (subspace_radio_avail())	gmap[x][y].layerB = gmap[x][y].layerA;
			gmap[x][y].moved = false;
		}
}

int get_num_free_message_slots()
{
	int i, r = 0;
	for (i = 0; i < MAX_MESSAGES; i++)
	{
		if (messages[i] < 0) 
		{
			r++;
		}
	}
	return r;
}

void queue_message(int message)
{
	int i;
	
	if ((strlen(message_text[message]) < 20)&&(message != 0)&&(message != 43))
	{
		sprintf (vc_text, message_text[message]);
		vc_print(vc_text);
	}
	
	for (i = message_index; i < MAX_MESSAGES; i++)
	{
		if (messages[i] < 0) 
		{
			messages[i] = message;
			return;
		}
	}
	for (i = 0; i < message_index; i++)
	{
		if (messages[i] < 0) 
		{
			messages[i] = message;
			return;
		}
	}
}

//handle game over delay
time_t last_secs_gd = 0;
bool gd_sound = false;
void set_game_over_display_delay()
{
    gettimeofday(&t_act, NULL);
    last_secs_gd = t_act.tv_sec;
    gd_sound = true;
}

bool game_over_display_on()
{
    gettimeofday(&t_act, NULL);
    if ((last_secs_gd + 5) <= t_act.tv_sec) return true;
    return false;
}

void damage(int damagelevel)
{
	int component = rand()%(7);
	int grade = (10*game_level+50)/((shield&&shield_avail())?4:1); 
	int ran = rand()%(200);
	bool damaged = (ran<grade);
	
	if (damagelevel > 0) return;
	
	if ((damagelevel == 0)&&!(shield&&shield_avail())) overall_damage_count += (50/((shield&&shield_avail())?2:1))*game_level;
	if (overall_damage_count > 100)
	{
		aborted = ZYLON_FIRE;
		set_game_over_display_delay();
		gamestate = GAME_END;
	}
	
	if (gamestate == GAME_RUNNING)
	{
		if (damaged)
		{
			if (damagelevel == 0) overall_damage_count += 7;
			if (overall_damage_count > 100)
			{
			}
			
			queue_message(30); //DAMAGE CONTROL
			setSoundFX(DAMAGE_REPORT);
			switch(component)
			{
				case 0:
					if (photon_left_avail)
					{
						if (photon_right_avail)
						{
							queue_message(31); // PHOTONS DAMAGED
						} else queue_message(32); // PHOTONS DESTROYED
						photon_left_avail = false;
					break;
					}
				case 1:
					if (photon_right_avail)
					{
						if (photon_left_avail)
						{
							queue_message(31); // PHOTONS DAMAGED
						} else queue_message(32); // PHOTONS DESTROYED
						photon_right_avail = false;
					break;
					}
				case 2:
					if (computer_avail())
					{
						queue_message(38); // COMPUTER DESTROYED
						set_computer_state(DESTROYED);
						break;
					}
				case 3:
					if (get_sector_scan_state()==WORKING)
					{
						queue_message(40); // SECTOR SCAN DAMAGED
						set_sector_scan_state(DAMAGED);
						break;
					}
				case 4:
					if (engines_avail())
					{
						queue_message(33); // ENGINES DAMAGED
						set_engines_state(DAMAGED);
						break;
					}
				case 5:
					if (shield_state == WORKING)
					{
						queue_message(35); // SHIELDS DAMAGED
						set_shield_state(DAMAGED);
						break;
					} else if (shield_state == DAMAGED)
					{
						queue_message(36); // SHIELDS DESTROYED
						set_shield_state(DESTROYED);
						break;;
					}
				case 6:
					if (subspace_radio_avail())
					{
						queue_message(42); // SUB-SPACE RADIO DESTROYED
						set_subspace_radio_state(DESTROYED);
						break;
					}
				default:
					damage(damagelevel++);
					break;
			}
		}
	}
}

Vector3 face_to(Vector3 act_view, Vector3 dir)
{
	float rho = VectorLength(dir);
	float phi = asin (dir.Y/rho); //-90 - 90
	float theta = atan2(dir.X, dir.Z); //-180 - +180
	
	return (Vector3) {phi,theta, 0.0,0};
}

void drain_energy(enum energy_consume consume)
{
    switch(consume) {
        case SECONDLY:
            energy -= 0.25; // energy consumed by life support system
            overall_energy += 0.25;
            energy -= speed_costs[speed]; // energy consumed for twin-ion speed
            overall_energy += speed_costs[speed];
            if (shield && shield_avail())
            {
                energy -= 2.0; // energy consumed by shields
                overall_energy += 2.0;
            }
            if (computer && computer_avail())
            {
                energy -= 0.5; // energy consumed by computer
                overall_energy += 0.5;
            }
            break;
        case ASTEROID_HIT:
            sprintf (vc_text, "HIT BY ASTEROID");
            vc_print(vc_text);
            energy -= 50.0; // energy consumed by asteroid hit
            overall_energy += 50.0;
            break;
        case PHOTONT_HIT:
            sprintf (vc_text, "HIT BY PHOTONTORPEDO");
            vc_print(vc_text);
            energy -= 100.0; // energy consumed by photon torpedo hit
            overall_energy += 100;
            break;
        case PHOTONT_FIRE:
            energy -= 10.0; // energy consumed by fiering photon torpedo
            overall_energy += 10.0;
            break;
        case HYPERSPACE:
            energy -= HYPERWARP_COSTS;
            overall_energy += HYPERWARP_COSTS;
            
        default:
            break;
    }
    
    if (energy<0) energy = 0;
    if (energy <= 0)
    {
        set_shield_state(DESTROYED);
        set_computer_state(DESTROYED);
        photon_left_avail = false;
        photon_right_avail = false;
        maxspeed--;
    }
    if (maxspeed < 0) maxspeed = 0;
    
    if (energy < 1000.0)
    {
        toggle_energy_color = !toggle_energy_color;
    } else {
        toggle_energy_color = false;
    }
}

void create_new_enemy_photont(Vector3 source)
{
    Vector3 dest;
    int i;
    i = get_enemie_slot();
    if (i>=0)
    {
        render_object[i].active = true;
        render_object[i].objecttype = ENEMYPHOTONT;
        render_object[i].mesh = PhotonT;
        render_object[i].mesh.Position.X = source.X;
        render_object[i].mesh.Position.Y = source.Y;
        render_object[i].mesh.Position.Z = source.Z;
        render_object[i].xr = 0.02;
        render_object[i].yr = 0.02;
        render_object[i].zr = 0.02;
        render_object[i].xt = 0.0;
        render_object[i].yt = 0.0;
        render_object[i].zt = 0.0;
        dest = VectorScalar(VectorNormal(source), 10);
        render_object[i].move.x = dest.X;
        render_object[i].move.y = dest.Y-0.5;
        render_object[i].move.z = dest.Z;
        setSoundFX (PHOTONT_FIRE_E);
    }
}

void create_new_friendly_photont()
{
    int i;
    i = get_enemie_slot();
    if (i>=0)
    {
        if (((photon_left_ready)&&(photon_left_avail))||((!photon_left_ready)&&(photon_right_avail)))
        {
            render_object[i].active = true;
            render_object[i].objecttype = PHOTONT;
            render_object[i].mesh = PhotonT;
            render_object[i].mesh.Position.X = (photon_left_ready)?-10:10;
            render_object[i].mesh.Position.Y = -30;
            render_object[i].mesh.Position.Z = 100*(front_view?1:(-1));
            render_object[i].xr = 0.02;
            render_object[i].yr = 0.02;
            render_object[i].zr = 0.02;
            render_object[i].xt = 0.0;
            render_object[i].yt = 0.0;
            render_object[i].zt = 0.0;
    
            render_object[i].move.x = 0.0;
            render_object[i].move.y = -0.5;
            render_object[i].move.z = (velocity[speed] + 10);
            if (front_view) render_object[i].move.z = (velocity[speed] + 10)*(-1);
            drain_energy(PHOTONT_FIRE);
            setSoundFX (photon_left_ready?PHOTONT_FIRE_L:PHOTONT_FIRE_R);
        }
        photon_left_ready = !photon_left_ready;
    }
}

void create_new_asteroid()
{
    int i;
    i = get_enemie_slot();
    if (i>=0)
    {
        render_object[i].active = true;
        render_object[i].objecttype = ASTEROID;
        render_object[i].mesh = Asteroid;
        render_object[i].mesh.Position.X = (float)rand_range((MAX_DIST/16)*(10-speed), 100);
        render_object[i].mesh.Position.Y = (float)rand_range((MAX_DIST/16)*(10-speed), 100);
        render_object[i].mesh.Position.Z = MAX_DIST;
        render_object[i].xr = (((float)(rand() % 20))-10)/1000;
        render_object[i].yr = (((float)(rand() % 20))-10)/1000;
        render_object[i].zr = (((float)(rand() % 20))-10)/1000;
        render_object[i].xt = ((float)(rand() % 30) - 15);
        render_object[i].yt = ((float)(rand() % 30) - 15);
        render_object[i].zt = (((float)MAX_DIST*speed) / 10.0)/VectorLength((Vector3){0.0,0.0,(float)MAX_DIST,0.0})*VectorLength(render_object[i].mesh.Position);
        Vector3 Move = VectorNormal(VectorSub(render_object[i].mesh.Position, (Vector3){render_object[i].xt,render_object[i].yt,render_object[i].zt,0.0}));

        int length = VectorLength(VectorSub(render_object[i].mesh.Position, (Vector3){render_object[i].xt,render_object[i].yt,render_object[i].zt,0.0}));
    
        render_object[i].move.x = Move.X; render_object[i].mesh.Position.X / length;
        render_object[i].move.y = Move.Y; render_object[i].mesh.Position.Y / length;
        render_object[i].move.z = Move.Z; render_object[i].mesh.Position.Z / length;
    }
}

void move_star(int i);

void create_new_star(int i)
{
    stars[i].x = (float)(rand()%MAX_DIST*2-MAX_DIST);
    stars[i].y = (float)(rand()%MAX_DIST*2-MAX_DIST);
    stars[i].z = (float)(rand()%MAX_DIST*2-MAX_DIST);

    trail_stars[i] = stars[i];
    move_star(i);
}

void move_objects()
{
	int i;
	for (i=0; i < MAX_NUM_OF_ENEMIES; i++) //move every render object in sector
	{
		if (render_object[i].active)
		{
			render_object[i].mesh.Position.Z -= (warp_state == NOT_ENGAGED)?velocity[speed]:(int)warp_speed; //move by cruiser speed
			
			if (render_object[i].objecttype == ENEMYPHOTONT)
			{
				Vector3 Direction = (Vector3) {render_object[i].move.x, render_object[i].move.y, render_object[i].move.z,0};
				Direction = VectorScalar(VectorNormal(Direction), (velocity[speed] + 10)); //ajust speed so we have a chance to see it
				render_object[i].move.x = Direction.X;
				render_object[i].move.y = Direction.Y;
				render_object[i].move.z = Direction.Z;
				
				if(abs(render_object[i].mesh.Position.Z) < 20) //check photon torpedo hit
					if(abs(render_object[i].mesh.Position.Y) < 40)
						if(abs(render_object[i].mesh.Position.X) < 40)
						{
									damage(0);
									render_object[i].active = false;
									drain_energy(PHOTONT_HIT);
									setSoundFX(HIT);
									continue;
						}
			}
			
			if (render_object[i].objecttype == PHOTONT)
			{
				Vector3 Direction = (Vector3) {render_object[i].move.x, render_object[i].move.y, render_object[i].move.z,0};
				Direction = VectorScalar(VectorNormal(Direction), (velocity[speed] + 10)); //ajust speed so we have a chance to see it
				render_object[i].move.x = Direction.X;
				render_object[i].move.y = Direction.Y;
				render_object[i].move.z = Direction.Z;
				
				int j;
				for (j=0; j < MAX_NUM_OF_ENEMIES; j++) //check photon torpedo hit
				{
					if ((render_object[j].objecttype == FIGHTER) ||
					(render_object[j].objecttype == RAIDER) ||
					(render_object[j].objecttype == STARBASE) ||
					(render_object[j].objecttype == ZYLONBASE))
					{
						if(abs(render_object[i].mesh.Position.Z - render_object[j].mesh.Position.Z) < 20)
							if(abs(render_object[i].mesh.Position.Y - render_object[j].mesh.Position.Y) < 20)
								if(abs(render_object[i].mesh.Position.X - render_object[j].mesh.Position.X) < 20)
								{
									hit(render_object[j].mesh.Position.X, render_object[j].mesh.Position.Y, render_object[j].mesh.Position.Z);
									render_object[i].active = false; //photon torpedo is gone
									if (render_object[j].active)
									{
										render_object[j].active = false; //enemy is gone
										if (render_object[j].objecttype != STARBASE)
										{
											gmap[cruiser_sector_x][cruiser_sector_y].layerA--;
											overall_enemies_destroyed++;
										} else gmap[cruiser_sector_x][cruiser_sector_y].layerA = 0;
									}
									continue;
								}
					}
				}
			}			
			
			render_object[i].mesh.Rotation.X -= render_object[i].xr; //rotate objects as they need
			render_object[i].mesh.Rotation.Y -= render_object[i].yr;
			render_object[i].mesh.Rotation.Z -= render_object[i].zr;				
				
			render_object[i].mesh.Position.X -= render_object[i].move.x;
			render_object[i].mesh.Position.Y -= render_object[i].move.y;
			render_object[i].mesh.Position.Z -= render_object[i].move.z;
		
			

		
			if (((render_object[i].objecttype == PHOTONT)||(render_object[i].objecttype == ENEMYPHOTONT)) && (VectorLength(render_object[i].mesh.Position) > 1000)) //end of life for photon torpedos
			{
				render_object[i].active = false;
			}
			
		}
		if ((render_object[i].objecttype == FIGHTER) ||
			(render_object[i].objecttype == RAIDER) ||
			(render_object[i].objecttype == STARBASE) ||
			(render_object[i].objecttype == ZYLONBASE))
			{
				if (render_object[i].objecttype != STARBASE)
				{
					render_object[i].mesh.Rotation = face_to(render_object[i].mesh.Rotation, (Vector3) {render_object[i].move.x,render_object[i].move.y,render_object[i].move.z,0});
				}
				if (VectorLength(render_object[i].mesh.Position) < 100) render_object[i].mesh.Position = VectorScalar(VectorNormal(render_object[i].mesh.Position),100);
			}
	}
}

void move_star(int i)
{
	
	switch(warp_state) {
		case ABORTED:
		case ENGAGED:
			if (abs(stars[i].z) < (MAX_DIST - 10))
				stars[i].z -= 10;
			if (abs(stars[i].z) > MAX_DIST) create_new_star(i);
			break;
		case NOT_ENGAGED:
			stars[i].z -= velocity[speed]+speed;
			if (abs(stars[i].z) > MAX_DIST) create_new_star(i);
			break;
		case IN_HYPERSPACE:
		default:
			break;
	}
}

int max_speed()
{
    if (energy <= 0) return maxspeed;
    if (!engines_avail()) return rand()%3+1;
    return 9;
}

// enemy ai

int maneuver_loop = 0;
void maneuver()
{
    int i;
    float r = 1000.0;
    
        
    for (i=0; i < MAX_NUM_OF_ENEMIES; i++)
    {
        if (render_object[i].active)
        {
            if ((render_object[i].objecttype == FIGHTER) ||
                (render_object[i].objecttype == RAIDER) ||
                (render_object[i].objecttype == ZYLONBASE))
            {
                Vector3 dir = (Vector3){render_object[i].mesh.Position.X, render_object[i].mesh.Position.Y, render_object[i].mesh.Position.Z-500, 0.0};
                if (VectorLength(render_object[i].mesh.Position) < r)
                {
                    dir = VectorScalar(VectorNormal((Vector3){rand()%20-10,rand()%20-10,rand()%20-10, 0.0}), rand()%3);
                    dir = VectorAdd(dir, (Vector3){0.0,0.0,(float)velocity[speed]*(-1.0),0.0});
                } else {
                    dir = VectorScalar(VectorNormal(dir), velocity[7]);
                }
                
                render_object[i].move.x = (render_object[i].move.x + dir.X)/2;
                render_object[i].move.y = (render_object[i].move.y + dir.Y)/2;
                render_object[i].move.z = (render_object[i].move.z + dir.Z)/2;
            }
        }
    }
}

int get_initial_num_starbases(int level)
{
    return 3+level;
}

int get_initial_num_enemies(int level)
{
    // has to be dividable by 9!
    return (3+level)*9; //27, 36, 45, 54
}

int get_num_of_enemies()
{
    int i,j,r = 0;
    for (i=0; i< GMAP_MAX_X; i++)
        for(j=0; j< GMAP_MAX_Y; j++)
            if ((gmap[i][j].layerA > 0) && (gmap[i][j].layerA < 5)) r+=gmap[i][j].layerA;
    return r;
}

int get_num_of_starbases()
{
    int i,j,r = 0;
    for (i=0; i< GMAP_MAX_X; i++)
        for(j=0; j< GMAP_MAX_Y; j++)
            if (gmap[i][j].layerA == 5) r++;
    return r;
}

void init_game()
{
    int i,j;
    front_view = true;
    computer = false;
    set_computer_state(WORKING);
    shield = false;
    set_shield_state(WORKING);
    photon_right_avail = true;
    photon_left_avail = true;
    set_engines_state(WORKING);
    set_subspace_radio_state(WORKING);
    set_sector_scan_state(WORKING);
    toggle_energy_color = false;
    menu_item_selected = 0;
    message_index = 0;
    starbase_destruction_timer = 999999;
    ASlot = 0;
    last_secs = 0;
    last_secs2 = 0;
    last_secs9 = 0;
    last_secs_min = 0;
    rand_secs = 0;
    energy = 9999.0; // to prevent messages
    target_lock = -1;
    
    red_alert = false;
    
    maxspeed = 9;
    speed = 6;
    
    display_diagnostic = false;
    
    focal_distance = FOCAL_DISTANCE;
    
    gamestate = GAME_SPLASH;
    bottomscreen_state = NO_SCREEN;
    warp_state = NOT_ENGAGED;
    warp_sound = 0;
    
    for (i = 0; i < MAX_MESSAGES; i++)
        messages[i] = -1;
    sprintf (message1, "\0");
    sprintf (message2, "\0");
    queue_message(0);
    
    gettimeofday(&t_start, NULL);
    gettimeofday(&t_game, NULL);
    gettimeofday(&t_last, NULL);
    gettimeofday(&t_act, NULL);
    
    vc_init();
    
    for (i=0; i < NUM_STARS; i++)
        create_new_star(i);
    for (i=0; i < sizeof debris / sizeof *debris; i++)
    {
        debris[i].active = false;
        debris[i].ttl = 0;
    }
    for (i=0;i < 16;i++)
        for (j=0; j < 8; j++)
            gmap[i][j].layerA = 0;
}

void init_sector()
{
    int i, j;
    docking_state = NONE;
    target_lock = -1;
    
    for (i=0; i<MAX_NUM_OF_ENEMIES; i++) //clear all enemy slots
    {
        render_object[i].active = false;
    }
    
    switch (gmap[cruiser_sector_x][cruiser_sector_y].layerA) {
        case 1:
        case 2:
        case 3:
        case 4: //fill with enemies
            sprintf (vc_text, "%d ENEMIES IN SECTOR", gmap[cruiser_sector_x][cruiser_sector_y].layerA);
            vc_print(vc_text);
            for (i=0; i<gmap[cruiser_sector_x][cruiser_sector_y].layerA; i++)
            {
                j = get_enemie_slot();
                if (j >= 0)
                {
                    Vector3 dir = VectorNormal((Vector3){1.0,1.0,1.0, 1.0});
                    dir = VectorScalar(dir, 12);
                    render_object[j].move.x = dir.X;
                    render_object[j].move.y = dir.Y;
                    render_object[j].move.z = dir.Z;
                    render_object[j].dir.x = game_level;
                    render_object[j].dir.y = 0;
                    render_object[j].dir.z = 600;
                    render_object[j].xr = 0;
                    render_object[j].yr = 0;
                    render_object[j].zr = 0;
                    render_object[j].active = true;
                    render_object[j].mesh = (rand()%2 == 0)?ZylonFighter:TieFighter;
                    render_object[j].mesh.Position.X = (float)rand_range(1600, 1500);
                    render_object[j].mesh.Position.Y = (float)rand_range(1600, 1500);
                    render_object[j].mesh.Position.Z = (float)rand_range(1600, 1500);
                    render_object[j].objecttype = FIGHTER;
                }
            }
            red_alert = true;
            break;
        case 5: //set up starbase
            sprintf (vc_text, "STARBASE SECTOR");
            vc_print(vc_text);
            j = get_enemie_slot();
            if (j >= 0)
            {
                Vector3 dir = VectorNormal((Vector3){0.0,0.0,0.0, 1.0});
                dir = VectorScalar(dir, 0); //dont move
                render_object[j].move.x = dir.X;
                render_object[j].move.y = dir.Y;
                render_object[j].move.z = dir.Z;
                render_object[j].active = true;
                render_object[j].mesh = Starbase;
                render_object[j].mesh.Position.X = (float)rand_range(1600, 1500);
                render_object[j].mesh.Position.Y = (float)rand_range(1600, 1500);
                render_object[j].mesh.Position.Z = (float)rand_range(1600, 1500);
                render_object[j].objecttype = STARBASE;
                render_object[j].xr = 0;
                render_object[j].yr = 0.174533/5; //rotate slightly
                render_object[j].zr = 0;
            }
            
            break;
        default:
            break;
    }
    hyperwarp_target_sector_x = -1;
    hyperwarp_target_sector_y = -1;
    maneuver(); //set up enemy directions
    create_new_asteroid(); //set up first asteroid
}

void check_starbase_destroyed()
{
    if (starbase_surrounded() > 0)
    {
        target_starbase_x = starbase_surrounded()/GMAP_MAX_X;
        target_starbase_y = starbase_surrounded()%GMAP_MAX_X;
        if ((target_starbase_x > 0) && (target_starbase_x < GMAP_MAX_X) && (target_starbase_y > 0) && (target_starbase_y < GMAP_MAX_Y))
        {
            target_starbase_valid = true;
            if (!target_timer_running)
            {
                starbase_destruction_timer = secs + 60;
                target_timer_running = true;
            }
        }
    } else {
        target_timer_running = false;
    }
    
    if (target_starbase_valid)
    {
        if (is_starbase_surrounded(target_starbase_x, target_starbase_y)&&(countdown() < 0)) //if (starbase_surrounded()&&(starbase_destruction_timer < sec))
        {
            if (subspace_radio_avail())
            {
                queue_message(9);
                queue_message(0);
                queue_message(9);
                queue_message(0);
                queue_message(9);
                setSoundFX(MESSAGE);
            }
            gmap[target_starbase_x][target_starbase_y].layerA = 2;
            gmap[target_starbase_x][target_starbase_y].ttm = secs + rand()%(60-game_level*5);
            if ((target_starbase_x == cruiser_sector_x)&&(target_starbase_y == cruiser_sector_y))
            {
                int x, y, z;
                int s = get_active_slot(STARBASE);
                if (s >=0)
                {
                    x = (int)(render_object[s].mesh.Position.X);
                    y = (int)(render_object[s].mesh.Position.Y);
                    z = (int)(render_object[s].mesh.Position.Z);
                }
                init_sector();
                if (s >=0) hit(x,y,z);
                stopSoundFX(DOCKING);
                docking_state = BROKEN;
            }
            overall_starbases_destroyed++;
            target_timer_running = false;
            target_starbase_valid = false;
        }
    } else     set_target_starbase();
}


void heartbeat()
{
    int msec = 0;
    int i, j;
    
    gettimeofday(&t_act, NULL);
    secs = t_act.tv_sec - t_game.tv_sec;

    if ((last_secs + 1) <= secs)
    {
        // call secondly tasks
        blink_on = !blink_on;
        last_secs = secs;
        counter++;
        
        fps = fps_counter;
        fps_counter = 0;
        
        copygmap();
        check_starbase_destroyed();
        
        for (i=0; i < MAX_NUM_OF_ENEMIES; i++)
        {
            if (render_object[i].active)
            {
                if ((render_object[i].objecttype == FIGHTER) ||
                    (render_object[i].objecttype == RAIDER) ||
                    (render_object[i].objecttype == ZYLONBASE))
                    {
                        if(VectorLength(render_object[i].mesh.Position) < FIRE_RANGE)
                        {
                            int photon_rand = rand()%(300);
                            
                            if (photon_rand<(10*game_level+50))  // gl0:50 gl1:60 gl2:70 gl3:80
                            {
                                create_new_enemy_photont(render_object[i].mesh.Position);
                            }
                        }
                    }
            }
        }
        
        // handle messages
        
        if (messages[message_index] < sizeof(message_text)/sizeof(message_text[0]))
        {
            if (messages[message_index] >= 0)
            {
                sprintf(act_message, message_text[messages[message_index]]);
                messages[message_index] = -1;
                message_index++;
                if (message_index >= MAX_MESSAGES) message_index = 0;
            } else act_message[0] = '\0';
        } else     if (messages[message_index] == sizeof(message_text)/sizeof(message_text[0]))
            {
                sprintf(act_message,"%s", message1);
                messages[message_index] = -1;
                message_index++;
                if (message_index >= MAX_MESSAGES) message_index = 0;
            } else    if (messages[message_index] == sizeof(message_text)/sizeof(message_text[0]) + 1)
                    {
                        sprintf(act_message, "%s", message2);
                        messages[message_index] = -1;
                        message_index++;
                        if (message_index >= MAX_MESSAGES) message_index = 0;
                    } else act_message[0] = '\0';

        if (gamestate == GAME_RUNNING)
        {
            drain_energy(SECONDLY);
            // end of hyperspace
            if (warp_state == IN_HYPERSPACE)
            {
                if (energy == 0) sprintf (debug_string, "HEARTBEAT1");
                if (hyperspace_ttr-- >= 0)
                {
                    setSoundFX(TIC);
                    drain_energy(HYPERSPACE);
                    cruiser_sector_x = hyperwarp_target_sector_x + (int)(dx*hyperspace_ttr);
                    cruiser_sector_y = hyperwarp_target_sector_y + (int)(dy*hyperspace_ttr);
                    if (cruiser_sector_x < 0) cruiser_sector_x = GMAP_MAX_X-1;
                    if (cruiser_sector_x >= GMAP_MAX_X) cruiser_sector_x = 0;
                    if (cruiser_sector_y < 0) cruiser_sector_y = GMAP_MAX_Y-1;
                    if (cruiser_sector_y >= GMAP_MAX_Y) cruiser_sector_y = 0;
                } else {
                    queue_message(11); // HYPERSPACE COMPLETE
                    warp_state = ABORTED;
                    cruiser_sector_x = hyperwarp_target_sector_x;
                    cruiser_sector_y = hyperwarp_target_sector_y;
                    if (energy == 0) {
                        sprintf (debug_string, "HEARTBEAT2");
                    } else {
                        init_sector();
                        if (energy == 0) sprintf (debug_string, "HEARTBEAT3");
                    }
                }
            }
        }
    }
    
    if ((last_secs2 + 3) <= secs)
    {
        // call 3-secondly tasks
        blink_3s_on = !blink_3s_on;
        
        if (gamestate == GAME_END)
        {
            switch (aborted)
            {
            case MISSION_COMPLETED:
                if (get_num_free_message_slots() > 3)
                {
                    queue_message(17); // STAR FLEET TO
                    queue_message(19); // STAR CRUISER 7
                    queue_message(28); // MISSION COMPLETE
                }
                break;
            case NO_ENERGY:
                if (get_num_free_message_slots() > 2)
                {
                    queue_message(23); // MISSION ABORTED
                    queue_message(24); // ZERO ENERGY
                }
                break;
            case HIT_BY_ASTEROID:
                if (get_num_free_message_slots() > 5)
                {
                    queue_message(17); // STAR FLEET TO
                    queue_message(18); // ALL UNITS
                    queue_message(19); // STAR CRUISER 7
                    queue_message(20); // DESTROYED
                    queue_message(01); // BY HUMAN ERROR
                }
            case ZYLON_FIRE:
                if (get_num_free_message_slots() > 5)
                {
                    queue_message(17); // STAR FLEET TO
                    queue_message(18); // ALL UNITS
                    queue_message(19); // STAR CRUISER 7
                    queue_message(20); // DESTROYED
                    queue_message(21); // ZYLON FIRE
                }
                break;
            case NO_STARBASES_LEFT:
                if (get_num_free_message_slots() > 3)
                {
                    queue_message(17); // STAR FLEET TO
                    queue_message(19); // STAR CRUISER 7
                    queue_message(23); // MISSION ABORTED
                }
                break;
            default:
                //queue_message(0); // BLANK
                break;
            }
        }
        
            last_secs2 = secs;
    }
    
    if ((last_secs9 + rand_secs) <= secs)
    {
        // call random tasks
        maneuver();
        last_secs9 = secs;
        rand_secs = rand()%(5-game_level)+2;
    }

    t_last = t_act;
}


void init_level(int level)
{
    int i,j,x,y;
    
    gettimeofday(&t_act, NULL);
    secs = t_act.tv_sec - t_game.tv_sec;
    last_secs = secs;
    last_secs2 = secs;
    last_secs9 = secs;
    last_secs_min = secs;
    rand_secs = secs;
    
    bottomscreen_state = COCKPIT;
    class = -1;
    rank = -1;
    energy = 9999.0;
    maxspeed = 9;
    overall_enemies_destroyed = 0;
    overall_energy = 0.0;
    overall_damage_count = 0;
    speed = 6;
    computer = true;
    aborted = RUNNING;
    target_starbase_valid = false;
    red_alert = false;
    target_lock = -1;
    
    for (x=0; x<GMAP_MAX_X; x++)
        for (y=0; y<GMAP_MAX_Y; y++)
        {
            gmap[x][y].layerA = 0;
            gmap[x][y].layerB = 0;
            gmap[x][y].moved = false;
            gmap[x][y].ttm = 30;
        }
        
    for (i=0; i < get_initial_num_starbases(level); i++)
    {
        do {
            x = rand()%14+1;
            y = rand()%6+1;
        } while ((gmap[x][y].layerA != 0)||(!has_no_neighbour(x,y)));
        
        gmap[x][y].layerA = 5;
    }
    
    for (i=1; i<=get_initial_num_enemies(level)/9;i++)
        for (j=2;j<5;j++)
        {
            do {
                x = rand()%14+1;
                y = rand()%6+1;
            } while ((gmap[x][y].layerA != 0)&&(starbase_surrounded() ==0));
            
            gmap[x][y].layerA = j;
            gmap[x][y].ttm = secs + rand()%(60-game_level*5);
        }
        
    do {
            x = rand()%14+1;
            y = rand()%6+1;
    } while ((gmap[x][y].layerA != 0)&&(has_no_neighbour(x,y)));
        
    cruiser_sector_x = x;
    cruiser_sector_y = y;
    hyperwarp_target_sector_x = x;
    hyperwarp_target_sector_y = y;
    
    warp_state = NOT_ENGAGED;
    warp_sound = 0;
    game_level=level;
    set_target_starbase();
    init_sector();
    heartbeat();
    setSoundFX(ENGINES);
}

void set_hyperwarp_marker(circlePosition joy)
{
    bool xmoved = (abs(joy.dx)>STICK_THRESHOLD);
    bool ymoved = (abs(joy.dy)>STICK_THRESHOLD);

    if ((xmoved)||(ymoved))
    {
        joystick.x = joy.dx/50;
        joystick.y = joy.dy/50;
        hyperwarp_location.x += joystick.x;
        hyperwarp_location.y += joystick.y;
    }
}

void repair()
{
    photon_left_avail = true;
    photon_right_avail = true;
    set_shield_state(WORKING);
    set_engines_state(WORKING);
    set_computer_state(WORKING);
    set_sector_scan_state(WORKING);
    set_subspace_radio_state(WORKING);
    
    maxspeed = 9;
    overall_damage_count = overall_damage_count /2;
    sprintf (vc_text, "OVERALL DAMAGE %d", overall_damage_count);
    vc_print(vc_text);
    toggle_energy_color = false;
    energy = 9999.0;
}
    
//calculate ranking
int get_rank_absolute()
{
    int r;
    
    r = get_rank_m();
    r = r+6*overall_enemies_destroyed;
    r = r-(int)(overall_energy/150.0);
    r = r-(t_act.tv_sec - t_game.tv_sec)/120;
    r = r-18*overall_starbases_destroyed;
    r = r-3*(get_initial_num_starbases(game_level)-overall_starbases_destroyed - get_num_of_starbases());
    return r;
}

int get_rank_m()
{
    int m = 60;
    switch (aborted)
    {
    case RUNNING:
    case MISSION_COMPLETED:
        switch (game_level)
        {
            case 0:
                m = 80;
                break;
            case 1:
                m = 76;
                break;
            case 2:
                m = 60;
                break;
            case 3:
                m = 111;
                break;
            default:
                break;
        }
        break;
    case NO_ENERGY:
    case NO_STARBASES_LEFT:
        switch (game_level)
        {
            case 0:
                m = 60;
                break;
            case 1:
                m = 60;
                break;
            case 2:
                m = 50;
                break;
            case 3:
                m = 100;
                break;
            default:
                break;
        }
        break;
    case HIT_BY_ASTEROID:
    case ZYLON_FIRE:
        switch (game_level)
        {
            case 0:
                m = 40;
                break;
            case 1:
                m = 50;
                break;
            case 2:
                m = 40;
                break;
            case 3:
                m = 90;
                break;
            default:
                break;
        }
        break;
    default:
        break;
    }
    return m;
}

int get_rank(int level)
{
    int rank = get_rank_absolute();
    rank = rank/31;
    if (rank < 0) rank = 0;
    return rank;
}

int get_class(int level)
{
    int rank = get_rank_absolute();    rank = rank%31/2+1;
    if (rank < 0) rank = 0;
    return rank;
}

void move_stars()
{
	int i;
	for (i=0; i < NUM_STARS; i++)
	{
		switch(warp_state) {
			case ABORTED:
			case ENGAGED:
				if (abs(stars[i].z) < (MAX_DIST - warp_speed))
				{
					stars[i].z -= warp_speed;
				} else create_new_star(i);
				break;
			case NOT_ENGAGED:
				stars[i].z -= velocity[speed]+speed;
				if (abs(stars[i].z) > MAX_DIST) create_new_star(i);
				break;
			case IN_HYPERSPACE:
			default:
				break;
		}
	}
}

void move_debris()
{
	int i;
	for (i=0; i < sizeof debris / sizeof *debris; i++)
	{
		if (debris[i].active)
			switch(warp_state) {
				case NOT_ENGAGED:
					debris[i].pos.x += debris[i].move.x;
					debris[i].pos.y += debris[i].move.y;
					debris[i].pos.z += debris[i].move.z;
					debris[i].ttl--;
					if (debris[i].ttl <0)
					{
						debris[i].ttl = 0;
						debris[i].active = false;
					}
					break;
				case IN_HYPERSPACE:
				case ABORTED:
				case ENGAGED:
				default:
					debris[i].active = false;
					break;
			}
	}
}

void move_shuttle()
{
	Vector3 Direction;
	int i = get_active_slot(SHUTTLE);
	
	if (i >= 0)
	{	
		render_object[i].mesh.Rotation = face_to(render_object[i].mesh.Rotation, (Vector3) {render_object[i].move.x,render_object[i].move.y,render_object[i].move.z,0});
		
		switch (docking_state)
		{
			case ESTABLISHED:
				if(abs(render_object[i].mesh.Position.Z) < 10)
				{
					docking_state = COMPLETE;
					queue_message(16);
					setSoundFX(DOUBLETIC);
					repair();
				}
			
				Direction = (Vector3) render_object[i].mesh.Position;
				Direction.X-=20;
				Direction.Y-=20;
				Direction = VectorScalar(VectorNormal(Direction), 0.3); 
				render_object[i].move.x = Direction.X;
				render_object[i].move.y = Direction.Y;
				render_object[i].move.z = Direction.Z;
				break;
			case BROKEN:
			case COMPLETE:
				render_object[i].dir.x = 1.5;
				render_object[i].dir.y = 0;
				render_object[i].dir.z = 0;
				int b = get_active_slot(STARBASE);
				if (b >= 0)
				{
					Direction = (Vector3) render_object[b].mesh.Position;
					Direction = VectorScalar(VectorNormal(Direction), -0.3); 
					render_object[i].move.x = Direction.X;
					render_object[i].move.y = Direction.Y;
					render_object[i].move.z = Direction.Z;
					if(abs(render_object[i].mesh.Position.Z - render_object[b].mesh.Position.Z) < 5)
					{
						render_object[i].active = false;
					}
				}
		}
	}
	
}

void hit(int x, int y, int z)
{
	int i, n = 0;
	for (i=0; i < sizeof debris / sizeof *debris; i++)
	{
		if (n < DEBRIS_PARTICLES_PER_EXPLOSION)
		{	
			if (debris[i].ttl <= 0)
				debris[i].active == false;
		
			if (!debris[i].active)
			{
				debris[i].pos.x=x;
				debris[i].pos.y=y;
				debris[i].pos.z=z;
				debris[i].move.x = (rand()% 8) - 4;
				debris[i].move.y = (rand()% 8) - 4;
				debris[i].move.z = (rand()% 8) - 4;
				debris[i].ttl = rand()%50 + 80;
				debris[i].active = true;
				n++;
			}
		}
	}
	setSoundFX (EXPLOSION);
}

void move_hyperwarp_mark()
{
	hyperwarp_location.x += hyperwarp_direction.x;
	hyperwarp_location.y += hyperwarp_direction.y;
	hyperwarp_location.z += hyperwarp_direction.z;
}

void move_asteroid()
{
	ASlot = get_active_slot(ASTEROID);
	if ((ASlot >= 0)&& (render_object[ASlot].active))
	{
		switch(warp_state) {
			case NOT_ENGAGED:

				if(abs(render_object[ASlot].mesh.Position.Z) > MAX_DIST)
				{
					render_object[ASlot].active = false;
					break;
				}
				if(render_object[ASlot].mesh.Position.Z < MAX_DIST * -0.5)
				{
					render_object[ASlot].active = false;
					break;
				}
				if (VectorLength(render_object[ASlot].mesh.Position) < 50)
				{
					drain_energy(ASTEROID_HIT);
					hit (render_object[ASlot].mesh.Position.X, render_object[ASlot].mesh.Position.Y, render_object[ASlot].mesh.Position.Z);
					overall_damage_count += (60/((shield&&shield_avail())?2:1))*(game_level+1);
					if (overall_damage_count > 100)
					{
						aborted = HIT_BY_ASTEROID;
						set_game_over_display_delay();
						gamestate = GAME_END;
					}
					render_object[ASlot].active = false;
					break;					
				}
				
				render_object[ASlot].mesh.Rotation.X += render_object[ASlot].xr;
				render_object[ASlot].mesh.Rotation.Y += render_object[ASlot].yr;
				render_object[ASlot].mesh.Rotation.Z += render_object[ASlot].zr;

				break;
			case ABORTED:
			case ENGAGED:
			case IN_HYPERSPACE:
			default:
				render_object[ASlot].active = false;
				break;
		}
	}
}

void rotate(circlePosition joy)
{
	bool xmoved = (abs(joy.dx)>STICK_THRESHOLD);
    bool ymoved = (abs(joy.dy)>STICK_THRESHOLD);
	
	float alpha = 0;
	float beta = 0;
	int dir = 1;
	
	if (!front_view) dir = dir * (-1); 
	
	
    if ((xmoved)||(ymoved))
    {
        if (xmoved)
        {
            beta = ((float)joy.dx) / -10000 * dir;
		}

        if (ymoved)
        {
            alpha = ((float)joy.dy) / -10000;
        }
        
		int i;
		for (i=0; i<NUM_STARS; i++)
		{
			float x, y, z;
						
			x = (((float)stars[i].x)*cos(beta)+((float)stars[i].z)*sin(beta));
			z = (((float)stars[i].z)*cos(beta)-((float)stars[i].x)*sin(beta));
			stars[i].x = x;
			stars[i].z = z;
			y = (((float)stars[i].y)*cos(alpha)-((float)stars[i].z)*sin(alpha));
			z = (((float)stars[i].z)*cos(alpha)+((float)stars[i].y)*sin(alpha));
			stars[i].y = y;
			stars[i].z = z;
		}
		
		for (i=0; i < MAX_NUM_OF_ENEMIES; i++)
		{
			if (render_object[i].active)
			{		
				render_object[i].mesh.Position = TransformCoordinate(RotationYawPitchRoll(-alpha, -beta, 0), render_object[i].mesh.Position);
				Vector3 Direction = (Vector3) {render_object[i].move.x, render_object[i].move.y, render_object[i].move.z,0};
				Direction = TransformCoordinate(RotationYawPitchRoll(-alpha, -beta, 0), Direction);
				
				render_object[i].move.x = Direction.X;
				render_object[i].move.y = Direction.Y;
				render_object[i].move.z = Direction.Z;
				continue;
				float x, y, z;
				Vector3 Position = render_object[i].mesh.Position;
				render_object[i].mesh.Rotation.X -= alpha/4;
				render_object[i].mesh.Rotation.Y -= beta/4;
				
				x = (Position.X*cos(beta))+(Position.Z*sin(beta));
				z = (Position.Z*cos(beta))-(Position.X*sin(beta));
				render_object[i].mesh.Position.X = x;
				render_object[i].mesh.Position.Z = z;
				y = (Position.Y*cos(alpha))-(Position.Z*sin(alpha));
				z = (Position.Z*cos(alpha))+(Position.Y*sin(alpha));
				render_object[i].mesh.Position.Y = y;
				render_object[i].mesh.Position.Z = z;
			}
		}
		
		for (i=0; i < MAX_NUM_DEBRIS; i++)
		{
			float x, y, z;
						
			x = (((float)debris[i].pos.x)*cos(beta)+((float)debris[i].pos.z)*sin(beta));
			z = (((float)debris[i].pos.z)*cos(beta)-((float)debris[i].pos.x)*sin(beta));
			debris[i].pos.x = x;
			debris[i].pos.z = z;
			y = (((float)debris[i].pos.y)*cos(alpha)-((float)debris[i].pos.z)*sin(alpha));
			z = (((float)debris[i].pos.z)*cos(alpha)+((float)debris[i].pos.y)*sin(alpha));
			debris[i].pos.y = y;
			debris[i].pos.z = z;
		}
	}
}

int move_on_gmap(int x, int y, int offset)
{
    float dist, d, t;
    int mx, my, i, j;
    mx = my = i = j = 99;
    d = t = 99.99;
    dist = sqrt(pow(abs(i-target_starbase_x),2)+pow(abs(j-target_starbase_y),2))+offset;
                        
    for (i = -1; i < 2; i++)
    {
        if ((i+x >= 0) && (i+x < GMAP_MAX_X))
        {
            for (j = -1; j < 2; j++)
            {
                if ((j+y >= 0) && (j+y < GMAP_MAX_Y))
                {
                    if (gmap[i+x][j+y].layerA == 0)
                    {
                        d = sqrt(pow(abs(i+x-target_starbase_x),2)+pow(abs(j+y-target_starbase_y), 2))+offset;
                        if ((d < dist) && (d < t))
                        {
                            mx = i+x;
                            my = j+y;
                            t = d;
                        }
                    }
                }
            }
        }
    }
    if (t<50.0)
    {
        return my*GMAP_MAX_X+mx;
    }
    return -1;
}


void game_tasks()
{
	int i, j;
	checkSoundFX();
	heartbeat();
	
	// run strategic movement
	if (!target_starbase_valid) set_target_starbase();	
		
	for (i=0; i<GMAP_MAX_X; i++)
		for (j=0; j<GMAP_MAX_Y; j++)
			if ((gmap[i][j].layerA > 0)&&(gmap[i][j].layerA < 5))
				if (gmap[i][j].ttm < secs)
				{
					gmap[i][j].ttm += (60-game_level*5);
					if (!target_timer_running)
						if (!((i == cruiser_sector_x)&&(j == cruiser_sector_y))&&(rand()%((gmap[i][j].layerA*2)+1) < 4)&&!gmap[i][j].moved)
						{
							int t = move_on_gmap(i,j,0);
						
							if ((t<0)&&(rand()%100 > 90)) t = move_on_gmap(i,j,1);
					
							if (t>0)
							{
								gmap[t%GMAP_MAX_X][t/GMAP_MAX_X].layerA = gmap[i][j].layerA;
								gmap[t%GMAP_MAX_X][t/GMAP_MAX_X].ttm = gmap[i][j].ttm;
								gmap[i][j].layerA = 0;
								gmap[t%GMAP_MAX_X][t/GMAP_MAX_X].moved = true;
								if ((t%GMAP_MAX_X == cruiser_sector_x)&&(t/GMAP_MAX_X == cruiser_sector_y)) init_sector();							}				
						}
				}
	if ((starbase_surrounded()> 0)&&!target_timer_running)
	{
		if (subspace_radio_avail()) 
		{
			queue_message(8);
			setSoundFX(MESSAGE);
		}
		starbase_destruction_timer = secs + 60;
		target_timer_running = true;
	}
	
	
	move_stars();
	if ((aborted == RUNNING)&&(gamestate == GAME_RUNNING))
	{
		if (energy <= 0)
		{
			if (warp_state == NOT_ENGAGED)
			{
				gamestate = GAME_END;
				aborted = NO_ENERGY;
				set_game_over_display_delay();
			} else warp_state = ABORTED;
		}
		if (get_num_of_starbases() < 1)
		{
			gamestate = GAME_END;
			aborted = NO_STARBASES_LEFT;
			set_game_over_display_delay();
		}
		if (get_num_of_enemies() < 1) 
		{
			gamestate = GAME_END;
			aborted = MISSION_COMPLETED;
			set_game_over_display_delay();
		}
	}
	
	if((gamestate == GAME_RUNNING) && (aborted == RUNNING) && is_starbase_sector() && (speed == 0))
	{
		if (VectorLength(render_object[get_active_slot(STARBASE)].mesh.Position) <200)
		{
			if (docking_state == NONE)
			{
				queue_message(13);
				queue_message(14);
				setSoundFX(DOCKING);
				docking_state = ESTABLISHED;
				int s = get_enemie_slot();
				if (s >= 0)
				{
					int b = get_active_slot(STARBASE);
					if (b >= 0)
					{
						Vector3 dir = VectorNormal((Vector3){0.0,0.0,0.0, 0.0});
						dir = VectorScalar(dir, 12);
						render_object[s].move.x = dir.X;
						render_object[s].move.y = dir.Y;
						render_object[s].move.z = dir.Z;
						render_object[s].dir.x = 0;
						render_object[s].dir.y = 0;
						render_object[s].dir.z = 0;
						render_object[s].xr = 0;
						render_object[s].yr = 0;
						render_object[s].zr = 0;
						render_object[s].active = true;
						render_object[s].mesh = Shuttle;
						render_object[s].mesh.Position.X = render_object[b].mesh.Position.X;
						render_object[s].mesh.Position.Y = render_object[b].mesh.Position.Y;
						render_object[s].mesh.Position.Z = render_object[b].mesh.Position.Z;
						render_object[s].objecttype = SHUTTLE;
					}
				}
			}
		} else if(docking_state == BROKEN) docking_state = NONE;
	}
	
	if (docking_state == ESTABLISHED|BROKEN) move_shuttle();

	
	if (warp_state == ENGAGED) move_hyperwarp_mark();
	move_debris();
	
	move_asteroid();

	move_objects();
	
	{ // set engine volume
		float engine_vol = 0.0;
		if (warp_state == NOT_ENGAGED)
		{
			warp_sound = 0;
			engine_vol = (1.0/95.0)*velocity[speed];
		} else if (warp_speed < 65)
		{
			engine_vol = 1.0/95.0*warp_speed;
		} else 
		{
			if ((warp_state == ENGAGED) && (warp_sound == 0))
			{
				setSoundFX(HWIN);
				warp_sound = 1;
			}
			if ((warp_state == ABORTED) && (warp_sound == 1))
			{
				setSoundFX(HWOUT);
				warp_sound = 2;
			}
		}
		//if (gamestate != RUNNING) engine_vol = 0.0;
		setSoundFXPrefs(ENGINES, engine_vol,0);
	}
}

int main() 
{
	// Initializations
	srvInit();			// services
	aptInit();			// applets
	hidInit();			// input
	gfxInitDefault();	// graphics
	gfxSet3D(true);		// stereoscopy (true: enabled / false: disabled)
	audio_init();
	
	counter=0;
	
	init_game();		// set all the game parameters
	 
	circlePosition c3po;
	
	srand(time(NULL));
	front_view = true;
	touchPosition touch;
	
	Vector3 Rotation = (Vector3){0,0,0,0};
	Vector3 Position = (Vector3){-20,-20,200,0};
	TieFighter.name = "TIEFighter";
	TieFighter.vertexes = TieV;
	TieFighter.num_of_vertexes = num_of_tie_vertexes;
	TieFighter.faces = TieF;
	TieFighter.num_of_faces = num_of_tie_faces;
	TieFighter.Position = Position;
	TieFighter.Rotation = Rotation;
	ZylonFighter.name = "ZylonFighter";
	ZylonFighter.vertexes = ZylV;
	ZylonFighter.num_of_vertexes = num_of_zyl_vertexes;
	ZylonFighter.faces = ZylF;
	ZylonFighter.num_of_faces = num_of_zyl_faces;
	ZylonFighter.Position = Position;
	ZylonFighter.Rotation = Rotation;
	Vector3 RotationA = (Vector3){0.8,0.8,0.8,0};
	Vector3 PositionA = (Vector3){-20,-20,200,0};
	Asteroid.name = "ASTEROID";
	Asteroid.vertexes = AsteroidV;
	Asteroid.num_of_vertexes = num_of_asteroid_vertexes;
	Asteroid.faces = AsteroidF;
	Asteroid.num_of_faces = num_of_asteroid_faces;
	Asteroid.Position = PositionA;
	Asteroid.Rotation = RotationA;
	PhotonT.name = "PHOTONTORPEDO";
	PhotonT.vertexes = PhotonTV;
	PhotonT.num_of_vertexes = num_of_photont_vertexes;
	PhotonT.faces = PhotonTF;
	PhotonT.num_of_faces = num_of_photont_faces;
	PhotonT.Position = PositionA;
	PhotonT.Rotation = RotationA;
	Starbase.name ="STARBASE";
	Starbase.vertexes = StarbaseV;
	Starbase.num_of_vertexes = num_of_starbase_vertexes;
	Starbase.faces = StarbaseF;
	Starbase.num_of_faces = num_of_starbase_faces;
	RotationA = (Vector3){0.0,0.0,0.0};
	Starbase.Position = PositionA;
	Starbase.Rotation = RotationA;
	Shuttle.name ="SHUTTLE";
	Shuttle.vertexes = ShuttleV;
	Shuttle.num_of_vertexes = num_of_shuttle_vertexes;
	Shuttle.faces = ShuttleF;
	Shuttle.num_of_faces = num_of_shuttle_faces;
	RotationA = (Vector3){0.0,0.0,0.0};
	Shuttle.Position = PositionA;
	Shuttle.Rotation = RotationA;
    
    // Zeitsteuerung
    double previousTime = get_current_time();
    double accumulator = 0.0;
  
  // Main loop
  while (aptMainLoop()&&(gamestate!=GAME_EXIT))
  {
      // Zeitberechnung für den aktuellen Frame
      double currentTime = get_current_time();
      double frameTime = currentTime - previousTime;
      previousTime = currentTime;

	  fps_counter++;
      circlePosition joy;
      checkSoundFX();
      
      // Read which buttons are currently pressed or not
      hidScanInput();
      
      //Read the touch screen coordinates
      hidTouchRead(&touch);
      
      if ((touch.px != 0) && (touch.py != 0))
		touch_down = true;
      
      //Read the CirclePad position
      hidCircleRead(&joy);
      
      gspWaitForVBlank(); // wait for frame
      
      switch (gamestate) {
		case GAME_RESUME:
			char* ritems[2] = {"RESUME", "RESTART GAME"};
			gspWaitForVBlank(); // wait for frame
			bottomscreen_state = NO_SCREEN; // blank screen
			draw_bottom_screen();
			draw_menu(ritems, 2);
			if ((hidKeysDown() & KEY_DDOWN)||(hidKeysDown() & KEY_CPAD_DOWN)){
				menu_item_selected++;
				if (menu_item_selected > 1) menu_item_selected=0;
				setSoundFX(TIC);
			}
	
			if ((hidKeysDown() & KEY_DUP)||(hidKeysDown() & KEY_CPAD_UP)){
				menu_item_selected--;
				if (menu_item_selected < 0) menu_item_selected=1;
				setSoundFX(TIC);
			}

			if ((hidKeysDown() & KEY_START)) //||(hidKeysDown() & KEY_A))
			{
				setSoundFX(DOUBLETIC);
				if(menu_item_selected==0)
				{
					gamestate = GAME_RUNNING;
					bottomscreen_state = COCKPIT; // reenable bottom screen
				} else {
					gamestate = GAME_SELECT;
					init_game();
				}
			}
			break;
		case GAME_EXIT: 
			break;
		case GAME_SPLASH:
		case GAME_SELECT:
			char* items[5] = {"NOVICE", "PILOT", "WARRIOR", "COMMANDER", "EXIT GAME"};
			
			gspWaitForVBlank(); // wait for frame
			
			bottomscreen_state = NO_SCREEN; // blank screen
			draw_bottom_screen();			
			draw_menu(items, 5);
			aborted = RUNNING; // to clear aborted game
            game_tasks(); // Spiel-Logik
			
            // handle menu input
			if ((hidKeysDown() & KEY_DDOWN)||(hidKeysDown() & KEY_CPAD_DOWN)){
				menu_item_selected++;
				if (menu_item_selected > 4) menu_item_selected=0;
				setSoundFX(TIC);
			}
			
			if ((hidKeysDown() & KEY_DUP)||(hidKeysDown() & KEY_CPAD_UP)){
				menu_item_selected--;
				if (menu_item_selected < 0) menu_item_selected=4;
				setSoundFX(TIC);
			}

			if ((hidKeysDown() & KEY_START)) //||(hidKeysDown() & KEY_A))
			{
				setSoundFX(DOUBLETIC);
				if(menu_item_selected!=4)
				{
					gamestate = GAME_RUNNING;
					gettimeofday(&t_game, NULL);
					init_level(menu_item_selected);
				} else {
					gamestate = GAME_EXIT;
				}
			}
			break;
		case GAME_END:
			char* eitems[] = {"RESTART GAME"};
			gspWaitForVBlank(); // wait for frame

			if (true)
			{		
				if (class < 0)
				{
					rank = get_rank(game_level);
					class = get_class(game_level);
				}
				
				if (game_over_display_on())
				{
					bottomscreen_state = NO_SCREEN; // blank screen
					draw_bottom_screen();
					draw_menu(eitems, 1);
					menu_item_selected=1;

					if ((hidKeysDown() & KEY_START)) //||(hidKeysDown() & KEY_A))
					{
						gamestate = GAME_SELECT;
						init_game();
					}
				}
			}
			computer = false;
			warp_state = NOT_ENGAGED;
			game_tasks();
				
			break;
		case GAME_RUNNING:
			menu_item_selected = 0;

            // Begrenzen des frameTime (z. B. bei Pausen oder Verzögerungen)
            if (frameTime > 0.25)
                frameTime = 0.25;

            accumulator += frameTime;

            
            // Simulation in festen Zeitschritten aktualisieren
            while (accumulator >= FIXED_DELTA_TIME)
            {
                game_tasks(); // Spiel-Logik
                accumulator -= FIXED_DELTA_TIME;
            }

			if (hidKeysDown() & KEY_R) lock_next_target(true);
			if (hidKeysDown() & KEY_L) lock_next_target(false);
			if (hidKeysDown() & KEY_Y){ // HYPERWARP
				
				switch(warp_state) {
					case ENGAGED:
					case IN_HYPERSPACE:
						queue_message(10); // HYPERWARP ABORTED
						warp_state = ABORTED;
						//setSoundFX(HWOUT);
						break;
					case NOT_ENGAGED:						
						while ((hyperwarp_target_sector_x < 0) || (hyperwarp_target_sector_y < 0) || (hyperwarp_target_sector_x >= GMAP_MAX_X) || (hyperwarp_target_sector_y >= GMAP_MAX_Y))
						{
							int t;
							t = rand()%3 - 1; 
							hyperwarp_target_sector_x = cruiser_sector_x + t;
							t = rand()%3 - 1; 
							hyperwarp_target_sector_y = cruiser_sector_y + t;
						}
						warp_state = ENGAGED;
						warp_speed = velocity[speed]+speed;
						front_view = true;
						hyperwarp_location.x=WIDTH_TOP/2;
						hyperwarp_location.y=HEIGHT/2;
						hyperwarp_location.z= 200;
						hyperwarp_direction.x=0;
						hyperwarp_direction.y=0;
						hyperwarp_direction.z=0;
						int i;
						for (i=0; i<NUM_STARS; i++)
						{
							trail_stars[i] = stars[i];
						}
						break;
					default:
						break;
				}
			}
              
			if ((warp_state == NOT_ENGAGED)||(warp_speed < 50))
			{
				if (red_alert)
				{
					queue_message(43);
					queue_message(0);
					queue_message(43);
					queue_message(0);
					queue_message(43);
					setSoundFX(ALERT);
					red_alert = false;
				}
				
				if (hidKeysDown() & KEY_START)
				{
					gamestate = GAME_RESUME;
					break;
				}
				rotate(joy);
				if ((hidKeysDown() & KEY_A)&&(warp_state != ENGAGED)){ // FIRE
					create_new_friendly_photont(); 
				}
				if (hidKeysDown() & KEY_B){ // SHIELDS
					shield = !shield;
					setSoundFX(DOUBLETIC);
					queue_message(shield?3:4);
				}			
				if (warp_state != ENGAGED)
				{
					if (hidKeysDown() & KEY_X){ // AFT VIEW					
						front_view = !front_view;
						if (!front_view) queue_message(44);
					}	

					if (hidKeysDown() & KEY_DUP){
						speed++;
						if (docking_state == ESTABLISHED)
						{
							queue_message(15);
							stopSoundFX(DOCKING);
							docking_state = BROKEN;
						}
					}
					if (get_engines_state() != WORKING)
						if (speed > 0)
							speed = 9;
					if (speed > max_speed()) speed = max_speed();
					if (hidKeysDown() & KEY_DDOWN){
						speed--;
					}
					if (speed < 0) speed = 0;
				}
				if (hidKeysDown() & KEY_SELECT){
					//bottomscreen_state = DEBUG_SCREEN;
					display_diagnostic = !display_diagnostic;
				}
				if (hidKeysDown() & KEY_DRIGHT){
					if(bottomscreen_state==COCKPIT) {
						bottomscreen_state = GALACTIC_MAP;
					} else 
					if(bottomscreen_state==GALACTIC_MAP) {
						//bottomscreen_state = DEBUG_SCREEN;
						bottomscreen_state = LONG_RANGE;
					} else
					if(bottomscreen_state==DEBUG_SCREEN) {
						bottomscreen_state = LONG_RANGE;
					} else 	
					if(bottomscreen_state==LONG_RANGE) {
						bottomscreen_state = COCKPIT;
					}
				}
				if (hidKeysDown() & KEY_DLEFT){
					if(bottomscreen_state==COCKPIT) {
						bottomscreen_state = LONG_RANGE;
					} else 
					if(bottomscreen_state==LONG_RANGE) {
						//bottomscreen_state = DEBUG_SCREEN;
						bottomscreen_state = GALACTIC_MAP;
					} else
					if (bottomscreen_state==DEBUG_SCREEN) {
						bottomscreen_state = GALACTIC_MAP;
					} else
					if(bottomscreen_state==GALACTIC_MAP) {
						bottomscreen_state = COCKPIT;					
					}
				}
			
				if (bottomscreen_state==COCKPIT)
				{
					if (touch_down && (touch.px == 0) && (touch.py == 0))
					{
						computer = !computer;
						setSoundFX(DOUBLETIC);
					}
				}
			
				if (bottomscreen_state==GALACTIC_MAP)
				{
					touch.py = HEIGHT-touch.py;
					if ((touch.px > 7)&&(touch.px < 7+17*18))
					{
						hyperwarp_target_sector_x = (touch.px - 7) / 19;
					}
					if ((touch.py > 25+18)&&(touch.py < 25+18+19*8+2))
					{
						hyperwarp_target_sector_y = (touch.py-25-18) / 19;
					}
					touch_down = false;
				}
			
				ASlot = get_active_slot(ASTEROID);
				if ((ASlot<0) && (rand() % 200 < (game_level+1))) create_new_asteroid();			

			} 
			if (warp_state == ENGAGED)
			{
					set_hyperwarp_marker(joy);
					warp_speed += 0.5;
					if (warp_speed > 30)
					{
						if (rand()%100 < 8)
						{
							hyperwarp_direction.x=game_level*((float)(rand()%10))/20 - 0.5;
							hyperwarp_direction.y=game_level*((float)(rand()%10))/20 - 0.5;
							hyperwarp_direction.z=0;
						}
					}
					if (warp_speed > 100)
					{						
						warp_speed = 100; // 16 = infinite
						warp_state = IN_HYPERSPACE;
						queue_message(12); //HYPERSPACE
						/*
						soll 200 - 200 dividiert durch 10 - gl (10 und 5)*5 50 bis 25
						
						(x - xt)/ttr      
						y - yt 
						*/
						hyperwarp_target_sector_x += (int)((hyperwarp_location.x-WIDTH_TOP/2)/((10-game_level)*5));
						hyperwarp_target_sector_y += (int)((hyperwarp_location.y-HEIGHT/2)/((10-game_level)*5));
						if (hyperwarp_target_sector_x < 0) hyperwarp_target_sector_x = GMAP_MAX_X-1;
						if (hyperwarp_target_sector_x >= GMAP_MAX_X) hyperwarp_target_sector_x = 0;
						if (hyperwarp_target_sector_y < 0) hyperwarp_target_sector_y = GMAP_MAX_Y-1;
						if (hyperwarp_target_sector_y >= GMAP_MAX_Y) hyperwarp_target_sector_y = 0;
						
						hyperspace_ttr = (int)((
							sqrt(
								pow(abs(cruiser_sector_x - hyperwarp_target_sector_x), 2)+
								pow(abs(cruiser_sector_y - hyperwarp_target_sector_y), 2)
								)
							) / 18 *23);
							
						dx = ((float)(cruiser_sector_x - hyperwarp_target_sector_x))/((float)hyperspace_ttr);
						dy = ((float)(cruiser_sector_y - hyperwarp_target_sector_y))/((float)hyperspace_ttr);
					}
			}
			if (warp_state == ABORTED)
			{
				warp_speed -= 1;
				if (warp_speed < velocity[speed]) warp_state = NOT_ENGAGED;
				hyperwarp_target_sector_x = -1;
				hyperwarp_target_sector_y = -1;
			}
				
			draw_bottom_screen();
			break;
		default:		
			init_game();
			break;
	}

    if ((touch.px == 0) && (touch.py == 0))
		touch_down = false;
	
	if (gamestate == GAME_EXIT)
		break;
              
    draw_top_screen();

    gfxFlushBuffers();	// flush framebuffers
    gfxSwapBuffers();	// swap framebuffers	
  }
  
	// exit & return
	audio_stop();
	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}

int countdown()
{
	return(starbase_destruction_timer-secs);
}
