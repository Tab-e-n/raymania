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


#define VALIDATE_DEMO_FILE "Demos/validation.dm\0"
#define MAX_PARTY_PROFILES 12
#define MAX_PARTY_TIMER 20
#define MIN_PARTY_TIMER 1


typedef enum GameScreen {PROFILES, MENU, EDITOR, RACE} GameScreen;
typedef enum MenuOption {MENU_PLAY, MENU_EDITOR, MENU_PARTY, MENU_OPTIONS, MENU_EXIT, MENU_RACE = 10, MENU_DEMO, MENU_PLAY_EXIT} MenuOption;
typedef enum EditorOption {EDITOR_EXIT = 9, EDITOR_LOAD, EDITOR_SAVE, EDITOR_CLEAR, EDITOR_ENVIROMENT, EDITOR_CAR, EDITOR_PAGE_JUMP, EDITOR_MEDALS, EDITOR_VALIDATE,} EditorOption;
typedef enum PopupType {POPUP_OFF, POPUP_EDITOR_EXIT, POPUP_EDITOR_CLEAR, POPUP_VALIDATE, POPUP_NO_START, POPUP_RESET_MEDALS} PopupType;
typedef enum FileListType {FL_OFF, FL_TRACK, FL_DEMO} FileListType;
typedef enum DemoPlay {DEMO_OFF, DEMO_INIT, DEMO_PLAY, DEMO_GHOST_INIT, DEMO_GHOST_PLAY} DemoPlay;

void DrawCursor(Vector2 cursor_pos, float size, Color color)
{
	DrawLine(cursor_pos.x, cursor_pos.y, cursor_pos.x + size, cursor_pos.y, color);
	DrawLine(cursor_pos.x, cursor_pos.y, cursor_pos.x, cursor_pos.y + size, color);
	DrawLine(cursor_pos.x, cursor_pos.y + size, cursor_pos.x + size, cursor_pos.y + size, color);
	DrawLine(cursor_pos.x + size, cursor_pos.y, cursor_pos.x + size, cursor_pos.y + size, color);
}

void DrawPartyMenu(int current_opt, unsigned int party_count, Profile* profiles)
{
	const Vector2 SIZE = (Vector2){768, 512};
	const Vector2 POSITION = (Vector2){128, 64};

	DrawRectangle(POSITION.x - 28, POSITION.y - 28, SIZE.x + 56, SIZE.y + 56, BLACK);
	DrawRectangle(POSITION.x - 24, POSITION.y - 24, SIZE.x + 48, SIZE.y + 48, PURPLE);
	DrawRectangle(POSITION.x, POSITION.y, SIZE.x, SIZE.y, VIOLET);

	Color text_color = BLACK;
	if(current_opt == 0) text_color = RAYWHITE;
	DrawText(TextFormat("Number of players: %i", party_count), POSITION.x + 8, POSITION.y + 8, 32, text_color);

	text_color = BLACK;
	if(current_opt == MAX_PARTY_PROFILES + 1) text_color = RAYWHITE;
	DrawText("> Next", POSITION.x + 8, POSITION.y + SIZE.y - 40, 32, text_color);

	for(int i = 0; i < party_count; i++)
	{
		text_color = BLACK;
		if(i + 1 == current_opt)
		{
			text_color = RAYWHITE;
		}
		unsigned char name[PROFILE_NAME_LENGHT] = {0};
		for(int j = 0; j < PROFILE_NAME_LENGHT; j++)
		{
			name[j] = profiles[i].name[j];
		}
		DrawText(TextFormat("%s", name), POSITION.x + 8, POSITION.y + 64 + 32 * i, 32, text_color);
	}
}

int main(void)
{
	InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "RAYMANIA");

	//TraceLog(LOG_INFO, "Size of Track %i.", sizeof(Track));
	//TraceLog(LOG_INFO, "Size of Blocks in Track %i.", sizeof(Block) * MAX_BLOCK_AMOUNT);

	SetTargetFPS(60);
	HideCursor();
	SetExitKey(KEY_NULL);

	if(!DEBUG)
	{
		RaylibLogo();
		TabinLogo();
	}

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

	Vector2int load_placement = (Vector2int){0, 0};
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

	CarStats car_stats = DefaultStats(track.car);

	Racecar car = InitRacecar(&car_stats);
	car.position = Vector2Zero();
	car.rotation = UP_VECTOR;

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
	double game_time = 0.0;
	double timer = 0.0;
	double start_time = 0.0;
	bool start_countdown = true;
	bool finished = false;

	bool paused = false;
	int pause_option = 0;
	unsigned char playing_demo = DEMO_OFF;

	// EDITOR VAR

	bool reset_editor = false;

	PieceInfo cursor_info = (PieceInfo){0};
	cursor_info.placement = (Vector2int){15, 15};

	bool placing_pieces = true;

	Block held_piece[BLOCKS_PER_PIECE] = {0}, piece_catalogue[PIECE_CATALOGUE_LENGHT] = {0};
	unsigned int piece_catalogue_page[9] = {0};
	unsigned int piece_catalogue_page_num = 0;
	unsigned int piece_catalogue_item = 0;
	float piece_catalogue_pulled = 0.0;

	bool validating_track = false;
	bool saving_track = false;
	unsigned char editor_four_option_selector = 0, efos_opt = 0;

	// PARTY VAR
	
	bool party_mode = false;
	char party_current_menu = 0;
	int party_current_opt = 0;

	unsigned int party_count = 2;
	Profile* party_profiles = PNULL;
	unsigned int party_timer_base = 10;
	double* party_timers = PNULL;

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
	MakeTrackBlocks(&track, blocks);


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
		if(InputPressed(input, INPUT_BACK) && popup_opt == 0)
		{
			popup_confirmed = true;
		}
		if(InputHeld(input, INPUT_LEFT))
		{
			popup_opt = 1;
		}
		if(InputHeld(input, INPUT_RIGHT) || InputPressed(input, INPUT_BACK))
		{
			popup_opt = 0;
		}
		if(InputPressed(input, INPUT_ENTER))
		{
			popup_confirmed = true;
		}
		if(InputPressed(input, INPUT_ESC))
		{
			popup_opt = 0;
			popup = POPUP_OFF;
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
			}
			if(popup == POPUP_RESET_MEDALS)
			{
				ResetMedalTimes(&track);
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
				MoveProfileSelectorCursor(fpl.count, &current_profile, -1);
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				MoveProfileSelectorCursor(fpl.count, &current_profile, 1);
			}
			if(InputHeld(menu_input, INPUT_LEFT))
			{
				MoveProfileSelectorCursor(fpl.count, &current_profile, -PROFILE_SELECTOR_PAGE_ITEMS);
			}
			if(InputHeld(menu_input, INPUT_RIGHT))
			{
				MoveProfileSelectorCursor(fpl.count, &current_profile, PROFILE_SELECTOR_PAGE_ITEMS);
			}
			if(InputPressed(input, INPUT_ENTER))
			{
				if(current_profile == fpl.count)
				{
					entering_profile_name = true;
					inputing_name = 1;
					name_lenght = PROFILE_NAME_LENGHT;
					for(int i = 0; i < PROFILE_NAME_LENGHT; i++)
					{
						profile_name[i] = 0;
					}
				}
				else
				{
					current_game_screen = MENU;
					const char* path = (const char*)fpl.paths[current_profile];
					if(party_mode)
					{
						party_profiles[party_current_opt - 1] = LoadProfile(path);
					}
					else
					{
						profile = LoadProfile(path);
						reset_menu = true;
					}
					UnloadDirectoryFiles(fpl);
					fpl.count = 0;
					//PrintProfile(&profile);
				}
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
					reset_menu = true;
				}
				current_game_screen = MENU;
				UnloadDirectoryFiles(fpl);
				fpl.count = 0;
			}
			if(party_mode)
			{
				if(InputPressed(input, INPUT_ESC))
				{
					current_game_screen = MENU;
					UnloadDirectoryFiles(fpl);
					fpl.count = 0;
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
				}
				else if(party_current_opt > 0)
				{
					party_current_opt--;
				}
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(party_current_opt < MAX_PARTY_PROFILES + 1)
				{
					party_current_opt++;
					if(party_current_opt > party_count)
					{
						party_current_opt = MAX_PARTY_PROFILES + 1;
					}
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
					}
				}
				if(InputHeld(menu_input, INPUT_LEFT))
				{
					if(party_count > 1)
					{
						party_count--;
						party_profiles = _realloc(party_profiles, sizeof(Profile) * party_count);
					}
				}
			}
			if(InputPressed(input, INPUT_BACK))
			{
				party_mode = false;
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
				}
				else if(party_current_opt == MAX_PARTY_PROFILES + 1)
				{
					party_current_menu = 1;
					party_current_opt = 0;
				}
				else
				{
					current_game_screen = PROFILES;
					load_profiles = true;
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
					}
				}
				if(InputHeld(menu_input, INPUT_LEFT))
				{
					if(party_timer_base > MIN_PARTY_TIMER)
					{
						party_timer_base--;
					}
				}
			}
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(party_current_opt > 0)
				{
					party_current_opt--;
				}
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(party_current_opt < 4)
				{
					party_current_opt++;
				}
			}
			if(InputPressed(input, INPUT_BACK))
			{
				party_current_menu = 0;
				party_current_opt = 0;
			}
			else if(InputPressed(input, INPUT_ENTER))
			{
				if(party_current_opt == 1)
				{
					file_list_active = FL_TRACK;
					load_file_list = true;
				}
				else if(party_current_opt == 2)
				{
					current_game_screen = EDITOR;
					reset_editor = true;
				}
				else if(party_current_opt == 3)
				{
					party_current_menu = 0;
					party_current_opt = 0;
				}
			}
		}
		else
		{
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(menu_option > MENU_PLAY && menu_option != MENU_RACE) menu_option -= 1;
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(menu_option < MENU_EXIT) menu_option += 1;
				else if(menu_option >= MENU_RACE && menu_option < MENU_PLAY_EXIT) menu_option += 1;
			}

			if(InputPressed(input, INPUT_BACK))
			{
				if(menu_option >= MENU_RACE)
				{
					if(menu_option == MENU_PLAY_EXIT) menu_option = MENU_PLAY;
					else menu_option = MENU_PLAY_EXIT;
				}
				else
				{
					if(menu_option == MENU_EXIT) exit = true;
					else menu_option = MENU_EXIT;
				}
			}
			if(InputPressed(input, INPUT_ENTER))
			{
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
			piece_catalogue_page_num = 0;
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
				efos_opt -= 1;
				if(efos_opt < 0)
				{
					efos_opt = 0;
				}
			}
			if(InputHeld(menu_input, INPUT_RIGHT))
			{
				efos_opt += 1;
				if(efos_opt > 3)
				{
					efos_opt = 3;
				}
			}
			if(InputPressed(input, INPUT_ESC) || InputPressed(input, INPUT_BACK))
			{
				end_efos = true;
			}
			else if(InputPressed(input, INPUT_ENTER))
			{
				end_efos = true;
				switch(editor_four_option_selector)
				{
					case(1): track.car = efos_opt; break;
					case(2): track.env = efos_opt; break;
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
			}
			if(InputHeld(menu_input, INPUT_LEFT))
			{
				MoveEditorCursor(&cursor_info, -1, 0);
				GetPiece(&cursor_info, held_piece);
			}
			if(InputHeld(menu_input, INPUT_RIGHT))
			{
				MoveEditorCursor(&cursor_info, 1, 0);
				GetPiece(&cursor_info, held_piece);
			}
			if(InputHeld(menu_input, INPUT_UP))
			{
				MoveEditorCursor(&cursor_info, 0, -1);
				GetPiece(&cursor_info, held_piece);
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				MoveEditorCursor(&cursor_info, 0, 1);
				GetPiece(&cursor_info, held_piece);
			}

			if(InputHeld(menu_input, INPUT_ENTER))
			{
				AddPiece(&track, blocks, &cursor_info);
			}
			if(InputHeld(menu_input, INPUT_BACK))
			{
				DeletePiece(&track, blocks, cursor_info.placement);
			}

			if(InputPressed(input, INPUT_ESC))
			{
				placing_pieces = false;
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
				}
				else
				{
					piece_catalogue_item = 9;
				}
			}
			if(InputPressed(input, INPUT_ESC))
			{
				placing_pieces = true;
			}
			if(piece_catalogue_item >= 0 && piece_catalogue_item < 9)
			{
				if(InputHeld(menu_input, INPUT_LEFT))
				{
					if(piece_catalogue_item == 0)
					{
						if(piece_catalogue_page_num > 0)
						{
							piece_catalogue_page_num--;
							PiecesInPage(piece_catalogue_page, piece_catalogue_page_num);
							LoadPieceCataloguePage(piece_catalogue, piece_catalogue_page);
							piece_catalogue_item = 8;
						}
					}
					else piece_catalogue_item--;
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
						}
					}
					else piece_catalogue_item++;
				}
				if(InputHeld(menu_input, INPUT_DOWN))
				{
					piece_catalogue_item += 9;
				}

				if(InputPressed(input, INPUT_ENTER))
				{
					cursor_info.id = piece_catalogue_page[piece_catalogue_item];
					GetPiece(&cursor_info, held_piece);
					placing_pieces = true;
				}
			}
			else
			{
				if(InputHeld(menu_input, INPUT_LEFT))
				{
					if(piece_catalogue_item > 9)
					{
						piece_catalogue_item--;
					}
				}
				if(InputHeld(menu_input, INPUT_RIGHT))
				{
					if(piece_catalogue_item < 17)
					{
						piece_catalogue_item++;
					}
				}
				if(InputHeld(menu_input, INPUT_UP))
				{
					piece_catalogue_item -= 9;
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
							name_lenght = TRACK_NAME_LENGHT;
							//for(int i = 0; i < TRACK_NAME_LENGHT; i++)
							//{
							//	track_name[i] = 0;
							//}
							break;
						case(EDITOR_CLEAR):
							popup = POPUP_EDITOR_CLEAR;
							break;
						case(EDITOR_ENVIROMENT):
							editor_four_option_selector = 2;
							break;
						case(EDITOR_CAR):
							editor_four_option_selector = 1;
							break;
						case(EDITOR_PAGE_JUMP):
							inputing_number = 1;
							in_type = 2;
							in_num = piece_catalogue_page_num;
							break;
						case(EDITOR_MEDALS):
							editor_four_option_selector = 3;
							break;
						case(EDITOR_VALIDATE):
							if(track.has_start)
							{
								current_game_screen = RACE;
								reset_race = true;
								validating_track = true;
							}
							break;
					}
				}
			}

			if(piece_catalogue_pulled < 1.0)
			{
				pcp_dir = 1.0;
			}
		}

		if(pcp_dir != 0.0)
		{
			float target = (pcp_dir + 1.0) * 0.5;
			float dif = absf(target - piece_catalogue_pulled);
			piece_catalogue_pulled += 0.10 * pcp_dir * dif;
			if(piece_catalogue_pulled < 0.0) piece_catalogue_pulled = 0.0;
			if(piece_catalogue_pulled > 1.0) piece_catalogue_pulled = 1.0;
		}
		break;
	case RACE:
		if(reset_race)
		{
			reset_race = false;

			ResetRacecar(&car, track.start_pos, track.start_rot, car_stats.size);
			ResetRacecar(&dcar, track.start_pos, track.start_rot, car_stats.size);
			MoveCameraInstant(&camera, car.position);

			dcheck_got = 0;
			dcheck_pos = track.start_pos;
			dcheck_rot = track.start_rot;

			checkpoints_gotten = 0;
			check_pos = track.start_pos;
			check_rot = track.start_rot;

			timer = 0.0;
			start_time = game_time;

			start_countdown = true;
			paused = false;
			stop_inputs = false;
			finished = false;

			load_placement = RacecarPlacement(&car);
			d_placement = RacecarPlacement(&car);
			LoadNearbyBlockWalls(blocks, block_walls, load_placement);
			LoadNearbyBlockWalls(blocks, dblock_walls, d_placement);

			if(playing_demo == DEMO_OFF)
			{
				ClearDemo(demo);
				demo = InitDemo();
				CopyNameToDemo(demo, profile.name);
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
				CopyNameToDemo(demo, profile.name);
				StartDemo(ghost_demo);
				playing_demo = DEMO_GHOST_INIT;
			}
		}

		bool cp_reset = false;

		ZoomCameraSmooth(&camera, 1.0, CAM_ZOOM_SPEED);
		if(paused)
		{
			if(InputHeld(menu_input, INPUT_UP))
			{
				if(pause_option > 0)
				{
					pause_option--;
				}
			}
			if(InputHeld(menu_input, INPUT_DOWN))
			{
				if(pause_option < 3)
				{
					pause_option++;
				}
			}
			bool pause_exit = false;
			if(InputPressed(input, INPUT_BACK))
			{
				if(pause_option == 3)
				{
					pause_exit = true;
				}
				else
				{
					pause_option = 3;
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
					reset_race = true;
					paused = false;
				}
				if(pause_option == 3)
				{
					pause_exit = true;
				}
			}
			if(InputPressed(input, INPUT_ESC))
			{
				paused = false;
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
		else if(start_countdown)
		{
			if(InputPressed(input, INPUT_ESC))
			{
				paused = true;
				pause_option = 0;
			}

			MoveCameraSmooth(&camera, car.position, 0.10);

			if(start_time + COUNTDOWN_TIME <= game_time)
			{
				start_countdown = false;
				if(playing_demo == DEMO_INIT)
				{
					playing_demo = DEMO_PLAY;
				}
				if(playing_demo == DEMO_GHOST_INIT && GetProfileBool(&profile, PRF_BOOL_GHOST_ENABLED))
				{
					playing_demo = DEMO_GHOST_PLAY;
					demo_input = (RMInput){0};
				}
			}
		}
		else
		{
			if(InputPressed(input, INPUT_ESC))
			{
				paused = true;
				pause_option = 0;
			}

			if(!stop_inputs)
			{
				timer += FRAME;
				if(playing_demo != DEMO_PLAY)
				{
					demo = RecordDemoInput(demo, input.current);
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

			float speed_change = absf(previous_speed - Vector2Length(car.velocity));

			if(GetProfileBool(&profile, PRF_BOOL_SCREEN_SHAKE) && speed_change > car_stats.camera_shake_threshold)
			{
				shake_time += (speed_change - car_stats.camera_shake_threshold) * car_stats.speed_to_shake_ratio;
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
				}
			}
			if(meta.finish && !finished && checkpoints_gotten == track.checkpoint_amount)
			{
				stop_inputs = true;
				finished = true;
				if(validating_track && track.has_start)
				{
					if(!track.validated)
					{
						TraceLog(LOG_INFO, "TRACK VALIDATED.");
						track.validated = true;
						track.medal_author = timer;
					}
					else if(timer < track.medal_author)
					{
						TraceLog(LOG_INFO, "NEW AUTHOR TIME!");
						track.medal_author = timer;
					}
					CalculateMedalTimes(&track);
				}
				if(ghost_demo != PNULL)
				{
					TraceLog(LOG_INFO, "Times: player %.3f, ghost %.3f", timer, ghost_demo->time);
				}
				demo->time = timer;
				bool save_new = false;
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
						filename = DemoFilename(DEMO_DIRECTORY, (const char*)demo_track_name, profile.name);
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
					reset_race = true;
				}
				else
				{
					cp_reset = true;
				}
			}
			if(InputPressed(input, INPUT_BACK))
			{
				reset_race = true;
			}
			if(InputPressed(input, INPUT_R))
			{
				SaveDemoTime(demo, (unsigned char*)TrackFileName(track_dir, track_name));
			}

			if(GetProfileBool(&profile, PRF_BOOL_CAM_CENTERED))
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
		if(InputHeld(menu_input, INPUT_UP))
		{
			MoveFileListCursor(fpl.count, &selected_file, -1);
		}
		if(InputHeld(menu_input, INPUT_DOWN))
		{
			MoveFileListCursor(fpl.count, &selected_file, 1);
		}
		if(InputHeld(menu_input, INPUT_LEFT))
		{
			MoveFileListCursor(fpl.count, &selected_file, -FILE_LIST_PAGE_ITEMS);
		}
		if(InputHeld(menu_input, INPUT_RIGHT))
		{
			MoveFileListCursor(fpl.count, &selected_file, FILE_LIST_PAGE_ITEMS);
		}
		if(InputPressed(input, INPUT_BACK))
		{
			unsigned char* file_dir;
			if(file_list_active == FL_TRACK)
			{
				file_dir = track_dir;
			}
			else if(file_list_active == FL_DEMO)
			{
				file_dir = demo_dir;
			}

			if(!TextIsEqual(demo_dir, TRACK_DIRECTORY))
			{
				ReturnToParentDirectory(demo_dir);
				load_file_list = true;
			}
			else
			{
				file_list_exit = true;
			}
		}

		if(fpl.count > 0 && (skip_validate_check || InputPressed(input, INPUT_ENTER)))
		{
			const char* path = (const char*)fpl.paths[selected_file];
			if(DirectoryExists(path))
			{
				ChangeToDirectory(demo_dir, (unsigned char*)path, true);
				load_file_list = true;
			}
			else if(file_list_active == FL_TRACK)
			{
				if(current_game_screen == MENU)
				{
					Track temp = LoadTrack(path);
					TrackNameFromFilename(path, track_name);
					if(temp.validated || skip_validate_check)
					{
						track = temp;
						MakeTrackBlocks(&track, blocks);
						file_list_active = FL_OFF;
						current_game_screen = RACE;
						reset_race = true;

						if(!validating_track)
						{
							const char* filename = DemoFilename(DEMO_DIRECTORY, TrackFileName(track_dir,track_name), profile.name);
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
					}
					else
					{
						popup = POPUP_VALIDATE;
					}
				}
				else if(current_game_screen == EDITOR)
				{
					track = LoadTrack(path);
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
					TrackNameFromFilename((const char*)demosave->track_name, track_name);
					MakeTrackBlocks(&track, blocks);

					file_list_active = FL_OFF;
					current_game_screen = RACE;
					reset_race = true;
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
		}
		else if(InputPressed(input, INPUT_ENTER))
		{
			inputing_name = 0;
			if(saving_track)
			{
				SaveTrack(&track, TrackFileName(track_dir, track_name), profile.name);
			}
			if(entering_profile_name)
			{
				const char* fname = ProfileFilename(PROFILE_DIRECTORY, profile_name);
				if(!FileExists(fname))
				{
					Profile new_prof = DefaultProfile();
					for(int i = 0; i < PROFILE_NAME_LENGHT; i++)
					{
						new_prof.name[i] = profile_name[i];
					}
					SaveProfile(&new_prof, fname);
					UnloadDirectoryFiles(fpl);
					fpl.count = 0;
					current_game_screen = MENU;
					if(party_mode)
					{
						party_profiles[party_current_opt - 1] = new_prof;
					}
					else
					{
						profile = new_prof;
						reset_menu = true;
					}
				}
				else
				{
					inputing_name = 2;
				}
			}
		}
		else if(InputPressed(input, INPUT_BACK))
		{
			if(name[0] == 0)
			{
				inputing_name = 0;
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
		}
		else if(InputPressed(input, INPUT_ENTER))
		{
			inputing_number = 0;
			if(in_type == 1)
			{
				switch(efos_opt)
				{
					case(0):
						track.medal_bronz = (float)in_num * 0.001;
						break;
					case(1):
						track.medal_silver = (float)in_num * 0.001;
						break;
					case(2):
						track.medal_gold = (float)in_num * 0.001;
						break;
				}
				CalculateMedalTimes(&track);
			}
			if(in_type == 2)
			{
				piece_catalogue_page_num = in_num;
				if(piece_catalogue_page_num < 0)
				{
					piece_catalogue_page_num = 0;
				}
				if(piece_catalogue_page_num >= PIECE_CATALOGUE_PAGE_AMOUNT)
				{
					piece_catalogue_page_num = PIECE_CATALOGUE_PAGE_AMOUNT - 1;
				}
				PiecesInPage(piece_catalogue_page, piece_catalogue_page_num);
				LoadPieceCataloguePage(piece_catalogue, piece_catalogue_page);
			}
		}
		else if(InputPressed(input, INPUT_BACK))
		{
			if(in_num >= 10)
			{
				in_num *= 0.1;
			}
			else if(in_num == 0)
			{
				inputing_number = 0;
			}
			else
			{
				in_num = 0;
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

		ClearBackground(RAYWHITE);
		if(track.env == ENV_STADIUM)
		{
			ClearBackground(LIME);
		}
		if(track.env == ENV_QUARRY)
		{
			ClearBackground(BROWN);
		}
		if(track.env == ENV_ISLAND)
		{
			ClearBackground(BLUE);
		}

		// Gamespace

		BeginMode2D(camera.data);

		for(int i = 0; i <= BLOCK_SIZE * TRACK_GRID_SIZE; i += BLOCK_SIZE)
		{
			DrawLine(i, 0, i, BLOCK_SIZE * TRACK_GRID_SIZE, GRAY);
			DrawLine(0, i, BLOCK_SIZE * TRACK_GRID_SIZE, i, GRAY);
		}

		DrawPlacedBlocksDebug(blocks, 0);
		DrawLoadedBlockWallsDebug(block_walls);

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
					DrawBlockDebug(held_piece[i]);
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
					DrawRacecarWalls(&dcar, true);
				}
				DrawRacecarWalls(&car, false);
			} break;
		}

		DrawPlacedBlocksDebug(blocks, 1);

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

				if(party_mode && party_current_menu == 0)
				{
					DrawPartyMenu(party_current_opt, party_count, party_profiles);
				}
				else if(party_mode && party_current_menu == 1)
				{
					const Vector2 SIZE = (Vector2){768, 512};
					const Vector2 POSITION = (Vector2){128, 64};

					DrawRectangle(POSITION.x - 28, POSITION.y - 28, SIZE.x + 56, SIZE.y + 56, BLACK);
					DrawRectangle(POSITION.x - 24, POSITION.y - 24, SIZE.x + 48, SIZE.y + 48, PURPLE);
					DrawRectangle(POSITION.x, POSITION.y, SIZE.x, SIZE.y, VIOLET);

					for(int i = 0; i < 4; i++)
					{
						Color text_color = BLACK;
						if(party_current_opt == i) text_color = RAYWHITE;
						Vector2 pos = POSITION;
						pos.y += 8 + i * 32;
						pos.x += 8;
						if(i == 0)
						{
							DrawText(TextFormat("Timer: %.3f", (float)party_timer_base * 30.0), pos.x, pos.y, 32, text_color);
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
				else if(menu_option >= MENU_RACE) for(int i = 0; i < 3; i++)
				{
					int y_pos = 192 + 80 * i;
					if(i + MENU_RACE == menu_option)
					{
						DrawRectangle(316, y_pos - 4, 392, 72, RAYWHITE);
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
						DrawRectangle(316, y_pos - 4, 392, 72, RAYWHITE);
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
							//DrawRectangle(320, y_pos, 384, 64, GRAY);
							//DrawText("PARTY", 512 - 111, y_pos, 64, DARKGRAY);
							DrawRectangle(320, y_pos, 384, 64, PURPLE);
							DrawText("PARTY", 512 - 111, y_pos, 64, DARKPURPLE);
						break;
						case(MENU_OPTIONS):
							DrawRectangle(320, y_pos, 384, 64, GRAY);
							DrawText("OPTIONS", 512 - 146, y_pos, 64, DARKGRAY);
							//DrawRectangle(320, y_pos, 384, 64, SKYBLUE);
							//DrawText("OPTIONS", 512 - 146, y_pos, 64, DARKBLUE);
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
						DrawRectangle(pos.x - 8, pos.y - 8, 80, 80, RAYWHITE);
						switch(i)
						{
							case(EDITOR_EXIT):
								DrawText("EXIT", pos.x, pos.y, TEXT_SIZE, RED);
								break;
							case(EDITOR_LOAD):
								DrawText("LOD", pos.x, pos.y, TEXT_SIZE, DARKGREEN);
								break;
							case(EDITOR_SAVE):
								DrawText("SAV", pos.x, pos.y, TEXT_SIZE, DARKBLUE);
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
								DrawText("JUMP", pos.x, pos.y, TEXT_SIZE, VIOLET);
								break;
							case(EDITOR_MEDALS):
								DrawText("TIME", pos.x, pos.y, TEXT_SIZE, GRAY);
								break;
							case(EDITOR_VALIDATE):
								if(track.has_start)
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
						DrawRectangle(328 + i * 96, 256, 80, 80, RAYWHITE);
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
						DrawText("AUTHOR", 624, 264, 16, BLACK);
						break;
					default: break;
					}
				}
			} break;
			case RACE:
			{
				if(DEBUG)
				{
					DrawText(TextFormat("pos %i %i", load_placement.x, load_placement.y), 8, 16, 32, BLACK);
					DrawText(TextFormat("shake_time %.3f", shake_time), 8, 48, 32, BLACK);
				}
				if(finished)
				{
					if(validating_track)
					{
					}
					else if(timer < track.medal_author)
					{
						DrawText("AUTHOR MEDAL!!!", 192, 88, 96, BLACK);
					}
					else if(timer < track.medal_gold)
					{
						DrawText("GOLD MEDAL!!", 256, 88, 96, BLACK);
					}
					else if(timer < track.medal_silver)
					{
						DrawText("Silver medal!", 256, 88, 96, BLACK);
					}
					else if(timer < track.medal_bronz)
					{
						DrawText("Bronz medal.", 256, 88, 96, BLACK);
					}
					DrawText(TextFormat("%.3f", timer), 384, 192, 96, BLACK);
				}
				else
				{
					DrawText(TextFormat("%.3f", timer), 8, 572, 64, BLACK);
					if(track.checkpoint_amount > 0)
					{
						DrawText(TextFormat("%i/%i", checkpoints_gotten, track.checkpoint_amount), 896, 576, 64, BLUE);
					}
				}
				if(start_countdown)
				{
					DrawText(TextFormat("%i", 3 - (int)((game_time - start_time) / COUNTDOWN_TIME * 3)), 576, 256, 128, BLACK);
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
							DrawRectangle(276, pos_y, 472, 48, RAYWHITE);
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
				//TraceLog(LOG_INFO, "Text size %i.", MeasureText("Resume", 32) / 2);
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
					float compare_timer = 0.0;
					if(efos_opt == 0) compare_timer = track.medal_silver;
					if(efos_opt == 1) compare_timer = track.medal_gold;
					if(efos_opt == 2) compare_timer = track.medal_author;
					DrawText("Enter Time:", 384, 264, 32, BLACK);
					DrawText(TextFormat("Medal above %.3f", compare_timer), 104, 300, 32, BLACK);
					DrawText(TextFormat("%.3f", (float)in_num * 0.001), 104, 336, 32, BLACK);
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
			DrawRectangle(348 + (1 - popup_opt) * 192, 332, 136, 48, RAYWHITE);
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
			}
		}

		if(current_game_screen == PROFILES)
		{
			DrawFade(&camera, RAYWHITE);
		}

	EndDrawing();
	}

	CloseWindow();

	return 0;
}
