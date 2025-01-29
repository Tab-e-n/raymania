#include <raylib.h>
#include "rmlib.h"
#include "asset.h"


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
		tri.a = Vector2Add(tri.a, pos);
	}
	else if(point == 1)
	{
		tri.b = Vector2Add(tri.b, pos);
	}
	else
	{
		tri.c = Vector2Add(tri.c, pos);
	}
	return tri;
}

int main(void)
{
	const int TRI_AMOUNT = 256;

	Asset* asset = MallocAsset(TRI_AMOUNT);

	int current_tri = 0, current_point = -1;
	char bg = 0;
	bool draw_pixels = true, edit_points = false;
	Camera2D camera = (Camera2D){0};
	camera.zoom = 4.0;

	InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "triray - tri editor");

	SetTargetFPS(15);

	while(!WindowShouldClose())
	{
		Vector2 mouse = GetMousePosition();
		mouse = Vector2Scale(mouse, .25);

		if(IsKeyPressed(KEY_D))
		{
			draw_pixels = !draw_pixels;
		}
		if(IsKeyPressed(KEY_E))
		{
			edit_points = !edit_points;
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
		if(IsKeyPressed(KEY_DELETE))
		{
			asset->tris[current_tri] = (Tri){0};
			current_point = -1;
		}
		if(edit_points)
		{
			if(IsKeyPressed(KEY_UP))
			{
				Vector2 shift = (Vector2){0, -1};
				asset->tris[current_tri] = MovePoint(asset->tris[current_tri], shift, current_point);
			}
			if(IsKeyPressed(KEY_DOWN))
			{
				Vector2 shift = (Vector2){0, 1};
				asset->tris[current_tri] = MovePoint(asset->tris[current_tri], shift, current_point);
			}
			if(IsKeyPressed(KEY_LEFT))
			{
				Vector2 shift = (Vector2){-1, 0};
				asset->tris[current_tri] = MovePoint(asset->tris[current_tri], shift, current_point);
			}
			if(IsKeyPressed(KEY_RIGHT))
			{
				Vector2 shift = (Vector2){1, 0};
				asset->tris[current_tri] = MovePoint(asset->tris[current_tri], shift, current_point);
			}
		}
		else
		{
			if(IsKeyPressed(KEY_UP))
			{
				Vector2 shift = (Vector2){0, -1};
				asset->tris[current_tri] = MoveTri(asset->tris[current_tri], shift);
			}
			if(IsKeyPressed(KEY_DOWN))
			{
				Vector2 shift = (Vector2){0, 1};
				asset->tris[current_tri] = MoveTri(asset->tris[current_tri], shift);
			}
			if(IsKeyPressed(KEY_LEFT))
			{
				Vector2 shift = (Vector2){-1, 0};
				asset->tris[current_tri] = MoveTri(asset->tris[current_tri], shift);
			}
			if(IsKeyPressed(KEY_RIGHT))
			{
				Vector2 shift = (Vector2){1, 0};
				asset->tris[current_tri] = MoveTri(asset->tris[current_tri], shift);
			}
		}

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
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
				asset->tris[current_tri] = SetPoint(asset->tris[current_tri], mouse, current_point);
			}
			else
			{
				current_tri = 0;
				current_point = -1;
			}
		}
		if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			for(int i = 0; i < TRI_AMOUNT; i++)
			{
				Tri tri = asset->tris[i];
				if(CheckCollisionPointTriangle(mouse, tri.a, tri.b, tri.c))
				{
					current_tri = i;
					current_point = -1;
					break;
				}
			}
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
					if(point == 0)
					{
						DrawPixel(tri.a.x, tri.a.y, color);
					}
					else if(point == 1)
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
		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
