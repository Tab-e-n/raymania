#include <raylib.h>
#include "rmlib.h"
#include "asset.h"


#define COLOR_SELECT_SIZE 8
#define TRI_AMOUNT 256


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
	for(int i = 0; i < TRI_AMOUNT; i++)
	{
		Tri tri = asset->tris[i];
		if(!TriIsPoint(tri))
		{
			TraceLog(LOG_INFO, "%i: %f %f %f %f %f %f", i, tri.a.x, tri.a.y, tri.b.x, tri.b.y, tri.c.x, tri.c.y);
		}
	}
}

bool LoadAsset(Asset* asset, const char* filename)
{
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
		PrintAsset(asset);
		return loaded;
	}
	return false;
}

bool SaveAsset(Asset* asset, const char* filename)
{
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

	PrintAsset(asset);
	return success;
}

int main(void)
{
	Asset* asset = MallocAsset(TRI_AMOUNT);

	int current_tri = 0, current_point = -1;
	char bg = 0, tricol = 0, input_timer = 0;
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
			draw_pixels = !draw_pixels;
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
		}
		if(IsKeyPressed(KEY_F))
		{
			Tri tri = asset->tris[current_tri];
			Vector2 t = tri.a;
			tri.a = tri.b;
			tri.b = t;
			asset->tris[current_tri] = tri;
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
			PrintAsset(asset);
		}
		if(IsKeyPressed(KEY_C))
		{
			color_select = !color_select;
		}
		if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L))
		{
			LoadAsset(asset, "tri.ass");
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
			if(edit_points)
			{
				asset->tris[current_tri] = MovePoint(asset->tris[current_tri], shift, current_point);
			}
			else
			{
				asset->tris[current_tri] = MoveTri(asset->tris[current_tri], shift);
			}
			input_timer = 12;
		}

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if(color_select)
			{
				int i = (int)mouse.x / (COLOR_SELECT_SIZE * 4);
				if(i >= 0 && i < 25)
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
			for(int i = 0; i < TRI_AMOUNT; i++)
			{
				Tri tri = asset->tris[i];
				if(TriIsPoint(tri))
				{
					continue;
				}
				last_tri = i;
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
				for(int i = 0; i < 25; i++)
				{
					DrawRectangle(i * COLOR_SELECT_SIZE, 0, COLOR_SELECT_SIZE, COLOR_SELECT_SIZE, ColorFromIndex(i));
				}
			}
		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	_free(asset);

	return 0;
}
