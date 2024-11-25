#include <raylib.h>
#include <time.h>
#include "rmlib.h"
#include "track.h"
#include "profiles.h"
#include "demos.h"

Demo* InitDemo(void)
{
	if(DEMO_DEBUG)
	{
		TraceLog(LOG_INFO, "DEMO: Initializing new demo");
	}
	TraceLog(LOG_INFO, "MALLOC: new demo");
	Demo* demo;
	demo = (Demo*)_malloc(sizeof(Demo) + DEMO_INPUT_ALLOC_SIZE);
	*demo = (Demo){0};
	demo->input_amount = 1;
	if(DEMO_DEBUG)
	{
		TraceLog(LOG_INFO, "DEMO: Init %i", demo);
		TraceLog(LOG_INFO, " -- -- -- -- -- -- -- -- ");
	}
	return demo;
}

void ClearDemo(Demo* demo)
{
	if(DEMO_DEBUG) TraceLog(LOG_INFO, "DEMO: Clearing demo file %i", demo);
	if(demo != PNULL)
	{
		TraceLog(LOG_INFO, "FREE: clear demo");
		_free(demo);
	}
	else if(DEMO_DEBUG) TraceLog(LOG_WARNING, "DEMO: Demo pointer already null pointer");
	if(DEMO_DEBUG) TraceLog(LOG_INFO, " -- -- -- -- -- -- -- -- ");
}

Demo* CopyDemo(Demo* demo)
{
	if(DEMO_DEBUG) TraceLog(LOG_INFO, "DEMO: Copying demo file %i", demo);
	TraceLog(LOG_INFO, "MALLOC: copy demo");
	Demo* copy = (Demo*)_malloc(sizeof(Demo) + demo->input_amount * DEMO_INPUT_ALLOC_SIZE);
	*copy = *demo;
	for(int i = 0; i < demo->input_amount * DEMO_INPUT_ALLOC_SIZE; i++)
	{
		copy->inputs[i] = demo->inputs[i];
	}
	if(DEMO_DEBUG)
	{
		TraceLog(LOG_INFO, "New demo at %i", copy);
		TraceLog(LOG_INFO, " -- -- -- -- -- -- -- -- ");
	}
	return copy;
}

Demo* AllocateNewDemoSpace(Demo* demo)
{
	if(DEMO_DEBUG) TraceLog(LOG_INFO, "DEMO: Allocating more memory %i", demo);

	demo->input_amount++;
	unsigned int size = sizeof(Demo) + demo->input_amount * DEMO_INPUT_ALLOC_SIZE;

	TraceLog(LOG_INFO, "REALLOC: more_demo %i", size);
	Demo* new_demo = (Demo*)_realloc(demo, size);
	if(new_demo == PNULL)
	{
		TraceLog(LOG_WARNING, "DEMO: Failed to realloc demo");
		demo->input_amount = 0;
	}

	if(DEMO_DEBUG)
	{
		TraceLog(LOG_INFO, "DEMO: demo now at %i", demo);
		TraceLog(LOG_INFO, " -- -- -- -- -- -- -- -- ");
	}

	return new_demo;
}

Demo* RecordDemoInput(Demo* demo, unsigned char input)
{
	if(demo->input_amount == 0)
	{
		return demo;
	}
	demo->inputs[demo->input_current] = input;
	demo->input_current++;
	if(demo->input_current >= demo->input_amount * DEMO_INPUT_ALLOC_SIZE - 8)
	{
		demo = AllocateNewDemoSpace(demo);
	}
	return demo;
}

void StartDemo(Demo* demo)
{
	demo->input_current = 0;
}

void StopDemo(Demo* demo)
{
	demo->input_current = demo->input_amount * DEMO_INPUT_ALLOC_SIZE;
}

unsigned char GetDemoInput(Demo* demo)
{
	if(demo->input_amount * DEMO_INPUT_ALLOC_SIZE <= demo->input_current)
	{
		return 0;
	}
	unsigned char input = demo->inputs[demo->input_current];
	demo->input_current++;
	return input;
}

void CopyNameToDemo(Demo* demo, unsigned char name[PROFILE_NAME_LENGHT])
{
	for(int i = 0; i < PROFILE_NAME_LENGHT; i++)
	{
		demo->profile[i] = name[i];
	}
}

const char* DemoFilename(unsigned char* demo_dir, const char* track_filepath, unsigned char prof_name[PROFILE_NAME_LENGHT])
{
	unsigned int dir_len = TextLength(demo_dir),
		     track_len = TextLength(track_filepath) - 4,
		     prof_len = PROFILE_NAME_LENGHT,
		     path_len = 0;
	for(int i = PROFILE_NAME_LENGHT - 1; i >= 0; i--)
	{
		if(prof_name[i] == 0)
		{
			prof_len--;
		}
		else
		{
		       	break;
		}
	}
	if(prof_len == 0)
	{
		prof_name[0] = 'U';
		prof_name[1] = 'n';
		prof_name[2] = 'n';
		prof_name[3] = 'a';
		prof_name[4] = 'm';
		prof_name[5] = 'e';
		prof_name[6] = 'd';
		prof_name[7] = '\0';
		prof_len = 8;
	}
	path_len = dir_len + track_len + prof_len + 6;

	TraceLog(LOG_INFO, "MALLOC: demo_filename %i", path_len);
	unsigned char* filepath = (unsigned char*)_malloc(path_len);
	for(int i = 0; i < dir_len; i++)
	{
		filepath[i] = demo_dir[i];
	}
	filepath[dir_len] = '/';
	for(int i = 0; i < track_len; i++)
	{
		filepath[i + dir_len + 1] = track_filepath[i];
	}
	filepath[dir_len + track_len + 1] = '_';
	for(int i = 0; i < prof_len; i++)
	{
		filepath[i + dir_len + track_len + 2] = prof_name[i];
	}
	filepath[path_len - 4] = '.';
	filepath[path_len - 3] = 'd';
	filepath[path_len - 2] = 'm';
	filepath[path_len - 1] = '\0';

	if(DEMO_DEBUG) TraceLog(LOG_INFO, " -- -- -- -- -- -- -- -- ");

	return (const char*)filepath;
}

int SizeOfDemoSave(Demo* demo)
{
	int size = sizeof(Demo) + DEMO_CHECKSUM_SIZE;
	size += DEMO_INPUT_ALLOC_SIZE * demo->input_amount;
	return size;
}

void ChecksumDemo(Demo* demo, unsigned char checksum[DEMO_CHECKSUM_SIZE])
{
	unsigned int current = 0, cur_check = 0;
	while(current < demo->input_amount * DEMO_INPUT_ALLOC_SIZE)
	{
		checksum[cur_check] += demo->inputs[current];
		cur_check += 1;
		if(cur_check == DEMO_CHECKSUM_SIZE)
		{
			cur_check = 0;
		}
		current += 1;
	}
}

bool ChecksumsMatch(unsigned char checksum_1[DEMO_CHECKSUM_SIZE], unsigned char checksum_2[DEMO_CHECKSUM_SIZE])
{
	for(int i = 0; i < DEMO_CHECKSUM_SIZE; i++)
	{
		if(checksum_1[i] - checksum_2[i] != 0)
		{
			return false;
		}
	}
	return true;
}

DemoSave* LoadDemo(bool* result, const char* filename)
{
	if(DEMO_DEBUG) TraceLog(LOG_INFO, "DEMO: Loading demo");
	int data_size;
	unsigned char* file_data = LoadFileData(filename, &data_size);
	TraceLog(LOG_INFO, "MALLOC: demosave");
	DemoSave* demosave = (DemoSave*)_malloc(sizeof(DemoSave));

	if(file_data == PNULL)
	{
		if(DEMO_DEBUG)
		{
			TraceLog(LOG_INFO, "DEMO: Demo could not be loaded. %s", filename);
			TraceLog(LOG_INFO, " -- -- -- -- -- -- -- -- ");
		}
		*result = false;
		return demosave;
	}
	unsigned int track_name_len = 1;
	while(file_data[track_name_len] != 0 && track_name_len < data_size - sizeof(DemoSave))
	{
		track_name_len++;
	}
	TraceLog(LOG_INFO, "MALLOC: load demo track_name %i", track_name_len);
	demosave->track_name = (unsigned char*)_malloc(track_name_len);
	for(int i = 0; i < track_name_len; i++)
	{
		demosave->track_name[i] = (unsigned char)file_data[i];
	}

	for(int i = 0; i < DEMO_CHECKSUM_SIZE; i++)
	{
		demosave->checksum[i] = file_data[track_name_len + i];
	}
	Demo* save_demo = (Demo*)(file_data + track_name_len + DEMO_CHECKSUM_SIZE);
	unsigned char checksum[DEMO_CHECKSUM_SIZE];
	ChecksumDemo(save_demo, checksum);
	if(!ChecksumsMatch(checksum, demosave->checksum))
	{
		// Checksum Failed, file is inaccurate
		TraceLog(LOG_WARNING, "FILEIO: [%s] Demo file is corrupted, checksums do not match.", filename);
	}

	TraceLog(LOG_INFO, "MALLOC: load demo");
	demosave->demo = (Demo*)_malloc(sizeof(Demo) + save_demo->input_amount * DEMO_INPUT_ALLOC_SIZE);
	*demosave->demo = *save_demo;
	for(int i = 0; i < save_demo->input_amount * DEMO_INPUT_ALLOC_SIZE; i++)
	{
		demosave->demo->inputs[i] = save_demo->inputs[i];
	}

	TraceLog(LOG_INFO, "FILEIO: [%s] Demo loaded successfully.", filename);

	UnloadFileData(file_data);

	if(DEMO_DEBUG)
	{
		TraceLog(LOG_INFO, "DEMO: Loaded demo at %i", demosave->demo);
		TraceLog(LOG_INFO, " -- -- -- -- -- -- -- -- ");
	}

	*result = true;

	return demosave;
}

bool SaveDemoTime(Demo* demo, unsigned char* track_name)
{
	return SaveDemo(demo, track_name, TextFormat("Demos/%s_%i.dm\0", track_name, time(0)));
}

bool SaveDemo(Demo* demo, unsigned char* track_name, const char* filename)
{
	if(DEMO_DEBUG) TraceLog(LOG_INFO, "DEMO: Saving demo");
	bool success = false;
	int data_size;
	int save_data_size;
	unsigned char* file_data = LoadFileData(filename, &data_size);
	unsigned char* save_file_data = PNULL;
	unsigned int trname = TextLength(track_name) + 1;

	if(file_data != PNULL)
	{
		save_data_size = SizeOfDemoSave(demo) + trname;
		if(data_size != save_data_size)
		{
			TraceLog(LOG_INFO, "REALLOC: save demo %i", save_data_size);
			save_file_data = (unsigned char*)_realloc(file_data, save_data_size);

			if(save_file_data != PNULL)
			{
				for(int i = 0; i < trname - 1; i++)
				{
					save_file_data[i] = (unsigned char)track_name[i];
				}
				save_file_data[trname - 1] = 0; 
				
				unsigned char checksum[DEMO_CHECKSUM_SIZE];
				ChecksumDemo(demo, checksum);
				for(int i = 0; i < DEMO_CHECKSUM_SIZE; i++)
				{
					save_file_data[trname + i] = checksum[i];
				}

				Demo* savefile = (Demo*)(save_file_data + trname + DEMO_CHECKSUM_SIZE);
				*savefile = *demo;
				for(int i = 0; i < demo->input_amount * DEMO_INPUT_ALLOC_SIZE; i++)
				{
					savefile->inputs[i] = demo->inputs[i];
				}
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

			for(int i = 0; i < trname - 1; i++)
			{
				save_file_data[i] = (unsigned char)track_name[i];
			}
			save_file_data[trname - 1] = 0; 
			
			unsigned char checksum[DEMO_CHECKSUM_SIZE];
			ChecksumDemo(demo, checksum);
			for(int i = 0; i < DEMO_CHECKSUM_SIZE; i++)
			{
				save_file_data[trname + i] = checksum[i];
			}

			Demo* savefile = (Demo*)(save_file_data + trname + DEMO_CHECKSUM_SIZE);
			*savefile = *demo;
			for(int i = 0; i < demo->input_amount * DEMO_INPUT_ALLOC_SIZE; i++)
			{
				savefile->inputs[i] = demo->inputs[i];
			}
		}

		success = SaveFileData(filename, save_file_data, save_data_size);
		UnloadFileData(save_file_data);
	}
	else
	{
		data_size = SizeOfDemoSave(demo) + trname;
		TraceLog(LOG_INFO, "MALLOC: new demo save");
		file_data = (unsigned char*)_malloc(data_size);

		for(int i = 0; i < trname - 1; i++)
		{
			file_data[i] = (unsigned char)track_name[i];
		}
		file_data[trname - 1] = 0; 
			
		unsigned char checksum[DEMO_CHECKSUM_SIZE];
		ChecksumDemo(demo, checksum);
		for(int i = 0; i < DEMO_CHECKSUM_SIZE; i++)
		{
			file_data[trname + i] = checksum[i];
		}

		Demo* savefile = (Demo*)(file_data + trname + DEMO_CHECKSUM_SIZE);
		*savefile = *demo;
		for(int i = 0; i < demo->input_amount * DEMO_INPUT_ALLOC_SIZE; i++)
		{
			savefile->inputs[i] = demo->inputs[i];
		}

		success = SaveFileData(filename, file_data, data_size);
		UnloadFileData(file_data);
	}

	if(DEMO_DEBUG) TraceLog(LOG_INFO, " -- -- -- -- -- -- -- -- ");

	return success;

}

void LoadDemoDirectory(FilePathList* fpl, unsigned char* demo_dir)
{
	if(fpl->count != 0) UnloadDirectoryFiles(*fpl);
	*fpl = LoadDirectoryFiles(TextFormat("%s", demo_dir));
}


