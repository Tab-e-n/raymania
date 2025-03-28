#include <raylib.h>
#include <stdio.h>
#include "rmlib.h"
#include "asset.h"


#define COLOR_SELECT_SIZE 4
#define TRI_AMOUNT 256


const char* rmcname(char c)
{
	switch(c)
	{
		case(RL_GRAY0): return "RL_GRAY0\0";
		case(RL_GRAY1): return "RL_GRAY1\0";
		case(RL_GRAY2): return "RL_GRAY2\0";
		case(RL_YELLOW0): return "RL_YELLOW0\0";
		case(RL_YELLOW1): return "RL_YELLOW1\0";
		case(RL_YELLOW2): return "RL_YELLOW2\0";
		case(RL_RED0): return "RL_RED0\0";
		case(RL_RED1): return "RL_RED1\0";
		case(RL_RED2): return "RL_RED2\0";
		case(RL_GREEN0): return "RL_GREEN0\0";
		case(RL_GREEN1): return "RL_GREEN1\0";
		case(RL_GREEN2): return "RL_GREEN2\0";
		case(RL_BLUE0): return "RL_BLUE0\0";
		case(RL_BLUE1): return "RL_BLUE1\0";
		case(RL_BLUE2): return "RL_BLUE2\0";
		case(RL_PURPLE0): return "RL_PURPLE0\0";
		case(RL_PURPLE1): return "RL_PURPLE1\0";
		case(RL_PURPLE2): return "RL_PURPLE2\0";
		case(RL_BROWN0): return "RL_BROWN0\0";
		case(RL_BROWN1): return "RL_BROWN1\0";
		case(RL_BROWN2): return "RL_BROWN2\0";
		case(RL_WHITE): return "RL_WHITE\0";
		case(RL_BLACK): return "RL_BLACK\0";
		case(RL_MAGENTA): return "RL_MAGENTA\0";
		case(RL_RAYWHITE): return "RL_RAYWHITE\0";
		case(RM_WHITE0): return "RM_WHITE0\0";
		case(RM_WHITE1): return "RM_WHITE1\0";
		case(RM_WHITE2): return "RM_WHITE2\0";
		case(RM_WHITE3): return "RM_WHITE3\0";
		case(RM_WHITE4): return "RM_WHITE4\0";
		case(RM_WHITE5): return "RM_WHITE5\0";
		case(RM_WHITE6): return "RM_WHITE6\0";
		case(RM_WHITE7): return "RM_WHITE7\0";
		case(RM_WHITE8): return "RM_WHITE8\0";
		case(RM_RED0): return "RM_RED0\0";
		case(RM_RED1): return "RM_RED1\0";
		case(RM_RED2): return "RM_RED2\0";
		case(RM_RED3): return "RM_RED3\0";
		case(RM_RED4): return "RM_RED4\0";
		case(RM_RED5): return "RM_RED5\0";
		case(RM_RED6): return "RM_RED6\0";
		case(RM_YELLOW0): return "RM_YELLOW0\0";
		case(RM_YELLOW1): return "RM_YELLOW1\0";
		case(RM_YELLOW2): return "RM_YELLOW2\0";
		case(RM_YELLOW3): return "RM_YELLOW3\0";
		case(RM_YELLOW4): return "RM_YELLOW4\0";
		case(RM_YELLOW5): return "RM_YELLOW5\0";
		case(RM_YELLOW6): return "RM_YELLOW6\0";
		case(RM_GREEN0): return "RM_GREEN0\0";
		case(RM_GREEN1): return "RM_GREEN1\0";
		case(RM_GREEN2): return "RM_GREEN2\0";
		case(RM_GREEN3): return "RM_GREEN3\0";
		case(RM_GREEN4): return "RM_GREEN4\0";
		case(RM_GREEN5): return "RM_GREEN5\0";
		case(RM_GREEN6): return "RM_GREEN6\0";
		case(RM_CYAN0): return "RM_CYAN0\0";
		case(RM_CYAN1): return "RM_CYAN1\0";
		case(RM_CYAN2): return "RM_CYAN2\0";
		case(RM_CYAN3): return "RM_CYAN3\0";
		case(RM_CYAN4): return "RM_CYAN4\0";
		case(RM_CYAN5): return "RM_CYAN5\0";
		case(RM_CYAN6): return "RM_CYAN6\0";
		case(RM_NAVY0): return "RM_NAVY0\0";
		case(RM_NAVY1): return "RM_NAVY1\0";
		case(RM_NAVY2): return "RM_NAVY2\0";
		case(RM_NAVY3): return "RM_NAVY3\0";
		case(RM_NAVY4): return "RM_NAVY4\0";
		case(RM_NAVY5): return "RM_NAVY5\0";
		case(RM_NAVY6): return "RM_NAVY6\0";
		case(RM_PINK0): return "RM_PINK0\0";
		case(RM_PINK1): return "RM_PINK1\0";
		case(RM_PINK2): return "RM_PINK2\0";
		case(RM_PINK3): return "RM_PINK3\0";
		case(RM_PINK4): return "RM_PINK4\0";
		case(RM_PINK5): return "RM_PINK5\0";
		case(RM_PINK6): return "RM_PINK6\0";
		case(RM_ORANGE0): return "RM_ORANGE0\0";
		case(RM_ORANGE1): return "RM_ORANGE1\0";
		case(RM_ORANGE2): return "RM_ORANGE2\0";
		case(RM_ORANGE3): return "RM_ORANGE3\0";
		case(RM_ORANGE4): return "RM_ORANGE4\0";
		case(RM_ORANGE5): return "RM_ORANGE5\0";
		case(RM_ORANGE6): return "RM_ORANGE6\0";
		case(RM_LIME0): return "RM_LIME0\0";
		case(RM_LIME1): return "RM_LIME1\0";
		case(RM_LIME2): return "RM_LIME2\0";
		case(RM_LIME3): return "RM_LIME3\0";
		case(RM_LIME4): return "RM_LIME4\0";
		case(RM_LIME5): return "RM_LIME5\0";
		case(RM_LIME6): return "RM_LIME6\0";
		case(RM_TEAL0): return "RM_TEAL0\0";
		case(RM_TEAL1): return "RM_TEAL1\0";
		case(RM_TEAL2): return "RM_TEAL2\0";
		case(RM_TEAL3): return "RM_TEAL3\0";
		case(RM_TEAL4): return "RM_TEAL4\0";
		case(RM_TEAL5): return "RM_TEAL5\0";
		case(RM_TEAL6): return "RM_TEAL6\0";
		case(RM_BLUE0): return "RM_BLUE0\0";
		case(RM_BLUE1): return "RM_BLUE1\0";
		case(RM_BLUE2): return "RM_BLUE2\0";
		case(RM_BLUE3): return "RM_BLUE3\0";
		case(RM_BLUE4): return "RM_BLUE4\0";
		case(RM_BLUE5): return "RM_BLUE5\0";
		case(RM_BLUE6): return "RM_BLUE6\0";
		case(RM_PURPLE0): return "RM_PURPLE0\0";
		case(RM_PURPLE1): return "RM_PURPLE1\0";
		case(RM_PURPLE2): return "RM_PURPLE2\0";
		case(RM_PURPLE3): return "RM_PURPLE3\0";
		case(RM_PURPLE4): return "RM_PURPLE4\0";
		case(RM_PURPLE5): return "RM_PURPLE5\0";
		case(RM_PURPLE6): return "RM_PURPLE6\0";
		case(RM_MAGENTA0): return "RM_MAGENTA0\0";
		case(RM_MAGENTA1): return "RM_MAGENTA1\0";
		case(RM_MAGENTA2): return "RM_MAGENTA2\0";
		case(RM_MAGENTA3): return "RM_MAGENTA3\0";
		case(RM_MAGENTA4): return "RM_MAGENTA4\0";
		case(RM_MAGENTA5): return "RM_MAGENTA5\0";
		case(RM_MAGENTA6): return "RM_MAGENTA6\0";
	}
}

Tri SetPoint(Tri tri, Vector2 pos, int point)
{
	if(point == 0)
	{
		tri.a = pos;
	}
	else if(point == 1)
	{
		tri.b = pos;
	}
	else
	{
		tri.c = pos;
	}
	return tri;
}

Tri MovePoint(Tri tri, Vector2 shift, int point)
{
	if(point == 0)
	{
		tri.a = Vector2Add(tri.a, shift);
	}
	else if(point == 1)
	{
		tri.b = Vector2Add(tri.b, shift);
	}
	else
	{
		tri.c = Vector2Add(tri.c, shift);
	}
	return tri;
}

void PrintAsset(Asset* asset)
{
	TraceLog(LOG_INFO, "PRINTING ASSET");
	int id = 0;
	for(int i = 0; i < TRI_AMOUNT; i++)
	{
		Tri tri = asset->tris[i];
		if(!TriIsPoint(tri))
		{
			printf("%i: %f %f %f %f %f %f\n", id, tri.a.x, tri.a.y, tri.b.x, tri.b.y, tri.c.x, tri.c.y);
			id++;
		}
	}
}

Vector2 borders(Vector2 border, float point)
{
	border.x = point < border.x ? point : border.x;
	border.y = point > border.y ? point : border.y;
	return border;
}

void PrintAssetCentered(Asset* asset)
{
	TraceLog(LOG_INFO, "PRINTING ASSET (GAME)");
	Vector2 border_hori = (Vector2){128, 128};
	Vector2 border_veri = (Vector2){80, 80};
	for(int i = 0; i < TRI_AMOUNT; i++)
	{
		Tri tri = asset->tris[i];
		if(!TriIsPoint(tri))
		{
			border_hori = borders(border_hori, tri.a.x);
			border_veri = borders(border_veri, tri.a.y);
			border_hori = borders(border_hori, tri.b.x);
			border_veri = borders(border_veri, tri.b.y);
			border_hori = borders(border_hori, tri.c.x);
			border_veri = borders(border_veri, tri.c.y);
			//TraceLog(LOG_INFO, "h: %f %f", border_hori.x, border_hori.y);
			//TraceLog(LOG_INFO, "v: %f %f", border_veri.x, border_veri.y);
		}
	}
	Vector2 center = (Vector2){(border_hori.x + border_hori.y) * -0.5, (border_veri.x + border_veri.y) * -0.5};
	int id = 0;
	for(int i = 0; i < TRI_AMOUNT; i++)
	{
		Tri tri = asset->tris[i];
		if(!TriIsPoint(tri))
		{
			//TraceLog(LOG_INFO, "%f %f", center.x, center.y);
			tri = MoveTri(tri, center);
			printf("asset->tris[%i] = (Tri){%f, %f, %f, %f, %f, %f, %s};\n", id, tri.a.x, tri.a.y, tri.b.x, tri.b.y, tri.c.x, tri.c.y, rmcname(tri.color));
			id++;
		}
	}
}

void PrintAssetBU(Asset* asset)
{
	TraceLog(LOG_INFO, "PRINTING ASSET (BU)");
	Vector2 pivot = (Vector2){-96, -48};
	int id = 0;
	for(int i = 0; i < TRI_AMOUNT; i++)
	{
		Tri tri = asset->tris[i];
		if(!TriIsPoint(tri))
		{
			tri = MoveTri(tri, pivot);
			tri.a = Vector2Scale(tri.a, .125);
			tri.b = Vector2Scale(tri.b, .125);
			tri.c = Vector2Scale(tri.c, .125);
			printf("asset->tris[%i] = (Tri){%g*BU, %g*BU, %g*BU, %g*BU, %g*BU, %g*BU, %s};\n", id, tri.a.x, tri.a.y, tri.b.x, tri.b.y, tri.c.x, tri.c.y, rmcname(tri.color));
			id++;
		}
	}
}

bool LoadAsset(Asset* asset, const char* filename)
{
	TraceLog(LOG_INFO, "LOADING ASSET");
	int data_size;
	unsigned char* file_data = LoadFileData(filename, &data_size);

	if(file_data != (void*)0)
	{
		bool loaded = true;
		if(data_size != SizeOfAsset(TRI_AMOUNT))
		{
			TraceLog(LOG_WARNING, "FILEIO: [%s] Asset size missmatch. (a: %i, r: %i)", filename, data_size, SizeOfAsset(TRI_AMOUNT));
			loaded = false;
		}
		else
		{
			Asset* asset_save = (Asset*)file_data;
			asset->tri_amount = asset_save->tri_amount;
			for(int i = 0; i < TRI_AMOUNT; i++)
			{
				asset->tris[i] = asset_save->tris[i];
			}

			TraceLog(LOG_INFO, "FILEIO: [%s] Asset loaded successfully", filename);
		}

		UnloadFileData(file_data);
		//PrintAsset(asset);
		return loaded;
	}
	return false;
}

bool SaveAsset(Asset* asset, const char* filename)
{
	TraceLog(LOG_INFO, "SAVING ASSET");
	bool success = false;
	int data_size;
	int save_data_size;
	unsigned char* file_data = LoadFileData(filename, &data_size);
	unsigned char* save_file_data = (void*)0;

	if(file_data != (void*)0)
	{
		if(data_size != SizeOfAsset(TRI_AMOUNT))
		{
			save_data_size = SizeOfAsset(TRI_AMOUNT);
			TraceLog(LOG_INFO, "REALLOC: save asset");
			save_file_data = (unsigned char*)_realloc(file_data, save_data_size);

			if(save_file_data != (void*)0)
			{
				Asset* savefile = (Asset*)save_file_data;
				savefile->tri_amount = asset->tri_amount;
				for(int i = 0; i < TRI_AMOUNT; i++)
				{
					savefile->tris[i] = asset->tris[i];
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

			Asset* savefile = (Asset*)save_file_data;
			savefile->tri_amount = asset->tri_amount;
			for(int i = 0; i < TRI_AMOUNT; i++)
			{
				savefile->tris[i] = asset->tris[i];
			}
		}

		success = SaveFileData(filename, save_file_data, save_data_size);
		TraceLog(LOG_INFO, "FREE: asset save data");
		_free(save_file_data);
	}
	else
	{
		data_size = SizeOfAsset(TRI_AMOUNT);
		TraceLog(LOG_INFO, "MALLOC: asset save data");
		file_data = (unsigned char*)_malloc(data_size);
		Asset* savefile = (Asset*)file_data;

		savefile->tri_amount = asset->tri_amount;
		for(int i = 0; i < TRI_AMOUNT; i++)
		{
			savefile->tris[i] = asset->tris[i];
		}

		success = SaveFileData(filename, file_data, data_size);
		UnloadFileData(file_data);
	}

	//PrintAsset(asset);
	return success;
}

int main(void)
{
	/*
	Image image = LoadImage("palette.png");
	while(!IsImageReady(image))
	{
	}
	int count = 0;
	Color* palette = LoadImagePalette(image, 128, &count);
	for(int i = 0; i < count; i++)
	{
		printf("case(%s): return (Color){%i, %i, %i, 255};\n", rmcname(i + 25), palette[i].r, palette[i].g, palette[i].b);
	}
	UnloadImagePalette(palette);
	UnloadImage(image);
	*/

	Asset* asset = MallocAsset(TRI_AMOUNT);

	int current_tri = 0, current_point = -1;
	char bg = 0, tricol = 0, input_timer = 0, draw_hitbox = 0;
	bool draw_pixels = true, edit_points = false, color_select = false;
	Camera2D camera = (Camera2D){0};
	camera.zoom = 4.0;

	InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "triray - tri editor");

	SetTargetFPS(15);

	while(!WindowShouldClose())
	{
		Vector2 mouse = GetMousePosition();
		Vector2 world_pos = Vector2Scale(mouse, .25);
		world_pos.x = (int)world_pos.x;
		world_pos.y = (int)world_pos.y;

		if(IsKeyPressed(KEY_D))
		{
			if(IsKeyDown(KEY_LEFT_CONTROL))
			{
				draw_hitbox++;
				if(draw_hitbox >= 3) draw_hitbox = 0;
				TraceLog(LOG_INFO, "Drawing hitbox: %i", draw_hitbox);
			}
			else
			{
				draw_pixels = !draw_pixels;
				TraceLog(LOG_INFO, "Toggle drawing pixels");
			}
		}
		if(IsKeyPressed(KEY_E))
		{
			if(edit_points)
			{
				current_point++;
				if(current_point >= 3)
				{
					edit_points = false;
				}
			}
			else
			{
				current_point = 0;
				edit_points = true;
			}
			if(edit_points)
			{
				TraceLog(LOG_INFO, "Editing point: %i", current_point);
			}
			else
			{
				TraceLog(LOG_INFO, "Editing tri");
			}
		}
		if(IsKeyPressed(KEY_F))
		{
			Tri tri = asset->tris[current_tri];
			Vector2 t = tri.a;
			tri.a = tri.b;
			tri.b = t;
			asset->tris[current_tri] = tri;
			TraceLog(LOG_INFO, "Flip tri points");
		}
		if(IsKeyPressed(KEY_T))
		{
			if(current_tri != 0)
			{
				if(IsKeyDown(KEY_LEFT_CONTROL))
				{
					Tri base = asset->tris[current_tri];
					Tri tri;
					int start = current_tri;
					do
					{
						current_tri++;
						if(current_tri == TRI_AMOUNT)
						{
							current_tri = 0;
						}
						tri = asset->tris[current_tri];
					}
					while(!TriIsPoint(tri) && !(start == current_tri));
					if(start != current_tri)
					{
						asset->tris[current_tri] = base;
						edit_points = false;
						current_point = -1;
						TraceLog(LOG_INFO, "Duplicated tri");
					}
					else
					{
						TraceLog(LOG_WARNING, "Cannot duplicate tri");
					}
				}
				else if(IsKeyDown(KEY_LEFT_SHIFT))
				{
					Tri zero = asset->tris[0];
					asset->tris[0] = asset->tris[current_tri];
					for(int i = current_tri; i > 0; i--)
					{
						asset->tris[i] = asset->tris[i - 1];
					}
					asset->tris[1] = zero;
					current_tri = 0;
					TraceLog(LOG_INFO, "Move tri to background");
				}
				else
				{
					Tri temp = asset->tris[current_tri];
					asset->tris[current_tri] = asset->tris[current_tri - 1];
					asset->tris[current_tri - 1] = temp;
					current_tri--;
					TraceLog(LOG_INFO, "Move tri back");
				}
			}
		}
		if(IsKeyPressed(KEY_B))
		{
			bg++;
			if(bg >= 3)
			{
				bg = 0;
			}
		}
		if(IsKeyPressed(KEY_P))
		{
			if(IsKeyDown(KEY_LEFT_CONTROL))
			{
				PrintAssetBU(asset);
			}
			else if(IsKeyDown(KEY_LEFT_SHIFT))
			{
				PrintAssetCentered(asset);
			}
			else
			{
				PrintAsset(asset);
			}
		}
		if(IsKeyPressed(KEY_C))
		{
			if(IsKeyDown(KEY_LEFT_SHIFT))
			{
				asset->tris[current_tri].color = tricol;
			}
			else if(IsKeyDown(KEY_LEFT_CONTROL))
			{
				tricol = asset->tris[current_tri].color;
			}
			else
			{
				color_select = !color_select;
			}
		}
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L))
		{
			LoadAsset(asset, "tri.ass");
			for(int i = TRI_AMOUNT; i >= 0; i--)
			{
				Tri tri = asset->tris[i];
				if(!TriIsPoint(tri))
				{
					current_tri = i;
					break;
				}
			}
		}
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S))
		{
			SaveAsset(asset, "tri.ass");
		}
		if(IsKeyPressed(KEY_DELETE))
		{
			asset->tris[current_tri] = (Tri){0};
			current_point = -1;
		}

		Vector2 shift = (Vector2){0, 0};
		if(IsKeyDown(KEY_UP))
		{
			shift.y -= 1;
		}
		if(IsKeyDown(KEY_DOWN))
		{
			shift.y += 1;
		}
		if(IsKeyDown(KEY_LEFT))
		{
			shift.x -= 1;
		}
		if(IsKeyDown(KEY_RIGHT))
		{
			shift.x += 1;
		}
		if(IsKeyPressed(KEY_UP))
		{
			input_timer = 60;
		}
		if(IsKeyPressed(KEY_DOWN))
		{
			input_timer = 60;
		}
		if(IsKeyPressed(KEY_LEFT))
		{
			input_timer = 60;
		}
		if(IsKeyPressed(KEY_RIGHT))
		{
			input_timer = 60;
		}
		if(shift.x != 0 || shift.y != 0)
		{
			input_timer++;
		}
		else
		{
			input_timer = 0;
		}
		if(input_timer >= 15)
		{
			if(IsKeyDown(KEY_LEFT_SHIFT))
			{
				shift = Vector2Scale(shift, 8);
			}
			if(IsKeyDown(KEY_LEFT_CONTROL))
			{
				for(int i = 0; i < TRI_AMOUNT; i++)
				{
					Tri tri = asset->tris[i];
					if(!TriIsPoint(tri))
					{
						asset->tris[i] = MoveTri(tri, shift);
					}
				}
			}
			else if(edit_points)
			{
				asset->tris[current_tri] = MovePoint(asset->tris[current_tri], shift, current_point);
			}
			else
			{
				asset->tris[current_tri] = MoveTri(asset->tris[current_tri], shift);
			}
			input_timer = 13;
		}

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if(color_select)
			{
				int i = (int)mouse.x / (COLOR_SELECT_SIZE * 4) + ((int)mouse.y / (COLOR_SELECT_SIZE * 4)) * 59;
				if(i >= 0 && i < 118)
				{
					tricol = i;
					asset->tris[current_tri].color = tricol;
					color_select = false;
				}
			}
			else
			{
				current_point++;
				if(current_point >= 3)
				{
					current_point = 0;
					do
					{
						current_tri++;
					}
					while(!TriIsPoint(asset->tris[current_tri]) && current_tri < TRI_AMOUNT);
				}
				if(current_tri < TRI_AMOUNT && current_tri >= 0)
				{
					asset->tris[current_tri] = SetPoint(asset->tris[current_tri], world_pos, current_point);
					asset->tris[current_tri].color = tricol;
				}
				else
				{
					current_tri = 0;
					current_point = -1;
				}
			}
		}
		if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			int last_tri = 0;
			bool found = false;
			for(int i = TRI_AMOUNT - 1; i >= 0; i--)
			{
				Tri tri = asset->tris[i];
				if(TriIsPoint(tri))
				{
					continue;
				}
				if(last_tri == 0)
				{
					last_tri = i;
				}
				if(!edit_points && i == current_tri)
				{
					continue;
				}
				if(CheckCollisionPointTriangle(world_pos, tri.a, tri.b, tri.c))
				{
					current_tri = i;
					current_point = -1;
					found = true;
					break;
				}
			}
			if(!found)
			{
				current_tri = last_tri;
				current_point = 3;
			}
			edit_points = false;
		}

		BeginDrawing();
		BeginMode2D(camera);
			switch(bg)
			{
				case 0:
					ClearBackground(RAYWHITE);
					break;
				case 1:
					ClearBackground(BLACK);
					break;
				case 2:
					ClearBackground(GRAY);
					break;
			}
			if(draw_hitbox == 1)
			{
				Vector2 car_size = (Vector2){56, 92};
				Vector2 pos = (Vector2){SCREEN_CENTER.x * .25 - car_size.x * .5, SCREEN_CENTER.y * .25 - car_size.y * .5};
				DrawRectangleLines(pos.x, pos.y, car_size.x, car_size.y, DARKGRAY);
			}
			if(draw_hitbox == 2)
			{
				Vector2 block_size = (Vector2){64, 64};
				Vector2 pos = (Vector2){SCREEN_CENTER.x * .25 - block_size.x * .5, SCREEN_CENTER.y * .25 - block_size.y * .5};
				DrawRectangleLines(pos.x, pos.y, block_size.x, block_size.y, DARKGRAY);
			}
			Vector2 pos = (Vector2){0.0, 0.0};
			DrawAsset(asset, 1.0, pos);
			if(draw_pixels)
			{
				Color color = BLACK;
				if(bg == 1)
				{
					color = GRAY;
				}
				DrawAssetPixels(asset, 1.0, pos, color);
			}
			if(current_tri < TRI_AMOUNT && current_tri >= 0)
			{
				Color color = RAYWHITE;
				if(bg == 0)
				{
					color = GRAY;
				}
				Tri tri = asset->tris[current_tri];
				if(edit_points)
				{
					if(current_point == 0)
					{
						DrawPixel(tri.a.x, tri.a.y, color);
					}
					else if(current_point == 1)
					{
						DrawPixel(tri.b.x, tri.b.y, color);
					}
					else
					{
						DrawPixel(tri.c.x, tri.c.y, color);
					}
				}
				else
				{
					DrawPixel(tri.a.x, tri.a.y, color);
					DrawPixel(tri.b.x, tri.b.y, color);
					DrawPixel(tri.c.x, tri.c.y, color);
				}
			}
			if(color_select)
			{
				for(int i = 0; i < 118; i++)
				{
					Vector2 pos = (Vector2){i * COLOR_SELECT_SIZE, 0};
					if(i > 58)
					{
						pos.x -= 59 * COLOR_SELECT_SIZE;
						pos.y = COLOR_SELECT_SIZE;
					}
					DrawRectangle(pos.x, pos.y, COLOR_SELECT_SIZE, COLOR_SELECT_SIZE, rmc(i));
				}
			}
		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	_free(asset);

	return 0;
}
