//this file generated with SNES GSS tool

#define SOUND_EFFECTS_ALL	0

#define MUSIC_ALL	1

//music effect aliases

enum {
	MUS_MOONBEAMS=0
};

//music names

const char* const musicNames[MUSIC_ALL]={
	"MOONBEAMS"	//0
};

extern const unsigned char spc700_code_1[];
extern const unsigned char spc700_code_2[];
extern const unsigned char music_1_data[];

const unsigned char* const musicData[MUSIC_ALL]={
	music_1_data
};
