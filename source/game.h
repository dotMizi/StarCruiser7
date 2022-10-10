#define _GAME_H_

#define NUM_STARS 12
#define MAX_DIST 1600

#define STICK_THRESHOLD 5

#define MAX_NUM_OF_ENEMIES 20
#define MAX_NUM_DEBRIS 40

#define FIRE_RANGE	1000
#define MAX_MESSAGES 15

enum game_states {GAME_SPLASH, GAME_SELECT, GAME_RUNNING, GAME_RESUME, GAME_END, GAME_EXIT};
enum bottom_screen_states {NO_SCREEN, COCKPIT, LONG_RANGE, GALACTIC_MAP, DEBUG_SCREEN};
enum warp_states {NOT_ENGAGED, ENGAGED, IN_HYPERSPACE, ABORTED};
enum object_type {BLA, TIE, RAIDER, ZYLONBASE, ASTEROID, STARBASE, PHOTONT, ENEMYPHOTONT, SHUTTLE};
enum energy_consume {SECONDLY, ASTEROID_HIT, PHOTONT_HIT, PHOTONT_FIRE, HYPERSPACE};
enum abort_reason {RUNNING, MISSION_COMPLETED, NO_ENERGY, HIT_BY_ASTEROID, ZYLON_FIRE, NO_STARBASES_LEFT};
enum docking_states {NONE, ESTABLISHED, COMPLETE, BROKEN};
enum subsystem_states {WORKING, DAMAGED, DESTROYED};

typedef struct star{
    float x;
	float y;
	float z;
} star;

typedef struct gmapentry {
	int layerA;
	int layerB;
	bool moved;
} gmapentry;

typedef struct floater{
	star pos;
	star move;
	bool clockwise;
	star dir;
	Mesh mesh;
	bool active;
	enum object_type objecttype;
	int ttl;
	float xr;
	float yr;
	float zr;
	float xt;
	float yt;
	float zt;
	float o;
	float p;
	float q;
} floater;

extern enum game_states gamestate;
extern enum bottom_screen_states bottomscreen_state;
extern enum warp_states warp_state;
extern enum abort_reason aborted;
extern enum subsystem_states get_photon_state();
extern enum subsystem_states get_shield_state();
extern enum subsystem_states get_computer_state();
extern enum subsystem_states get_engines_state();
extern enum subsystem_states get_sector_scan_state();
extern enum subsystem_states get_subspace_radio_state();

extern bool front_view;
extern floater asteroid;

extern struct floater debris[];
extern star stars[NUM_STARS];
extern star trail_stars[NUM_STARS];
extern int target_starbase;
extern Vertex enemies_pos[MAX_NUM_OF_ENEMIES];

extern struct timeval t_game, t_start, t_last, t_act;
extern bool blink_on;
extern bool blink_3s_on;
extern bool show_game_over_message;

extern bool shield;
extern bool shield_avail();
extern bool computer;
extern bool computer_avail();
extern bool sector_scan_avail();
extern bool engines_avail();

extern int overall_damage_count;
extern float energy;
extern float overall_energy;
extern int rank;
extern int class;
extern int counter;
extern float hyperwarp_energy;
extern bool toggle_energy_color;
extern float hyperwarp_costs[];
extern int velocity[];
extern int speed;
extern float warp_speed;

extern int ASlot;

extern int game_level;
extern struct gmapentry gmap[16][8];
extern int cruiser_sector_x;
extern int cruiser_sector_y;
extern int hyperwarp_target_sector_x;
extern int hyperwarp_target_sector_y;
extern star hyperwarp_location;
extern struct floater render_object[MAX_NUM_OF_ENEMIES];

extern char act_message[];
extern char message1[];
extern char message2[];
extern const char* message_text[];
extern int message_index;
extern int messages[];

extern const char* rank_message[];

extern void hit(int x, int y, int z);
extern int get_enemie_slot();
extern bool game_over_display_on();

extern int target_starbase_x;
extern int target_starbase_y;
extern bool target_starbase_valid;
extern int starbase_destruction_timer;
extern int countdown();
extern int get_rank_m();
