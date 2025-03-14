#ifndef RM_PROFILES
#define RM_PROFILES

#include <raylib.h>
#include "rmlib.h"
#include "racecar.h"


#define PROFILE_DIRECTORY "Profiles\0"
#define PROFILE_NAME_LENGHT 32
#define PROFILE_SELECTOR_PAGE_ITEMS 16
#define PROFILE_SIZE sizeof(Profile)


// Once there is more than 64 bools here, you will need to add another "long long int booleans_x;"
typedef enum ProfileBoolean {
	PRF_BOOL_SCREEN_SHAKE = 0,
	PRF_BOOL_CAM_CENTERED,
	PRF_BOOL_GHOST_ENABLED,
	PRF_BOOL_BLOCKMIXING,
	PRF_BOOL_HIDE_PARTY_TIME,
} ProfileBoolean;

typedef struct Profile {
	unsigned char name[PROFILE_NAME_LENGHT];
	unsigned char car_road_palette, car_drift_palette, car_grip_palette, car_terrain_palette, 
		       car_road_model, car_drift_model, car_grip_model, car_terrain_model;
	long long int booleans_0;
	char master_volume, sfx_volume, music_volume;
	float camera_zoom;
} Profile;


Profile DefaultProfile(void);
void FillProfile(Profile* out_profile, int done);
void PrintProfile(Profile* profile);
const char* ProfileFilename(unsigned char* dir, unsigned char name[PROFILE_NAME_LENGHT]);

Profile LoadProfile(const char* filename);
bool SaveProfile(Profile* profile, const char* filename);

void LoadProfileDirectory(FilePathList* fpl, unsigned char* dir);
void MoveProfileSelectorCursor(unsigned int count, int* current, int move);
void DrawProfileSelector(FilePathList fpl, int current);

bool GetProfileBool(Profile* profile, ProfileBoolean n);
void SetProfileBool(Profile* profile, ProfileBoolean n, bool state);


#endif
