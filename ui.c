#include <raylib.h>
#include "rmlib.h"
#include "profiles.h"
#include "asset.h"
#include "ui.h"

bool MoveFileListCursor(unsigned int count, int* current, int move)
{
	int previous = *current;
	*current = *current + move;
	if(*current < 0)
	{
		*current = 0;
	}
	if(*current >= count)
	{
		*current = count - 1;
	}
	return previous != *current;
}

void DrawFileList(FilePathList fpl, int current, Color bg1, Color bg2)
{
	const Vector2 SIZE = (Vector2){512, 448};
	const Vector2 POSITION = (Vector2){256, 160};

	DrawRectangle(POSITION.x - 4, POSITION.y - 4, SIZE.x + 8, SIZE.y + 8, BLACK);
	DrawRectangle(POSITION.x, POSITION.y, SIZE.x, SIZE.y, bg1); // GREEN
	DrawRectangle(POSITION.x + 24, POSITION.y + 24, SIZE.x - 48, SIZE.y - 80, bg2); // LIME

	int page = current / FILE_LIST_PAGE_ITEMS;
	DrawText(TextFormat("%i/%i", page + 1, (fpl.count - 1) / FILE_LIST_PAGE_ITEMS + 1), POSITION.x + 24, POSITION.y + SIZE.y - 44, 32, BLACK);
	DrawText("Backspace to return.", POSITION.x + 140, POSITION.y + SIZE.y - 44, 32, BLACK);

	for(int i = 0; i < FILE_LIST_PAGE_ITEMS; i++)
	{
		int item = i + page * FILE_LIST_PAGE_ITEMS;
		if(item >= fpl.count)
		{
			break;
		}
		unsigned char* filepath = fpl.paths[item];
#ifdef WINDOWS
		unsigned int pos = TextFindLastChar(filepath, '\\'), len = TextLength(filepath);
#else
		unsigned int pos = TextFindLastChar(filepath, '/'), len = TextLength(filepath);
#endif
		//TraceLog(LOG_INFO, "RL_MALLOC: draw file list");
		unsigned char* file = (unsigned char*)RL_MALLOC(len - pos);
		for(int j = 0; j < len - pos - 1; j++)
		{
			file[j] = filepath[j + pos + 1];
		}
		file[len - pos - 1] = '\0';
		Color color = BLACK;
		if(item == current)
		{
			color = RAYWHITE;
		}
		DrawText(TextFormat("%s", file), POSITION.x + 32, POSITION.y + 32 + 32 * i, 32, color);
		//TraceLog(LOG_INFO, "RL_FREE: draw file list");
		RL_FREE(file);
	}
}

unsigned char* ChangeToDirectory(unsigned char* dir, unsigned char* new_dir, bool overwrite)
{
	if(overwrite)
	{
		TraceLog(LOG_INFO, "RL_FREE: overwrite dir");
		_free(dir);
		TraceLog(LOG_INFO, "RL_MALLOC: overwrite dir");
		dir = (unsigned char*)_malloc(TextLength(new_dir));
		TextCopy(dir, new_dir);
	}
	else
	{
#ifdef WINDOWS
		if(!DirectoryExists(TextFormat("%s\\%s", dir, new_dir)))
#else
		if(!DirectoryExists(TextFormat("%s/%s", dir, new_dir)))
#endif
		{
			TraceLog(LOG_INFO, "Can't change: %s", dir);
			return dir;
		}
#ifdef WINDOWS
		const char* full_new_dir = TextFormat("%s\\%s", dir, new_dir);
#else
		const char* full_new_dir = TextFormat("%s/%s", dir, new_dir);
#endif
		TraceLog(LOG_INFO, "RL_REALLOC: change dir");
		dir = (unsigned char*)_realloc(dir, TextLength(full_new_dir));
		TextCopy(dir, full_new_dir);
	}
	TraceLog(LOG_INFO, "New dir: %s", dir);

	return dir;
}

unsigned char* ReturnToParentDirectory(unsigned char* dir)
{
#ifdef WINDOWS
	unsigned int pos = TextFindLastChar(dir, '\\');
#else
	unsigned int pos = TextFindLastChar(dir, '/');
#endif
	if(pos == 0)
	{
		TraceLog(LOG_INFO, "Return failed: %s", dir);
		return dir;
	}
	TraceLog(LOG_INFO, "RL_REALLOC: return dir");
	dir = (unsigned char*)RL_REALLOC(dir, pos + 1);
	dir[pos] = '\0';
	TraceLog(LOG_INFO, "Return success: %s", dir);
	return dir;
}

void DrawCursor(Vector2 cursor_pos, float size, Color color)
{
	DrawLine(cursor_pos.x, cursor_pos.y, cursor_pos.x + size, cursor_pos.y, color);
	DrawLine(cursor_pos.x, cursor_pos.y, cursor_pos.x, cursor_pos.y + size, color);
	DrawLine(cursor_pos.x, cursor_pos.y + size, cursor_pos.x + size, cursor_pos.y + size, color);
	DrawLine(cursor_pos.x + size, cursor_pos.y, cursor_pos.x + size, cursor_pos.y + size, color);
}

void DrawGuideArrow(Vector2 start, Vector2 target)
{
	Vector2 difference = Vector2Subtract(target, start);
	Vector2 arrow_direction = Vector2Normalize(difference);
	Vector2 arrow_offset = Vector2Scale(arrow_direction, 128);
	Vector2 arrow_pos = Vector2Add(SCREEN_CENTER, arrow_offset);
	Tri arrow = (Tri){0};
	arrow.color = RM_LIME3;
	arrow.a = (Vector2){16 * arrow_direction.x + arrow_pos.x, 16 * arrow_direction.y + arrow_pos.y};
	arrow.b = (Vector2){8 * arrow_direction.y + arrow_pos.x, -8 * arrow_direction.x + arrow_pos.y};
	arrow.c = (Vector2){-8 * arrow_direction.y + arrow_pos.x, 8 * arrow_direction.x + arrow_pos.y};
	DrawTriangle(arrow.a, arrow.b, arrow.c, rmc(arrow.color));
	DrawPixel(arrow_pos.x, arrow_pos.y, BLACK);
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
	DrawText("> Next", POSITION.x + 8, POSITION.y + SIZE.y - 72, 32, text_color);

	text_color = BLACK;
	if(current_opt == MAX_PARTY_PROFILES + 2) text_color = RAYWHITE;
	DrawText("< Back", POSITION.x + 8, POSITION.y + SIZE.y - 40, 32, text_color);

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

void DrawOptions(int current, int page, int max, Profile* profile)
{
	// TODO: Instead of the ifs, make a struct that will store page info, and this will just draw that struct
	const Vector2 SIZE = (Vector2){768, 512};
	const Vector2 POSITION = (Vector2){128, 64};

	DrawRectangle(POSITION.x - 28, POSITION.y - 28, SIZE.x + 56, SIZE.y + 56, BLACK);
	DrawRectangle(POSITION.x - 24, POSITION.y - 24, SIZE.x + 48, SIZE.y + 48, SKYBLUE);
	DrawRectangle(POSITION.x, POSITION.y, SIZE.x, SIZE.y, BLUE);

	Color text_color = BLACK;

	// OPTIONS DRAW
	if(page == OPTPAGE_MAIN)
	{
		for(int i = 0; i < max; i++)
		{
			text_color = BLACK;
			if(i == current)
			{
				text_color = RAYWHITE;
			}
			Vector2 pos = (Vector2){POSITION.x + 8, POSITION.y + 32 * i};
			if(i == 0)
			{
				DrawText("PROFILE", pos.x, pos.y, 32, text_color);
			}
			else if(i == 1)
			{
				DrawText("GAMEPLAY", pos.x, pos.y, 32, text_color);
			}
			else if(i == 2)
			{
				DrawText("AUDIO", pos.x, pos.y, 32, text_color);
			}
			else if(i == 3)
			{
				DrawText("EDITOR", pos.x, pos.y, 32, text_color);
			}
			else
			{
				DrawText("PARTY", pos.x, pos.y, 32, text_color);
			}
		}
	}
	if(page == OPTPAGE_PROFILES)
	{
		for(int i = 0; i < max; i++)
		{
			text_color = BLACK;
			if(i == current)
			{
				text_color = RAYWHITE;
			}
			Vector2 pos = (Vector2){POSITION.x + 8, POSITION.y + 32 * i};
			if(i == 0)
			{
				DrawText("Change Profile", pos.x, pos.y, 32, text_color);
			}
			else if(i == 1)
			{
				DrawText(TextFormat("Name: %s", profile->name), pos.x, pos.y, 32, text_color);
			}
			else if(i == 2)
			{
				DrawText("MODELS", pos.x, pos.y, 32, text_color);
			}
			else
			{
				DrawText("PALETTES", pos.x, pos.y, 32, text_color);
			}
		}
	}
	if(page == OPTPAGE_GAMEPLAY)
	{
		for(int i = 0; i < max; i++)
		{
			text_color = BLACK;
			if(i == current)
			{
				text_color = RAYWHITE;
			}
			Vector2 pos = (Vector2){POSITION.x + 8, POSITION.y + 32 * i};
			if(i == 0)
			{
				if(GetProfileBool(profile, PRF_BOOL_SCREEN_SHAKE))
				{
					DrawText("Screen Shake: ON", pos.x, pos.y, 32, text_color);
				}
				else
				{
					DrawText("Screen Shake: OFF", pos.x, pos.y, 32, text_color);
				}
			}
			else if(i == 1)
			{
				if(GetProfileBool(profile, PRF_BOOL_CAM_CENTERED))
				{
					DrawText("Centered Camera: ON", pos.x, pos.y, 32, text_color);
				}
				else
				{
					DrawText("Centered Camera: OFF", pos.x, pos.y, 32, text_color);
				}
			}
			else if(i == 2)
			{
				if(GetProfileBool(profile, PRF_BOOL_GHOST_ENABLED))
				{
					DrawText("Ghosts: ON", pos.x, pos.y, 32, text_color);
				}
				else
				{
					DrawText("Ghosts: OFF", pos.x, pos.y, 32, text_color);
				}
			}
			else
			{
				DrawText(TextFormat("Camera Zoom: %f", profile->camera_zoom), pos.x, pos.y, 32, text_color);
			}
		}
	}
	if(page == OPTPAGE_AUDIO)
	{
		for(int i = 0; i < max; i++)
		{
			text_color = BLACK;
			if(i == current)
			{
				text_color = RAYWHITE;
			}
			Vector2 pos = (Vector2){POSITION.x + 8, POSITION.y + 32 * i};
			if(i == 0)
			{
				if(profile->master_volume == -11)
				{
					DrawText("Master: OFF", pos.x, pos.y, 32, text_color);
				}
				else
				{
					DrawText(TextFormat("Master: %i", profile->master_volume), pos.x, pos.y, 32, text_color);
				}
			}
			else if(i == 1)
			{
				if(profile->sfx_volume == -11)
				{
					DrawText("SFX: OFF", pos.x, pos.y, 32, text_color);
				}
				else
				{
					DrawText(TextFormat("SFX: %i", profile->sfx_volume), pos.x, pos.y, 32, text_color);
				}
			}
			else
			{
				if(profile->music_volume == -11)
				{
					DrawText("Music: OFF", pos.x, pos.y, 32, text_color);
				}
				else
				{
					DrawText(TextFormat("Music: %i", profile->music_volume), pos.x, pos.y, 32, text_color);
				}
			}
		}
	}
	if(page == OPTPAGE_EDITOR)
	{
		for(int i = 0; i < max; i++)
		{
			text_color = BLACK;
			if(i == current)
			{
				text_color = RAYWHITE;
			}
			Vector2 pos = (Vector2){POSITION.x + 8, POSITION.y + 32 * i};
			if(i == 0)
			{
				if(GetProfileBool(profile, PRF_BOOL_BLOCKMIXING))
				{
					DrawText("Blockmixing: ON (WARNING: Buggy)", pos.x, pos.y, 32, text_color);
				}
				else
				{
					DrawText("Blockmixing: OFF", pos.x, pos.y, 32, text_color);
				}
			}
		}
	}
	if(page == OPTPAGE_PARTY)
	{
		for(int i = 0; i < max; i++)
		{
			text_color = BLACK;
			if(i == current)
			{
				text_color = RAYWHITE;
			}
			Vector2 pos = (Vector2){POSITION.x + 8, POSITION.y + 32 * i};
			if(i == 0)
			{
				if(GetProfileBool(profile, PRF_BOOL_HIDE_PARTY_TIME))
				{
					DrawText("Hide remaining time: ON", pos.x, pos.y, 32, text_color);
				}
				else
				{
					DrawText("Hide remaining time: OFF", pos.x, pos.y, 32, text_color);
				}
			}
		}
	}
	text_color = BLACK;
	if(max == current)
	{
		text_color = RAYWHITE;
	}
	Vector2 pos = (Vector2){POSITION.x + 8, POSITION.y + 32 * max};
	DrawText("< Exit", pos.x, pos.y, 32, text_color);
}

void DrawOptionsCustomization(int car, int current, int max, int set)
{
	const Vector2 SIZE = (Vector2){768, 192};
	const Vector2 HALF_SIZE = Vector2Scale(SIZE, 0.5);
	const Vector2 POSITION = Vector2Subtract(SCREEN_CENTER, HALF_SIZE);

	DrawRectangle(POSITION.x - 28, POSITION.y - 28, SIZE.x + 56, SIZE.y + 56, BLACK);
	DrawRectangle(POSITION.x - 24, POSITION.y - 24, SIZE.x + 48, SIZE.y + 48, SKYBLUE);
	DrawRectangle(POSITION.x, POSITION.y, SIZE.x - 512, SIZE.y, BLUE);
	DrawRectangle(POSITION.x + 280, POSITION.y, SIZE.x - 280, SIZE.y, RAYWHITE);

	Color text_color = BLACK;
	Vector2 pos = (Vector2){0, 0};

	for(int i = 0; i < 5; i++)
	{
		text_color = BLACK;
		if(i == car)
		{
			text_color = RAYWHITE;
		}
		pos = (Vector2){POSITION.x + 8, POSITION.y + 32 * i};
		if(i == 0)
		{
			DrawText("ROAD CAR", pos.x, pos.y, 32, text_color);
		}
		else if(i == 1)
		{
			DrawText("DRIFT CAR", pos.x, pos.y, 32, text_color);
		}
		else if(i == 2)
		{
			DrawText("GRIP CAR", pos.x, pos.y, 32, text_color);
		}
		else if(i == 3)
		{
			DrawText("TERRA CAR", pos.x, pos.y, 32, text_color);
		}
		else
		{
			DrawText("< Exit", pos.x, pos.y, 32, text_color);
		}
	}
	if(car < 4)
	{
		pos = (Vector2){POSITION.x + 280, POSITION.y};
		Vector2 pos_set = pos;
		if(max <= 5 || current < 3)
		{
			pos.x += 96 * current;
		}
		else if(current + 2 >= max)
		{
			pos.x += 96 * (current - max + 5);
			set = set - max + 5;
		}
		else
		{
			pos.x += 96 * 2;
			set = set - current + 2;
		}
		if(set >= 0 && set < 5)
		{
			pos_set.x += 96 * set;
			DrawRectangle(pos_set.x, pos_set.y, 96, SIZE.y, GREEN);
		}
		DrawLine(pos.x,      pos.y,          pos.x + 96, pos.y,          BLACK);
		DrawLine(pos.x,      pos.y,          pos.x,      pos.y + SIZE.y, BLACK);
		DrawLine(pos.x,      pos.y + SIZE.y, pos.x + 96, pos.y + SIZE.y, BLACK);
		DrawLine(pos.x + 96, pos.y,          pos.x + 96, pos.y + SIZE.y, BLACK);
	}
}

void DrawOptionsCars(int current, int max, unsigned char palette)
{
	const Vector2 HALF_SIZE = (Vector2){104, 96};
	const Vector2 POSITION = Vector2Subtract(SCREEN_CENTER, HALF_SIZE);

	int start = 0, end = 0;
	if(max <= 5 || current < 3)
	{
		start = 0;
		end = max > 5 ? 5 : max;
	}
	else if(current + 2 >= max)
	{
		start = max - 5;
		end = max;
	}
	else
	{
		start = current - 2;
		end = current + 3;
	}

	for(int i = start; i < end; i++)
	{
		Asset* asset = AllocRacecarAsset((unsigned char)i);
		PaintAsset(asset, GetPalette(palette));
		Vector2 pos = POSITION;
		pos.x += 46 + 96 * (i - start);
		pos.y += 96;
		DrawAsset(asset, 1.5, pos);
		FreeAsset(asset);
	}
}

void DrawOptionsPalettes(int current, int max)
{
	const Vector2 HALF_SIZE = (Vector2){104, 96};
	const Vector2 POSITION = Vector2Subtract(SCREEN_CENTER, HALF_SIZE);

	int start = 0, end = 0;
	if(max <= 5 || current < 3)
	{
		start = 0;
		end = max > 5 ? 5 : max;
	}
	else if(current + 2 >= max)
	{
		start = max - 5;
		end = max;
	}
	else
	{
		start = current - 2;
		end = current + 3;
	}

	for(int i = start; i < end; i++)
	{
		Palette palette = GetPalette(i);
		for(int j = 0; j < CAR_COLOR_AMOUNT; j++)
		{
			Vector2 pos = POSITION;
			pos.x += 12 + 96 * (i - start) + 24 * (j % 3);
			pos.y += 8 + 20 * (j / 3);
			DrawRectangle(pos.x, pos.y, 24, 20, rmc(palette.colors[j]));
		}
	}
}
