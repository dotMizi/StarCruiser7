#define SAMPLERATE 22050
#define SAMPLESPERBUF (SAMPLERATE / 30)
#define BYTESPERSAMPLE 4

#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

enum sound_effects {HIT = 0, PHOTONT_FIRE_E = 1, PHOTONT_FIRE_L = 2, PHOTONT_FIRE_R = 3, RED_ALERT = 4, TIC = 5, EXPLOSION = 6, MESSAGE = 7, DAMAGE_REPORT = 9, ENGINES = 10, DOCKING = 11, ALERT = 12, DOUBLETIC = 13, HWIN = 14, HWOUT = 15, CONGRATULATIONS = 16, GAME_OVER = 17};

extern void audio_init();
extern void audio_stop();
extern void checkSoundFX();
extern void setSoundFX (enum sound_effects effect);
extern void setSoundFXPrefs (enum sound_effects effect, float volume, int balance);
extern void stopSoundFX (enum sound_effects effect);
extern float getSoundVol(enum sound_effects effect);

extern const unsigned char congratulations_Amy[];
#define CONGRATULATIONS_AMY_TERMINATION 0x00000000
#define CONGRATULATIONS_AMY_START       0x00000000
#define CONGRATULATIONS_AMY_FINISH      0x0001AE5A
#define CONGRATULATIONS_AMY_LENGTH      0x0001AE5A

extern const unsigned char game_over_Amy[];
#define GAME_OVER_AMY_TERMINATION 0x00000000
#define GAME_OVER_AMY_START       0x00000000
#define GAME_OVER_AMY_FINISH      0x00015700
#define GAME_OVER_AMY_LENGTH      0x00015700

extern const unsigned char laserrocket_5984[];
#define LASERROCKET_5984_TERMINATION 0x00000000
#define LASERROCKET_5984_START       0x00000000
#define LASERROCKET_5984_FINISH      0x000179A8
#define LASERROCKET_5984_LENGTH      0x000179A8

extern const unsigned char  hq_explosion_6288[];
#define HQ_EXPLOSION_6288_TERMINATION 0x00000000
#define HQ_EXPLOSION_6288_START       0x00000000
#define HQ_EXPLOSION_6288_FINISH      0x0003C7DE
#define HQ_EXPLOSION_6288_LENGTH      0x0003C7DE

extern const unsigned char docking_established_standby_vicki[];
#define DOCKING_ESTABLISHED_STANDBY_VICKI_TERMINATION 0x00000000
#define DOCKING_ESTABLISHED_STANDBY_VICKI_START       0x00000000
#define DOCKING_ESTABLISHED_STANDBY_VICKI_FINISH      0x00019FDE
#define DOCKING_ESTABLISHED_STANDBY_VICKI_LENGTH      0x00019FDE

extern const unsigned char warning_vicki[];
#define WARNING_VICKI_TERMINATION 0x00000000
#define WARNING_VICKI_START       0x00000000
#define WARNING_VICKI_FINISH      0x000082DE
#define WARNING_VICKI_LENGTH      0x000082DE

extern const unsigned char blippy_31899_1[];
#define BLIPPY_31899_1_TERMINATION 0x00000000
#define BLIPPY_31899_1_START       0x00000000
#define BLIPPY_31899_1_FINISH      0x00003802
#define BLIPPY_31899_1_LENGTH      0x00003802

extern const unsigned char blippy_31899_2[];
#define BLIPPY_31899_2_TERMINATION 0x00000000
#define BLIPPY_31899_2_START       0x00000000
#define BLIPPY_31899_2_FINISH      0x00001AE2
#define BLIPPY_31899_2_LENGTH      0x00001AE2

extern const unsigned char blaster_2_81267[];
#define BLASTER_2_81267_TERMINATION 0x00000000
#define BLASTER_2_81267_START       0x00000000
#define BLASTER_2_81267_FINISH      0x000111BE
#define BLASTER_2_81267_LENGTH      0x000111BE

extern const unsigned char power_on_39172[];
#define POWER_ON_39172_TERMINATION 0x00000000
#define POWER_ON_39172_START       0x00000000
#define POWER_ON_39172_FINISH      0x0001D45E
#define POWER_ON_39172_LENGTH      0x0001D45E

extern const unsigned char musket_explosion_6383[];
#define MUSKET_EXPLOSION_6383_TERMINATION 0x00000000
#define MUSKET_EXPLOSION_6383_START       0x00000000
#define MUSKET_EXPLOSION_6383_FINISH      0x0000CAC2
#define MUSKET_EXPLOSION_6383_LENGTH      0x0000CAC2

