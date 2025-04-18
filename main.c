#include <sys/stat.h>
	// Used for mkdir().
#include <raylib.h>
#include "rmlib.h"
#include "blocks.h"
#include "racecar.h"
#include "camera.h"
#include "track.h"
#include "profiles.h"
#include "demos.h"
#include "asset.h"
#include "ui.h"


#ifdef WINDOWS

#define VALIDATE_DEMO_FILE "demos\\validation.dm\0"

#define SFX_START_BLEEP "sounds\\start_bleep_0.wav\0"
#define SFX_CRASH_SMALL "sounds\\crash_small.wav\0"
#define SFX_CRASH "sounds\\crash_big.wav\0"
#define SFX_ENGINE_ROAD "sounds\\car_road.wav\0"
#define SFX_ENGINE_DRIFT "sounds\\car_drift.wav\0"
#define SFX_ENGINE_GRIP "sounds\\car_grip.wav\0"
#define SFX_ENGINE_TERRA "sounds\\car_terra.wav\0"
#define SFX_CHECKPOINT "sounds\\checkpoint.wav\0"

#define SFX_TICK "sounds\\tick.wav\0"
#define SFX_CLICK "sounds\\click.wav\0"
#define SFX_BACK "sounds\\back.wav\0"

#define SFX_ROTATE "sounds\\rotate.wav\0"
#define SFX_PULL_DOWN "sounds\\pull_down.wav\0"
#define SFX_PULL_UP "sounds\\pull_up.wav\0"

#else

#define VALIDATE_DEMO_FILE "demos/validation.dm\0"

#define SFX_START_BLEEP "sounds/start_bleep_0.wav\0"
#define SFX_CRASH_SMALL "sounds/crash_small.wav\0"
#define SFX_CRASH "sounds/crash_big.wav\0"
#define SFX_ENGINE_ROAD "sounds/car_road.wav\0"
#define SFX_ENGINE_DRIFT "sounds/car_drift.wav\0"
#define SFX_ENGINE_GRIP "sounds/car_grip.wav\0"
#define SFX_ENGINE_TERRA "sounds/car_terra.wav\0"
#define SFX_CHECKPOINT "sounds/checkpoint.wav\0"

#define SFX_TICK "sounds/tick.wav\0"
#define SFX_CLICK "sounds/click.wav\0"
#define SFX_BACK "sounds/back.wav\0"

#define SFX_ROTATE "sounds/rotate.wav\0"
#define SFX_PULL_DOWN "sounds/pull_down.wav\0"
#define SFX_PULL_UP "sounds/pull_up.wav\0"

#endif


typedef enum GameScreen {PROFILES, MENU, EDITOR, RACE, OPTIONS} GameScreen;
typedef enum MenuOption {MENU_PLAY, MENU_EDITOR, MENU_PARTY, MENU_OPTIONS, MENU_EXIT, MENU_RACE = 10, MENU_DEMO, MENU_PLAY_EXIT} MenuOption;
typedef enum EditorOption {EDITOR_EXIT = 9, EDITOR_LOAD, EDITOR_SAVE, EDITOR_CLEAR, EDITOR_ENVIROMENT, EDITOR_CAR, EDITOR_PAGE_JUMP, EDITOR_MEDALS, EDITOR_VALIDATE,} EditorOption;
typedef enum PopupType {POPUP_OFF, POPUP_EDITOR_EXIT, POPUP_EDITOR_CLEAR, POPUP_VALIDATE, POPUP_NO_START, POPUP_RESET_MEDALS, POPUP_OVERRIDE_TRACK} PopupType;
typedef enum FileListType {FL_OFF, FL_TRACK, FL_DEMO} FileListType;
typedef enum DemoPlay {DEMO_OFF, DEMO_INIT, DEMO_PLAY, DEMO_GHOST_INIT, DEMO_GHOST_PLAY} DemoPlay;

void CarSetVis(Racecar* car, Profile* profile, DefaultCar type)
{
	switch(type)
	{
		case CAR_ROAD:
			car->palette = profile->car_road_palette;
			car->model = profile->car_road_model;
			break;
		case CAR_DRIFT:
			car->palette = profile->car_drift_palette;
			car->model = profile->car_drift_model;
			break;
		case CAR_GRIP:
			car->palette = profile->car_grip_palette;
			car->model = profile->car_grip_model;
			break;
		case CAR_TERRAIN:
			car->palette = profile->car_terrain_palette;
			car->model = profile->car_terrain_model;
			break;
	}
}

void UpdateVolume(Profile* profile, float* sfx, float* music)
{
	float master = AudioVolume(profile->master_volume);
	*sfx = AudioVolume(profile->sfx_volume) * master;
	*music = AudioVolume(profile->music_volume) * master;
	if(DEBUG) TraceLog(LOG_INFO, "sfx: %g", *sfx);
	if(DEBUG) TraceLog(LOG_INFO, "music: %g", *music);
}

void UpdateMenuSFX(float sfx, Sound tick, Sound click, Sound back)
{
	SetSoundVolume(tick, sfx);
	SetSoundVolume(click, sfx);
	SetSoundVolume(back, sfx);
}

int main(void)
{
	InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "RAYMANIA");
	InitAudioDevice();

	SetMasterVolume(4.0);

	if(DEBUG) TraceLog(LOG_INFO, "Master: %f", GetMasterVolume());

	//TraceLog(LOG_INFO, "Size of %i.", sizeof(Asset));
	//TraceLog(LOG_INFO, "Size of Blocks in Track %i.", sizeof(Block) * MAX_BLOCK_AMOUNT);

	SetTargetFPS(60);
	HideCursor();
	SetExitKey(KEY_NULL);

#ifdef WINDOWS
	TraceLog(LOG_INFO, "OS: Windows");
#elifdef LINUX
	TraceLog(LOG_INFO, "OS: Linux");
#else
	TraceLog(LOG_INFO, "OS: Other");
#endif // WINDOWS

	if(!DEBUG)
	{
		RaylibLogo();
		TabinLogo();
	}
	bool intro = false;

#ifdef _IO_H_
	if(!DirectoryExists(DEMO_DIRECTORY))
	{
		mkdir(DEMO_DIRECTORY);
	}
	if(!DirectoryExists(TRACK_DIRECTORY))
	{
		mkdir(TRACK_DIRECTORY);
	}
	if(!DirectoryExists(TextFormat("%s/%s", DEMO_DIRECTORY, TRACK_DIRECTORY)))
	{
		mkdir(TextFormat("%s/%s", DEMO_DIRECTORY, TRACK_DIRECTORY));
	}
	if(!DirectoryExists(PROFILE_DIRECTORY))
	{
		mkdir(PROFILE_DIRECTORY);
		intro = true;
	}
#else
	if(!DirectoryExists(DEMO_DIRECTORY))
	{
		mkdir(DEMO_DIRECTORY, 0b111111111);
	}
	if(!DirectoryExists(TRACK_DIRECTORY))
	{
		mkdir(TRACK_DIRECTORY, 0b111111111);
	}
	if(!DirectoryExists(TextFormat("%s/%s", DEMO_DIRECTORY, TRACK_DIRECTORY)))
	{
		mkdir(TextFormat("%s/%s", DEMO_DIRECTORY, TRACK_DIRECTORY), 0b111111111);
	}
	if(!DirectoryExists(PROFILE_DIRECTORY))
	{
		mkdir(PROFILE_DIRECTORY, 0b111111111);
		intro = true;
	}
#endif

	// GENERIC VAR

	GameScreen current_game_screen = PROFILES;

	bool exit = false;
	bool stop_inputs = false;

	RMInput input = (RMInput){0};
	RMInput demo_input = (RMInput){0};

	float input_block = 0.0;

	CameraRM camera = InitCamera();
	camera.data.target = (Vector2){3968, 3968};
	camera.fade = 1.0;
	const float CAM_ZOOM_SPEED = 0.10;
	float shake_time = 0.0;

	FilePathList fpl = (FilePathList){0};
	bool load_file_list = false;
	unsigned char file_list_active = 0;
	int selected_file = 0;

	Track track;
	ClearTrack(&track);

	Demo* demo = InitDemo();
	Demo* ghost_demo = InitDemo();

	Block blocks[MAX_BLOCK_AMOUNT] = {0};
	ClearPlacedBlocks(blocks);

	BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS] = {0};
	BlockWallArray dblock_walls[MAX_LOADED_BLOCK_WALLS] = {0};

	Vector2int load_placement = PositionToPlacement(camera.data.target); //(Vector2int){0, 0};
	Vector2int d_placement = (Vector2int){0, 0};

	PopupType popup = POPUP_OFF;
	bool popup_confirmed = false;
	int popup_opt = 0;

	unsigned char inputing_name = 0;
	unsigned char inputing_number = 0;
	unsigned char in_type = 0;
	unsigned int in_num = 0;
	unsigned int name_lenght = 0;

	// PROFILE VAR

	Profile profile = DefaultProfile();

	bool load_profiles = true;
	bool entering_profile_name = false;
	unsigned char profile_name[PROFILE_NAME_LENGHT] = {0};

	int current_profile = 0;

	// MENU VAR

	bool reset_menu = false;

	MenuOption menu_option = MENU_PLAY;

	Vector2 camera_velocity = Vector2Zero();
	Vector2 vel_sign = Vector2Zero();

	// RACE VAR

	bool reset_race = false;
	bool partial_reset_race = false;

	CarStats car_stats = DefaultStats(track.car);

	Racecar car = InitRacecar(&car_stats);
	car.position = Vector2Zero();
	car.rotation = UP_VECTOR;

	SkidLinePoint skid_line[SKID_LINE_COUNT] = {0};

	unsigned int checkpoints_gotten = 0;
	Vector2int checkpoints[MAX_CHECKPOINTS] = {0};
	Vector2 check_pos = Vector2Zero();
	Vector2 check_rot = UP_VECTOR;

	Racecar dcar = InitRacecar(&car_stats);
	dcar.position = Vector2Zero();
	dcar.rotation = UP_VECTOR;

	unsigned int dcheck_got = 0;
	Vector2int dcheck[MAX_CHECKPOINTS] = {0};
	Vector2 dcheck_pos = Vector2Zero();
	Vector2 dcheck_rot = UP_VECTOR;

	const double COUNTDOWN_TIME = 1.5;
	int countdown_digit = 0;
	double game_time = 0.0;
	double timer = 0.0;
	double start_time = 0.0;
	bool start_countdown = true;
	bool finished = false;

	bool paused = false;
	int pause_option = 0;
	unsigned char playing_demo = DEMO_OFF;

	bool race_showcase = false;

	// EDITOR VAR

	bool reset_editor = false;

	PieceInfo cursor_info = (PieceInfo){0};
	cursor_info.placement = (Vector2int){15, 15};

	bool placing_pieces = true;

	Block held_piece[BLOCKS_PER_PIECE] = {0}, piece_catalogue[PIECE_CATALOGUE_LENGHT] = {0};
	unsigned int piece_catalogue_page[9] = {0};
	unsigned int piece_catalogue_page_num = PIECE_CATALOGUE_START_PAGE;
	unsigned int piece_catalogue_item = 0;
	float piece_catalogue_pulled = 0.0;

	bool validating_track = false;
	bool saving_track = false;
	bool holding_start = false;
	unsigned char editor_four_option_selector = 0, efos_opt = 0;

	unsigned int overlaping_blocks = 0;

	// OPTIONS VAR

	bool reset_options = false;
	bool save_options = true;
	bool back_to_opt = false;
	unsigned char options_current_car = 0;
	unsigned char options_customization = 0;

	int options_current = 0;
	int options_page = 0;
	int options_max = 0;

	// PARTY VAR

	bool party_mode = false;
	char party_current_menu = 0;
	int party_current_opt = 0;

	unsigned int party_count = 2;
	Profile* party_profiles = PNULL;
	unsigned int party_timer_base = 10;
	double* party_timers = PNULL;
	double* party_best_times = PNULL;

	bool reset_party = false;
	bool party_draw = false;
	bool dnf = false;

	// TRACKS SELECTOR

	TraceLog(LOG_INFO, "MALLOC: track_dir");
	unsigned char* track_dir = (unsigned char*)_malloc(TextLength(TRACK_DIRECTORY));
	TextCopy(track_dir, TRACK_DIRECTORY);
	TraceLog(LOG_INFO, "MALLOC: demo_dir");
	unsigned char* demo_dir = (unsigned char*)_malloc(TextLength(DEMO_DIRECTORY));
	TextCopy(demo_dir, DEMO_DIRECTORY);
	bool skip_validate_check = false;

	// TRACK

	unsigned char track_name[TRACK_NAME_LENGHT] = {0};
       	track_name[0] = 'M';
	track_name[1] = 'E';
	track_name[2] = 'N';
	track_name[3] = 'U';

	track = LoadTrack(TrackFileName(track_dir, track_name));
	TrackBlockmixed(&track, GetProfileBool(&profile, PRF_BOOL_BLOCKMIXING));
	MakeTrackBlocks(&track, blocks);

	// AUDIO VAR

	float sfx = AudioVolume(0), music = AudioVolume(0);
	// MENU
	Sound sfx_tick, sfx_click, sfx_back;
	// EDITOR
	Sound sfx_rotate, sfx_pull_down, sfx_pull_up;
	// RACE
	Sound sfx_crash, sfx_crash_small, sfx_start_bleep, sfx_checkpoint;
	Music sfx_engine;

	sfx_tick = LoadSound(SFX_TICK);
	sfx_click = LoadSound(SFX_CLICK);
	sfx_back = LoadSound(SFX_BACK);
	UpdateMenuSFX(sfx, sfx_tick, sfx_click, sfx_back);

	// CONTROLS INTRO

	//TraceLog(LOG_INFO, "INTRO: %i", intro);
	bool intro_loop = intro;
	float intro_fade = 1.0;

	while(intro_loop && !WindowShouldClose())
	{
		game_time = GetTime();
		RMInput menu_input = (RMInput){0};
		CheckKeyboardInput(&input);
		CheckMenuInput(&menu_input, input, &input_block);

		if(InputHeld(menu_input, INPUT_UP))
		{
			if(options_current > 0)
			{
				options_current--;
				PlaySound(sfx_tick);
			}
		}
		if(InputHeld(menu_input, INPUT_DOWN))
		{
			if(options_current < 2)
			{
				options_current++;
				PlaySound(sfx_tick);
			}
		}
		char* volume;
		if(options_current == 0)
		{
			volume = &profile.master_volume;
		}
		if(options_current == 1)
		{
			volume = &profile.sfx_volume;
		}
		if(options_current == 2)
		{
			volume = &profile.music_volume;
		}
		if(InputHeld(menu_input, INPUT_RIGHT))
		{
			if(*volume < 10)
			{
				*volume += 1;
				PlaySound(sfx_tick);
			}
			UpdateVolume(&profile, &sfx, &music);
			UpdateMenuSFX(sfx, sfx_tick, sfx_click, sfx_back);
		}
		if(InputHeld(menu_input, INPUT_LEFT))
		{
			if(*volume > -11)
			{
				*volume -= 1;
				PlaySound(sfx_tick);
			}
			UpdateVolume(&profile, &sfx, &music);
			UpdateMenuSFX(sfx, sfx_tick, sfx_click, sfx_back);
		}
		if(InputPressed(input, INPUT_ENTER))
		{
			intro_fade = -FRAME;
			PlaySound(sfx_click);
		}

		if(intro_fade > 0.0)
		{
			intro_fade -= FRAME;
			if(intro_fade < 0.0)
			{
				intro_fade = 0.0;
			}
		}
		if(intro_fade < 0.0)
		{
			if(intro_fade <= -1.0)
			{
				intro_loop = false;
			}
			else
			{
				intro_fade -= FRAME;
				if(intro_fade < -1.0)
				{
					intro_fade = -1.0;
				}
			}
		}

		BeginDrawing();

		if(intro_loop)
		{
			Vector2 pos = (Vector2){0, 0};
			DrawBackgroundTris(pos, 1.0, game_time * 0.25, RM_WHITE0, RM_WHITE1);
			DrawText("VOLUME SETTINGS", 96, 56, 80, BLACK);

			if(profile.master_volume == -11) DrawText("MASTER: OFF", 160, 160, 32, BLACK);
			else DrawText(TextFormat("MASTER: %i", profile.master_volume), 160, 160, 32, BLACK);
			if(profile.sfx_volume == -11) DrawText("SFX: OFF", 160, 192, 32, BLACK);
			else DrawText(TextFormat("SFX: %i", profile.sfx_volume), 160, 192, 32, BLACK);
			if(profile.music_volume == -11) DrawText("MUSIC: OFF", 160, 224, 32, BLACK);
			else DrawText(TextFormat("MUSIC: %i", profile.music_volume), 160, 224, 32, BLACK);

			DrawText(">", 128, 160 + options_current * 32, 32, BLACK);
			DrawText("Up and down arrow to move cursor.", 160, 288, 32, BLACK);
			DrawText("Left and right arrow to change volume.", 160, 320, 32, BLACK);
			DrawText("Enter to confirm.", 160, 352, 32, BLACK);

			if(intro_fade > 0.0)
			{
				Color col = rmc(RM_WHITE0);
				col.a = (int){intro_fade * 255.0};
				DrawRectangle(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y, col);
			}
			if(intro_fade < 0.0)
			{
				Color col = rmc(RM_WHITE0);
				col.a = (int){-intro_fade * 255.0};
				DrawRectangle(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y, col);
			}
		}
		else
		{
			ClearBackground(rmc(RM_WHITE0));
		}

		EndDrawing();
	}

	// GAME LOOP

	while(!WindowShouldClose() && !exit)
	{

		game_time = GetTime();

		RMInput menu_input = (RMInput){0};

		if(playing_demo == DEMO_PLAY && !paused)
		{
			CheckKeyboardInput(&input);
			unsigned char d_input = GetDemoInput(demo);
			input.current = (d_input & 0b00101111) | (input.current & 0b11010000);
		}
		else
		{
			CheckKeyboardInput(&input);
		}
		CheckMenuInput(&menu_input, input, &input_block);

	if(popup)
	{
		if(InputPressed(input, INPUT_BACK))
		{
			if(popup_opt == 0)
			{
				popup_confirmed = true;
				PlaySound(sfx_click);
			}
			else
			{
				popup_opt = 0;
				PlaySound(sfx_tick);
			}
		}
		if(InputPressed(input, INPUT_RIGHT))
		{
			popup_opt = 0;
			PlaySound(sfx_tick);
		}
		if(InputPressed(input, INPUT_LEFT))
		{
			popup_opt = 1;
			PlaySound(sfx_tick);
		}
		if(InputPressed(input, INPUT_ENTER))
		{
			popup_confirmed = true;
			PlaySound(sfx_click);
		}
		if(InputPressed(input, INPUT_ESC))
		{
			popup_opt = 0;
			popup = POPUP_OFF;
			PlaySound(sfx_back);
		}
		if(popup_confirmed && popup_opt)
		{
			if(popup == POPUP_EDITOR_EXIT)
			{
				current_game_screen = MENU;
				reset_menu = true;
			}
			if(popup == POPUP_EDITOR_CLEAR)
			{
				ClearTrack(&track);
				TrackBlockmixed(&track, GetProfileBool(&profile, PRF_BOOL_BLOCKMIXING));
				ClearPlacedBlocks(blocks);
			}
			if(popup == POPUP_VALIDATE)
			{
				skip_validate_check = true;
				if(!party_mode)
				{
					validating_track = true;
				}
			}
			if(popup == POPUP_NO_START)
			{
				current_game_screen = RACE;
				reset_race = true;
				validating_track = true;
				race_showcase = true;
			}
			if(popup == POPUP_RESET_MEDALS)
			{
				ResetMedalTimes(&track);
			}
			if(popup == POPUP_OVERRIDE_TRACK)
			{
				SaveTrack(&track, TrackFileName(track_dir, track_name), profile.name);
			}
		}
	}
	switch(current_game_screen)
	{
	case PROFILES:
		if(load_profiles)
		{
			current_profile = 0;

			LoadProfileDirectory(&fpl, PROFILE_DIRECTORY);

			load_profiles = false;
		}
		FadeIn(&camera, 0.05);
		if(entering_profile_name)
		{
		}
		else
		{
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(MoveProfileSelectorCursor(fpl.count, &current_profile, -1))
				{
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(MoveProfileSelectorCursor(fpl.count, &current_profile, 1))
				{
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_LEFT))
			{
				if(MoveProfileSelectorCursor(fpl.count, &current_profile, -PROFILE_SELECTOR_PAGE_ITEMS))
				{
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_RIGHT))
			{
				if(MoveProfileSelectorCursor(fpl.count, &current_profile, PROFILE_SELECTOR_PAGE_ITEMS))
				{
					PlaySound(sfx_tick);
				}
			}
			if(InputPressed(input, INPUT_ENTER))
			{
				if(current_profile == fpl.count)
				{
					entering_profile_name = true;
					inputing_name = 1;
					name_lenght = PROFILE_NAME_LENGHT - 1;
					ClearString(profile_name, PROFILE_NAME_LENGHT);
				}
				else
				{
					current_game_screen = MENU;
					if(back_to_opt)
					{
						current_game_screen = OPTIONS;
					}
					const char* path = (const char*)fpl.paths[current_profile];
					if(party_mode)
					{
						party_profiles[party_current_opt - 1] = LoadProfile(path);
					}
					else
					{
						profile = LoadProfile(path);
						UpdateVolume(&profile, &sfx, &music);
						UpdateMenuSFX(sfx, sfx_tick, sfx_click, sfx_back);
						if(back_to_opt)
						{
							reset_options = true;
						}
						else
						{
							reset_menu = true;
						}
						save_options = true;
					}
					UnloadDirectoryFiles(fpl);
					fpl.count = 0;
					//PrintProfile(profile);
				}
				PlaySound(sfx_click);
			}
			if(InputPressed(input, INPUT_BACK))
			{
				if(party_mode)
				{
					party_profiles[party_current_opt - 1] = DefaultProfile();
				}
				else
				{
					profile = DefaultProfile();
					UpdateVolume(&profile, &sfx, &music);
					UpdateMenuSFX(sfx, sfx_tick, sfx_click, sfx_back);
					if(back_to_opt)
					{
						reset_options = true;
					}
					else
					{
						reset_menu = true;
					}
					save_options = false;
				}
				current_game_screen = MENU;
				if(back_to_opt)
				{
					current_game_screen = OPTIONS;
				}
				UnloadDirectoryFiles(fpl);
				fpl.count = 0;
				PlaySound(sfx_back);
			}
			if(party_mode)
			{
				if(InputPressed(input, INPUT_ESC))
				{
					current_game_screen = MENU;
					UnloadDirectoryFiles(fpl);
					fpl.count = 0;
					PlaySound(sfx_back);
				}
			}
		}
		break;
	case MENU:
		if(reset_menu)
		{
			reset_menu = false;

			camera.cam_pos = camera.data.target;

			camera_velocity = Vector2Zero();
			vel_sign = (Vector2){AirQuotesNoise(game_time, true), AirQuotesNoise(game_time, false)};

			playing_demo = DEMO_OFF;
			ClearDemo(demo);
			demo = InitDemo();
			ClearDemo(ghost_demo);
			ghost_demo = InitDemo();
			if(menu_option == MENU_RACE)
			{
				file_list_active = FL_TRACK;
				load_file_list = true;
			}
			else if(menu_option == MENU_DEMO)
			{
				file_list_active = FL_DEMO;
				load_file_list = true;
			}
		}

		ZoomCameraSmooth(&camera, 0.5, CAM_ZOOM_SPEED);

		const Vector2 cam_bounds = (Vector2){BLOCK_SIZE * 4, BLOCK_SIZE * (TRACK_GRID_SIZE - 4)};
		Vector2 speed = (Vector2){0.10, 0.10};
		if(camera.cam_pos.x < cam_bounds.x)
		{
			vel_sign.x = AirQuotesNoise(game_time, true);
			speed.x = 0.40;
		}
		if(camera.cam_pos.x > cam_bounds.y)
		{
			vel_sign.x = -AirQuotesNoise(game_time, true);
			speed.x = 0.40;
		}
		if(camera.cam_pos.y < cam_bounds.x)
		{
			vel_sign.y = AirQuotesNoise(game_time, false);
			speed.y = 0.40;
		}
		if(camera.cam_pos.y > cam_bounds.y)
		{
			vel_sign.y = -AirQuotesNoise(game_time, false);
			speed.y = 0.40;
		}
		camera_velocity = Vector2Add(camera_velocity, Vector2Multiply(vel_sign, speed));
		camera.cam_pos = Vector2Add(camera.cam_pos, camera_velocity);
		MoveCameraSmooth(&camera, camera.cam_pos, 0.05);

		if(absf(camera_velocity.x) >= 96)
		{
			vel_sign.x = -sign(camera_velocity.x) * AirQuotesNoise(game_time, true);
		}
		if(absf(camera_velocity.y) >= 96)
		{
			vel_sign.y = -sign(camera_velocity.y) * AirQuotesNoise(game_time, false);
		}

		load_placement = PositionToPlacement(camera.data.target);

		if(file_list_active || popup)
		{
		}
		else if(party_mode && party_current_menu == 0)
		{
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(party_current_opt == MAX_PARTY_PROFILES + 1)
				{
					party_current_opt = party_count;
					PlaySound(sfx_tick);
				}
				else if(party_current_opt > 0)
				{
					party_current_opt--;
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(party_current_opt < MAX_PARTY_PROFILES + 2)
				{
					party_current_opt++;
					if(party_current_opt > party_count && party_current_opt < MAX_PARTY_PROFILES)
					{
						party_current_opt = MAX_PARTY_PROFILES + 1;
					}
					PlaySound(sfx_tick);
				}
			}
			if(party_current_opt == 0)
			{
				if(InputHeld(menu_input, INPUT_RIGHT))
				{
					if(party_count < MAX_PARTY_PROFILES)
					{
						party_count++;
						party_profiles = _realloc(party_profiles, sizeof(Profile) * party_count);
						party_profiles[party_count - 1] = DefaultProfile();
						PlaySound(sfx_tick);
					}
				}
				if(InputHeld(menu_input, INPUT_LEFT))
				{
					if(party_count > 1)
					{
						party_count--;
						party_profiles = _realloc(party_profiles, sizeof(Profile) * party_count);
						PlaySound(sfx_tick);
					}
				}
			}
			if(InputPressed(input, INPUT_BACK))
			{
				if(party_current_opt == MAX_PARTY_PROFILES + 2)
				{
					party_mode = false;
					PlaySound(sfx_click);
				}
				else
				{
					party_current_opt = MAX_PARTY_PROFILES + 2;
					PlaySound(sfx_tick);
				}
			}
			else if(InputPressed(input, INPUT_ENTER))
			{
				if(party_current_opt == 0)
				{
					party_count++;
					if(party_count > MAX_PARTY_PROFILES)
					{
						party_count = 1;
					}
					party_profiles = _realloc(party_profiles, sizeof(Profile) * party_count);
					if(party_count != 1)
					{
						party_profiles[party_count - 1] = DefaultProfile();
					}
					PlaySound(sfx_tick);
				}
				else if(party_current_opt == MAX_PARTY_PROFILES + 1)
				{
					party_current_menu = 1;
					party_current_opt = 0;
					PlaySound(sfx_click);
				}
				else if(party_current_opt == MAX_PARTY_PROFILES + 2)
				{
					party_mode = false;
					PlaySound(sfx_click);
				}
				else
				{
					current_game_screen = PROFILES;
					load_profiles = true;
					PlaySound(sfx_click);
				}
			}
			if(!party_mode)
			{
					_free(party_profiles);
			}
		}
		else if(party_mode && party_current_menu == 1)
		{
			if(party_current_opt == 0)
			{
				if(InputHeld(menu_input, INPUT_RIGHT))
				{
					if(party_timer_base < MAX_PARTY_TIMER)
					{
						party_timer_base++;
						PlaySound(sfx_tick);
					}
				}
				if(InputHeld(menu_input, INPUT_LEFT))
				{
					if(party_timer_base > MIN_PARTY_TIMER)
					{
						party_timer_base--;
						PlaySound(sfx_tick);
					}
				}
			}
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(party_current_opt > 0)
				{
					party_current_opt--;
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(party_current_opt < 3)
				{
					party_current_opt++;
					PlaySound(sfx_tick);
				}
			}
			if(InputPressed(input, INPUT_BACK))
			{
				if(party_current_opt == 3)
				{
					party_current_menu = 0;
					party_current_opt = 0;
					PlaySound(sfx_click);
				}
				else
				{
					party_current_opt = 3;
					PlaySound(sfx_tick);
				}
			}
			else if(InputPressed(input, INPUT_ENTER))
			{
				if(party_current_opt == 0)
				{
					party_timer_base++;
					if(party_timer_base > MAX_PARTY_TIMER)
					{
						party_timer_base = MIN_PARTY_TIMER;
					}
					PlaySound(sfx_tick);
				}
				if(party_current_opt == 1)
				{
					file_list_active = FL_TRACK;
					load_file_list = true;
					reset_party = true;
					PlaySound(sfx_click);
				}
				else if(party_current_opt == 2)
				{
					current_game_screen = EDITOR;
					reset_editor = true;
					reset_party = true;
					PlaySound(sfx_click);
				}
				else if(party_current_opt == 3)
				{
					party_current_menu = 0;
					party_current_opt = 0;
					PlaySound(sfx_click);
				}
			}
		}
		else if(party_mode && party_current_menu == 2)
		{
			if(InputPressed(input, INPUT_BACK) || InputPressed(input, INPUT_ENTER))
			{
				party_current_menu = 1;
				party_current_opt = 0;
				PlaySound(sfx_click);
			}
		}
		else
		{
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(menu_option > MENU_PLAY && menu_option != MENU_RACE)
				{
					menu_option -= 1;
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(menu_option < MENU_EXIT)
				{
					menu_option += 1;
					PlaySound(sfx_tick);
				}
				else if(menu_option >= MENU_RACE && menu_option < MENU_PLAY_EXIT)
				{
					menu_option += 1;
					PlaySound(sfx_tick);
				}
			}

			if(InputPressed(input, INPUT_BACK))
			{
				if(menu_option >= MENU_RACE)
				{
					if(menu_option == MENU_PLAY_EXIT)
					{
						menu_option = MENU_PLAY;
						PlaySound(sfx_click);
					}
					else
					{
						menu_option = MENU_PLAY_EXIT;
						PlaySound(sfx_tick);
					}
				}
				else
				{
					if(menu_option == MENU_EXIT)
					{
						exit = true;
						PlaySound(sfx_click);
					}
					else
					{
						menu_option = MENU_EXIT;
						PlaySound(sfx_tick);
					}
				}
			}
			if(InputPressed(input, INPUT_ENTER))
			{
				PlaySound(sfx_click);
				switch(menu_option)
				{
					case(MENU_PLAY):
						menu_option = MENU_RACE;
						break;
					case(MENU_EDITOR):
						current_game_screen = EDITOR;
						reset_editor = true;
						break;
					case(MENU_PARTY):
						party_mode = true;
						party_count = 2;
						party_profiles = _malloc(sizeof(Profile) * party_count);
						party_profiles[0] = profile;
						party_profiles[1] = DefaultProfile();
						break;
					case(MENU_OPTIONS):
						current_game_screen = OPTIONS;
						reset_options = true;
						break;
					case(MENU_EXIT):
						exit = true;
						break;
					case(MENU_RACE):
						file_list_active = FL_TRACK;
						load_file_list = true;
						break;
					case(MENU_DEMO):
						file_list_active = FL_DEMO;
						load_file_list = true;
						break;
					case(MENU_PLAY_EXIT):
						menu_option = MENU_PLAY;
						break;
				}
			}
		}
		break;
	case EDITOR:
		if(reset_editor)
		{
			reset_editor = false;

			placing_pieces = true;
			piece_catalogue_page_num = PIECE_CATALOGUE_START_PAGE;
			piece_catalogue_item = 0;
			piece_catalogue_pulled = 0.0;
			editor_four_option_selector = 0;
			efos_opt = 0;

			PiecesInPage(piece_catalogue_page, piece_catalogue_page_num);
			LoadPieceCataloguePage(piece_catalogue, piece_catalogue_page);

			cursor_info = (PieceInfo){0};
			cursor_info.placement = PositionToPlacement(camera.data.target);
			MoveEditorCursor(&cursor_info, 0, 0);
			GetPiece(&cursor_info, held_piece);
			holding_start = false;

			sfx_rotate = LoadSound(SFX_ROTATE);
			SetSoundVolume(sfx_rotate, sfx);
			sfx_pull_down = LoadSound(SFX_PULL_DOWN);
			SetSoundVolume(sfx_pull_down, sfx);
			sfx_pull_up = LoadSound(SFX_PULL_UP);
			SetSoundVolume(sfx_pull_up, sfx);
		}

		ZoomCameraSmooth(&camera, 0.5, CAM_ZOOM_SPEED);
		MoveCameraSmooth(&camera, CheckPosition(cursor_info.placement), 0.20);

		float pcp_dir = 0.0;

		if(popup > 0 || inputing_number > 0 || saving_track)
		{
		}
		else if(editor_four_option_selector > 0)
		{
			bool end_efos = false;
			if(InputHeld(menu_input, INPUT_LEFT))
			{
				if(efos_opt > 0)
				{
					efos_opt -= 1;
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_RIGHT))
			{
				if(efos_opt < 3)
				{
					efos_opt += 1;
					PlaySound(sfx_tick);
				}
			}
			if(InputPressed(input, INPUT_ESC) || InputPressed(input, INPUT_BACK))
			{
				end_efos = true;
				PlaySound(sfx_back);
			}
			else if(InputPressed(input, INPUT_ENTER))
			{
				end_efos = true;
				switch(editor_four_option_selector)
				{
					case(1):
						track.car = efos_opt;
						ResetMedalTimes(&track);
					break;
					case(2):
						track.env = efos_opt;
						ResetMedalTimes(&track);
					break;
					case(3):
						if(efos_opt == 3)
						{
							popup = POPUP_RESET_MEDALS;
						}
						else
						{
							inputing_number = 1;
							in_type = 1;
							if(efos_opt == 0) in_num = track.medal_bronz * 1000;
							if(efos_opt == 1) in_num = track.medal_silver * 1000;
							if(efos_opt == 2) in_num = track.medal_gold * 1000;
						}
						end_efos = false;
					break;
				}
				PlaySound(sfx_click);
			}
			if(end_efos)
			{
				editor_four_option_selector = 0;
			}
		}
		else if(placing_pieces && !file_list_active)
		{
			if(InputPressed(input, INPUT_R))
			{
				if(cursor_info.rot == 3)
				{
					cursor_info.rot = 0;
				}
				else
				{
					cursor_info.rot += 1;
				}
				GetPiece(&cursor_info, held_piece);
				PlaySound(sfx_rotate);
			}

			if(InputHeld(menu_input, INPUT_ENTER))
			{
				AddPiece(&track, blocks, &cursor_info);
				overlaping_blocks = OverlapingPieces(&track, cursor_info.placement);
				ResetMedalTimes(&track);
			}
			if(InputHeld(menu_input, INPUT_BACK))
			{
				DeletePiece(&track, blocks, cursor_info.placement);
				overlaping_blocks = OverlapingPieces(&track, cursor_info.placement);
				ResetMedalTimes(&track);
			}
			if(InputHeld(menu_input, INPUT_LEFT))
			{
				MoveEditorCursor(&cursor_info, -1, 0);
				GetPiece(&cursor_info, held_piece);
				overlaping_blocks = OverlapingPieces(&track, cursor_info.placement);
			}
			if(InputHeld(menu_input, INPUT_RIGHT))
			{
				MoveEditorCursor(&cursor_info, 1, 0);
				GetPiece(&cursor_info, held_piece);
				overlaping_blocks = OverlapingPieces(&track, cursor_info.placement);
			}
			if(InputHeld(menu_input, INPUT_UP))
			{
				MoveEditorCursor(&cursor_info, 0, -1);
				GetPiece(&cursor_info, held_piece);
				overlaping_blocks = OverlapingPieces(&track, cursor_info.placement);
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				MoveEditorCursor(&cursor_info, 0, 1);
				GetPiece(&cursor_info, held_piece);
				overlaping_blocks = OverlapingPieces(&track, cursor_info.placement);
			}

			if(InputPressed(input, INPUT_ESC))
			{
				placing_pieces = false;
				PlaySound(sfx_pull_up);
			}

			if(piece_catalogue_pulled > 0.0)
			{
				pcp_dir = -1.0;
			}
		}
		else if (!file_list_active)
		{
			if(InputPressed(input, INPUT_BACK))
			{
				if(piece_catalogue_item == 9)
				{
					popup = POPUP_EDITOR_EXIT;
					PlaySound(sfx_click);
				}
				else
				{
					piece_catalogue_item = 9;
					PlaySound(sfx_tick);
				}
			}
			if(InputPressed(input, INPUT_ESC))
			{
				placing_pieces = true;
				PlaySound(sfx_pull_down);
			}
			if(piece_catalogue_item >= 0 && piece_catalogue_item < 9)
			{
				if(InputHeld(menu_input, INPUT_LEFT))
				{
					if(piece_catalogue_item == 0)
					{
						if(piece_catalogue_page_num > 1)
						{
							piece_catalogue_page_num--;
							PiecesInPage(piece_catalogue_page, piece_catalogue_page_num);
							LoadPieceCataloguePage(piece_catalogue, piece_catalogue_page);
							piece_catalogue_item = 8;
							PlaySound(sfx_tick);
						}
					}
					else 
					{
						piece_catalogue_item--;
						PlaySound(sfx_tick);
					}
				}
				if(InputHeld(menu_input, INPUT_RIGHT))
				{
					if(piece_catalogue_item == 8)
					{
						if(piece_catalogue_page_num < PIECE_CATALOGUE_PAGE_AMOUNT - 1)
						{
							piece_catalogue_page_num++;
							PiecesInPage(piece_catalogue_page, piece_catalogue_page_num);
							LoadPieceCataloguePage(piece_catalogue, piece_catalogue_page);
							piece_catalogue_item = 0;
							PlaySound(sfx_tick);
						}
					}
					else 
					{
						piece_catalogue_item++;
						PlaySound(sfx_tick);
					}
				}
				if(InputHeld(menu_input, INPUT_DOWN))
				{
					piece_catalogue_item += 9;
					PlaySound(sfx_tick);
				}

				if(InputPressed(input, INPUT_ENTER))
				{
					cursor_info.id = piece_catalogue_page[piece_catalogue_item];
					GetPiece(&cursor_info, held_piece);
					placing_pieces = true;
					holding_start = false;
					for(int i = 0; i < BLOCKS_PER_PIECE; i++)
					{
						if(held_piece[i].area.type == TYPE_START)
						{
							holding_start = true;
						}
					}
					PlaySound(sfx_click);
					PlaySound(sfx_pull_down);
				}
			}
			else
			{
				if(InputHeld(menu_input, INPUT_LEFT))
				{
					if(piece_catalogue_item > 9)
					{
						piece_catalogue_item--;
						PlaySound(sfx_tick);
					}
				}
				if(InputHeld(menu_input, INPUT_RIGHT))
				{
					if(piece_catalogue_item < 17)
					{
						piece_catalogue_item++;
						PlaySound(sfx_tick);
					}
				}
				if(InputHeld(menu_input, INPUT_UP))
				{
					piece_catalogue_item -= 9;
					PlaySound(sfx_tick);
				}

				if(InputPressed(input, INPUT_ENTER))
				{
					switch(piece_catalogue_item)
					{
						case(EDITOR_EXIT):
							popup = POPUP_EDITOR_EXIT;
							break;
						case(EDITOR_LOAD):
							file_list_active = FL_TRACK;
							load_file_list = true;
							break;
						case(EDITOR_SAVE):
							saving_track = true;
							inputing_name = 1;
							name_lenght = TRACK_NAME_LENGHT - 1;
							ClearString(track_name, TRACK_NAME_LENGHT);
							break;
						case(EDITOR_CLEAR):
							popup = POPUP_EDITOR_CLEAR;
							break;
						case(EDITOR_ENVIROMENT):
							editor_four_option_selector = 2;
							efos_opt = track.env;
							break;
						case(EDITOR_CAR):
							editor_four_option_selector = 1;
							efos_opt = track.car;
							break;
						case(EDITOR_PAGE_JUMP):
							inputing_number = 1;
							in_type = 2;
							in_num = piece_catalogue_page_num;
							break;
						case(EDITOR_MEDALS):
							editor_four_option_selector = 3;
							efos_opt = 0;
							break;
						case(EDITOR_VALIDATE):
							if(track.has_start)
							{
								current_game_screen = RACE;
								reset_race = true;
								if(party_mode)
								{
									race_showcase = true;
								}
								else
								{
									validating_track = true;
									playing_demo = DEMO_OFF;
								}
							}
							break;
					}
					PlaySound(sfx_click);
				}
			}

			if(piece_catalogue_pulled < 1.0)
			{
				pcp_dir = 1.0;
			}
		}
		load_placement = cursor_info.placement;

		if(pcp_dir != 0.0)
		{
			float target = (pcp_dir + 1.0) * 0.5;
			float dif = absf(target - piece_catalogue_pulled);
			piece_catalogue_pulled += 0.10 * pcp_dir * dif;
			if(piece_catalogue_pulled < 0.005) piece_catalogue_pulled = 0.0;
			if(piece_catalogue_pulled > 1.0) piece_catalogue_pulled = 1.0;
		}
		if(current_game_screen != EDITOR)
		{
			StopSound(sfx_rotate);
			UnloadSound(sfx_rotate);
			StopSound(sfx_pull_down);
			UnloadSound(sfx_pull_down);
			StopSound(sfx_pull_up);
			UnloadSound(sfx_pull_up);
		}
		break;
	case RACE:
		bool demo_realloced = false;
		if(reset_party)
		{
			reset_party = false;
			party_timers = _malloc(sizeof(double) * party_count);
			party_best_times = _malloc(sizeof(double) * party_count);
			for(int i = 0; i < party_count; i++)
			{
				party_timers[i] = (double)party_timer_base * 30.0;
				party_best_times[i] = 0.0;
			}
			current_profile = 0;
		}
		Profile* playing_profile;
		if(party_mode)
		{
			playing_profile = &party_profiles[current_profile];
		}
		else
		{
			playing_profile = &profile;
		}
		if(reset_race)
		{
			if(party_mode) TraceLog(LOG_INFO, "%s", playing_profile->name);
			partial_reset_race = true;
			reset_race = false;

			car_stats = DefaultStats(track.car);

			sfx_crash = LoadSound(SFX_CRASH);
			SetSoundVolume(sfx_crash, sfx);
			sfx_crash_small = LoadSound(SFX_CRASH_SMALL);
			SetSoundVolume(sfx_crash_small, sfx);
			sfx_start_bleep = LoadSound(SFX_START_BLEEP);
			SetSoundVolume(sfx_start_bleep, sfx);
			sfx_checkpoint = LoadSound(SFX_CHECKPOINT);
			SetSoundVolume(sfx_checkpoint, sfx);
			if(track.car == CAR_ROAD)
			{
				sfx_engine = LoadMusicStream(SFX_ENGINE_ROAD);
			}
			else if(track.car == CAR_DRIFT)
			{
				sfx_engine = LoadMusicStream(SFX_ENGINE_DRIFT);
			}
			else if(track.car == CAR_GRIP)
			{
				sfx_engine = LoadMusicStream(SFX_ENGINE_GRIP);
			}
			else
			{
				sfx_engine = LoadMusicStream(SFX_ENGINE_TERRA);
			}
			SetMusicVolume(sfx_engine, sfx);
			PlayMusicStream(sfx_engine);
		}
		if(partial_reset_race)
		{
			partial_reset_race = false;

			ResetRacecar(&car, track.start_pos, track.start_rot, car_stats.size);
			ResetRacecar(&dcar, track.start_pos, track.start_rot, car_stats.size);
			CarSetVis(&car, &profile, track.car);
			CarSetVis(&dcar, &profile, track.car);
			MoveCameraInstant(&camera, car.position);
			ClearSkidLine(skid_line);

			dcheck_got = 0;
			dcheck_pos = track.start_pos;
			dcheck_rot = track.start_rot;

			checkpoints_gotten = 0;
			check_pos = track.start_pos;
			check_rot = track.start_rot;

			timer = 0.0;
			start_time = game_time;
			countdown_digit = -1;

			start_countdown = true;
			paused = false;
			stop_inputs = false;
			finished = false;

			load_placement = RacecarPlacement(&car);
			d_placement = RacecarPlacement(&car);
			LoadNearbyBlockWalls(blocks, block_walls, load_placement);
			LoadNearbyBlockWalls(blocks, dblock_walls, d_placement);

			if(party_mode)
			{
			}
			else if(playing_demo == DEMO_OFF)
			{
				ClearDemo(demo);
				demo = InitDemo();
				CopyNameToDemo(demo, playing_profile->name);
			}
			else if(playing_demo == DEMO_PLAY)
			{
				StartDemo(demo);
				playing_demo = DEMO_INIT;
			}
			else if(playing_demo == DEMO_GHOST_PLAY)
			{
				ClearDemo(demo);
				demo = InitDemo();
				CopyNameToDemo(demo, playing_profile->name);
				StartDemo(ghost_demo);
				playing_demo = DEMO_GHOST_INIT;
			}
		}

		bool cp_reset = false;

		if(race_showcase)
		{
			ZoomCameraSmooth(&camera, 0.70, CAM_ZOOM_SPEED);
		}
		else
		{
			ZoomCameraSmooth(&camera, playing_profile->camera_zoom, CAM_ZOOM_SPEED);
		}
		if(paused)
		{
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(pause_option > 0)
				{
					pause_option--;
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(pause_option < 3)
				{
					pause_option++;
					PlaySound(sfx_tick);
				}
			}
			bool pause_exit = false;
			if(InputPressed(input, INPUT_BACK))
			{
				if(pause_option == 3)
				{
					pause_exit = true;
					PlaySound(sfx_click);
				}
				else
				{
					pause_option = 3;
					PlaySound(sfx_tick);
				}
			}
			if(InputPressed(input, INPUT_ENTER))
			{
				if(pause_option == 0)
				{
					paused = false;
				}
				if(pause_option == 1)
				{
					cp_reset = true;
					paused = false;
				}
				if(pause_option == 2)
				{
					partial_reset_race = true;
					paused = false;
				}
				if(pause_option == 3)
				{
					pause_exit = true;
				}
				PlaySound(sfx_click);
			}
			if(InputPressed(input, INPUT_ESC))
			{
				paused = false;
				PlaySound(sfx_back);
			}
			if(pause_exit)
			{
				if(validating_track)
				{
					current_game_screen = EDITOR;
					reset_editor = true;
					validating_track = false;
				}
				else
				{
					current_game_screen = MENU;
					reset_menu = true;
				}
			}
		}
		else if(race_showcase)
		{
			if(InputPressed(input, INPUT_ESC))
			{
				current_game_screen = MENU;
				reset_menu = true;
				PlaySound(sfx_back);
			}
			if(InputPressed(input, INPUT_ENTER))
			{
				race_showcase = false;
				start_time = game_time;
				//PlaySound(sfx);
			}

			Vector2 cam_pos = camera.data.target;
			Vector2 offset = (Vector2){0, 0};

			if(InputHeld(input, INPUT_DOWN))
			{
				offset.y += 16;
			}
			if(InputHeld(input, INPUT_UP))
			{
				offset.y -= 16;
			}
			if(InputHeld(input, INPUT_RIGHT))
			{
				offset.x += 16;
			}
			if(InputHeld(input, INPUT_LEFT))
			{
				offset.x -= 16;
			}
			cam_pos = Vector2Add(cam_pos, offset);

			MoveCameraInstant(&camera, cam_pos);
			load_placement = PositionToPlacement(camera.data.target);
		}
		else if(start_countdown)
		{
			if(InputPressed(input, INPUT_ESC))
			{
				paused = true;
				pause_option = 0;
				//PlaySound(sfx_pause);
			}

			MoveCameraSmooth(&camera, car.position, 0.10);

			int time = (int)((game_time - start_time) / COUNTDOWN_TIME * 3);
			if(start_time + COUNTDOWN_TIME <= game_time)
			{
				start_countdown = false;
				if(playing_demo == DEMO_INIT)
				{
					playing_demo = DEMO_PLAY;
				}
				if(playing_demo == DEMO_GHOST_INIT && GetProfileBool(playing_profile, PRF_BOOL_GHOST_ENABLED))
				{
					playing_demo = DEMO_GHOST_PLAY;
					demo_input = (RMInput){0};
				}
				SetSoundPitch(sfx_start_bleep, 1.5);
				PlaySound(sfx_start_bleep);
			}
			else if(countdown_digit != time)
			{
				countdown_digit = time;
				SetSoundPitch(sfx_start_bleep, 1.0);
				PlaySound(sfx_start_bleep);
			}
		}
		else
		{
			if(InputPressed(input, INPUT_ESC))
			{
				paused = true;
				pause_option = 0;
				//PlaySound(sfx_pause);
			}

			if(!finished)
			{
				dnf = false;
			}

			if(!stop_inputs)
			{
				timer += FRAME;
				if(party_mode)
				{
					party_timers[current_profile] -= FRAME;
					if(party_timers[current_profile] <= 0.0)
					{
						dnf = true;
					}
				}
				if(playing_demo != DEMO_PLAY)
				{
					demo = RecordDemoInput(demo, input.current, &demo_realloced);
				}
			}
			else
			{
				input.current = input.current & 0b11110000;
			}

			Vector2int new_placement = RacecarPlacement(&car);
			if(!Vector2intEqual(load_placement, new_placement))
			{
				load_placement = new_placement;
				LoadNearbyBlockWalls(blocks, block_walls, load_placement);
			}

			float previous_speed = Vector2Length(car.velocity);
			MetaInfo meta;
			meta.checkpoint = false;
			meta.finish = false;
			meta = ProcessRacecar(&car, &car_stats, blocks, block_walls, input, track.env);
			AddSkidLinePoint(&car, skid_line);

			float speed_change = absf(previous_speed - Vector2Length(car.velocity));

			if(GetProfileBool(playing_profile, PRF_BOOL_SCREEN_SHAKE) && speed_change > car_stats.camera_shake_threshold)
			{
				shake_time += (speed_change - car_stats.camera_shake_threshold) * car_stats.speed_to_shake_ratio;
			}
			if(speed_change > car_stats.camera_shake_threshold * 1.6) // TODO: Own value, not tied to cam
			{
				SetSoundPitch(sfx_crash, AirQuotesNoise(game_time, false) + 0.5);
				PlaySound(sfx_crash);
			}
			else if(speed_change > car_stats.camera_shake_threshold) // TODO: Own value, not tied to cam
			{
				SetSoundPitch(sfx_crash_small, AirQuotesNoise(game_time, false) + 0.5);
				PlaySound(sfx_crash_small);
			}

			if(!demo_realloced)
			{
				UpdateMusicStream(sfx_engine);
			}
			bool car_still = zero(car.velocity.x) && zero(car.velocity.y);
			bool car_was_still = zero(previous_speed);
			if(car_still != car_was_still)
			{
				if(car_still)
				{
					PauseMusicStream(sfx_engine);
				}
				else
				{
					ResumeMusicStream(sfx_engine);
				}
			}
			if(IsMusicStreamPlaying(sfx_engine))
			{
				float engine_pitch = (previous_speed - car.gear) * 0.3;
				SetMusicPitch(sfx_engine, engine_pitch);
				float engine_volume = 1.0;
				if(car.gear == 0 && car.gear_shift <= 0.0)
				{
					engine_volume = min(.2 + .8 * (previous_speed / car_stats.gears[0]), 1.0);
				}
				SetMusicVolume(sfx_engine, engine_volume * sfx);
			}

			if(meta.checkpoint && checkpoints_gotten != track.checkpoint_amount)
			{
				bool new_check = true;
				for(int i = 0; i < checkpoints_gotten; i++)
				{
					Vector2int c = checkpoints[i];
					if(c.x == new_placement.x && c.y == new_placement.y)
					{
						new_check = false;
						break;
					}
				}
				if(new_check)
				{
					checkpoints[checkpoints_gotten] = new_placement;
					checkpoints_gotten++;
					check_pos = meta.check_pos;
					check_rot = meta.check_rot;
					PlaySound(sfx_checkpoint);
				}
			}
			bool did_finish = meta.finish && !finished && checkpoints_gotten == track.checkpoint_amount;
			if(party_mode && (did_finish || dnf))
			{
				stop_inputs = true;
				finished = true;
				if(dnf)
				{
				}
				else if(timer < party_best_times[current_profile] || party_best_times[current_profile] == 0.0)
				{
					party_best_times[current_profile] = timer;
				}
				int profs_left = 0;
				double best_time = 0.0;
				for(int i = 0; i < party_count; i++)
				{
					if(party_timers[i] > 0.0)
					{
						profs_left++;
						if(best_time == 0.0 || party_best_times[i] < best_time)
						{
							best_time = party_best_times[i];
						}
					}
				}
				if(profs_left <= 1 && best_time != 0.0)
				{
					current_game_screen = MENU;
					reset_menu = true;
					party_current_menu = 2;
					best_time = 0.0;
					party_draw = true;
					for(int i = 0; i < party_count; i++)
					{
						if(party_best_times[i] != 0.0)
						{
							if(best_time == 0.0 || party_best_times[i] < best_time)
							{
								best_time = party_best_times[i];
								current_profile = i;
								party_draw = false;
							}
							else if(party_best_times[i] == best_time)
							{
								party_draw = true;
							}
						}
					}
				}
				else
				{
					double worst_time = 0.0;
					int worst_prof = -1;
					for(int i = party_count - 1; i >= 0; i--)
					{
						if(party_timers[i] > 0.0 && party_best_times[i] == 0.0)
						{
							worst_prof = i;
						}
					}
					if(worst_prof == -1) for(int i = 0; i < party_count; i++)
					{
						if(party_timers[i] > 0.0 && party_best_times[i] > worst_time)
						{
							worst_time = party_best_times[i];
							worst_prof = i;
						}
					}
					if(worst_prof >= 0)
					{
						current_profile = worst_prof;
					}
					else
					{
						current_game_screen = MENU;
						reset_menu = true;
						party_current_menu = 2;
						party_draw = true;
					}
				}
			}
			else if(did_finish)
			{
				stop_inputs = true;
				finished = true;
				bool save_new = false;
				if(validating_track && track.has_start)
				{
					if(!track.validated)
					{
						TraceLog(LOG_INFO, "TRACK VALIDATED.");
						track.validated = true;
						track.medal_author = timer;
						CalculateMedalTimes(&track);
						save_new = true;
					}
					else if(timer < track.medal_author)
					{
						TraceLog(LOG_INFO, "NEW AUTHOR TIME!");
						track.medal_author = timer;
						CalculateMedalTimes(&track);
						save_new = true;
					}
				}
				if(ghost_demo != PNULL)
				{
					TraceLog(LOG_INFO, "Times: player %.3f, ghost %.3f", timer, ghost_demo->time);
				}
				demo->time = timer;
				if(playing_demo == DEMO_PLAY || playing_demo == DEMO_INIT)
				{
					save_new = false;
				}
				else if(ghost_demo == PNULL)
				{
					save_new = true;
				}
				else if(timer < ghost_demo->time || ghost_demo->time == 0.0)
				{
					save_new = true;
				}
				if(save_new)
				{
					TraceLog(LOG_INFO, "NEW BEST TIME!");
					unsigned char* demo_track_name = (unsigned char*)TrackFileName(track_dir, track_name);
					const char* filename;
					if(validating_track)
					{
						filename = VALIDATE_DEMO_FILE;
					}
					else
					{
						filename = DemoFilename(DEMO_DIRECTORY, (const char*)demo_track_name, playing_profile->name);
					}
					SaveDemo(demo, demo_track_name, filename);
					ClearDemo(ghost_demo);
					ghost_demo = CopyDemo(demo);
					StopDemo(ghost_demo);
					playing_demo = DEMO_GHOST_PLAY;
					if(demo_track_name != PNULL)
					{
						_free(demo_track_name);
					}
				}
			}

			if(InputPressed(input, INPUT_ENTER))
			{
				if(checkpoints_gotten == 0 || (stop_inputs && playing_demo != DEMO_PLAY))
				{
					partial_reset_race = true;
				}
				else
				{
					cp_reset = true;
				}
			}
			if(InputPressed(input, INPUT_BACK))
			{
				partial_reset_race = true;
			}
			if(InputPressed(input, INPUT_R))
			{
				SaveDemoTime(demo, (unsigned char*)TrackFileName(track_dir, track_name));
			}

			if(GetProfileBool(playing_profile, PRF_BOOL_CAM_CENTERED))
			{
				MoveCameraInstant(&camera, car.position);
			}
			else
			{
				MoveCameraSmoothInvert(&camera, car.position, 0.10);
			}

			if(playing_demo == DEMO_GHOST_PLAY)
			{
				new_placement = RacecarPlacement(&dcar);
				if(!Vector2intEqual(d_placement, new_placement))
				{
					d_placement = new_placement;
					LoadNearbyBlockWalls(blocks, dblock_walls, d_placement);
				}
				demo_input.past = demo_input.current;
				demo_input.current = GetDemoInput(ghost_demo) & 0b00101111;

				meta.checkpoint = false;
				meta.finish = false;
				meta = ProcessRacecar(&dcar, &car_stats, blocks, dblock_walls, demo_input, track.env);
				if(meta.checkpoint && dcheck_got != track.checkpoint_amount)
				{
					bool new_check = true;
					for(int i = 0; i < dcheck_got; i++)
					{
						Vector2int c = dcheck[i];
						if(c.x == d_placement.x && c.y == d_placement.y)
						{
							new_check = false;
							break;
						}
					}
					if(new_check)
					{
						dcheck[dcheck_got] = d_placement;
						dcheck_got++;
						dcheck_pos = meta.check_pos;
						dcheck_rot = meta.check_rot;
					}
				}
				if(meta.finish && dcheck_got == track.checkpoint_amount)
				{
					StopDemo(ghost_demo);
				}
				if(InputPressed(demo_input, INPUT_ENTER))
				{
					ResetRacecar(&dcar, dcheck_pos, dcheck_rot, car_stats.size);
				}
			}
		}

		if(cp_reset)
		{
			ResetRacecar(&car, check_pos, check_rot, car_stats.size);
			MoveCameraInstant(&camera, car.position);
			ClearSkidLine(skid_line);
		}
		if(current_game_screen != RACE)
		{
			StopSound(sfx_crash);
			UnloadSound(sfx_crash);
			StopSound(sfx_crash_small);
			UnloadSound(sfx_crash_small);
			StopSound(sfx_start_bleep);
			UnloadSound(sfx_start_bleep);
			StopSound(sfx_checkpoint);
			UnloadSound(sfx_checkpoint);

			TraceLog(LOG_INFO, "Unloading Engine Sound");
			StopMusicStream(sfx_engine);
			UnloadMusicStream(sfx_engine);
			if(party_mode)
			{
				_free(party_timers);
				_free(party_best_times);
			}
		}

		break;
	case OPTIONS:
		if(reset_options)
		{
			reset_options = false;
			options_current = 0;
			options_max = 3;
			options_page = OPTPAGE_MAIN;
			options_current_car = 0;
			options_customization = 0;
		}
		bool exit_options = false;
		if(InputPressed(input, INPUT_ESC))
		{
			exit_options = true;
			PlaySound(sfx_back);
		}

		if(options_customization)
		{
			unsigned char hori_limit = 0;
			if(options_customization == 1)
			{
				hori_limit = MODEL_AMOUNT;
			}
			else
			{
				hori_limit = PALETTE_AMOUNT;
			}
			bool reset_opt_curr = false;
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(options_current_car < 4)
				{
					options_current_car++;
					reset_opt_curr = true;
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(options_current_car > 0)
				{
					options_current_car--;
					reset_opt_curr = true;
					PlaySound(sfx_tick);
				}
			}
			if(reset_opt_curr || options_current == -1)
			{
				if(options_customization == 1)
				{
					if(options_current_car == CAR_ROAD)
					{
						options_current = profile.car_road_model;
					}
					if(options_current_car == CAR_DRIFT)
					{
						options_current = profile.car_drift_model;
					}
					if(options_current_car == CAR_GRIP)
					{
						options_current = profile.car_grip_model;
					}
					if(options_current_car == CAR_TERRAIN)
					{
						options_current = profile.car_terrain_model;
					}
				}
				else
				{
					if(options_current_car == CAR_ROAD)
					{
						options_current = profile.car_road_palette;
					}
					if(options_current_car == CAR_DRIFT)
					{
						options_current = profile.car_drift_palette;
					}
					if(options_current_car == CAR_GRIP)
					{
						options_current = profile.car_grip_palette;
					}
					if(options_current_car == CAR_TERRAIN)
					{
						options_current = profile.car_terrain_palette;
					}
				}
			}
			if(InputHeld(menu_input, INPUT_RIGHT))
			{
				if(options_current < hori_limit - 1)
				{
					options_current++;
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_LEFT))
			{
				if(options_current > 0)
				{
					options_current--;
					PlaySound(sfx_tick);
				}
			}
			if(InputPressed(input, INPUT_BACK))
			{
				if(options_current_car == 4)
				{
					options_customization = 0;
					options_page = OPTPAGE_PROFILES;
					options_current = 0;
					PlaySound(sfx_click);
				}
				else
				{
					options_current_car = 4;
					PlaySound(sfx_tick);
				}
			}
			if(InputPressed(input, INPUT_ENTER))
			{
				if(options_current_car == 4)
				{
					options_customization = 0;
					options_page = OPTPAGE_PROFILES;
					options_current = 0;
				}
				else if(options_customization == 1)
				{
					if(options_current_car == CAR_ROAD)
					{
						profile.car_road_model = options_current;
					}
					if(options_current_car == CAR_DRIFT)
					{
						profile.car_drift_model = options_current;
					}
					if(options_current_car == CAR_GRIP)
					{
						profile.car_grip_model = options_current;
					}
					if(options_current_car == CAR_TERRAIN)
					{
						profile.car_terrain_model = options_current;
					}
				}
				else if(options_customization == 2)
				{
					if(options_current_car == CAR_ROAD)
					{
						profile.car_road_palette = options_current;
					}
					if(options_current_car == CAR_DRIFT)
					{
						profile.car_drift_palette = options_current;
					}
					if(options_current_car == CAR_GRIP)
					{
						profile.car_grip_palette = options_current;
					}
					if(options_current_car == CAR_TERRAIN)
					{
						profile.car_terrain_palette = options_current;
					}
				}
				PlaySound(sfx_click);
			}
		}
		else if(!entering_profile_name)
		{
			switch(options_page)
			{
				case OPTPAGE_EDITOR:
				case OPTPAGE_PARTY:
					options_max = 1;
					break;
				case OPTPAGE_AUDIO:
					options_max = 3;
					break;
				case OPTPAGE_PROFILES:
				case OPTPAGE_GAMEPLAY:
					options_max = 4;
					break;
				case OPTPAGE_MAIN:
					options_max = 5;
					break;
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(options_current < options_max)
				{
					options_current++;
					PlaySound(sfx_tick);
				}
			}
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(options_current > 0)
				{
					options_current--;
					PlaySound(sfx_tick);
				}
			}
			// OPTIONS SELECT
			if(InputHeld(menu_input, INPUT_RIGHT))
			{
				if(options_page == OPTPAGE_GAMEPLAY)
				{
					if(options_current == 3)
					{
						if(profile.camera_zoom < 2.0)
						{
							profile.camera_zoom += .1;
							PlaySound(sfx_tick);
						}
						else profile.camera_zoom = 2.0;
					}
				}
				if(options_page == OPTPAGE_AUDIO)
				{
					if(options_current == 0)
					{
						if(profile.master_volume < 10)
						{
							profile.master_volume += 1;
							PlaySound(sfx_tick);
						}
					}
					else if(options_current == 1)
					{
						if(profile.sfx_volume < 10)
						{
							profile.sfx_volume += 1;
							PlaySound(sfx_tick);
						}
					}
					else if(options_current == 2)
					{
						if(profile.music_volume < 10)
						{
							profile.music_volume += 1;
							PlaySound(sfx_tick);
						}
					}
					UpdateVolume(&profile, &sfx, &music);
					UpdateMenuSFX(sfx, sfx_tick, sfx_click, sfx_back);
				}
			}
			if(InputHeld(menu_input, INPUT_LEFT))
			{
				if(options_page == OPTPAGE_GAMEPLAY)
				{
					if(options_current == 3)
					{
						if(profile.camera_zoom > 0.7)
						{
							profile.camera_zoom -= .1;
							PlaySound(sfx_tick);
						}
						else profile.camera_zoom = 0.7;
					}
				}
				else if(options_page == OPTPAGE_AUDIO)
				{
					if(options_current == 0)
					{
						if(profile.master_volume > -11)
						{
							profile.master_volume -= 1;
							PlaySound(sfx_tick);
						}
					}
					else if(options_current == 1)
					{
						if(profile.sfx_volume > -11)
						{
							profile.sfx_volume -= 1;
							PlaySound(sfx_tick);
						}
					}
					else if(options_current == 2)
					{
						if(profile.music_volume > -11)
						{
							profile.music_volume -= 1;
							PlaySound(sfx_tick);
						}
					}
					UpdateVolume(&profile, &sfx, &music);
					UpdateMenuSFX(sfx, sfx_tick, sfx_click, sfx_back);
				}
			}
			if(InputPressed(input, INPUT_BACK))
			{
				if(options_current == options_max)
				{
					if(options_page == OPTPAGE_MAIN)
					{
						exit_options = true;
					}
					else
					{
						options_page = OPTPAGE_MAIN;
						options_current = 0;
					}
					PlaySound(sfx_click);
				}
				else
				{
					options_current = options_max;
					PlaySound(sfx_tick);
				}
			}
			if(InputPressed(input, INPUT_ENTER))
			{
				if(options_current == options_max)
				{
					if(options_page == OPTPAGE_MAIN)
					{
						exit_options = true;
						PlaySound(sfx_click);
					}
					else
					{
						options_page = OPTPAGE_MAIN;
						options_current = 0;
						PlaySound(sfx_tick);
					}
				}
				else if(options_page == OPTPAGE_MAIN)
				{
					if(options_current == 0)
					{
						options_page = OPTPAGE_PROFILES;
						options_current = 0;
					}
					else if(options_current == 1)
					{
						options_page = OPTPAGE_GAMEPLAY;
						options_current = 0;
					}
					else if(options_current == 2)
					{
						options_page = OPTPAGE_AUDIO;
						options_current = 0;
					}
					else if(options_current == 3)
					{
						options_page = OPTPAGE_EDITOR;
						options_current = 0;
					}
					else if(options_current == 4)
					{
						options_page = OPTPAGE_PARTY;
						options_current = 0;
					}
					PlaySound(sfx_click);
				}
				else if(options_page == OPTPAGE_PROFILES)
				{
					if(options_current == 0)
					{
						current_game_screen = PROFILES;
						load_profiles = true;
						back_to_opt = true;
						PlaySound(sfx_click);
					}
					else if(options_current == 1)
					{
						entering_profile_name = true;
						inputing_name = 1;
						name_lenght = PROFILE_NAME_LENGHT - 1;
						PlaySound(sfx_click);
					}
					else if(options_current == 2)
					{
						options_current_car = 0;
						options_customization = 1;
						options_current = -1;
						PlaySound(sfx_click);
					}
					else
					{
						options_current_car = 0;
						options_customization = 2;
						options_current = -1;
						PlaySound(sfx_click);
					}
				}
				else if(options_page == OPTPAGE_GAMEPLAY)
				{
					if(options_current == 0)
					{
						bool b = GetProfileBool(&profile, PRF_BOOL_SCREEN_SHAKE);
						SetProfileBool(&profile, PRF_BOOL_SCREEN_SHAKE, !b);
						PlaySound(sfx_click);
					}
					else if(options_current == 1)
					{
						bool b = GetProfileBool(&profile, PRF_BOOL_CAM_CENTERED);
						SetProfileBool(&profile, PRF_BOOL_CAM_CENTERED, !b);
						PlaySound(sfx_click);
					}
					else
					{
						bool b = GetProfileBool(&profile, PRF_BOOL_GHOST_ENABLED);
						SetProfileBool(&profile, PRF_BOOL_GHOST_ENABLED, !b);
						PlaySound(sfx_click);
					}
				}
				else if(options_page == OPTPAGE_EDITOR)
				{
					if(options_current == 0)
					{
						bool b = GetProfileBool(&profile, PRF_BOOL_BLOCKMIXING);
						SetProfileBool(&profile, PRF_BOOL_BLOCKMIXING, !b);
						PlaySound(sfx_click);
					}
				}
				else if(options_page == OPTPAGE_PARTY)
				{
					if(options_current == 0)
					{
						bool b = GetProfileBool(&profile, PRF_BOOL_HIDE_PARTY_TIME);
						SetProfileBool(&profile, PRF_BOOL_HIDE_PARTY_TIME, !b);
						PlaySound(sfx_click);
					}
				}
			}
		}
		if(exit_options)
		{
			current_game_screen = MENU;
			reset_menu = true;
			if(save_options)
			{
				const char* fname = ProfileFilename(PROFILE_DIRECTORY, profile.name);
				SaveProfile(&profile, fname);
			}
			UpdateVolume(&profile, &sfx, &music);
			UpdateMenuSFX(sfx, sfx_tick, sfx_click, sfx_back);
		}
		break;
	}
	if(popup)
	{
		if(popup_confirmed)
		{
			popup_opt = 0;
			popup = POPUP_OFF;
			popup_confirmed = false;
		}
	}
	else if(load_file_list)
	{
		if(file_list_active == FL_TRACK)
		{
			LoadTrackDirectory(&fpl, track_dir);
		}
		else if(file_list_active == FL_DEMO)
		{
			LoadTrackDirectory(&fpl, demo_dir);
		}
		selected_file = 0;
		load_file_list = false;
	}
	else if(file_list_active)
	{
		bool file_list_exit = false;
		unsigned char* file_dir;
		const char* top_dir;
		if(file_list_active == FL_TRACK)
		{
			file_dir = track_dir;
			top_dir = TRACK_DIRECTORY;
		}
		else if(file_list_active == FL_DEMO)
		{
			file_dir = demo_dir;
			top_dir = DEMO_DIRECTORY;
		}
		//TraceLog(LOG_INFO, "%s, %s", file_dir, top_dir);

		if(InputHeld(menu_input, INPUT_UP))
		{
			if(MoveFileListCursor(fpl.count, &selected_file, -1))
			{
				PlaySound(sfx_tick);
			}
		}
		if(InputHeld(menu_input, INPUT_DOWN))
		{
			if(MoveFileListCursor(fpl.count, &selected_file, 1))
			{
				PlaySound(sfx_tick);
			}
		}
		if(InputHeld(menu_input, INPUT_LEFT))
		{
			if(MoveFileListCursor(fpl.count, &selected_file, -FILE_LIST_PAGE_ITEMS))
			{
				PlaySound(sfx_tick);
			}
		}
		if(InputHeld(menu_input, INPUT_RIGHT))
		{
			if(MoveFileListCursor(fpl.count, &selected_file, FILE_LIST_PAGE_ITEMS))
			{
				PlaySound(sfx_tick);
			}
		}
		if(InputPressed(input, INPUT_BACK))
		{
			if(!TextIsEqual(file_dir, top_dir))
			{
				ReturnToParentDirectory(file_dir);
				load_file_list = true;
				PlaySound(sfx_back);
			}
			else
			{
				file_list_exit = true;
				PlaySound(sfx_back);
			}
		}

		if(fpl.count > 0 && (skip_validate_check || InputPressed(input, INPUT_ENTER)))
		{
			if(InputPressed(input, INPUT_ENTER))
			{
				PlaySound(sfx_click);
			}
			const char* path = (const char*)fpl.paths[selected_file];
			if(DirectoryExists(path))
			{
				ChangeToDirectory(file_dir, (unsigned char*)path, true);
				load_file_list = true;
			}
			else if(file_list_active == FL_TRACK)
			{
				if(current_game_screen == MENU)
				{
					Track temp = LoadTrack(path);
					TrackBlockmixed(&track, GetProfileBool(&profile, PRF_BOOL_BLOCKMIXING));
					TrackNameFromFilename(path, track_name);
					if(temp.validated || skip_validate_check)
					{
						track = temp;
						MakeTrackBlocks(&track, blocks);
						file_list_active = FL_OFF;
						current_game_screen = RACE;
						reset_race = true;
						race_showcase = true;

						if(!validating_track && !party_mode)
						{
							const char* filename = DemoFilename(DEMO_DIRECTORY, TrackFileName(track_dir, track_name), profile.name);
							TraceLog(LOG_INFO, "validating %i", validating_track);
							DemoSave* demosave = LoadDemo(filename);
							if(demosave->result)
							{
								ghost_demo = demosave->demo;
								StartDemo(ghost_demo);
								playing_demo = DEMO_GHOST_INIT;
								TraceLog(LOG_INFO, "FREE: trackname");
								_free(demosave->track_name);
								TraceLog(LOG_INFO, "FREE: demosave");
								_free(demosave);
							}
							else
							{
								TraceLog(LOG_INFO, "FREE: demosave");
								_free(demosave);
								playing_demo = DEMO_OFF;
							}
						}
						else
						{
							playing_demo = DEMO_OFF;
						}
					}
					else
					{
						popup = POPUP_VALIDATE;
					}
				}
				else if(current_game_screen == EDITOR)
				{
					track = LoadTrack(path);
					TrackBlockmixed(&track, GetProfileBool(&profile, PRF_BOOL_BLOCKMIXING));
					TrackNameFromFilename(path, track_name);
					MakeTrackBlocks(&track, blocks);
					file_list_active = FL_OFF;
					cursor_info.placement = PositionToPlacement(track.start_pos);
				}
			}
			else if(file_list_active == FL_DEMO)
			{
				DemoSave* demosave = LoadDemo(path);
				if(demosave->result)
				{
					demo = demosave->demo;
					StartDemo(demo);
					playing_demo = DEMO_INIT;

					track = LoadTrack((const char*)demosave->track_name);
					TrackBlockmixed(&track, GetProfileBool(&profile, PRF_BOOL_BLOCKMIXING));
					TrackNameFromFilename((const char*)demosave->track_name, track_name);
					MakeTrackBlocks(&track, blocks);

					file_list_active = FL_OFF;
					current_game_screen = RACE;
					reset_race = true;
					race_showcase = true;
					TraceLog(LOG_INFO, "FREE: demosave track_name");
					_free(demosave->track_name);
					TraceLog(LOG_INFO, "FREE: demosave");
					_free(demosave);
				}
				else
				{
					TraceLog(LOG_INFO, "FREE: demosave");
					_free(demosave);
				}
			}
			skip_validate_check = false;
		}
		else if(InputPressed(input, INPUT_ESC))
		{
			file_list_exit = true;
			PlaySound(sfx_back);
		}
		if(file_list_exit)
		{
			UnloadDirectoryFiles(fpl);
			fpl.count = 0;
			file_list_active = FL_OFF;
			skip_validate_check = false;
		}
	}

	if(inputing_name == 1)
	{
		inputing_name = 2;
	}
	else if(inputing_name)
	{
		unsigned char* name = (void*)0;
		if(saving_track)
		{
			name = track_name;
		}
		if(entering_profile_name)
		{
			name = profile_name;
		}
		if(InputPressed(input, INPUT_ESC))
		{
			inputing_name = 0;
			PlaySound(sfx_back);
		}
		else if(InputPressed(input, INPUT_ENTER))
		{
			inputing_name = 0;
			if(saving_track)
			{
				const char* track_filename = TrackFileName(track_dir, track_name);
				if(FileExists(track_filename))
				{
					popup = POPUP_OVERRIDE_TRACK;
				}
				else
				{
					SaveTrack(&track, track_filename, profile.name);
				}
			}
			if(entering_profile_name)
			{
				const char* fname = ProfileFilename(PROFILE_DIRECTORY, profile_name);
				if(!FileExists(fname))
				{
					Profile new_prof;
					if(current_game_screen == OPTIONS || intro)
					{
						intro = false;
						new_prof = profile;
					}
					else
					{
						new_prof = DefaultProfile();
					}
					for(int i = 0; i < PROFILE_NAME_LENGHT; i++)
					{
						new_prof.name[i] = profile_name[i];
					}
					if(current_game_screen != OPTIONS || save_options)
					{
						SaveProfile(&new_prof, fname);
					}
					if(fpl.count > 0)
					{
						UnloadDirectoryFiles(fpl);
						fpl.count = 0;
					}
					if(party_mode)
					{
						party_profiles[party_current_opt - 1] = new_prof;
						current_game_screen = MENU;
					}
					else
					{
						profile = new_prof;
						if(current_game_screen != OPTIONS)
						{
							current_game_screen = MENU;
							reset_menu = true;
						}
						UpdateVolume(&profile, &sfx, &music);
						UpdateMenuSFX(sfx, sfx_tick, sfx_click, sfx_back);
					}
				}
				else
				{
					inputing_name = 2;
				}
			}
			PlaySound(sfx_click);
		}
		else if(InputPressed(input, INPUT_BACK))
		{
			if(name[0] == 0)
			{
				inputing_name = 0;
				PlaySound(sfx_back);
			}
			else
			{
				int del = 0;
				for(int i = 0; i < name_lenght; i++)
				{
					if(name[i] != 0)
					{
						del = i;
					}
					else
					{
						break;
					}
				}
				name[del] = 0;
				PlaySound(sfx_tick);
			}
		}
		else
		{
			unsigned char c = GetCharPressed();
			if(c != 0)
			{
				for(int i = 0; i < name_lenght; i++)
				{
					if(name[i] == 0)
					{
						name[i] = c;
						break;
					}
				}
				PlaySound(sfx_tick);
			}
		}

		if(inputing_name == 0)
		{
			saving_track = false;
			entering_profile_name = false;
		}
	}
	//TraceLog(LOG_INFO, "%i %i", inputing_number, in_type);
	if(inputing_number == 1)
	{
		inputing_number = 2;
	}
	else if(inputing_number > 0)
	{
		if(InputPressed(input, INPUT_ESC))
		{
			inputing_number = 0;
			PlaySound(sfx_back);
		}
		else if(InputPressed(input, INPUT_ENTER))
		{
			inputing_number = 0;
			if(in_type == 1)
			{
				switch(efos_opt)
				{
					case(0):
						track.medal_bronz = (double)in_num * 0.001;
						break;
					case(1):
						track.medal_silver = (double)in_num * 0.001;
						break;
					case(2):
						track.medal_gold = (double)in_num * 0.001;
						break;
				}
				CalculateMedalTimes(&track);
			}
			if(in_type == 2)
			{
				piece_catalogue_page_num = in_num;
				if(piece_catalogue_page_num < 1)
				{
					piece_catalogue_page_num = 1;
				}
				if(piece_catalogue_page_num >= PIECE_CATALOGUE_PAGE_AMOUNT)
				{
					piece_catalogue_page_num = PIECE_CATALOGUE_PAGE_AMOUNT - 1;
				}
				PiecesInPage(piece_catalogue_page, piece_catalogue_page_num);
				LoadPieceCataloguePage(piece_catalogue, piece_catalogue_page);
			}
			PlaySound(sfx_click);
		}
		else if(InputPressed(input, INPUT_BACK))
		{
			if(in_num >= 10)
			{
				in_num *= 0.1;
				PlaySound(sfx_tick);
			}
			else if(in_num == 0)
			{
				inputing_number = 0;
				PlaySound(sfx_back);
			}
			else
			{
				in_num = 0;
				PlaySound(sfx_tick);
			}
		}
		else
		{
			unsigned char c = GetCharPressed();
			if(c != 0)
			{
				in_num *= 10;
				switch(c)
				{
					case('1'):
						in_num += 1;
						break;
					case('2'):
						in_num += 2;
						break;
					case('3'):
						in_num += 3;
						break;
					case('4'):
						in_num += 4;
						break;
					case('5'):
						in_num += 5;
						break;
					case('6'):
						in_num += 6;
						break;
					case('7'):
						in_num += 7;
						break;
					case('8'):
						in_num += 8;
						break;
					case('9'):
						in_num += 9;
						break;
				}
				PlaySound(sfx_tick);
			}
		}

		if(inputing_number == 0)
		{
			in_type = 0;
			in_num = 0;
		}
	}

	if(shake_time > 0)
	{
		ShakeCamera(
				&camera,
				game_time,
				shake_time * car_stats.shake_amplitude,
				(1 + shake_time) * car_stats.shake_frequency,
				car_stats.shake_weight
		);
		shake_time -= FRAME;
		if(shake_time <= 0)
		{
			CenterCameraOffset(&camera, true);
		}
	}

	BeginDrawing();

		ClearBackground(rmc(RM_WHITE0));
		if(track.env == ENV_VOID)
		{
			DrawBackgroundVoid(camera.data.target, camera.data.zoom, game_time);
		}
		if(track.env == ENV_MEADOW)
		{
			ClearBackground(rmc(RM_GREEN5));
		}
		if(track.env == ENV_QUARRY)
		{
			ClearBackground(rmc(RM_ORANGE6));
		}
		if(track.env == ENV_ISLAND)
		{
			DrawBackgroundWater(camera.data.target, camera.data.zoom, game_time);
			//ClearBackground(BLUE);
		}

		// Gamespace

		BeginMode2D(camera.data);

		if(current_game_screen == EDITOR)
		{
			for(int i = 0; i <= BLOCK_SIZE * TRACK_GRID_SIZE; i += BLOCK_SIZE)
			{
				DrawLine(i, 0, i, BLOCK_SIZE * TRACK_GRID_SIZE, GRAY);
				DrawLine(0, i, BLOCK_SIZE * TRACK_GRID_SIZE, i, GRAY);
			}
		}

		int layers[Z_LAYERS][MAX_LOADED_BLOCKS];

		LoadNearbyBlocks(blocks, layers, load_placement);

		if(BLOCK_DEBUG)
		{
			for(int i = 0; i < PLAYER_LAYER; i++)
			{
				DrawPlacedBlocksDebug(blocks, layers[i]);
			}
			DrawLoadedBlockWallsDebug(block_walls);
		}
		else
		{
			for(int i = 0; i < PLAYER_LAYER; i++)
			{
				DrawLoadedBlocks(blocks, layers[i], game_time);
			}
		}

		switch(current_game_screen)
		{
			case PROFILES:
			{
			} break;
			case MENU:
			{
			} break;
			case EDITOR:
			{
				for(int i = 0; i < BLOCKS_PER_PIECE; i++)
				{
					DrawBlock(held_piece[i], game_time);
				}
				Vector2 cursor_pos = (Vector2){BLOCK_SIZE * cursor_info.placement.x, BLOCK_SIZE * cursor_info.placement.y};
				DrawCursor(cursor_pos, BLOCK_SIZE, BLACK);
				Vector2 cursor_center = Vector2AddValue(cursor_pos, BLOCK_SIZE * 0.5);
				Vector2 cursor_end = Vector2AddValue(cursor_pos, BLOCK_SIZE);
				int rot_hint_lenght = 16 / camera.data.zoom;
				switch(cursor_info.rot)
				{
				case(ROT_NORTH):DrawLine(cursor_center.x, cursor_pos.y, cursor_center.x, cursor_pos.y - rot_hint_lenght, BLACK);break;
				case(ROT_EAST):DrawLine(cursor_end.x, cursor_center.y, cursor_end.x + rot_hint_lenght, cursor_center.y, BLACK);break;
				case(ROT_SOUTH):DrawLine(cursor_center.x, cursor_end.y, cursor_center.x, cursor_end.y + rot_hint_lenght, BLACK);break;
				case(ROT_WEST):DrawLine(cursor_pos.x, cursor_center.y, cursor_pos.x - rot_hint_lenght, cursor_center.y, BLACK);break;
				}
			} break;
			case RACE:
			{
				if(playing_demo == DEMO_GHOST_PLAY)
				{
					DrawRacecar(&dcar, true);
				}
				DrawSkidLine(skid_line, car.model, rmc(RM_WHITE8));
				DrawRacecar(&car, false);
				if(DEBUG)
				{
					DrawRacecarWalls(&car, false);
				}
			} break;
		}

		if(BLOCK_DEBUG)
		{
			for(int i = PLAYER_LAYER; i < Z_LAYERS; i ++)
			{
				DrawPlacedBlocksDebug(blocks, layers[i]);
			}
		}
		else
		{
			for(int i = PLAYER_LAYER; i < Z_LAYERS; i ++)
			{
				DrawLoadedBlocks(blocks, layers[i], game_time);
			}
		}

		EndMode2D();

		// UI

		if(DEBUG) DrawFPS(8, 0);

		switch(current_game_screen)
		{
			case PROFILES:
			{
				DrawProfileSelector(fpl, current_profile);
			} break;
			case MENU:
			{
				DrawText("RAYMANIA", 258, 56, 96, BLACK);

				if(reset_menu)
				{
				}
				else if(party_mode && party_current_menu == 0)
				{
					DrawPartyMenu(party_current_opt, party_count, party_profiles);
				}
				else if(party_mode && party_current_menu == 1)
				{
					const Vector2 SIZE = (Vector2){480, 192};
					const Vector2 POSITION = (Vector2){272, 64};

					DrawRectangle(POSITION.x - 28, POSITION.y - 28, SIZE.x + 56, SIZE.y + 56, BLACK);
					DrawRectangle(POSITION.x - 24, POSITION.y - 24, SIZE.x + 48, SIZE.y + 48, PURPLE);
					DrawRectangle(POSITION.x, POSITION.y, SIZE.x, SIZE.y, VIOLET);

					for(int i = 0; i < 4; i++)
					{
						Color text_color = BLACK;
						if(party_current_opt == i) text_color = rmc(RM_WHITE0);
						Vector2 pos = POSITION;
						pos.y += 8 + i * 32;
						pos.x += 8;
						if(i == 0)
						{
							DrawText(TextFormat("Timer: %.3f", (double)party_timer_base * 30.0), pos.x, pos.y, 32, text_color);
						}
						else if(i == 1)
						{
							DrawText("o Load track", pos.x, pos.y, 32, text_color);
						}
						else if(i == 2)
						{
							DrawText("+ Make new track", pos.x, pos.y, 32, text_color);
						}
						else if(i == 3)
						{
							DrawText("< Change players", pos.x, pos.y, 32, text_color);
						}
					}
				}
				else if(party_mode && party_current_menu == 2)
				{
					if(party_draw)
					{
						DrawText("DRAW...", 256, 256, 64, BLACK);
					}
					else
					{
						DrawText(TextFormat("%s", party_profiles[current_profile].name), 256, 192, 64, BLACK);
						DrawText("IS THE WINNER!!!", 256, 256, 64, BLACK);
					}
				}
				else if(menu_option >= MENU_RACE) for(int i = 0; i < 3; i++)
				{
					int y_pos = 192 + 80 * i;
					if(i + MENU_RACE == menu_option)
					{
						DrawRectangle(316, y_pos - 4, 392, 72, rmc(RM_WHITE0));
					}
					else
					{
						DrawRectangle(316, y_pos - 4, 392, 72, BLACK);
					}

					switch(i + MENU_RACE)
					{
						case(MENU_RACE):
							DrawRectangle(320, y_pos, 384, 64, GREEN);
							DrawText("RACE", 512 - 82, y_pos, 64, DARKGREEN);
						break;
						case(MENU_DEMO):
							DrawRectangle(320, y_pos, 384, 64, GREEN);
							DrawText("DEMO", 512 - 82, y_pos, 64, DARKGREEN);
						break;
						case(MENU_PLAY_EXIT):
							DrawRectangle(320, y_pos, 384, 64, PINK);
							DrawText("EXIT", 512 - 79, y_pos, 64, MAROON);
						break;
					}
				}
				else for(int i = 0; i < 5; i++)
				{
					int y_pos = 192 + 80 * i;
					if(i == menu_option)
					{
						DrawRectangle(316, y_pos - 4, 392, 72, rmc(RM_WHITE0));
					}
					else
					{
						DrawRectangle(316, y_pos - 4, 392, 72, BLACK);
					}

					switch(i)
					{
						case(MENU_PLAY):
							DrawRectangle(320, y_pos, 384, 64, GREEN);
							DrawText("PLAY", 512 - 82, y_pos, 64, DARKGREEN);
						break;
						case(MENU_EDITOR):
							DrawRectangle(320, y_pos, 384, 64, YELLOW);
							DrawText("EDITOR", 512 - 123, y_pos, 64, ORANGE);
						break;
						case(MENU_PARTY):
							DrawRectangle(320, y_pos, 384, 64, PURPLE);
							DrawText("PARTY", 512 - 111, y_pos, 64, DARKPURPLE);
						break;
						case(MENU_OPTIONS):
							DrawRectangle(320, y_pos, 384, 64, SKYBLUE);
							DrawText("OPTIONS", 512 - 146, y_pos, 64, DARKBLUE);
						break;
						case(MENU_EXIT):
							DrawRectangle(320, y_pos, 384, 64, PINK);
							DrawText("EXIT", 512 - 79, y_pos, 64, MAROON);
						break;
					}
				}

				if(DEBUG)
				{
					DrawText(TextFormat("pos %.0f %.0f", camera.cam_pos.x, camera.cam_pos.y), 8, 16, 32, BLACK);
					DrawText(TextFormat("velocity %.3f %.3f", camera_velocity.x, camera_velocity.y), 8, 48, 32, BLACK);
					DrawText(TextFormat("dir %.3f %.3f", vel_sign.x, vel_sign.y), 8, 80, 32, BLACK);
				}

				//DrawText(TextFormat("%i", MeasureText("PLAY", 64) / 2), 8, 8, 16, BLACK);
			} break;
			case EDITOR:
			{
				DrawText(TextFormat("x: %i", cursor_info.placement.x), 8, 8, 16, BLACK);
				DrawText(TextFormat("y: %i", cursor_info.placement.y), 8, 24, 16, BLACK);
				if(track.validated)
				{
					DrawText("TRACK VALIDATED!", 8, 40, 16, BLACK);
				}
				else
				{
					DrawText("Track not validated.", 8, 40, 16, BLACK);
				}
				if(track.blockmixed)
				{
					DrawText("BLOCKMIXING ENABLED", 8, 56, 16, BLACK);
					DrawText(TextFormat("overlaping blocks: %i", overlaping_blocks), 8, 72, 16, BLACK);
				}
				if(track.has_start && holding_start)
				{
					DrawGuideArrow(camera.data.target, track.start_pos);
				}
				if(piece_catalogue_pulled > 0.0)
				{
					int catalogue_pos_y = 640 - piece_catalogue_pulled * 224;
					DrawRectangle(64, catalogue_pos_y, 896, 224, BLACK);
					DrawRectangle(68, catalogue_pos_y + 4, 888, 220, LIGHTGRAY);

					Vector2 pos = (Vector2){88, catalogue_pos_y + 24};
					DrawPieceCatalogue(piece_catalogue, pos);

					pos = (Vector2){96, catalogue_pos_y + 128};
					const int TEXT_SIZE = 24;
					for(int i = 9; i < 18; i ++)
					{
						DrawRectangle(pos.x - 8, pos.y - 8, 80, 80, rmc(RM_WHITE0));
						switch(i)
						{
							case(EDITOR_EXIT):
								DrawText("EXIT", pos.x, pos.y, TEXT_SIZE, RED);
								break;
							case(EDITOR_LOAD):
								DrawText("LOAD", pos.x, pos.y, TEXT_SIZE, DARKGREEN);
								break;
							case(EDITOR_SAVE):
								DrawText("SAVE", pos.x, pos.y, TEXT_SIZE, DARKBLUE);
								break;
							case(EDITOR_CLEAR):
								DrawText("CLR", pos.x, pos.y, TEXT_SIZE, MAROON);
								break;
							case(EDITOR_ENVIROMENT):
								DrawText("ENV", pos.x, pos.y, TEXT_SIZE, GOLD);
								break;
							case(EDITOR_CAR):
								DrawText("CAR", pos.x, pos.y, TEXT_SIZE, ORANGE);
								break;
							case(EDITOR_PAGE_JUMP):
								DrawText("PAGE", pos.x, pos.y, TEXT_SIZE, VIOLET);
								DrawText(TextFormat("%i", piece_catalogue_page_num), pos.x, pos.y + TEXT_SIZE, TEXT_SIZE, VIOLET);
								break;
							case(EDITOR_MEDALS):
								DrawText("TIME", pos.x, pos.y, TEXT_SIZE, GRAY);
								break;
							case(EDITOR_VALIDATE):
								if(party_mode)
								{
									DrawText("PLAY", pos.x, pos.y, TEXT_SIZE, GREEN);
								}
								else if(track.has_start)
								{
									DrawText("VALI", pos.x, pos.y, TEXT_SIZE, GREEN);
								}
								else
								{
									DrawRectangle(pos.x - 8, pos.y - 8, 80, 80, GRAY);
									DrawText("VALI", pos.x, pos.y, TEXT_SIZE, BLACK);
								}
								break;
						}
						pos.x += 96;
					}

					pos.x = CataloguePosition(88, piece_catalogue_item % 9);
					pos.y = catalogue_pos_y + 24 + (piece_catalogue_item / 9) * 96;
					DrawCursor(pos, BLOCK_SIZE * CATALOGUE_BLOCK_SCALE, BLACK);
				}
				if(editor_four_option_selector > 0)
				{
					DrawRectangle(312, 240, 400, 112, LIGHTGRAY);
					for(int i = 0; i < 4; i++)
					{
						DrawRectangle(328 + i * 96, 256, 80, 80, rmc(RM_WHITE0));
						if(efos_opt == i)
						{
							Vector2 pos = (Vector2){328 + i * 96, 256};
							DrawCursor(pos, BLOCK_SIZE * CATALOGUE_BLOCK_SCALE, BLACK);
						}
					}
					const int TEXT_SIZE = 18;
					switch(editor_four_option_selector)
					{
					case(1):
						DrawText("ROAD", 336, 264, TEXT_SIZE, BLACK);
						DrawText("DRIFT", 432, 264, TEXT_SIZE, BLACK);
						DrawText("GRIP", 528, 264, TEXT_SIZE, BLACK);
						DrawText("TERRA", 624, 264, TEXT_SIZE, BLACK);
						break;
					case(2):
						DrawText("VOID", 336, 264, TEXT_SIZE, BLACK);
						DrawText("MEADOW", 432, 264, 16, BLACK);
						DrawText("QUARRY", 528, 264, 16, BLACK);
						DrawText("ISLE", 624, 264, TEXT_SIZE, BLACK);
						break;
					case(3):
						DrawText("BRONZ", 336, 264, TEXT_SIZE, BLACK);
						DrawText("SILVER", 432, 264, 16, BLACK);
						DrawText("GOLD", 528, 264, TEXT_SIZE, BLACK);
						DrawText("CLEAR", 624, 264, 16, BLACK);
						break;
					default: break;
					}
				}
			} break;
			case RACE:
			{
				if(DEBUG)
				{
					DrawText(TextFormat("pos %i %i", load_placement.x, load_placement.y), 8, 96, 32, BLACK);
					DrawText(TextFormat("velocity %.3f %.3f", car.velocity.x, car.velocity.y), 8, 128, 32, BLACK);
					DrawText(TextFormat("shake_time %.3f", shake_time), 8, 160, 32, BLACK);
				}
				if(finished)
				{
					if(dnf || validating_track)
					{
					}
					else if(timer < track.medal_author)
					{
						DrawText("AUTHOR MEDAL!!!", 160, 88, 96, BLACK);
					}
					else if(timer < track.medal_gold)
					{
						DrawText("GOLD MEDAL!!", 192, 88, 96, BLACK);
					}
					else if(timer < track.medal_silver)
					{
						DrawText("Silver medal!", 192, 88, 96, BLACK);
					}
					else if(timer < track.medal_bronz)
					{
						DrawText("Bronz medal.", 192, 88, 96, BLACK);
					}
					if(dnf)
					{
						DrawText("DNF", 384, 192, 96, BLACK);
					}
					else
					{
						DrawText(TextFormat("%.3f", timer), 384, 192, 96, BLACK);
					}
					int replay_pos = 352;
					if(party_mode)
					{
						DrawText(TextFormat("Up next: %i. %s", current_profile + 1, party_profiles[current_profile].name), 384, 352, 32, BLACK);
						replay_pos += 32;
					}
					DrawText("Save replay with R.", 384, replay_pos, 32, BLACK);
				}
				else if(!race_showcase)
				{
					DrawText(TextFormat("%.3f", timer), 8, 572, 64, BLACK);
					float speed = Vector2Length(car.velocity) * 100;
					Color color = BLACK;
					if(car.gear_shift > 0.0)
					{
						color = BLACK;
					}
					else if(car.gear == 0)
					{
						color = DARKPURPLE;
					}
					else if(car.gear == 1)
					{
						color = DARKBLUE;
					}
					else if(car.gear == 2)
					{
						color = DARKGREEN;
					}
					else if(car.gear == 3)
					{
						color = RED;
					}
					else if(car.gear == 4)
					{
						color = GOLD;
					}
					DrawText(TextFormat("%.0f", speed), 480, 572, 48, color);
					if(track.checkpoint_amount > 0)
					{
						DrawText(TextFormat("%i/%i", checkpoints_gotten, track.checkpoint_amount), 896, 576, 64, BLUE);
					}
				}
				if(race_showcase || paused)
				{
					DrawRectangle(0, 0, 172, 144, rmc(RM_WHITE0));
					DrawText(TextFormat("%s", track_name), 8, 8, 16, BLACK);
					DrawText(TextFormat("By: %s", track.author), 8, 24, 16, BLACK);
					DrawText(TextFormat("Bronz: %.3f", track.medal_bronz), 8, 40, 16, BLACK);
					DrawText(TextFormat("Silver: %.3f", track.medal_silver), 8, 56, 16, BLACK);
					DrawText(TextFormat("Gold: %.3f", track.medal_gold), 8, 72, 16, BLACK);
					float pb = track.medal_bronz;
					if(ghost_demo != PNULL)
					{
						DrawText(TextFormat("PB: %.3f", ghost_demo->time), 8, 104, 16, BLACK);
						pb = ghost_demo->time;
					}
					if(validating_track || pb < track.medal_gold)
					{
						DrawText(TextFormat("Author: %.3f", track.medal_author), 8, 88, 16, BLACK);
					}
					if(race_showcase)
					{
						DrawText("Press enter to play", 8, 120, 16, BLACK);
					}
				}
				else if(start_countdown)
				{
					DrawText(TextFormat("%i", 3 - countdown_digit), 576, 256, 128, BLACK);
				}
				if(paused)
				{
					DrawRectangle(256, 160, 512, 280, BLACK);
					DrawRectangle(260, 164, 504, 272, GREEN);
					int pos_y = 180;
					for(int i = 0; i < 4; i++)
					{
						if(i == pause_option)
						{
							DrawRectangle(276, pos_y, 472, 48, rmc(RM_WHITE0));
						}
						else
						{
							DrawRectangle(276, pos_y, 472, 48, BLACK);
						}
						DrawRectangle(280, pos_y + 4, 464, 40, LIME);
						switch(i)
						{
							case(0):
								DrawText("Resume", 455, pos_y + 8, 32, BLACK);
								break;
							case(1):
								DrawText("Reset To Check", 383, pos_y + 8, 32, BLACK);
								break;
							case(2):
								DrawText("Restart Race", 403, pos_y + 8, 32, BLACK);
								break;
							case(3):
								DrawText("Exit", 482, pos_y + 8, 32, BLACK);
								break;
						}
						pos_y += 64;
					}
				}
				if(party_mode && !reset_party)
				{
					Color ctext = BLACK, btext = VIOLET;
					bool render_timers = true;
					if(race_showcase)
					{
						render_timers = false;
					}
					if(!GetProfileBool(&profile, PRF_BOOL_HIDE_PARTY_TIME) || timer == 0.0 || stop_inputs)
					{
						ctext.a = 255;
						btext.a = 255;
					}
					else if(timer < 2.0)
					{
						ctext.a = (int)(128 * (2.0 - timer));
						btext.a = (int)(128 * (2.0 - timer));
					}
					else
					{
						render_timers = false;
					}
					if(render_timers)
					{
						for(int i = 0; i < party_count; i++)
						{
							DrawText(TextFormat("%.3f", party_best_times[i]), 16, i * 32 + 16, 16, ctext);
							DrawText(TextFormat("%i. %s", i + 1, party_profiles[i].name), 96, i * 32 + 16, 16, ctext);
							DrawRectangle(16, i * 32 + 34, party_timers[i] / party_timer_base * 10, 12, btext);
						}
					}
				}
				//TraceLog(LOG_INFO, "Text size %i.", MeasureText("Resume", 32) / 2);
			} break;
			case OPTIONS:
			{
				if(options_customization)
				{
					unsigned char model = 0;
					unsigned char palette = 0;
					if(options_current_car == CAR_ROAD)
					{
						model = profile.car_road_model;
						palette = profile.car_road_palette;
					}
					if(options_current_car == CAR_DRIFT)
					{
						model = profile.car_drift_model;
						palette = profile.car_drift_palette;
					}
					if(options_current_car == CAR_GRIP)
					{
						model = profile.car_grip_model;
						palette = profile.car_grip_palette;
					}
					if(options_current_car == CAR_TERRAIN)
					{
						model = profile.car_terrain_model;
						palette = profile.car_terrain_palette;
					}
					if(options_customization == 1)
					{
						DrawOptionsCustomization(options_current_car, options_current, MODEL_AMOUNT, model);
						if(options_current_car < 4) DrawOptionsCars(options_current, MODEL_AMOUNT, palette);
					}
					else
					{
						DrawOptionsCustomization(options_current_car, options_current, PALETTE_AMOUNT, palette);
						if(options_current_car < 4) DrawOptionsPalettes(options_current, PALETTE_AMOUNT);
					}
				}
				else
				{
					DrawOptions(options_current, options_page, options_max, &profile);
				}
			} break;
		}

		if(file_list_active)
		{
			Color bg1 = GREEN, bg2 = LIME;
			if(party_mode)
			{
				bg1 = PURPLE;
				bg2 = VIOLET;
			}
			else if(current_game_screen == EDITOR)
			{
				bg1 = GOLD;
				bg2 = ORANGE;
			}
			DrawFileList(fpl, selected_file, bg1, bg2);
		}

		if(inputing_name > 0)
		{
			DrawRectangle(88, 256, 848, 128, LIGHTGRAY);
			DrawLine(104, 368, 920, 368, GRAY);
			if(saving_track)
			{
				DrawText("Name of Track:", 384, 272, 32, BLACK);
				DrawText(TextFormat("%s", track_name), 104, 336, 32, BLACK);
			}
			if(entering_profile_name)
			{
				DrawText("Name of Profile:", 352, 272, 32, BLACK);
				DrawText(TextFormat("%s", profile_name), 104, 336, 32, BLACK);
			}
		}
		if(inputing_number > 0)
		{
			DrawRectangle(88, 256, 848, 128, LIGHTGRAY);
			DrawLine(104, 368, 920, 368, GRAY);
			switch(in_type)
			{
				case(1):
					double compare_timer = 0.0;
					if(efos_opt == 0) compare_timer = track.medal_silver;
					if(efos_opt == 1) compare_timer = track.medal_gold;
					if(efos_opt == 2) compare_timer = track.medal_author;
					DrawText("Enter Time:", 384, 264, 32, BLACK);
					DrawText(TextFormat("Medal above %.3f", compare_timer), 104, 300, 32, BLACK);
					DrawText(TextFormat("%.3f", (double)in_num * 0.001), 104, 336, 32, BLACK);
					break;
				case(2):
					DrawText("Enter Page Number:", 352, 272, 32, BLACK);
					DrawText(TextFormat("%i", in_num), 104, 336, 32, BLACK);
					break;
			}
		}

		if(popup)
		{
			DrawRectangle(316, 236, 392, 168, BLACK);
			DrawRectangle(320, 240, 384, 160, LIGHTGRAY);
			DrawRectangle(348 + (1 - popup_opt) * 192, 332, 136, 48, rmc(RM_WHITE0));
			DrawRectangle(352, 336, 128, 40, GREEN);
			DrawRectangle(544, 336, 128, 40, PINK);
			DrawText("YES", 382, 340, 32, LIME);
			DrawText("NO", 584, 340, 32, RED);
			switch(popup)
			{
				case(POPUP_EDITOR_EXIT):
					DrawText("Did you save?", 400, 272, 32, BLACK);
					break;
				case(POPUP_EDITOR_CLEAR):
					DrawText("Clear track?", 416, 272, 32, BLACK);
					break;
				case(POPUP_VALIDATE):
					DrawText("Track is not validated.", 328, 248, 32, BLACK);
					if(party_mode)
					{
						DrawText("Play anyway?", 424, 280, 32, BLACK);
					}
					else
					{
						DrawText("Validate?", 440, 280, 32, BLACK);
					}
					break;
				case(POPUP_RESET_MEDALS):
					DrawText("Clear medals?", 416, 272, 32, BLACK);
					break;
				case(POPUP_OVERRIDE_TRACK):
					DrawText("Override the", 416, 248, 32, BLACK);
					DrawText("existing track?", 400, 280, 32, BLACK);
					break;
			}
		}

		if(current_game_screen == PROFILES)
		{
			DrawFade(&camera, rmc(RM_WHITE0));
		}

	EndDrawing();
	}

	StopSound(sfx_tick);
	UnloadSound(sfx_tick);
	StopSound(sfx_click);
	UnloadSound(sfx_click);
	StopSound(sfx_back);
	UnloadSound(sfx_back);

	CloseAudioDevice();
	CloseWindow();

	return 0;
}
