#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"
#include "asset.h"

void RaylibLogo(void)
{
    // Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
    // Initialization
    //--------------------------------------------------------------------------------------
    int logoPositionX = (int)(SCREEN_SIZE.x*0.5) - 128;
    int logoPositionY = (int)(SCREEN_SIZE.y*0.5) - 128;

    int framesCounter = 0;
    int lettersCount = 0;

    int topSideRecWidth = 16;
    int leftSideRecHeight = 16;

    int bottomSideRecWidth = 16;
    int rightSideRecHeight = 16;

    int state = 0;                  // Tracking animation states (State Machine)
    float alpha = 1.0f;             // Useful for fading

    // Main game loop
    while (state != 4)    // Detect window close button or ESC key
    {
	if(GetKeyPressed() && state != 3)
	{
	    state = 3;
            topSideRecWidth = 256;
            leftSideRecHeight = 256;
            bottomSideRecWidth = 256;
            rightSideRecHeight = 256;
	    lettersCount = 6;
	}
        // Update
        if (state == 0)                 // State 0: Small box blinking
        {
            framesCounter++;

            if (framesCounter == 120)
            {
                state = 1;
                framesCounter = 0;      // Reset counter... will be used later...
            }
        }
        else if (state == 1)            // State 1: Top and left bars growing
        {
            topSideRecWidth += 4;
            leftSideRecHeight += 4;

            if (topSideRecWidth == 256) state = 2;
        }
        else if (state == 2)            // State 2: Bottom and right bars growing
        {
            bottomSideRecWidth += 4;
            rightSideRecHeight += 4;

            if (bottomSideRecWidth == 256) state = 3;
        }
        else if (state == 3)            // State 3: Letters appearing (one by one)
        {
            framesCounter++;

            if (framesCounter/12)       // Every 12 frames, one more letter!
            {
                lettersCount++;
                framesCounter = 0;
            }

            if (lettersCount >= 10)     // When all letters have appeared, just fade out everything
            {
                alpha -= 0.02f;

                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                    state = 4;
                }
            }
        }

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (state == 0)
            {
                if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
            }
            else if (state == 1)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
            }
            else if (state == 2)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);

                DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
            }
            else if (state == 3)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));

                DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

                DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, alpha));

                DrawText(TextSubtext("raylib", 0, lettersCount), GetScreenWidth()/2 - 44, GetScreenHeight()/2 + 48, 50, Fade(BLACK, alpha));
            }

        EndDrawing();
    }
}

void TabinLogo(void)
{
	// TODO: Unfinished
	
	typedef enum State {START, JUMP_IN, WAIT, JUMP_OUT, END} State;
	const int FRAMES_PER_LETTER = 6;
	const float FRAME_TO_SIZE = 32 / FRAMES_PER_LETTER;
	const char* text = "HOWDOESONENAME";

	State state = START;
	state++;
	bool state_change = true;
	float frames = 0.0;
	unsigned int letters = 0;

	while(state != END)
	{
		if(GetKeyPressed() && state < WAIT)
		{
			state_change = true;
			state = WAIT;
		}
		else if(state == JUMP_IN)
		{
			frames++;
			if(frames >= FRAMES_PER_LETTER)
			{
				letters++;
				frames = 0;
			}
			if(letters >= 14)
			{
				state_change = true;
				state = WAIT;
			}
		}
		else if(state == WAIT)
		{
			frames++;
			if(frames > 48)
			{
				state_change = true;
				state = JUMP_OUT;
			}
		}
		else if(state == JUMP_OUT)
		{
			frames++;
			if(frames > 90)
			{
				state_change = true;
				state = END;
			}
		}
		if(state_change)
		{
			state_change = false;
			frames = 0;
			if(state == JUMP_IN)
			{
				letters = 0;
			}
			if(state == WAIT)
			{
			}
		}
		BeginDrawing();

		ClearBackground(RAYWHITE);

		const Vector2 text_pos = (Vector2){SCREEN_CENTER.x - 166, SCREEN_CENTER.y - 16};
		
		if(state == JUMP_IN)
		{
			for(int i = 0; i < letters; i++)
			{
				DrawText(TextFormat("%c", text[i]), text_pos.x + i * 28, text_pos.y, 32, BLACK);
			}
			int half_letters = FRAMES_PER_LETTER / 2;
			DrawText(TextFormat("%c", text[letters]), text_pos.x + letters * 28, text_pos.y + (absi(half_letters - frames) - half_letters) * 4, 32, BLACK);
		}
		if(state == WAIT)
		{
			for(int i = 0; i < 14; i++)
			{
				DrawText(TextFormat("%c", text[i]), text_pos.x + i * 28, text_pos.y, 32, BLACK);
			}
		}
		if(state == JUMP_OUT)
		{
			for(int i = 0; i < 14; i++)
			{
				int f = max(frames - i * 2, 0);
				int fy = absi(12 - f);
				int mid = i - 7;
				if(mid >= 0) mid++;
				DrawText(TextFormat("%c", text[i]), text_pos.x + i * 28 + mid * f * 2, text_pos.y - 72 + fy * fy * 0.5, 32, BLACK);
			}
		}
		EndDrawing();
	}
}

Tri MoveTri(Tri tri, Vector2 position)
{
	tri.a = Vector2Add(tri.a, position);
	tri.b = Vector2Add(tri.b, position);
	tri.c = Vector2Add(tri.c, position);
	return tri;
}

Color TriColor(Tri tri)
{
	return ColorFromIndex(tri.color);
}

void PrintTri(Tri tri)
{
	TraceLog(LOG_INFO, "%.0f, %.0f | %.0f, %.0f | %.0f, %.0f | %i",
			tri.a.x, tri.a.y,
			tri.b.x, tri.b.y,
			tri.c.x, tri.c.y,
			tri.color
		);
}

Asset* RotateAsset(Asset* asset, BlockRotation rot, Vector2 size)
{
	if(rot == ROT_NORTH)
	{
		return asset;
	}
	if(rot == ROT_EAST)
	{
		// switch, s-x
		for(int i = 0; i < asset->tri_amount; i++)
		{
			asset->tris[i].a = Vector2Swap(asset->tris[i].a);
			asset->tris[i].b = Vector2Swap(asset->tris[i].b);
			asset->tris[i].c = Vector2Swap(asset->tris[i].c);
			asset->tris[i].a.x = size.y - asset->tris[i].a.x;
			asset->tris[i].b.x = size.y - asset->tris[i].b.x;
			asset->tris[i].c.x = size.y - asset->tris[i].c.x;
		}
	}
	if(rot == ROT_SOUTH)
	{
		// s-xy
		for(int i = 0; i < asset->tri_amount; i++)
		{
			asset->tris[i].a = Vector2Subtract(size, asset->tris[i].a);
			asset->tris[i].b = Vector2Subtract(size, asset->tris[i].b);
			asset->tris[i].c = Vector2Subtract(size, asset->tris[i].c);
		}
	}
	if(rot == ROT_WEST)
	{
		// switch, s-y
		for(int i = 0; i < asset->tri_amount; i++)
		{
			asset->tris[i].a = Vector2Swap(asset->tris[i].a);
			asset->tris[i].b = Vector2Swap(asset->tris[i].b);
			asset->tris[i].c = Vector2Swap(asset->tris[i].c);
			asset->tris[i].a.y = size.x - asset->tris[i].a.y;
			asset->tris[i].b.y = size.x - asset->tris[i].b.y;
			asset->tris[i].c.y = size.x - asset->tris[i].c.y;
		}
	}
	return asset;
}

Asset* MallocAsset(int tri_count)
{
	Asset* asset = (Asset*)_malloc(sizeof(Asset) + sizeof(Tri) * tri_count);
	asset->tri_amount = tri_count;
	return asset;
}

Asset* AllocAsset(int asset_id, BlockRotation rot, double game_time)
{
	//TraceLog(LOG_INFO, "MALLOC: Asset");
	/*
	 * IMPORTANT!!!!!
	 * Triangle points always need to go in counter clockwise order!
	 * 
	 *    A**
	 *    *  **
	 *   *   ***C
	 *   B***
	 *
	 */
	//asset->tris[i] = (Tri){ax, ay, bx, by, cx, cy, color};
	Asset* asset;
	Vector2 size = (Vector2){BLOCK_SIZE, BLOCK_SIZE};
	switch(asset_id)
	{
		case(1):
			int anim_time = (int)(Wrap((float)game_time, 0.0, 4.0) * 16.0);
			asset = MallocAsset(1);
			asset->tris[0] = (Tri){0, 0, 0, BU, BU, anim_time, 24};
			//PrintTri(asset->tris[0]);
			break;
		case(2):
			size = (Vector2){BLOCK_SIZE * 2, BLOCK_SIZE};
			asset = MallocAsset(4);
			asset->tris[0] = (Tri){0, 0, 0, BU, BU, 0, 3};
			asset->tris[1] = (Tri){16*BU, 0, 15*BU, 0, 16*BU, BU, 6};
			asset->tris[2] = (Tri){16*BU, 8*BU, 16*BU, 7*BU, 15*BU, 8*BU, 9};
			asset->tris[3] = (Tri){0, 8*BU, BU, 8*BU, 0, 7*BU, 12};
			break;
		case(3):
			size = (Vector2){BLOCK_SIZE * 2, BLOCK_SIZE * 2};
			asset = MallocAsset(4);
			asset->tris[0] = (Tri){0, 0, 0, BU, BU, 0, 3};
			asset->tris[1] = (Tri){8*BU, 0, 7*BU, 0, 8*BU, BU, 6};
			asset->tris[2] = (Tri){8*BU, 8*BU, 8*BU, 7*BU, 7*BU, 8*BU, 9};
			asset->tris[3] = (Tri){0, 8*BU, BU, 8*BU, 0, 7*BU, 12};
			break;
		default:
			asset = MallocAsset(4);
			asset->tris[0] = (Tri){0, 0, 0, BU, BU, 0, 3};
			asset->tris[1] = (Tri){8*BU, 0, 7*BU, 0, 8*BU, BU, 6};
			asset->tris[2] = (Tri){8*BU, 8*BU, 8*BU, 7*BU, 7*BU, 8*BU, 9};
			asset->tris[3] = (Tri){0, 8*BU, BU, 8*BU, 0, 7*BU, 12};
			break;
	}
	return RotateAsset(asset, rot, size);
}

void FreeAsset(Asset* asset)
{
	if(asset != PNULL)
	{
		//TraceLog(LOG_INFO, "FREE: Free asset.");
		_free(asset);
	}
}

void DrawAsset(Asset* asset, float scale, Vector2 position)
{
	//TraceLog(LOG_INFO, "Drawing");
	for(int i = 0; i < asset->tri_amount; i++)
	{
		Tri tri = MoveTri(asset->tris[i], position);
		//PrintTri(tri);
		Color color = TriColor(tri);
		DrawTriangle(tri.a, tri.b, tri.c, color);
	}
}

