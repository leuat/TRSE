//this file generated with SNES GSS tool

#define SOUND_EFFECTS_ALL	0

#define MUSIC_ALL	2

//music effect aliases

enum {
	MUS_UNTITLED=0,
	MUS_UNTITLED=1
};

//music names

const char* const musicNames[MUSIC_ALL]={
	"UNTITLED",	//0
	"UNTITLED"	//1
};

extern const unsigned char spc700_code_1[];
extern const unsigned char spc700_code_2[];
extern const unsigned char music_1_data[];
extern const unsigned char music_2_data[];

const unsigned char* const musicData[MUSIC_ALL]={
	music_1_data,
	music_2_data
};
