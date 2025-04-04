#ifndef RM_DEMOS
#define RM_DEMOS

#include <raylib.h>
#include <time.h>
#include "rmlib.h"
#include "track.h"
#include "profiles.h"

#define DEMO_DEBUG false

#define DEMO_INPUT_ALLOC_SIZE 512
#define DEMO_CHECKSUM_SIZE 32

#define DEMO_DIRECTORY "demos\0"


typedef struct Demo {
	unsigned char profile[PROFILE_NAME_LENGHT],
		      car_palette,
		      car_model;
	double time;
	unsigned int input_current;
	unsigned int input_amount;
	unsigned char inputs[];
} Demo;

typedef struct DemoSave {
	bool result;
	unsigned char* track_name;
	unsigned char checksum[DEMO_CHECKSUM_SIZE];
	Demo* demo;
} DemoSave;


Demo* InitDemo(void);
void ClearDemo(Demo* demo);
Demo* CopyDemo(Demo* demo);
Demo* AllocateNewDemoSpace(Demo* demo);

Demo* RecordDemoInput(Demo* demo, unsigned char input);
void StartDemo(Demo* demo);
void StopDemo(Demo* demo);
unsigned char GetDemoInput(Demo* demo);

void CopyNameToDemo(Demo* demo, unsigned char name[PROFILE_NAME_LENGHT]);
const char* DemoFilename(unsigned char* demo_dir, const char* track_filepath, unsigned char prof_name[PROFILE_NAME_LENGHT]);

int SizeOfDemoSave(Demo* demo);
void ChecksumDemo(Demo* demo, unsigned char checksum[DEMO_CHECKSUM_SIZE]);
bool ChecksumsMatch(unsigned char checksum_1[DEMO_CHECKSUM_SIZE], unsigned char checksum_2[DEMO_CHECKSUM_SIZE]);

DemoSave* LoadDemo(const char* filename);
bool SaveDemoTime(Demo* demo, unsigned char* track_name);
bool SaveDemo(Demo* demo, unsigned char* track_name, const char* filename);

void LoadDemoDirectory(FilePathList* fpl, unsigned char* demo_dir);

#endif
