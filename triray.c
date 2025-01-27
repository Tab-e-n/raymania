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

int main(void)
{
	const int TRI_AMOUNT = 256;

	Asset* asset = MallocAsset(TRI_AMOUNT);

	int current_tri = 0, current_point = -1;
	bool draw_pixels = true;
	Camera2D camera = (Camera2D){0};
	camera.zoom = 4.0;

	InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "triray - tri editor");

	SetTargetFPS(15);

	while(!WindowShouldClose())
	{
		Vector2 mouse = GetMousePosition();
		mouse = Vector2Scale(mouse, .25);

		if(IsKeyPressed(KEY_P))
		{
			draw_pixels = !draw_pixels;
		}
		if(IsKeyPressed(KEY_F))
		{
			Tri tri = asset->tris[current_tri];
			Vector2 t = tri.a;
			tri.a = tri.b;
			tri.b = t;
			asset->tris[current_tri] = tri;
		}
		if(IsKeyPressed(KEY_DELETE))
		{
			asset->tris[current_tri] = (Tri){0};
			current_point = -1;
		}
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

		BeginDrawing();
		BeginMode2D(camera);
			ClearBackground(RAYWHITE);
			Vector2 pos = (Vector2){0.0, 0.0};
			DrawAsset(asset, 1.0, pos);
			if(draw_pixels)
			{
				DrawAssetPixels(asset, 1.0, pos);
			}
		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
