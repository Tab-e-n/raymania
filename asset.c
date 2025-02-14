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

Tri ScaleTri(Tri tri, float scale)
{
	tri.a = Vector2Scale(tri.a, scale);
	tri.b = Vector2Scale(tri.b, scale);
	tri.c = Vector2Scale(tri.c, scale);
	return tri;
}

bool TriIsPoint(Tri tri)
{
	return Vector2Equals(tri.a, tri.b) && Vector2Equals(tri.a, tri.c);
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

int SizeOfAsset(int tri_count)
{
	return sizeof(Asset) + sizeof(Tri) * tri_count;
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
	Asset* asset = (Asset*)_malloc(SizeOfAsset(tri_count));
	asset->tri_amount = tri_count;
	return asset;
}

Asset* AllocAsset(int asset_id, BlockRotation rot, double game_time)
{
	//TraceLog(LOG_INFO, "MALLOC: Asset");
	/*
	 * BLOCK ASSET DEFINITIONS
	 * IMPORTANT!!!!!
	 * Triangle points always need to go in counter clockwise order!
	 * 
	 *    A**
	 *    *  **
	 *   *   ***C
	 *   B***
	 *
	 */
	//asset->tris[i] = (Tri){ax*BU, ay*BU, bx*BU, by*BU, cx*BU, cy*BU, color};
	Asset* asset;
	Vector2 size = (Vector2){BLOCK_SIZE, BLOCK_SIZE};
	switch(asset_id)
	{
		case(D1x1GWN):
			asset = MallocAsset(4);
			asset->tris[0] = (Tri){0, 0, 0, BU, 8*BU, 0, 9};
			asset->tris[1] = (Tri){8*BU, 0, 0, BU, 8*BU, BU, 10};
			asset->tris[2] = (Tri){8*BU, 8*BU, 8*BU, 7*BU, 7*BU, 8*BU, 11};
			asset->tris[3] = (Tri){0, 8*BU, BU, 8*BU, 0, 7*BU, 10};
			break;
		case(D2x1AIO):
			size = (Vector2){BLOCK_SIZE * 2, BLOCK_SIZE};
			asset = MallocAsset(8);
			asset->tris[0] = (Tri){0, 0, 0, BU, BU, 0, 0};
			asset->tris[1] = (Tri){16*BU, 0, 15*BU, 0, 16*BU, BU, 1};
			asset->tris[2] = (Tri){16*BU, 8*BU, 16*BU, 7*BU, 15*BU, 8*BU, 2};
			asset->tris[3] = (Tri){0, 8*BU, BU, 8*BU, 0, 7*BU, 1};
			asset->tris[4] = (Tri){4*BU, 2*BU, 4*BU, 6*BU, 5*BU, 6*BU, 0};
			asset->tris[5] = (Tri){4*BU, 6*BU, 4*BU, 2*BU, 3*BU, 2*BU, 2};
			asset->tris[6] = (Tri){14*BU, 6*BU, 10*BU, 2*BU, 10*BU, 6*BU, 0};
			asset->tris[7] = (Tri){14*BU, 2*BU, 10*BU, 2*BU, 14*BU, 6*BU, 2};
			break;
		case(D2x2DIO):
			size = (Vector2){BLOCK_SIZE * 2, BLOCK_SIZE * 2};
			asset = MallocAsset(8);
			asset->tris[0] = (Tri){0, 0, 0, BU, BU, 0, 18};
			asset->tris[1] = (Tri){16*BU, 0, 15*BU, 0, 16*BU, BU, 19};
			asset->tris[2] = (Tri){16*BU, 16*BU, 16*BU, 15*BU, 15*BU, 16*BU, 20};
			asset->tris[3] = (Tri){0, 16*BU, BU, 16*BU, 0, 15*BU, 19};
			asset->tris[4] = (Tri){4*BU, 2*BU, 4*BU, 6*BU, 5*BU, 6*BU, 18};
			asset->tris[5] = (Tri){4*BU, 6*BU, 4*BU, 2*BU, 3*BU, 2*BU, 20};
			asset->tris[6] = (Tri){14*BU, 6*BU, 10*BU, 2*BU, 10*BU, 6*BU, 18};
			asset->tris[7] = (Tri){14*BU, 2*BU, 10*BU, 2*BU, 14*BU, 6*BU, 20};
			break;
		case(D1x1GWS):
			asset = MallocAsset(4);
			asset->tris[0] = (Tri){0, 0, 8*BU, 8*BU, BU, 0, 9};
			asset->tris[1] = (Tri){8*BU, 0, 7*BU, 0, 8*BU, BU, 10};
			asset->tris[2] = (Tri){8*BU, 8*BU, 0, 0, 7*BU, 8*BU, 11};
			asset->tris[3] = (Tri){0, 8*BU, BU, 8*BU, 0, 7*BU, 10};
			break;
		case(D1x1I):
			asset = MallocAsset(4);
			asset->tris[0] = (Tri){0, 0, 0, BU, BU, 0, 12};
			asset->tris[1] = (Tri){8*BU, 0, 7*BU, 0, 8*BU, BU, 13};
			asset->tris[2] = (Tri){8*BU, 8*BU, 8*BU, 7*BU, 7*BU, 8*BU, 14};
			asset->tris[3] = (Tri){0, 8*BU, BU, 8*BU, 0, 7*BU, 13};
			break;
		case(D1x1C):
			asset = MallocAsset(2);
			asset->tris[0] = (Tri){0, 3.5*BU, 0, 4.5*BU, 8*BU, 3.5*BU, 13};
			asset->tris[1] = (Tri){8*BU, 3.5*BU, 0, 4.5*BU, 8*BU, 4.5*BU, 13};
			break;
		case(D1x1F):
			asset = MallocAsset(2);
			asset->tris[0] = (Tri){0, 3.5*BU, 0, 4.5*BU, 8*BU, 3.5*BU, 7};
			asset->tris[1] = (Tri){8*BU, 3.5*BU, 0, 4.5*BU, 8*BU, 4.5*BU, 7};
			break;
		case(D1x1S):
			int anim_time = (int)(Wrap((float)game_time, 0.0, 4.0) * 16.0);
			asset = MallocAsset(3);
			asset->tris[0] = (Tri){0, 3.5*BU, 0, 4.5*BU, 8*BU, 3.5*BU, 9};
			asset->tris[1] = (Tri){8*BU, 3.5*BU, 0, 4.5*BU, 8*BU, 4.5*BU, 9};
			asset = RotateAsset(asset, rot, size);
			rot = ROT_NORTH;
			asset->tris[2] = (Tri){0, 0, 0, BU, BU, anim_time, 24};
			break;
		case(D1x1A):
			asset = MallocAsset(16);
			for(int i = 0; i < 16; i++)
			{
				asset->tris[i] = (Tri){0, 0.5*i*BU, 0, 0.5*BU + 0.5*i*BU, BU, 0.5*i*BU};
			}
			break;
		case(B1x1AR0):
			asset = MallocAsset(10);
			asset->tris[0] = (Tri){0.5*BU, 0*BU, 1*BU, 0*BU, 0.75*BU, -0.25*BU, 2};
			asset->tris[1] = (Tri){0.5*BU, 0*BU, 1*BU, 8*BU, 1*BU, 0*BU, 2};
			asset->tris[2] = (Tri){0.5*BU, 0*BU, 0.5*BU, 8*BU, 1*BU, 8*BU, 2};
			asset->tris[3] = (Tri){0.5*BU, 8*BU, 0.75*BU, 8.25*BU, 1*BU, 8*BU, 2};
			asset->tris[4] = (Tri){7*BU, 0*BU, 7.5*BU, 0*BU, 7.25*BU, -0.25*BU, 2};
			asset->tris[5] = (Tri){7*BU, 0*BU, 7.5*BU, 8*BU, 7.5*BU, 0*BU, 2};
			asset->tris[6] = (Tri){7*BU, 0*BU, 7*BU, 8*BU, 7.5*BU, 8*BU, 2};
			asset->tris[7] = (Tri){7*BU, 8*BU, 7.25*BU, 8.25*BU, 7.5*BU, 8*BU, 2};
			asset->tris[8] = (Tri){1*BU, 0*BU, 1*BU, 8*BU, 7*BU, 8*BU, 1};
			asset->tris[9] = (Tri){1*BU, 0*BU, 7*BU, 8*BU, 7*BU, 0*BU, 1};
			break;
		case(DRACECAR):
			asset = MallocAsset(24);
			asset->tris[0] = (Tri){-20.500000, -21.000000, -27.500000, 23.000000, 20.500000, -21.000000, 105};
			asset->tris[1] = (Tri){20.500000, -21.000000, -27.500000, 23.000000, 27.500000, 23.000000, 107};
			asset->tris[2] = (Tri){-8.500000, -21.000000, 8.500000, -21.000000, 5.500000, -45.000000, 105};
			asset->tris[3] = (Tri){-5.500000, -45.000000, -8.500000, -21.000000, 5.500000, -45.000000, 107};
			asset->tris[4] = (Tri){-27.500000, -45.000000, 20.500000, -53.000000, -20.500000, -53.000000, 108};
			asset->tris[5] = (Tri){20.500000, -53.000000, -27.500000, -45.000000, 27.500000, -45.000000, 108};
			asset->tris[6] = (Tri){12.500000, 23.000000, -12.500000, 23.000000, -12.500000, 44.000000, 107};
			asset->tris[7] = (Tri){12.500000, 23.000000, -12.500000, 44.000000, 12.500000, 44.000000, 107};
			asset->tris[8] = (Tri){-18.500000, 44.000000, -18.500000, 56.000000, 18.500000, 44.000000, 108};
			asset->tris[9] = (Tri){-18.500000, 56.000000, 18.500000, 56.000000, 18.500000, 44.000000, 108};
			asset->tris[10] = (Tri){-4.500000, -53.000000, 4.500000, -53.000000, 0.500000, -56.000000, 108};
			asset->tris[11] = (Tri){-27.500000, -43.000000, -27.500000, -26.000000, -14.500000, -43.000000, 110};
			asset->tris[12] = (Tri){-27.500000, -26.000000, -14.500000, -26.000000, -14.500000, -43.000000, 110};
			asset->tris[13] = (Tri){14.500000, -43.000000, 14.500000, -26.000000, 27.500000, -43.000000, 110};
			asset->tris[14] = (Tri){27.500000, -43.000000, 14.500000, -26.000000, 27.500000, -26.000000, 110};
			asset->tris[15] = (Tri){14.500000, 42.000000, 27.500000, 42.000000, 27.500000, 25.000000, 110};
			asset->tris[16] = (Tri){-27.500000, 42.000000, -14.500000, 42.000000, -14.500000, 25.000000, 110};
			asset->tris[17] = (Tri){-27.500000, 42.000000, -14.500000, 25.000000, -27.500000, 25.000000, 110};
			asset->tris[18] = (Tri){14.500000, 42.000000, 27.500000, 25.000000, 14.500000, 25.000000, 110};
			asset->tris[19] = (Tri){-6.500000, 7.000000, 0.500000, 17.000000, 6.500000, 7.000000, 110};
			asset->tris[20] = (Tri){6.500000, -6.000000, -6.500000, 7.000000, 6.500000, 7.000000, 110};
			asset->tris[21] = (Tri){-6.500000, -6.000000, -6.500000, 7.000000, 6.500000, -6.000000, 110};
			asset->tris[22] = (Tri){0.500000, -16.000000, -6.500000, -6.000000, 6.500000, -6.000000, 110};
			asset->tris[23] = (Tri){0.500000, -5.000000, -5.500000, 6.000000, 5.500000, 6.000000, 106};
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
		Tri tri = ScaleTri(asset->tris[i], scale);
		tri = MoveTri(tri, position);
		//PrintTri(tri);
		Color color = TriColor(tri);
		DrawTriangle(tri.a, tri.b, tri.c, color);
	}
}

void DrawAssetPixels(Asset* asset, float scale, Vector2 position, Color color)
{
	//TraceLog(LOG_INFO, "Drawing");
	for(int i = 0; i < asset->tri_amount; i++)
	{
		Tri tri = ScaleTri(asset->tris[i], scale);
		tri = MoveTri(tri, position);
		//PrintTri(tri);
		DrawPixel(tri.a.x, tri.a.y, color);
		DrawPixel(tri.b.x, tri.b.y, color);
		DrawPixel(tri.c.x, tri.c.y, color);
	}
}
