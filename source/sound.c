#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <3ds.h>

#include "sound.h"

#define ENGINE_BUFFER_LEN 20480

typedef struct sfx{
    s16 *data;
	bool loop;
	bool active;
	int offset;
	int length;
	int balance;
	int repeat;
	int repeated;
	float volume;
	bool reverse;
} sfx;

sfx sounds[20];

ndspWaveBuf waveBuf[2];
bool fillBlock = false;
u32* audioBuffer;

s16* engine_buffer;


void audio_stop()
{
	ndspExit();
	free(engine_buffer);
	linearFree(audioBuffer);
}

void audio_init()
{
	// from 3ds filters example
	audioBuffer = (u32*) linearAlloc(SAMPLESPERBUF * BYTESPERSAMPLE * 2);
	
	ndspInit();

	ndspSetOutputMode(NDSP_OUTPUT_STEREO);

	ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
	ndspChnSetRate(0, SAMPLERATE);
	ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);
	
	// Output at 100% on the first pair of left and right channels.

	float mix[12];
	memset(mix, 0, sizeof(mix));
	mix[0] = 1.0;
	mix[1] = 1.0;
	ndspChnSetMix(0, mix);
	
	// We set up two wave buffers and alternate between the two

	memset(waveBuf,0,sizeof(waveBuf));
	waveBuf[0].data_vaddr = &audioBuffer[0];
	waveBuf[0].nsamples = SAMPLESPERBUF;
	waveBuf[1].data_vaddr = &audioBuffer[SAMPLESPERBUF];
	waveBuf[1].nsamples = SAMPLESPERBUF;

	ndspChnWaveBufAdd(0, &waveBuf[0]);
	ndspChnWaveBufAdd(0, &waveBuf[1]);	
	
	for (int n = 0; n < ARRAY_SIZE(sounds); n++)
	{
		sounds[n].active = false;
		sounds[n].loop = false;
		sounds[n].offset = 0;
		sounds[n].balance = 0;
		sounds[n].reverse = true;
		sounds[n].repeat = 0;
		sounds[n].repeated = 0;
		sounds[n].reverse = false;
		sounds[n].volume = 1.0;
		switch (n) {
			case CONGRATULATIONS:
				sounds[n].data = (s16 *)congratulations_Amy;
				sounds[n].length = CONGRATULATIONS_AMY_LENGTH/2;				
				break;
			case GAME_OVER:
				sounds[n].data = (s16 *)game_over_Amy;
				sounds[n].length = GAME_OVER_AMY_LENGTH/2;				
				break;
			case MESSAGE:
				sounds[n].data = (s16 *)blippy_31899_1;
				sounds[n].length = BLIPPY_31899_1_LENGTH/2;		
				sounds[n].repeat = 2;				
				break;
			case DAMAGE_REPORT:
				sounds[n].data = (s16 *)blippy_31899_1;
				sounds[n].length = BLIPPY_31899_1_LENGTH/2;
				break;
			case HIT:
				sounds[n].data = (s16 *)musket_explosion_6383;
				sounds[n].length = MUSKET_EXPLOSION_6383_LENGTH/2;				
				break;
			case ENGINES:
				engine_buffer = (s16 *)malloc(ENGINE_BUFFER_LEN*2);
				sounds[n].data = engine_buffer;
				sounds[n].length = ENGINE_BUFFER_LEN;
				for (int m = 0; m < ENGINE_BUFFER_LEN; m++)
				{
					engine_buffer[m] = (s16)(rand()% 65535) - 32767;
				}
				sounds[n].loop = true;
				break;
			case HWIN:
				sounds[n].data = (s16 *)power_on_39172;
				sounds[n].length = POWER_ON_39172_LENGTH/2;				
				break;
			case HWOUT:
				sounds[n].data = (s16 *)power_on_39172;
				sounds[n].length = POWER_ON_39172_LENGTH/2;
				sounds[n].reverse = true;
				break;
			case PHOTONT_FIRE_E:
				sounds[n].data = (s16 *)laserrocket_5984;
				sounds[n].length = LASERROCKET_5984_LENGTH/2;
				sounds[n].volume = 0.3;
				break;
			case PHOTONT_FIRE_L:
				sounds[n].data = (s16 *)blaster_2_81267;
				sounds[n].length = BLASTER_2_81267_LENGTH/2;
				sounds[n].balance = -1;
				sounds[n].volume = 0.5;
				break;
			case PHOTONT_FIRE_R:
				sounds[n].data = (s16 *)blaster_2_81267;
				sounds[n].length = BLASTER_2_81267_LENGTH/2;
				sounds[n].balance = 1;
				sounds[n].volume = 0.5;
				break;
			case EXPLOSION:
				sounds[n].data = (s16 *)hq_explosion_6288;
				sounds[n].length = HQ_EXPLOSION_6288_LENGTH/2;
				break;
			case TIC:
				sounds[n].data = (s16 *)blippy_31899_2;
				sounds[n].length = BLIPPY_31899_2_LENGTH/2;
				break;
			case DOUBLETIC:
				sounds[n].data = (s16 *)blippy_31899_2;
				sounds[n].length = BLIPPY_31899_2_LENGTH/2;
				sounds[n].repeat = 1;
				break;
			case DOCKING:
				sounds[n].data = (s16 *)docking_established_standby_vicki;
				sounds[n].length = DOCKING_ESTABLISHED_STANDBY_VICKI_LENGTH/2;
				break;
			case ALERT:
				sounds[n].data = (s16 *)warning_vicki;
				sounds[n].length = WARNING_VICKI_LENGTH/2;
				break;
			default: 
				sounds[n].data = NULL;
				sounds[n].length = 0;
				break;
		}
	}
}

s16 mix_samples(s16 s1, s16 s2) // from http://www.vttoth.com/CMS/index.php/technical-notes/68
{
	s32 mix; // mixed result will go here
	
	s32 a; // first sample (-32768..32767)
	s32 b; // second sample

	// Make both samples unsigned (0..65535)
	a = s1 + 32768;
	b = s2 + 32768;

	// Pick the equation
	if ((a < 32768) || (b < 32768)) {
		// first equation when both sources are less than middle of the dynamic range
		mix = a * b / 32768;
	} else {
		// second equation when one or both sources are loud
		mix = 2 * (a + b) - (a * b) / 32768 - 65536;
	}

	// Output is unsigned (0..65536) so convert back to signed (-32768..32767)
	if (mix >= 65536) mix = 65535;
	mix -= 32768;
	
	return (s16)mix;
}

void fill_buffer(void* audioBuffer, size_t size) 
{
	u32* dest = (u32*) audioBuffer;
	for (int i = 0; i < size; i++)
	{
		s16 sample_l = 0;
		s16 sample_r = 0;
		for (int n = 0; n < ARRAY_SIZE(sounds); n++)
		{
			if (sounds[n].active)
			{
				if (sounds[n].offset >= sounds[n].length)
				{
					sounds[n].offset = 0;
					if ((!sounds[n].loop) && (sounds[n].repeated-- <= 0))
					{
						sounds[n].active = false;
						continue;
					}	
				}
				s16 sample = (sounds[n].reverse)?
											((s16) ((float)sounds[n].data[sounds[n].length - sounds[n].offset++] * sounds[n].volume)):
											((s16) ((float)sounds[n].data[sounds[n].offset++] * sounds[n].volume));
				if (sounds[n].balance >= 0) sample_l = mix_samples(sample_l, sample);
				if (sounds[n].balance <= 0) sample_r = mix_samples(sample_r, sample);
			}
		}
		// Stereo samples are interleaved: left and right channels.
		dest[i] = (sample_l << 16) | (sample_r & 0xffff);
	}

	DSP_FlushDataCache(audioBuffer, size);
}

void checkSoundFX()
{
	if (waveBuf[fillBlock].status == NDSP_WBUF_DONE) 
		{	
			fill_buffer(waveBuf[fillBlock].data_pcm16, waveBuf[fillBlock].nsamples);
			ndspChnWaveBufAdd(0, &waveBuf[fillBlock]);
			fillBlock = !fillBlock;
		}	
}

void setSoundFX (enum sound_effects effect)
{
	sounds[effect].active = true;
	//sounds[effect].volume = 1.0;
	sounds[effect].offset = 0;
	//sounds[effect].balance = 0;
	sounds[effect].repeated = sounds[effect].repeat;
}

void setSoundFXPrefs (enum sound_effects effect, float volume, int balance)
{
	sounds[effect].volume = volume;
	sounds[effect].balance = balance;
}

float getSoundVol(enum sound_effects effect)
{
	return sounds[effect].volume;
}

void stopSoundFX (enum sound_effects effect)
{
	sounds[effect].active = false;
}
