#include <raylib.h>
#include "rmlib.h"
#include "racecar.h"
#include "profiles.h"


Profile DefaultProfile(void)
{
	Profile profile = (Profile){0};
	FillProfile(&profile, 0);
	return profile;
}

void FillProfile(Profile* out_profile, int done)
{
	if(done >= PROFILE_SIZE)
	{
		return;
	}

	Profile profile = (Profile){0};

	profile.name[0] = 'U';
	profile.name[1] = 's';
	profile.name[2] = 'e';
	profile.name[3] = 'r';

	profile.car_road_palette = 0;
	profile.car_drift_palette = 1;
	profile.car_grip_palette = 2;
	profile.car_terrain_palette = 3;

	profile.car_road_model = 0;
	profile.car_drift_model = 0;
	profile.car_grip_model = 0;
	profile.car_terrain_model = 0;

	profile.booleans_0 = 0b00101;

	profile.master_volume = 0;
	profile.sfx_volume = 0;
	profile.music_volume = 0;

	profile.camera_zoom = 1.2;

	if(done == 0)
	{
		*out_profile = profile;
	}
	else
	{
		unsigned char* out_prof = (unsigned char*)out_profile;
		unsigned char* prof = (unsigned char*)&profile;
		for(int i = done; i < PROFILE_SIZE; i++)
		{
			out_prof[i] = prof[i];
		}
	}
}

void PrintProfile(Profile* profile)
{
	TraceLog(LOG_INFO, "Profile Name %s", profile->name);
	TraceLog(LOG_INFO, "Car Road Palette    %i", profile->car_road_palette);
	TraceLog(LOG_INFO, "Car Drift Palette   %i", profile->car_drift_palette);
	TraceLog(LOG_INFO, "Car Grip Palette    %i", profile->car_grip_palette);
	TraceLog(LOG_INFO, "Car Terrain Palette %i", profile->car_terrain_palette);

	TraceLog(LOG_INFO, "Car Road Model    %i", profile->car_road_model);
	TraceLog(LOG_INFO, "Car Drift Model   %i", profile->car_drift_model);
	TraceLog(LOG_INFO, "Car Grip Model    %i", profile->car_grip_model);
	TraceLog(LOG_INFO, "Car Terrain Model %i", profile->car_terrain_model);

	//TraceLog(LOG_INFO, "%i", GetProfileBool(profile, ));
	TraceLog(LOG_INFO, "Screen Shake %i", GetProfileBool(profile, PRF_BOOL_SCREEN_SHAKE));
	TraceLog(LOG_INFO, "Cam Centered %i", GetProfileBool(profile, PRF_BOOL_CAM_CENTERED));
}

const char* ProfileFilename(unsigned char* dir, unsigned char name[PROFILE_NAME_LENGHT])
{
	unsigned int size = 0;
	unsigned int spaces = 0;
	for(int i = PROFILE_NAME_LENGHT - 1; i >= 0; i--)
	{
		if(name[i] == 0)
		{
			spaces++;
		}
		else
		{
			break;
		}
	}
	size = PROFILE_NAME_LENGHT - spaces;
	if(size == 0)
	{
		name[0] = 'U';
		name[1] = 's';
		name[2] = 'e';
		name[3] = 'r';
		name[4] = '\0';
		size = 5;
	}

	unsigned int dir_len = TextLength(dir);
	TraceLog(LOG_INFO, "MALLOC: profile file name");
	unsigned char* filename = (unsigned char*)_malloc(dir_len + size + 6);
	for(int i = 0; i < dir_len; i++)
	{
		filename[i] = dir[i];
	}
#ifdef WINDOWS
	filename[dir_len] = '\\';
#else
	filename[dir_len] = '/';
#endif
	for(int i = 0; i < size; i++)
	{
		filename[i + dir_len + 1] = name[i];
	}
	filename[dir_len + size + 1] = '.';
	filename[dir_len + size + 2] = 'p';
	filename[dir_len + size + 3] = 'r';
	filename[dir_len + size + 4] = 'f';
	filename[dir_len + size + 5] = '\0';
	//const char* FILENAME = (const char*)_realloc(filename, size);
	const char* FILENAME = (const char*)filename;
	return FILENAME;
}

Profile LoadProfile(const char* filename)
{
	int data_size;
	unsigned char* file_data = LoadFileData(filename, &data_size);
	Profile profile = {0};

	if(file_data != (void*)0)
	{
		if(data_size > PROFILE_SIZE)
		{
			// File doesn't match requirements
			// File conversion code here
			TraceLog(LOG_WARNING, "FILEIO: [%s] Profile is bigger than expected, will not load.", filename);
		}
		else if(data_size < PROFILE_SIZE)
		{
			TraceLog(LOG_INFO, "REALLOC: load profile");
			Profile* profile_save = (Profile*)_realloc(file_data, PROFILE_SIZE);
			FillProfile(profile_save, data_size);
			profile = *profile_save;

			TraceLog(LOG_WARNING, "FILEIO: [%s] Profile is from an older version!", filename);
			TraceLog(LOG_INFO, "FILEIO: [%s] Profile loaded successfully.", filename);
		}
		else
		{
			Profile* profile_save = (Profile*)file_data;
			profile = *profile_save;

			TraceLog(LOG_INFO, "FILEIO: [%s] Profile loaded successfully.", filename);
		}

		UnloadFileData(file_data);
	}

	TraceLog(LOG_INFO, "Loading profile ended.");
	return profile;
}

bool SaveProfile(Profile* profile, const char* filename)
{
	bool success = false;
	int data_size;
	int save_data_size;
	unsigned char* file_data = LoadFileData(filename, &data_size);
	unsigned char* save_file_data = (void*)0;

	if(file_data != (void*)0)
	{
		if(data_size != PROFILE_SIZE)
		{
			save_data_size = PROFILE_SIZE;
			TraceLog(LOG_INFO, "REALLOC: save profile");
			save_file_data = (unsigned char*)_realloc(file_data, save_data_size);

			if(save_file_data != (void*)0)
			{
				Profile* savefile = (Profile*)save_file_data;
				*savefile = *profile;
			}
			else
			{
				TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data.", filename);

				save_file_data = file_data;
				save_data_size = data_size;
			}
		}
		else
		{
			save_file_data = file_data;
			save_data_size = data_size;

			Profile* savefile = (Profile*)save_file_data;
			*savefile = *profile;
		}

		success = SaveFileData(filename, save_file_data, save_data_size);
		TraceLog(LOG_INFO, "FREE: profile save data");
		_free(save_file_data);
	}
	else
	{
		data_size = PROFILE_SIZE;
		TraceLog(LOG_INFO, "MALLOC: profile save data");
		file_data = (unsigned char*)_malloc(data_size);
		Profile* savefile = (Profile*)file_data;

		*savefile = *profile;

		success = SaveFileData(filename, file_data, data_size);
		UnloadFileData(file_data);
	}

	TraceLog(LOG_INFO, "Saving profile ended.");
	return success;
}

void LoadProfileDirectory(FilePathList* fpl, unsigned char* dir)
{
	if(fpl->count != 0) UnloadDirectoryFiles(*fpl);
	*fpl = LoadDirectoryFilesEx(TextFormat("%s", dir), ".prf", false);
}

void MoveProfileSelectorCursor(unsigned int count, int* current, int move)
{
	*current = *current + move;
	if(*current < 0)
	{
		*current = 0;
	}
	if(*current > count)
	{
		*current = count;
	}
}

void DrawProfileSelector(FilePathList fpl, int current)
{
	const Vector2 SIZE = (Vector2){512, 608};
	const Vector2 POSITION = (Vector2){256, 16};

	DrawRectangle(POSITION.x - 4, POSITION.y - 4, SIZE.x + 8, SIZE.y + 8, BLACK);
	DrawRectangle(POSITION.x, POSITION.y, SIZE.x, SIZE.y, LIGHTGRAY);
	DrawRectangle(POSITION.x + 24, POSITION.y + 24, SIZE.x - 48, SIZE.y - 80, GRAY);

	int page = current / PROFILE_SELECTOR_PAGE_ITEMS;
	DrawText(TextFormat("%i/%i", page + 1, fpl.count / PROFILE_SELECTOR_PAGE_ITEMS + 1), POSITION.x + 24, POSITION.y + SIZE.y - 44, 32, BLACK);

	for(int i = 0; i < PROFILE_SELECTOR_PAGE_ITEMS; i++)
	{
		int item = i + page * PROFILE_SELECTOR_PAGE_ITEMS;
		Color color = BLACK;
		if(item == current)
		{
			color = RAYWHITE;
		}
		if(item >= fpl.count)
		{
			DrawText("+ New Profile", POSITION.x + 32, POSITION.y + 32 + 32 * i, 32, color);
			break;
		}
		unsigned char* filepath = fpl.paths[item];
#ifdef WINDOWS
		unsigned int pos = TextFindLastChar(filepath, '\\'), len = TextLength(filepath);
#else
		unsigned int pos = TextFindLastChar(filepath, '/'), len = TextLength(filepath);
#endif
		//TraceLog(LOG_INFO, "MALLOC: draw profile selector");
		unsigned char* file = (unsigned char*)_malloc(len - pos);
		for(int j = 0; j < len - pos - 1; j++)
		{
			file[j] = filepath[j + pos + 1];
		}
		file[len - pos - 1] = '\0';
		DrawText(TextFormat("%s", file), POSITION.x + 32, POSITION.y + 32 + 32 * i, 32, color);
		//TraceLog(LOG_INFO, "FREE: draw profile selector");
		_free(file);
	}
}

bool GetProfileBool(Profile* profile, ProfileBoolean n)
{
	return profile->booleans_0 & (long long int)0b1 << n;
}

void SetProfileBool(Profile* profile, ProfileBoolean n, bool state)
{
	if(state)
	{
		profile->booleans_0 = profile->booleans_0 | (long long int)0b1 << n;
	}
	else
	{
		profile->booleans_0 = profile->booleans_0 & ~((long long int)0b1 << n);
	}
}

