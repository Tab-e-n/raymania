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
    while (state != 4 && !WindowShouldClose())    // Detect window close button or ESC key
    {
	if(state != 3)
	{
		if(GetKeyPressed())
		{
		    state = 3;
		    topSideRecWidth = 256;
		    leftSideRecHeight = 256;
		    bottomSideRecWidth = 256;
		    rightSideRecHeight = 256;
		    lettersCount = 6;
		}
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

	while(state != END && !WindowShouldClose())
	{
		if(state < WAIT)
		{
			if(GetKeyPressed())
			{
				state_change = true;
				state = WAIT;
			}
		}
		if(state_change)
		{
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

Palette GetPalette(unsigned char id)
{
	Palette palette = (Palette){0};
	// PALETTE DEFINITIONS
	switch(id)
	{
		case 0: // DEBUG GREYSCALE
			palette.colors[CAR_WHEEL0] = RM_WHITE8;
			palette.colors[CAR_WHEEL1] = RM_WHITE8;
			palette.colors[CAR_WHEEL2] = RM_WHITE8;
			palette.colors[CAR_BASE0] = RM_WHITE4;
			palette.colors[CAR_BASE1] = RM_WHITE4;
			palette.colors[CAR_BASE2] = RM_WHITE4;
			palette.colors[CAR_ACCENT0] = RM_WHITE2;
			palette.colors[CAR_ACCENT1] = RM_WHITE2;
			palette.colors[CAR_ACCENT2] = RM_WHITE2;
			palette.colors[CAR_DRIVER] = RM_WHITE3;
			palette.colors[CAR_SEAT] = RM_WHITE7;
			palette.colors[CAR_HULL] = RM_WHITE8;
			palette.colors[CAR_WINDOW0] = RM_WHITE8;
			palette.colors[CAR_WINDOW1] = RM_WHITE8;
			palette.colors[CAR_WINDOW2] = RM_WHITE8;
			palette.colors[CAR_LIGHT_FRONT] = RM_WHITE0;
			palette.colors[CAR_LIGHT_BACK] = RM_WHITE1;
			palette.colors[CAR_LIGHT_OTHER] = RM_WHITE0;
			palette.colors[CAR_NUMPLATE_BASE] = RM_WHITE1;
			palette.colors[CAR_NUMPLATE_TEXT0] = RM_WHITE8;
			palette.colors[CAR_NUMPLATE_TEXT1] = RM_WHITE8;
			palette.colors[CAR_EXTRA0] = RM_WHITE5;
			palette.colors[CAR_EXTRA1] = RM_WHITE5;
			palette.colors[CAR_EXTRA2] = RM_WHITE6;
			palette.colors[CAR_TRAIL] = RM_WHITE6;
			break;
		case 1: // DEBUG YELLOW RED
			palette.colors[CAR_WHEEL0] = RL_BLACK;
			palette.colors[CAR_WHEEL1] = RL_GRAY2;
			palette.colors[CAR_WHEEL2] = RL_BLACK;
			palette.colors[CAR_BASE0] = RL_YELLOW0;
			palette.colors[CAR_BASE1] = RL_YELLOW1;
			palette.colors[CAR_BASE2] = RL_YELLOW2;
			palette.colors[CAR_ACCENT0] = RL_RED0;
			palette.colors[CAR_ACCENT1] = RL_RED1;
			palette.colors[CAR_ACCENT2] = RL_RED2;
			palette.colors[CAR_DRIVER] = RL_MAGENTA;
			palette.colors[CAR_SEAT] = RL_GRAY2;
			palette.colors[CAR_HULL] = RL_BLACK;
			palette.colors[CAR_WINDOW0] = RL_GRAY2;
			palette.colors[CAR_WINDOW1] = RL_GRAY1;
			palette.colors[CAR_WINDOW2] = RL_GRAY0;
			palette.colors[CAR_LIGHT_FRONT] = RL_WHITE;
			palette.colors[CAR_LIGHT_BACK] = RL_WHITE;
			palette.colors[CAR_LIGHT_OTHER] = RL_WHITE;
			palette.colors[CAR_NUMPLATE_BASE] = RL_RAYWHITE;
			palette.colors[CAR_NUMPLATE_TEXT0] = RL_BLACK;
			palette.colors[CAR_NUMPLATE_TEXT1] = RL_BLUE2;
			palette.colors[CAR_EXTRA0] = RL_GRAY0;
			palette.colors[CAR_EXTRA1] = RL_GRAY1;
			palette.colors[CAR_EXTRA2] = RL_GRAY2;
			palette.colors[CAR_TRAIL] = RL_MAGENTA;
			break;
		case 2: // DEBUG GREEN PURPLE
			palette.colors[CAR_WHEEL0] = RL_BLACK;
			palette.colors[CAR_WHEEL1] = RL_GRAY2;
			palette.colors[CAR_WHEEL2] = RL_BLACK;
			palette.colors[CAR_BASE0] = RL_GREEN0;
			palette.colors[CAR_BASE1] = RL_GREEN1;
			palette.colors[CAR_BASE2] = RL_GREEN2;
			palette.colors[CAR_ACCENT0] = RL_PURPLE0;
			palette.colors[CAR_ACCENT1] = RL_PURPLE1;
			palette.colors[CAR_ACCENT2] = RL_PURPLE2;
			palette.colors[CAR_DRIVER] = RL_PURPLE0;
			palette.colors[CAR_SEAT] = RL_GRAY2;
			palette.colors[CAR_HULL] = RL_BLACK;
			palette.colors[CAR_WINDOW0] = RL_GRAY2;
			palette.colors[CAR_WINDOW1] = RL_GRAY1;
			palette.colors[CAR_WINDOW2] = RL_GRAY0;
			palette.colors[CAR_LIGHT_FRONT] = RL_WHITE;
			palette.colors[CAR_LIGHT_BACK] = RL_WHITE;
			palette.colors[CAR_LIGHT_OTHER] = RL_WHITE;
			palette.colors[CAR_NUMPLATE_BASE] = RL_RAYWHITE;
			palette.colors[CAR_NUMPLATE_TEXT0] = RL_BLACK;
			palette.colors[CAR_NUMPLATE_TEXT1] = RL_GREEN2;
			palette.colors[CAR_EXTRA0] = RL_GRAY0;
			palette.colors[CAR_EXTRA1] = RL_GRAY1;
			palette.colors[CAR_EXTRA2] = RL_GRAY2;
			palette.colors[CAR_TRAIL] = RL_GREEN0;
			break;
		case 3: // DEBUG BLUE BROWN
			palette.colors[CAR_WHEEL0] = RL_BLACK;
			palette.colors[CAR_WHEEL1] = RL_GRAY2;
			palette.colors[CAR_WHEEL2] = RL_BLACK;
			palette.colors[CAR_BASE0] = RL_BLUE0;
			palette.colors[CAR_BASE1] = RL_BLUE1;
			palette.colors[CAR_BASE2] = RL_BLUE2;
			palette.colors[CAR_ACCENT0] = RL_BROWN0;
			palette.colors[CAR_ACCENT1] = RL_BROWN1;
			palette.colors[CAR_ACCENT2] = RL_BROWN2;
			palette.colors[CAR_DRIVER] = RL_YELLOW0;
			palette.colors[CAR_SEAT] = RL_GRAY2;
			palette.colors[CAR_HULL] = RL_BLACK;
			palette.colors[CAR_WINDOW0] = RL_GRAY2;
			palette.colors[CAR_WINDOW1] = RL_GRAY1;
			palette.colors[CAR_WINDOW2] = RL_GRAY0;
			palette.colors[CAR_LIGHT_FRONT] = RL_WHITE;
			palette.colors[CAR_LIGHT_BACK] = RL_WHITE;
			palette.colors[CAR_LIGHT_OTHER] = RL_WHITE;
			palette.colors[CAR_NUMPLATE_BASE] = RL_RAYWHITE;
			palette.colors[CAR_NUMPLATE_TEXT0] = RL_BLACK;
			palette.colors[CAR_NUMPLATE_TEXT1] = RL_BROWN2;
			palette.colors[CAR_EXTRA0] = RL_GRAY0;
			palette.colors[CAR_EXTRA1] = RL_GRAY1;
			palette.colors[CAR_EXTRA2] = RL_GRAY2;
			palette.colors[CAR_TRAIL] = RL_YELLOW0;
			break;
		case 4: // ASSIST (TEAL & CYAN)
			palette.colors[CAR_WHEEL0] = RM_WHITE8;
			palette.colors[CAR_WHEEL1] = RM_CYAN6;
			palette.colors[CAR_WHEEL2] = RM_WHITE8;
			palette.colors[CAR_BASE0] = RM_CYAN3;
			palette.colors[CAR_BASE1] = RM_CYAN4;
			palette.colors[CAR_BASE2] = RM_CYAN5;
			palette.colors[CAR_ACCENT0] = RM_TEAL2;
			palette.colors[CAR_ACCENT1] = RM_TEAL3;
			palette.colors[CAR_ACCENT2] = RM_TEAL4;
			palette.colors[CAR_DRIVER] = RM_LIME3;
			palette.colors[CAR_SEAT] = RM_CYAN6;
			palette.colors[CAR_HULL] = RM_WHITE8;
			palette.colors[CAR_WINDOW0] = RM_CYAN6;
			palette.colors[CAR_WINDOW1] = RM_CYAN6;
			palette.colors[CAR_WINDOW2] = RM_CYAN6;
			palette.colors[CAR_LIGHT_FRONT] = RM_TEAL0;
			palette.colors[CAR_LIGHT_BACK] = RM_TEAL0;
			palette.colors[CAR_LIGHT_OTHER] = RM_TEAL0;
			palette.colors[CAR_NUMPLATE_BASE] = RM_WHITE1;
			palette.colors[CAR_NUMPLATE_TEXT0] = RM_WHITE8;
			palette.colors[CAR_NUMPLATE_TEXT1] = RM_CYAN3;
			palette.colors[CAR_EXTRA0] = RM_WHITE2;
			palette.colors[CAR_EXTRA1] = RM_WHITE3;
			palette.colors[CAR_EXTRA2] = RM_WHITE4;
			palette.colors[CAR_TRAIL] = RM_LIME3;
			break;
		case 5: // HALLOWEEN (PURPLE & ORANGE)
			palette.colors[CAR_WHEEL0] = RM_WHITE8;
			palette.colors[CAR_WHEEL1] = RM_PURPLE6;
			palette.colors[CAR_WHEEL2] = RM_WHITE8;
			palette.colors[CAR_BASE0] = RM_PURPLE2;
			palette.colors[CAR_BASE1] = RM_PURPLE3;
			palette.colors[CAR_BASE2] = RM_PURPLE4;
			palette.colors[CAR_ACCENT0] = RM_ORANGE2;
			palette.colors[CAR_ACCENT1] = RM_ORANGE3;
			palette.colors[CAR_ACCENT2] = RM_ORANGE4;
			palette.colors[CAR_DRIVER] = RM_ORANGE3;
			palette.colors[CAR_SEAT] = RM_PURPLE6;
			palette.colors[CAR_HULL] = RM_WHITE8;
			palette.colors[CAR_WINDOW0] = RM_PURPLE6;
			palette.colors[CAR_WINDOW1] = RM_PURPLE6;
			palette.colors[CAR_WINDOW2] = RM_PURPLE6;
			palette.colors[CAR_LIGHT_FRONT] = RM_ORANGE0;
			palette.colors[CAR_LIGHT_BACK] = RM_ORANGE0;
			palette.colors[CAR_LIGHT_OTHER] = RM_ORANGE0;
			palette.colors[CAR_NUMPLATE_BASE] = RM_WHITE1;
			palette.colors[CAR_NUMPLATE_TEXT0] = RM_WHITE8;
			palette.colors[CAR_NUMPLATE_TEXT1] = RM_ORANGE3;
			palette.colors[CAR_EXTRA0] = RM_WHITE2;
			palette.colors[CAR_EXTRA1] = RM_WHITE3;
			palette.colors[CAR_EXTRA2] = RM_WHITE4;
			palette.colors[CAR_TRAIL] = RM_ORANGE3;
			break;
		case 6: // PINKY (PINK & MAGENTA)
			palette.colors[CAR_WHEEL0] = RM_WHITE8;
			palette.colors[CAR_WHEEL1] = RM_MAGENTA6;
			palette.colors[CAR_WHEEL2] = RM_WHITE8;
			palette.colors[CAR_BASE0] = RM_MAGENTA2;
			palette.colors[CAR_BASE1] = RM_MAGENTA3;
			palette.colors[CAR_BASE2] = RM_MAGENTA4;
			palette.colors[CAR_ACCENT0] = RM_PINK2;
			palette.colors[CAR_ACCENT1] = RM_PINK3;
			palette.colors[CAR_ACCENT2] = RM_PINK4;
			palette.colors[CAR_DRIVER] = RM_RED3;
			palette.colors[CAR_SEAT] = RM_MAGENTA6;
			palette.colors[CAR_HULL] = RM_WHITE8;
			palette.colors[CAR_WINDOW0] = RM_MAGENTA6;
			palette.colors[CAR_WINDOW1] = RM_MAGENTA6;
			palette.colors[CAR_WINDOW2] = RM_MAGENTA6;
			palette.colors[CAR_LIGHT_FRONT] = RM_PINK0;
			palette.colors[CAR_LIGHT_BACK] = RM_PINK0;
			palette.colors[CAR_LIGHT_OTHER] = RM_PINK0;
			palette.colors[CAR_NUMPLATE_BASE] = RM_WHITE1;
			palette.colors[CAR_NUMPLATE_TEXT0] = RM_WHITE8;
			palette.colors[CAR_NUMPLATE_TEXT1] = RM_RED3;
			palette.colors[CAR_EXTRA0] = RM_WHITE2;
			palette.colors[CAR_EXTRA1] = RM_WHITE3;
			palette.colors[CAR_EXTRA2] = RM_WHITE4;
			palette.colors[CAR_TRAIL] = RM_RED3;
			break;
		case 7: // SALMON (RED & NAVY)
			palette.colors[CAR_WHEEL0] = RM_WHITE8;
			palette.colors[CAR_WHEEL1] = RM_NAVY6;
			palette.colors[CAR_WHEEL2] = RM_WHITE8;
			palette.colors[CAR_BASE0] = RM_NAVY4;
			palette.colors[CAR_BASE1] = RM_NAVY5;
			palette.colors[CAR_BASE2] = RM_NAVY6;
			palette.colors[CAR_ACCENT0] = RM_RED2;
			palette.colors[CAR_ACCENT1] = RM_RED3;
			palette.colors[CAR_ACCENT2] = RM_RED4;
			palette.colors[CAR_DRIVER] = RM_RED1;
			palette.colors[CAR_SEAT] = RM_NAVY6;
			palette.colors[CAR_HULL] = RM_WHITE8;
			palette.colors[CAR_WINDOW0] = RM_NAVY6;
			palette.colors[CAR_WINDOW1] = RM_NAVY6;
			palette.colors[CAR_WINDOW2] = RM_NAVY6;
			palette.colors[CAR_LIGHT_FRONT] = RM_RED0;
			palette.colors[CAR_LIGHT_BACK] = RM_RED0;
			palette.colors[CAR_LIGHT_OTHER] = RM_RED0;
			palette.colors[CAR_NUMPLATE_BASE] = RM_WHITE1;
			palette.colors[CAR_NUMPLATE_TEXT0] = RM_WHITE8;
			palette.colors[CAR_NUMPLATE_TEXT1] = RM_NAVY3;
			palette.colors[CAR_EXTRA0] = RM_WHITE2;
			palette.colors[CAR_EXTRA1] = RM_WHITE3;
			palette.colors[CAR_EXTRA2] = RM_WHITE4;
			palette.colors[CAR_TRAIL] = RM_RED3;
			break;
		case 8: // GRASS (GREEN & LIME)
			palette.colors[CAR_WHEEL0] = RM_WHITE8;
			palette.colors[CAR_WHEEL1] = RM_GREEN6;
			palette.colors[CAR_WHEEL2] = RM_WHITE8;
			palette.colors[CAR_BASE0] = RM_GREEN2;
			palette.colors[CAR_BASE1] = RM_GREEN3;
			palette.colors[CAR_BASE2] = RM_GREEN4;
			palette.colors[CAR_ACCENT0] = RM_LIME2;
			palette.colors[CAR_ACCENT1] = RM_LIME3;
			palette.colors[CAR_ACCENT2] = RM_LIME4;
			palette.colors[CAR_DRIVER] = RM_LIME5;
			palette.colors[CAR_SEAT] = RM_GREEN6;
			palette.colors[CAR_HULL] = RM_WHITE8;
			palette.colors[CAR_WINDOW0] = RM_GREEN6;
			palette.colors[CAR_WINDOW1] = RM_GREEN6;
			palette.colors[CAR_WINDOW2] = RM_GREEN6;
			palette.colors[CAR_LIGHT_FRONT] = RM_LIME0;
			palette.colors[CAR_LIGHT_BACK] = RM_LIME0;
			palette.colors[CAR_LIGHT_OTHER] = RM_LIME0;
			palette.colors[CAR_NUMPLATE_BASE] = RM_WHITE1;
			palette.colors[CAR_NUMPLATE_TEXT0] = RM_WHITE8;
			palette.colors[CAR_NUMPLATE_TEXT1] = RM_LIME3;
			palette.colors[CAR_EXTRA0] = RM_WHITE2;
			palette.colors[CAR_EXTRA1] = RM_WHITE3;
			palette.colors[CAR_EXTRA2] = RM_WHITE4;
			palette.colors[CAR_TRAIL] = RM_LIME5;
			break;
		case 9: // SUNSHINE (YELLOW & BLUE)
			palette.colors[CAR_WHEEL0] = RM_WHITE8;
			palette.colors[CAR_WHEEL1] = RM_BLUE6;
			palette.colors[CAR_WHEEL2] = RM_WHITE8;
			palette.colors[CAR_BASE0] = RM_YELLOW2;
			palette.colors[CAR_BASE1] = RM_YELLOW3;
			palette.colors[CAR_BASE2] = RM_YELLOW4;
			palette.colors[CAR_ACCENT0] = RM_BLUE2;
			palette.colors[CAR_ACCENT1] = RM_BLUE3;
			palette.colors[CAR_ACCENT2] = RM_BLUE4;
			palette.colors[CAR_DRIVER] = RM_WHITE2;
			palette.colors[CAR_SEAT] = RM_BLUE6;
			palette.colors[CAR_HULL] = RM_WHITE8;
			palette.colors[CAR_WINDOW0] = RM_BLUE6;
			palette.colors[CAR_WINDOW1] = RM_BLUE6;
			palette.colors[CAR_WINDOW2] = RM_BLUE6;
			palette.colors[CAR_LIGHT_FRONT] = RM_YELLOW0;
			palette.colors[CAR_LIGHT_BACK] = RM_YELLOW0;
			palette.colors[CAR_LIGHT_OTHER] = RM_YELLOW0;
			palette.colors[CAR_NUMPLATE_BASE] = RM_WHITE1;
			palette.colors[CAR_NUMPLATE_TEXT0] = RM_WHITE8;
			palette.colors[CAR_NUMPLATE_TEXT1] = RM_WHITE4;
			palette.colors[CAR_EXTRA0] = RM_WHITE2;
			palette.colors[CAR_EXTRA1] = RM_WHITE3;
			palette.colors[CAR_EXTRA2] = RM_WHITE4;
			palette.colors[CAR_TRAIL] = RM_RED3;
			break;
		default:
			if(id & 1)
			{
				palette.colors[CAR_WHEEL0] = RM_WHITE8;
				palette.colors[CAR_WHEEL1] = RM_WHITE8;
				palette.colors[CAR_WHEEL2] = RM_WHITE8;
				palette.colors[CAR_BASE0] = RM_WHITE4;
				palette.colors[CAR_BASE1] = RM_WHITE4;
				palette.colors[CAR_BASE2] = RM_WHITE4;
				palette.colors[CAR_ACCENT0] = RM_WHITE2;
				palette.colors[CAR_ACCENT1] = RM_WHITE2;
				palette.colors[CAR_ACCENT2] = RM_WHITE2;
				palette.colors[CAR_DRIVER] = RM_WHITE3;
				palette.colors[CAR_SEAT] = RM_WHITE7;
				palette.colors[CAR_HULL] = RM_WHITE8;
				palette.colors[CAR_WINDOW0] = RM_WHITE8;
				palette.colors[CAR_WINDOW1] = RM_WHITE8;
				palette.colors[CAR_WINDOW2] = RM_WHITE8;
				palette.colors[CAR_LIGHT_FRONT] = RM_WHITE0;
				palette.colors[CAR_LIGHT_BACK] = RM_WHITE1;
				palette.colors[CAR_LIGHT_OTHER] = RM_WHITE0;
				palette.colors[CAR_NUMPLATE_BASE] = RM_WHITE1;
				palette.colors[CAR_NUMPLATE_TEXT0] = RM_WHITE8;
				palette.colors[CAR_NUMPLATE_TEXT1] = RM_WHITE8;
				palette.colors[CAR_EXTRA0] = RM_WHITE5;
				palette.colors[CAR_EXTRA1] = RM_WHITE5;
				palette.colors[CAR_EXTRA2] = RM_WHITE6;
				palette.colors[CAR_TRAIL] = RM_WHITE6;
			}
			else
			{
				palette.colors[CAR_WHEEL0] = RM_WHITE0;
				palette.colors[CAR_WHEEL1] = RM_WHITE0;
				palette.colors[CAR_WHEEL2] = RM_WHITE0;
				palette.colors[CAR_BASE0] = RM_WHITE4;
				palette.colors[CAR_BASE1] = RM_WHITE4;
				palette.colors[CAR_BASE2] = RM_WHITE4;
				palette.colors[CAR_ACCENT0] = RM_WHITE6;
				palette.colors[CAR_ACCENT1] = RM_WHITE6;
				palette.colors[CAR_ACCENT2] = RM_WHITE6;
				palette.colors[CAR_DRIVER] = RM_WHITE5;
				palette.colors[CAR_SEAT] = RM_WHITE1;
				palette.colors[CAR_HULL] = RM_WHITE0;
				palette.colors[CAR_WINDOW0] = RM_WHITE0;
				palette.colors[CAR_WINDOW1] = RM_WHITE0;
				palette.colors[CAR_WINDOW2] = RM_WHITE0;
				palette.colors[CAR_LIGHT_FRONT] = RM_WHITE8;
				palette.colors[CAR_LIGHT_BACK] = RM_WHITE7;
				palette.colors[CAR_LIGHT_OTHER] = RM_WHITE8;
				palette.colors[CAR_NUMPLATE_BASE] = RM_WHITE7;
				palette.colors[CAR_NUMPLATE_TEXT0] = RM_WHITE0;
				palette.colors[CAR_NUMPLATE_TEXT1] = RM_WHITE0;
				palette.colors[CAR_EXTRA0] = RM_WHITE3;
				palette.colors[CAR_EXTRA1] = RM_WHITE3;
				palette.colors[CAR_EXTRA2] = RM_WHITE2;
				palette.colors[CAR_TRAIL] = RM_WHITE2;
			}
			break;
	}
	return palette;
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
	return rmc(tri.color);
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
	int anim_time = 0;
	char color1 = RM_WHITE5, color2, color3, color4;
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
			anim_time = (int)(Wrap((float)game_time, 0.0, 4.0) * 16.0);
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
		case(DCONTROLS):
			asset = MallocAsset(36);
			asset->tris[0] = (Tri){4.5*BU, 2.875*BU, 4*BU, 1.875*BU, 3.5*BU, 2.875*BU, RM_WHITE5};
			asset->tris[1] = (Tri){3*BU, 1.125*BU, 5*BU, 1.375*BU, 5*BU, 1.125*BU, RM_WHITE5};
			asset->tris[2] = (Tri){5*BU, 1.375*BU, 3*BU, 1.125*BU, 3*BU, 1.375*BU, RM_WHITE5};
			asset->tris[3] = (Tri){5*BU, 3.625*BU, 3*BU, 3.375*BU, 3*BU, 3.625*BU, RM_WHITE5};
			asset->tris[4] = (Tri){3*BU, 3.375*BU, 5*BU, 3.625*BU, 5*BU, 3.375*BU, RM_WHITE5};
			asset->tris[5] = (Tri){5.25*BU, 3.375*BU, 5.25*BU, 1.375*BU, 5*BU, 3.375*BU, RM_WHITE5};
			asset->tris[6] = (Tri){5*BU, 3.375*BU, 5.25*BU, 1.375*BU, 5*BU, 1.375*BU, RM_WHITE5};
			asset->tris[7] = (Tri){2.75*BU, 3.375*BU, 3*BU, 1.375*BU, 2.75*BU, 1.375*BU, RM_WHITE5};
			asset->tris[8] = (Tri){3*BU, 3.375*BU, 3*BU, 1.375*BU, 2.75*BU, 3.375*BU, RM_WHITE5};
			asset->tris[9] = (Tri){3*BU, 3.875*BU, 5*BU, 4.125*BU, 5*BU, 3.875*BU, RM_WHITE5};
			asset->tris[10] = (Tri){5*BU, 4.125*BU, 3*BU, 3.875*BU, 3*BU, 4.125*BU, RM_WHITE5};
			asset->tris[11] = (Tri){5*BU, 6.375*BU, 3*BU, 6.125*BU, 3*BU, 6.375*BU, RM_WHITE5};
			asset->tris[12] = (Tri){3*BU, 6.125*BU, 5*BU, 6.375*BU, 5*BU, 6.125*BU, RM_WHITE5};
			asset->tris[13] = (Tri){5.25*BU, 6.125*BU, 5.25*BU, 4.125*BU, 5*BU, 6.125*BU, RM_WHITE5};
			asset->tris[14] = (Tri){3*BU, 6.125*BU, 3*BU, 4.125*BU, 2.75*BU, 6.125*BU, RM_WHITE5};
			asset->tris[15] = (Tri){2.75*BU, 6.125*BU, 3*BU, 4.125*BU, 2.75*BU, 4.125*BU, RM_WHITE5};
			asset->tris[16] = (Tri){5*BU, 6.125*BU, 5.25*BU, 4.125*BU, 5*BU, 4.125*BU, RM_WHITE5};
			asset->tris[17] = (Tri){5.5*BU, 6.125*BU, 5.75*BU, 4.125*BU, 5.5*BU, 4.125*BU, RM_WHITE5};
			asset->tris[18] = (Tri){7.75*BU, 6.125*BU, 8*BU, 4.125*BU, 7.75*BU, 4.125*BU, RM_WHITE5};
			asset->tris[19] = (Tri){2.25*BU, 6.125*BU, 2.5*BU, 4.125*BU, 2.25*BU, 4.125*BU, RM_WHITE5};
			asset->tris[20] = (Tri){0*BU, 6.125*BU, 0.25*BU, 4.125*BU, 0*BU, 4.125*BU, RM_WHITE5};
			asset->tris[21] = (Tri){0.25*BU, 6.125*BU, 2.25*BU, 6.375*BU, 2.25*BU, 6.125*BU, RM_WHITE5};
			asset->tris[22] = (Tri){5.75*BU, 6.125*BU, 7.75*BU, 6.375*BU, 7.75*BU, 6.125*BU, RM_WHITE5};
			asset->tris[23] = (Tri){5.75*BU, 3.875*BU, 7.75*BU, 4.125*BU, 7.75*BU, 3.875*BU, RM_WHITE5};
			asset->tris[24] = (Tri){0.25*BU, 3.875*BU, 2.25*BU, 4.125*BU, 2.25*BU, 3.875*BU, RM_WHITE5};
			asset->tris[25] = (Tri){0.25*BU, 3.875*BU, 0.25*BU, 4.125*BU, 2.25*BU, 4.125*BU, RM_WHITE5};
			asset->tris[26] = (Tri){0*BU, 6.125*BU, 0.25*BU, 6.125*BU, 0.25*BU, 4.125*BU, RM_WHITE5};
			asset->tris[27] = (Tri){0.25*BU, 6.375*BU, 2.25*BU, 6.375*BU, 0.25*BU, 6.125*BU, RM_WHITE5};
			asset->tris[28] = (Tri){2.25*BU, 6.125*BU, 2.5*BU, 6.125*BU, 2.5*BU, 4.125*BU, RM_WHITE5};
			asset->tris[29] = (Tri){5.5*BU, 6.125*BU, 5.75*BU, 6.125*BU, 5.75*BU, 4.125*BU, RM_WHITE5};
			asset->tris[30] = (Tri){7.75*BU, 6.125*BU, 8*BU, 6.125*BU, 8*BU, 4.125*BU, RM_WHITE5};
			asset->tris[31] = (Tri){5.75*BU, 6.125*BU, 5.75*BU, 6.375*BU, 7.75*BU, 6.375*BU, RM_WHITE5};
			asset->tris[32] = (Tri){7.75*BU, 4.125*BU, 5.75*BU, 3.875*BU, 5.75*BU, 4.125*BU, RM_WHITE5};
			asset->tris[33] = (Tri){4*BU, 5.625*BU, 4.5*BU, 4.625*BU, 3.5*BU, 4.625*BU, RM_WHITE5};
			asset->tris[34] = (Tri){1.75*BU, 5.625*BU, 1.75*BU, 4.625*BU, 0.75*BU, 5.125*BU, RM_WHITE5};
			asset->tris[35] = (Tri){7.25*BU, 5.125*BU, 6.25*BU, 4.625*BU, 6.25*BU, 5.625*BU, RM_WHITE5};
			break;
		case(DRACECAR):
			asset = MallocAsset(24);
			asset->tris[0] = (Tri){-20.500000, -21.000000, -27.500000, 23.000000, 20.500000, -21.000000, CAR_ACCENT0};
			asset->tris[1] = (Tri){20.500000, -21.000000, -27.500000, 23.000000, 27.500000, 23.000000, CAR_BASE0};
			asset->tris[2] = (Tri){-8.500000, -21.000000, 8.500000, -21.000000, 5.500000, -45.000000, CAR_ACCENT0};
			asset->tris[3] = (Tri){-5.500000, -45.000000, -8.500000, -21.000000, 5.500000, -45.000000, CAR_BASE0};
			asset->tris[4] = (Tri){-27.500000, -45.000000, 20.500000, -53.000000, -20.500000, -53.000000, CAR_BASE1};
			asset->tris[5] = (Tri){20.500000, -53.000000, -27.500000, -45.000000, 27.500000, -45.000000, CAR_BASE1};
			asset->tris[6] = (Tri){12.500000, 23.000000, -12.500000, 23.000000, -12.500000, 44.000000, CAR_BASE0};
			asset->tris[7] = (Tri){12.500000, 23.000000, -12.500000, 44.000000, 12.500000, 44.000000, CAR_BASE0};
			asset->tris[8] = (Tri){-18.500000, 44.000000, -18.500000, 56.000000, 18.500000, 44.000000, CAR_BASE1};
			asset->tris[9] = (Tri){-18.500000, 56.000000, 18.500000, 56.000000, 18.500000, 44.000000, CAR_BASE1};
			asset->tris[10] = (Tri){-4.500000, -53.000000, 4.500000, -53.000000, 0.500000, -56.000000, CAR_BASE1};
			asset->tris[11] = (Tri){-27.500000, -43.000000, -27.500000, -26.000000, -14.500000, -43.000000, CAR_WHEEL2};
			asset->tris[12] = (Tri){-27.500000, -26.000000, -14.500000, -26.000000, -14.500000, -43.000000, CAR_WHEEL2};
			asset->tris[13] = (Tri){14.500000, -43.000000, 14.500000, -26.000000, 27.500000, -43.000000, CAR_WHEEL2};
			asset->tris[14] = (Tri){27.500000, -43.000000, 14.500000, -26.000000, 27.500000, -26.000000, CAR_WHEEL2};
			asset->tris[15] = (Tri){14.500000, 42.000000, 27.500000, 42.000000, 27.500000, 25.000000, CAR_WHEEL2};
			asset->tris[16] = (Tri){-27.500000, 42.000000, -14.500000, 42.000000, -14.500000, 25.000000, CAR_WHEEL2};
			asset->tris[17] = (Tri){-27.500000, 42.000000, -14.500000, 25.000000, -27.500000, 25.000000, CAR_WHEEL2};
			asset->tris[18] = (Tri){14.500000, 42.000000, 27.500000, 25.000000, 14.500000, 25.000000, CAR_WHEEL2};
			asset->tris[19] = (Tri){-6.500000, 7.000000, 0.500000, 17.000000, 6.500000, 7.000000, CAR_HULL};
			asset->tris[20] = (Tri){6.500000, -6.000000, -6.500000, 7.000000, 6.500000, 7.000000, CAR_HULL};
			asset->tris[21] = (Tri){-6.500000, -6.000000, -6.500000, 7.000000, 6.500000, -6.000000, CAR_HULL};
			asset->tris[22] = (Tri){0.500000, -16.000000, -6.500000, -6.000000, 6.500000, -6.000000, CAR_HULL};
			asset->tris[23] = (Tri){0.500000, -5.000000, -5.500000, 6.000000, 5.500000, 6.000000, CAR_DRIVER};
			break;
		case(B1x1DR0):
			color1 = RM_ORANGE4;
		case(B1x1AR0):
			asset = MallocAsset(10);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, color1};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, color1};
			asset->tris[2] = (Tri){0.5*BU, 0*BU, 1*BU, 0*BU, 0.75*BU, -0.25*BU, RM_WHITE7};
			asset->tris[3] = (Tri){0.5*BU, 0*BU, 1*BU, 8*BU, 1*BU, 0*BU, RM_WHITE7};
			asset->tris[4] = (Tri){0.5*BU, 0*BU, 0.5*BU, 8*BU, 1*BU, 8*BU, RM_WHITE7};
			asset->tris[5] = (Tri){0.5*BU, 8*BU, 0.75*BU, 8.25*BU, 1*BU, 8*BU, RM_WHITE7};
			asset->tris[6] = (Tri){7*BU, 0*BU, 7.5*BU, 0*BU, 7.25*BU, -0.25*BU, RM_WHITE7};
			asset->tris[7] = (Tri){7*BU, 0*BU, 7.5*BU, 8*BU, 7.5*BU, 0*BU, RM_WHITE7};
			asset->tris[8] = (Tri){7*BU, 0*BU, 7*BU, 8*BU, 7.5*BU, 8*BU, RM_WHITE7};
			asset->tris[9] = (Tri){7*BU, 8*BU, 7.25*BU, 8.25*BU, 7.5*BU, 8*BU, RM_WHITE7};
			break;
		case(B1x1DT0):
			color1 = RM_ORANGE4;
		case(B1x1AT0):
			asset = MallocAsset(15);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, color1};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, color1};
			asset->tris[2] = (Tri){0*BU, 7.5*BU, 0.5*BU, 8*BU, 1*BU, 8*BU, RM_WHITE7};
			asset->tris[3] = (Tri){1*BU, 8*BU, 0*BU, 7*BU, 0*BU, 7.5*BU, RM_WHITE7};
			asset->tris[4] = (Tri){7*BU, 8*BU, 7.5*BU, 8*BU, 6.5*BU, 4.5*BU, RM_WHITE7};
			asset->tris[5] = (Tri){0*BU, 0.5*BU, 0*BU, 1*BU, 3.5*BU, 1.5*BU, RM_WHITE7};
			asset->tris[6] = (Tri){3.5*BU, 1.5*BU, 6.5*BU, 4.5*BU, 5.375*BU, 2.625*BU, RM_WHITE7};
			asset->tris[7] = (Tri){0*BU, 0.5*BU, 3.5*BU, 1.5*BU, 3.75*BU, 1*BU, RM_WHITE7};
			asset->tris[8] = (Tri){5.375*BU, 2.625*BU, 3.75*BU, 1*BU, 3.5*BU, 1.5*BU, RM_WHITE7};
			asset->tris[9] = (Tri){7.5*BU, 8*BU, 7*BU, 4.25*BU, 6.5*BU, 4.5*BU, RM_WHITE7};
			asset->tris[10] = (Tri){7*BU, 4.25*BU, 5.375*BU, 2.625*BU, 6.5*BU, 4.5*BU, RM_WHITE7};
			asset->tris[11] = (Tri){0*BU, 1*BU, 0*BU, 0.5*BU, -0.25*BU, 0.75*BU, RM_WHITE7};
			asset->tris[12] = (Tri){0*BU, 7.5*BU, 0*BU, 7*BU, -0.25*BU, 7.25*BU, RM_WHITE7};
			asset->tris[13] = (Tri){0.5*BU, 8*BU, 0.75*BU, 8.25*BU, 1*BU, 8*BU, RM_WHITE7};
			asset->tris[14] = (Tri){7*BU, 8*BU, 7.25*BU, 8.25*BU, 7.5*BU, 8*BU, RM_WHITE7};
			break;
		case(B1x1CR0):
			asset = MallocAsset(8);
			asset->tris[0] = (Tri){7.5*BU, 3.25*BU, .5*BU, 3.25*BU, .5*BU, 4.75*BU, RM_BLUE5};
			asset->tris[1] = (Tri){.5*BU, 4.75*BU, 7.5*BU, 4.75*BU, 7.5*BU, 3.25*BU, RM_BLUE5};
			asset->tris[2] = (Tri){3.875*BU, 4.5*BU, 3.375*BU, 4*BU, 3.125*BU, 4.5*BU, RM_BLUE0};
			asset->tris[3] = (Tri){3.875*BU, 3.5*BU, 3.125*BU, 3.5*BU, 3.375*BU, 4*BU, RM_BLUE0};
			asset->tris[4] = (Tri){4.375*BU, 3.5*BU, 4.125*BU, 4.5*BU, 4.375*BU, 4.5*BU, RM_BLUE0};
			asset->tris[5] = (Tri){4.375*BU, 3.5*BU, 4.375*BU, 4.25*BU, 4.875*BU, 3.875*BU, RM_BLUE0};
			asset->tris[6] = (Tri){4.375*BU, 3.5*BU, 4.125*BU, 3.5*BU, 4.125*BU, 4.5*BU, RM_BLUE0};
			asset->tris[7] = (Tri){3.125*BU, 3.5*BU, 3.125*BU, 4.5*BU, 3.375*BU, 4*BU, RM_BLUE0};
			break;
		case(B1x1SR0):
			asset = MallocAsset(2);
			asset->tris[0] = (Tri){7.5*BU, 1.25*BU, .5*BU, 1.25*BU, .5*BU, 2.75*BU, RM_GREEN4};
			asset->tris[1] = (Tri){.5*BU, 2.75*BU, 7.5*BU, 2.75*BU, 7.5*BU, 1.25*BU, RM_GREEN4};
			break;
		case(B1x1FR0):
			asset = MallocAsset(2);
			asset->tris[0] = (Tri){7.5*BU, 1.25*BU, .5*BU, 1.25*BU, .5*BU, 2.75*BU, RM_RED4};
			asset->tris[1] = (Tri){.5*BU, 2.75*BU, 7.5*BU, 2.75*BU, 7.5*BU, 1.25*BU, RM_RED4};
			break;
		case(B1x1VB0):
			asset = MallocAsset(6);
			asset->tris[0] = (Tri){4.75*BU, 4.25*BU, 4.75*BU, 3.75*BU, 4.25*BU, 3.25*BU, RM_WHITE7};
			asset->tris[1] = (Tri){4.75*BU, 4.25*BU, 4.25*BU, 3.25*BU, 4.25*BU, 4.75*BU, RM_WHITE7};
			asset->tris[2] = (Tri){4.25*BU, 4.75*BU, 4.25*BU, 3.25*BU, 3.75*BU, 4.75*BU, RM_WHITE7};
			asset->tris[3] = (Tri){3.25*BU, 3.75*BU, 3.25*BU, 4.25*BU, 3.75*BU, 4.75*BU, RM_WHITE7};
			asset->tris[4] = (Tri){3.25*BU, 3.75*BU, 3.75*BU, 4.75*BU, 3.75*BU, 3.25*BU, RM_WHITE7};
			asset->tris[5] = (Tri){3.75*BU, 3.25*BU, 3.75*BU, 4.75*BU, 4.25*BU, 3.25*BU, RM_WHITE7};
			break;
		case(B1x1VB1):
			asset = MallocAsset(6);
			asset->tris[0] = (Tri){2.75*BU, 6.25*BU, 2.75*BU, 5.75*BU, 2.25*BU, 5.25*BU, RM_WHITE7};
			asset->tris[1] = (Tri){2.75*BU, 6.25*BU, 2.25*BU, 5.25*BU, 2.25*BU, 6.75*BU, RM_WHITE7};
			asset->tris[2] = (Tri){2.25*BU, 6.75*BU, 2.25*BU, 5.25*BU, 1.75*BU, 6.75*BU, RM_WHITE7};
			asset->tris[3] = (Tri){1.25*BU, 5.75*BU, 1.25*BU, 6.25*BU, 1.75*BU, 6.75*BU, RM_WHITE7};
			asset->tris[4] = (Tri){1.25*BU, 5.75*BU, 1.75*BU, 6.75*BU, 1.75*BU, 5.25*BU, RM_WHITE7};
			asset->tris[5] = (Tri){1.75*BU, 5.25*BU, 1.75*BU, 6.75*BU, 2.25*BU, 5.25*BU, RM_WHITE7};
			break;
		case(B1x1VB2):
			asset = MallocAsset(12);
			asset->tris[0] = (Tri){6.75*BU, 2.25*BU, 6.75*BU, 1.75*BU, 6.25*BU, 1.25*BU, RM_WHITE7};
			asset->tris[1] = (Tri){6.75*BU, 2.25*BU, 6.25*BU, 1.25*BU, 6.25*BU, 2.75*BU, RM_WHITE7};
			asset->tris[2] = (Tri){6.25*BU, 2.75*BU, 6.25*BU, 1.25*BU, 5.75*BU, 2.75*BU, RM_WHITE7};
			asset->tris[3] = (Tri){5.25*BU, 1.75*BU, 5.25*BU, 2.25*BU, 5.75*BU, 2.75*BU, RM_WHITE7};
			asset->tris[4] = (Tri){5.25*BU, 1.75*BU, 5.75*BU, 2.75*BU, 5.75*BU, 1.25*BU, RM_WHITE7};
			asset->tris[5] = (Tri){5.75*BU, 1.25*BU, 5.75*BU, 2.75*BU, 6.25*BU, 1.25*BU, RM_WHITE7};
			asset->tris[6] = (Tri){2.75*BU, 6.25*BU, 2.75*BU, 5.75*BU, 2.25*BU, 5.25*BU, RM_WHITE7};
			asset->tris[7] = (Tri){2.75*BU, 6.25*BU, 2.25*BU, 5.25*BU, 2.25*BU, 6.75*BU, RM_WHITE7};
			asset->tris[8] = (Tri){2.25*BU, 6.75*BU, 2.25*BU, 5.25*BU, 1.75*BU, 6.75*BU, RM_WHITE7};
			asset->tris[9] = (Tri){1.25*BU, 5.75*BU, 1.25*BU, 6.25*BU, 1.75*BU, 6.75*BU, RM_WHITE7};
			asset->tris[10] = (Tri){1.25*BU, 5.75*BU, 1.75*BU, 6.75*BU, 1.75*BU, 5.25*BU, RM_WHITE7};
			asset->tris[11] = (Tri){1.75*BU, 5.25*BU, 1.75*BU, 6.75*BU, 2.25*BU, 5.25*BU, RM_WHITE7};
			break;
		case(B1x1VB3):
			asset = MallocAsset(12);
			asset->tris[0] = (Tri){6.75*BU, 6.25*BU, 6.75*BU, 5.75*BU, 6.25*BU, 5.25*BU, RM_WHITE7};
			asset->tris[1] = (Tri){6.75*BU, 6.25*BU, 6.25*BU, 5.25*BU, 6.25*BU, 6.75*BU, RM_WHITE7};
			asset->tris[2] = (Tri){6.25*BU, 6.75*BU, 6.25*BU, 5.25*BU, 5.75*BU, 6.75*BU, RM_WHITE7};
			asset->tris[3] = (Tri){5.25*BU, 5.75*BU, 5.25*BU, 6.25*BU, 5.75*BU, 6.75*BU, RM_WHITE7};
			asset->tris[4] = (Tri){5.25*BU, 5.75*BU, 5.75*BU, 6.75*BU, 5.75*BU, 5.25*BU, RM_WHITE7};
			asset->tris[5] = (Tri){5.75*BU, 5.25*BU, 5.75*BU, 6.75*BU, 6.25*BU, 5.25*BU, RM_WHITE7};
			asset->tris[6] = (Tri){2.75*BU, 2.25*BU, 2.75*BU, 1.75*BU, 2.25*BU, 1.25*BU, RM_WHITE7};
			asset->tris[7] = (Tri){2.75*BU, 2.25*BU, 2.25*BU, 1.25*BU, 2.25*BU, 2.75*BU, RM_WHITE7};
			asset->tris[8] = (Tri){2.25*BU, 2.75*BU, 2.25*BU, 1.25*BU, 1.75*BU, 2.75*BU, RM_WHITE7};
			asset->tris[9] = (Tri){1.25*BU, 1.75*BU, 1.25*BU, 2.25*BU, 1.75*BU, 2.75*BU, RM_WHITE7};
			asset->tris[10] = (Tri){1.25*BU, 1.75*BU, 1.75*BU, 2.75*BU, 1.75*BU, 1.25*BU, RM_WHITE7};
			asset->tris[11] = (Tri){1.75*BU, 1.25*BU, 1.75*BU, 2.75*BU, 2.25*BU, 1.25*BU, RM_WHITE7};
			break;
		case(B2x2DT0):
			color1 = RM_ORANGE4;
		case(B2x2AT0):
			size = (Vector2){BLOCK_SIZE * 2, BLOCK_SIZE * 2};
			asset = MallocAsset(18);
			asset->tris[0] = (Tri){0*BU, 16*BU, 16*BU, 16*BU, 16*BU, 0*BU, color1};
			asset->tris[1] = (Tri){0*BU, 0*BU, 0*BU, 16*BU, 16*BU, 0*BU, color1};
			asset->tris[2] = (Tri){8.5*BU, 16*BU, 9*BU, 16*BU, 7.5*BU, 12*BU, RM_WHITE7};
			asset->tris[3] = (Tri){0*BU, 7*BU, 0*BU, 7.5*BU, 4*BU, 8.5*BU, RM_WHITE7};
			asset->tris[4] = (Tri){0*BU, 7*BU, 4*BU, 8.5*BU, 4.25*BU, 8*BU, RM_WHITE7};
			asset->tris[5] = (Tri){8*BU, 11.75*BU, 4.25*BU, 8*BU, 4*BU, 8.5*BU, RM_WHITE7};
			asset->tris[6] = (Tri){9*BU, 16*BU, 8*BU, 11.75*BU, 7.5*BU, 12*BU, RM_WHITE7};
			asset->tris[7] = (Tri){8*BU, 11.75*BU, 4*BU, 8.5*BU, 7.5*BU, 12*BU, RM_WHITE7};
			asset->tris[8] = (Tri){15*BU, 16*BU, 15.5*BU, 16*BU, 14*BU, 8.75*BU, RM_WHITE7};
			asset->tris[9] = (Tri){15*BU, 16*BU, 14*BU, 8.75*BU, 13.5*BU, 9*BU, RM_WHITE7};
			asset->tris[10] = (Tri){14*BU, 8.75*BU, 7.25*BU, 2*BU, 7*BU, 2.5*BU, RM_WHITE7};
			asset->tris[11] = (Tri){13.5*BU, 9*BU, 14*BU, 8.75*BU, 7*BU, 2.5*BU, RM_WHITE7};
			asset->tris[12] = (Tri){7.25*BU, 2*BU, 0*BU, 0.5*BU, 0*BU, 1*BU, RM_WHITE7};
			asset->tris[13] = (Tri){0*BU, 1*BU, 7*BU, 2.5*BU, 7.25*BU, 2*BU, RM_WHITE7};
			asset->tris[14] = (Tri){0*BU, 1*BU, 0*BU, 0.5*BU, -0.25*BU, 0.75*BU, RM_WHITE7};
			asset->tris[15] = (Tri){0*BU, 7.5*BU, 0*BU, 7*BU, -0.25*BU, 7.25*BU, RM_WHITE7};
			asset->tris[16] = (Tri){8.5*BU, 16*BU, 8.75*BU, 16.25*BU, 9*BU, 16*BU, RM_WHITE7};
			asset->tris[17] = (Tri){15*BU, 16*BU, 15.25*BU, 16.25*BU, 15.5*BU, 16*BU, RM_WHITE7};
			break;
		case(B1x1DJ0):
			color1 = RM_ORANGE4;
		case(B1x1AJ0):
			asset = MallocAsset(10);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, color1};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, color1};
			asset->tris[2] = (Tri){-0.25*BU, 7.25*BU, 0.75*BU, 8.25*BU, 1*BU, 8*BU, RM_WHITE7};
			asset->tris[3] = (Tri){1*BU, 8*BU, 0*BU, 7*BU, -0.25*BU, 7.25*BU, RM_WHITE7};
			asset->tris[4] = (Tri){-0.25*BU, 0.75*BU, 1*BU, 0*BU, 0.75*BU, -0.25*BU, RM_WHITE7};
			asset->tris[5] = (Tri){1*BU, 0*BU, -0.25*BU, 0.75*BU, 0*BU, 1*BU, RM_WHITE7};
			asset->tris[6] = (Tri){7*BU, 0*BU, 7.5*BU, 0*BU, 7.25*BU, -0.25*BU, RM_WHITE7};
			asset->tris[7] = (Tri){7*BU, 0*BU, 7.5*BU, 8*BU, 7.5*BU, 0*BU, RM_WHITE7};
			asset->tris[8] = (Tri){7*BU, 0*BU, 7*BU, 8*BU, 7.5*BU, 8*BU, RM_WHITE7};
			asset->tris[9] = (Tri){7*BU, 8*BU, 7.25*BU, 8.25*BU, 7.5*BU, 8*BU, RM_WHITE7};
			break;
		case(B1x1DJ1):
			color1 = RM_ORANGE4;
		case(B1x1AJ1):
			asset = MallocAsset(10);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, color1};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, color1};
			asset->tris[2] = (Tri){-0.25*BU, 7.25*BU, 0.75*BU, 8.25*BU, 1*BU, 8*BU, RM_WHITE7};
			asset->tris[3] = (Tri){1*BU, 8*BU, 0*BU, 7*BU, -0.25*BU, 7.25*BU, RM_WHITE7};
			asset->tris[4] = (Tri){-0.25*BU, 0.75*BU, 1*BU, 0*BU, 0.75*BU, -0.25*BU, RM_WHITE7};
			asset->tris[5] = (Tri){1*BU, 0*BU, -0.25*BU, 0.75*BU, 0*BU, 1*BU, RM_WHITE7};
			asset->tris[6] = (Tri){8.25*BU, 7.25*BU, 7*BU, 8*BU, 7.25*BU, 8.25*BU, RM_WHITE7};
			asset->tris[7] = (Tri){7*BU, 8*BU, 8.25*BU, 7.25*BU, 8*BU, 7*BU, RM_WHITE7};
			asset->tris[8] = (Tri){8.25*BU, 0.75*BU, 7.25*BU, -0.25*BU, 7*BU, 0*BU, RM_WHITE7};
			asset->tris[9] = (Tri){7*BU, 0*BU, 8*BU, 1*BU, 8.25*BU, 0.75*BU, RM_WHITE7};
			break;
		case(B1x1BR0):
			anim_time = (int)Wrap((float)game_time, 0.0, 3.0);
			color1 = RM_CYAN0;
			color2 = RM_BLUE1;
			color3 = RM_MAGENTA2;
			if(anim_time == 1)
			{
				color1 = RM_MAGENTA2;
				color2 = RM_CYAN0;
				color3 = RM_BLUE1;
			}
			if(anim_time == 2)
			{
				color1 = RM_BLUE1;
				color2 = RM_MAGENTA2;
				color3 = RM_CYAN0;
			}
			asset = MallocAsset(16);
			asset->tris[0] = (Tri){1*BU, 8*BU, 7*BU, 8*BU, 7*BU, 7*BU, color1};
			asset->tris[1] = (Tri){1*BU, 7*BU, 1*BU, 8*BU, 7*BU, 7*BU, color2};
			asset->tris[2] = (Tri){1*BU, 7*BU, 7*BU, 7*BU, 7*BU, 6*BU, color2};
			asset->tris[3] = (Tri){1*BU, 7*BU, 7*BU, 6*BU, 1*BU, 6*BU, color3};
			asset->tris[4] = (Tri){1*BU, 6*BU, 7*BU, 6*BU, 7*BU, 5*BU, color3};
			asset->tris[5] = (Tri){1*BU, 6*BU, 7*BU, 5*BU, 1*BU, 5*BU, color1};
			asset->tris[6] = (Tri){1*BU, 5*BU, 7*BU, 5*BU, 7*BU, 4*BU, color1};
			asset->tris[7] = (Tri){7*BU, 4*BU, 1*BU, 4*BU, 1*BU, 5*BU, color2};
			asset->tris[8] = (Tri){1*BU, 4*BU, 7*BU, 3*BU, 1*BU, 3*BU, color3};
			asset->tris[9] = (Tri){1*BU, 4*BU, 7*BU, 4*BU, 7*BU, 3*BU, color2};
			asset->tris[10] = (Tri){1*BU, 3*BU, 7*BU, 3*BU, 7*BU, 2*BU, color3};
			asset->tris[11] = (Tri){1*BU, 3*BU, 7*BU, 2*BU, 1*BU, 2*BU, color1};
			asset->tris[12] = (Tri){1*BU, 2*BU, 7*BU, 2*BU, 7*BU, 1*BU, color1};
			asset->tris[13] = (Tri){7*BU, 1*BU, 1*BU, 1*BU, 1*BU, 2*BU, color2};
			asset->tris[14] = (Tri){1*BU, 1*BU, 7*BU, 1*BU, 7*BU, 0*BU, color2};
			asset->tris[15] = (Tri){1*BU, 1*BU, 7*BU, 0*BU, 1*BU, 0*BU, color3};
			break;
		case(B3x2DJ0):
			color1 = RM_ORANGE4;
		case(B3x2AJ0):
			size = (Vector2){BLOCK_SIZE * 3, BLOCK_SIZE * 2};
			asset = MallocAsset(28);
			asset->tris[0] = (Tri){24*BU, 0*BU, 0*BU, 16*BU, 24*BU, 16*BU, color1};
			asset->tris[1] = (Tri){0*BU, 0*BU, 0*BU, 16*BU, 24*BU, 0*BU, color1};
			asset->tris[2] = (Tri){8.5*BU, 16*BU, 9*BU, 16*BU, 7.5*BU, 12*BU, RM_WHITE7};
			asset->tris[3] = (Tri){0*BU, 7*BU, 0*BU, 7.5*BU, 4*BU, 8.5*BU, RM_WHITE7};
			asset->tris[4] = (Tri){0*BU, 7*BU, 4*BU, 8.5*BU, 4.25*BU, 8*BU, RM_WHITE7};
			asset->tris[5] = (Tri){8*BU, 11.75*BU, 4.25*BU, 8*BU, 4*BU, 8.5*BU, RM_WHITE7};
			asset->tris[6] = (Tri){9*BU, 16*BU, 8*BU, 11.75*BU, 7.5*BU, 12*BU, RM_WHITE7};
			asset->tris[7] = (Tri){8*BU, 11.75*BU, 4*BU, 8.5*BU, 7.5*BU, 12*BU, RM_WHITE7};
			asset->tris[8] = (Tri){12*BU, 6.75*BU, 7.25*BU, 2*BU, 7*BU, 2.5*BU, RM_WHITE7};
			asset->tris[9] = (Tri){12*BU, 7.5*BU, 12*BU, 6.75*BU, 7*BU, 2.5*BU, RM_WHITE7};
			asset->tris[10] = (Tri){7.25*BU, 2*BU, 0*BU, 0.5*BU, 0*BU, 1*BU, RM_WHITE7};
			asset->tris[11] = (Tri){0*BU, 1*BU, 7*BU, 2.5*BU, 7.25*BU, 2*BU, RM_WHITE7};
			asset->tris[12] = (Tri){24*BU, 1*BU, 24*BU, 0.5*BU, 16.75*BU, 2*BU, RM_WHITE7};
			asset->tris[13] = (Tri){17*BU, 2.5*BU, 24*BU, 1*BU, 16.75*BU, 2*BU, RM_WHITE7};
			asset->tris[14] = (Tri){17*BU, 2.5*BU, 16.75*BU, 2*BU, 12*BU, 6.75*BU, RM_WHITE7};
			asset->tris[15] = (Tri){17*BU, 2.5*BU, 12*BU, 6.75*BU, 12*BU, 7.5*BU, RM_WHITE7};
			asset->tris[16] = (Tri){15*BU, 16*BU, 15.5*BU, 16*BU, 16*BU, 11.75*BU, RM_WHITE7};
			asset->tris[17] = (Tri){24*BU, 7.5*BU, 24*BU, 7*BU, 19.75*BU, 8*BU, RM_WHITE7};
			asset->tris[18] = (Tri){15.5*BU, 16*BU, 16.5*BU, 12*BU, 16*BU, 11.75*BU, RM_WHITE7};
			asset->tris[19] = (Tri){20*BU, 8.5*BU, 24*BU, 7.5*BU, 19.75*BU, 8*BU, RM_WHITE7};
			asset->tris[20] = (Tri){16.5*BU, 12*BU, 20*BU, 8.5*BU, 19.75*BU, 8*BU, RM_WHITE7};
			asset->tris[21] = (Tri){19.75*BU, 8*BU, 16*BU, 11.75*BU, 16.5*BU, 12*BU, RM_WHITE7};
			asset->tris[22] = (Tri){0*BU, 1*BU, 0*BU, 0.5*BU, -0.25*BU, 0.75*BU, RM_WHITE7};
			asset->tris[23] = (Tri){0*BU, 7.5*BU, 0*BU, 7*BU, -0.25*BU, 7.25*BU, RM_WHITE7};
			asset->tris[24] = (Tri){24*BU, 7*BU, 24*BU, 7.5*BU, 24.25*BU, 7.25*BU, RM_WHITE7};
			asset->tris[25] = (Tri){24*BU, 0.5*BU, 24*BU, 1*BU, 24.25*BU, 0.75*BU, RM_WHITE7};
			asset->tris[26] = (Tri){15.5*BU, 16*BU, 15*BU, 16*BU, 15.25*BU, 16.25*BU, RM_WHITE7};
			asset->tris[27] = (Tri){9*BU, 16*BU, 8.5*BU, 16*BU, 8.75*BU, 16.25*BU, RM_WHITE7};
			break;
		case(B2x2DR0):
			color1 = RM_ORANGE4;
		case(B2x2AR0):
			size = (Vector2){BLOCK_SIZE * 2, BLOCK_SIZE * 2};
			asset = MallocAsset(18);
			asset->tris[0] = (Tri){16*BU, 0*BU, 0*BU, 16*BU, 16*BU, 16*BU, color1};
			asset->tris[1] = (Tri){0*BU, 0*BU, 0*BU, 16*BU, 16*BU, 0*BU, color1};
			asset->tris[2] = (Tri){8.5*BU, 16*BU, 9*BU, 16*BU, 7.25*BU, 12.125*BU, RM_WHITE7};
			asset->tris[3] = (Tri){9*BU, 16*BU, 7.75*BU, 11.875*BU, 7.25*BU, 12.125*BU, RM_WHITE7};
			asset->tris[4] = (Tri){15*BU, 16*BU, 15.5*BU, 16*BU, 13.75*BU, 8.875*BU, RM_WHITE7};
			asset->tris[5] = (Tri){15*BU, 16*BU, 13.75*BU, 8.875*BU, 13.25*BU, 9.125*BU, RM_WHITE7};
			asset->tris[6] = (Tri){7*BU, 0*BU, 8.25*BU, 4.125*BU, 7.5*BU, 0*BU, RM_WHITE7};
			asset->tris[7] = (Tri){7.5*BU, 0*BU, 8.25*BU, 4.125*BU, 8.75*BU, 3.875*BU, RM_WHITE7};
			asset->tris[8] = (Tri){0.5*BU, 0*BU, 2.25*BU, 7.125*BU, 1*BU, 0*BU, RM_WHITE7};
			asset->tris[9] = (Tri){1*BU, 0*BU, 2.25*BU, 7.125*BU, 2.75*BU, 6.875*BU, RM_WHITE7};
			asset->tris[10] = (Tri){2.25*BU, 7.125*BU, 7.25*BU, 12.125*BU, 2.75*BU, 6.875*BU, RM_WHITE7};
			asset->tris[11] = (Tri){7.25*BU, 12.125*BU, 7.75*BU, 11.875*BU, 2.75*BU, 6.875*BU, RM_WHITE7};
			asset->tris[12] = (Tri){8.75*BU, 3.875*BU, 8.25*BU, 4.125*BU, 13.25*BU, 9.125*BU, RM_WHITE7};
			asset->tris[13] = (Tri){13.25*BU, 9.125*BU, 13.75*BU, 8.875*BU, 8.75*BU, 3.875*BU, RM_WHITE7};
			asset->tris[14] = (Tri){9*BU, 16*BU, 8.5*BU, 16*BU, 8.75*BU, 16.25*BU, RM_WHITE7};
			asset->tris[15] = (Tri){15.5*BU, 16*BU, 15*BU, 16*BU, 15.25*BU, 16.25*BU, RM_WHITE7};
			asset->tris[16] = (Tri){0.5*BU, 0*BU, 1*BU, 0*BU, 0.75*BU, -0.25*BU, RM_WHITE7};
			asset->tris[17] = (Tri){7*BU, 0*BU, 7.5*BU, 0*BU, 7.25*BU, -0.25*BU, RM_WHITE7};
			break;
		case(B2x2DR1):
			color1 = RM_ORANGE4;
		case(B2x2AR1):
			size = (Vector2){BLOCK_SIZE * 2, BLOCK_SIZE * 2};
			asset = MallocAsset(18);
			asset->tris[0] = (Tri){16*BU, 16*BU, 0*BU, 0*BU, 0*BU, 16*BU, color1};
			asset->tris[1] = (Tri){16*BU, 16*BU, 16*BU, 0*BU, 0*BU, 0*BU, color1};
			asset->tris[2] = (Tri){7*BU, 16*BU, 7.5*BU, 16*BU, 8.75*BU, 12.125*BU, RM_WHITE7};
			asset->tris[3] = (Tri){8.25*BU, 11.875*BU, 7*BU, 16*BU, 8.75*BU, 12.125*BU, RM_WHITE7};
			asset->tris[4] = (Tri){0.5*BU, 16*BU, 1*BU, 16*BU, 2.25*BU, 8.875*BU, RM_WHITE7};
			asset->tris[5] = (Tri){2.25*BU, 8.875*BU, 1*BU, 16*BU, 2.75*BU, 9.125*BU, RM_WHITE7};
			asset->tris[6] = (Tri){7.75*BU, 4.125*BU, 9*BU, 0*BU, 8.5*BU, 0*BU, RM_WHITE7};
			asset->tris[7] = (Tri){7.75*BU, 4.125*BU, 8.5*BU, 0*BU, 7.25*BU, 3.875*BU, RM_WHITE7};
			asset->tris[8] = (Tri){13.75*BU, 7.125*BU, 15.5*BU, 0*BU, 15*BU, 0*BU, RM_WHITE7};
			asset->tris[9] = (Tri){13.75*BU, 7.125*BU, 15*BU, 0*BU, 13.25*BU, 6.875*BU, RM_WHITE7};
			asset->tris[10] = (Tri){8.75*BU, 12.125*BU, 13.75*BU, 7.125*BU, 13.25*BU, 6.875*BU, RM_WHITE7};
			asset->tris[11] = (Tri){8.25*BU, 11.875*BU, 8.75*BU, 12.125*BU, 13.25*BU, 6.875*BU, RM_WHITE7};
			asset->tris[12] = (Tri){7.75*BU, 4.125*BU, 7.25*BU, 3.875*BU, 2.75*BU, 9.125*BU, RM_WHITE7};
			asset->tris[13] = (Tri){2.25*BU, 8.875*BU, 2.75*BU, 9.125*BU, 7.25*BU, 3.875*BU, RM_WHITE7};
			asset->tris[14] = (Tri){7.5*BU, 16*BU, 7*BU, 16*BU, 7.25*BU, 16.25*BU, RM_WHITE7};
			asset->tris[15] = (Tri){1*BU, 16*BU, 0.5*BU, 16*BU, 0.75*BU, 16.25*BU, RM_WHITE7};
			asset->tris[16] = (Tri){15*BU, 0*BU, 15.5*BU, 0*BU, 15.25*BU, -0.25*BU, RM_WHITE7};
			asset->tris[17] = (Tri){8.5*BU, 0*BU, 9*BU, 0*BU, 8.75*BU, -0.25*BU, RM_WHITE7};
			break;
		case(B3x2DJ1):
			color1 = RM_ORANGE4;
		case(B3x2AJ1):
			size = (Vector2){BLOCK_SIZE * 3, BLOCK_SIZE * 2};
			asset = MallocAsset(28);
			asset->tris[0] = (Tri){24*BU, 0*BU, 0*BU, 16*BU, 24*BU, 16*BU, color1};
			asset->tris[1] = (Tri){0*BU, 0*BU, 0*BU, 16*BU, 24*BU, 0*BU, color1};
			asset->tris[2] = (Tri){8.5*BU, 16*BU, 9*BU, 16*BU, 7.25*BU, 12.125*BU, RM_WHITE7};
			asset->tris[3] = (Tri){9*BU, 16*BU, 7.75*BU, 11.875*BU, 7.25*BU, 12.125*BU, RM_WHITE7};
			asset->tris[4] = (Tri){15*BU, 16*BU, 15.5*BU, 16*BU, 16.75*BU, 12.125*BU, RM_WHITE7};
			asset->tris[5] = (Tri){15*BU, 16*BU, 16.75*BU, 12.125*BU, 16.25*BU, 11.875*BU, RM_WHITE7};
			asset->tris[6] = (Tri){7*BU, 0*BU, 8.25*BU, 4.125*BU, 7.5*BU, 0*BU, RM_WHITE7};
			asset->tris[7] = (Tri){7.5*BU, 0*BU, 8.25*BU, 4.125*BU, 8.75*BU, 3.875*BU, RM_WHITE7};
			asset->tris[8] = (Tri){0.5*BU, 0*BU, 2.25*BU, 7.125*BU, 1*BU, 0*BU, RM_WHITE7};
			asset->tris[9] = (Tri){1*BU, 0*BU, 2.25*BU, 7.125*BU, 2.75*BU, 6.875*BU, RM_WHITE7};
			asset->tris[10] = (Tri){2.25*BU, 7.125*BU, 7.25*BU, 12.125*BU, 2.75*BU, 6.875*BU, RM_WHITE7};
			asset->tris[11] = (Tri){7.25*BU, 12.125*BU, 7.75*BU, 11.875*BU, 2.75*BU, 6.875*BU, RM_WHITE7};
			asset->tris[12] = (Tri){8.75*BU, 3.875*BU, 8.25*BU, 4.125*BU, 12*BU, 7.875*BU, RM_WHITE7};
			asset->tris[13] = (Tri){12*BU, 7.875*BU, 12*BU, 7.25*BU, 8.75*BU, 3.875*BU, RM_WHITE7};
			asset->tris[14] = (Tri){9*BU, 16*BU, 8.5*BU, 16*BU, 8.75*BU, 16.25*BU, RM_WHITE7};
			asset->tris[15] = (Tri){15.5*BU, 16*BU, 15*BU, 16*BU, 15.25*BU, 16.25*BU, RM_WHITE7};
			asset->tris[16] = (Tri){0.5*BU, 0*BU, 1*BU, 0*BU, 0.75*BU, -0.25*BU, RM_WHITE7};
			asset->tris[17] = (Tri){7*BU, 0*BU, 7.5*BU, 0*BU, 7.25*BU, -0.25*BU, RM_WHITE7};
			asset->tris[18] = (Tri){21.75*BU, 7.125*BU, 23.5*BU, 0*BU, 23*BU, 0*BU, RM_WHITE7};
			asset->tris[19] = (Tri){21.75*BU, 7.125*BU, 23*BU, 0*BU, 21.25*BU, 6.875*BU, RM_WHITE7};
			asset->tris[20] = (Tri){21.25*BU, 6.875*BU, 16.25*BU, 11.875*BU, 21.75*BU, 7.125*BU, RM_WHITE7};
			asset->tris[21] = (Tri){16.25*BU, 11.875*BU, 16.75*BU, 12.125*BU, 21.75*BU, 7.125*BU, RM_WHITE7};
			asset->tris[22] = (Tri){23*BU, 0*BU, 23.5*BU, 0*BU, 23.25*BU, -0.25*BU, RM_WHITE7};
			asset->tris[23] = (Tri){16.5*BU, 0*BU, 15.25*BU, 3.875*BU, 17*BU, 0*BU, RM_WHITE7};
			asset->tris[24] = (Tri){15.75*BU, 4.125*BU, 17*BU, 0*BU, 15.25*BU, 3.875*BU, RM_WHITE7};
			asset->tris[25] = (Tri){12*BU, 7.875*BU, 15.75*BU, 4.125*BU, 12*BU, 7.25*BU, RM_WHITE7};
			asset->tris[26] = (Tri){15.75*BU, 4.125*BU, 15.25*BU, 3.875*BU, 12*BU, 7.25*BU, RM_WHITE7};
			asset->tris[27] = (Tri){16.5*BU, 0*BU, 17*BU, 0*BU, 16.75*BU, -0.25*BU, RM_WHITE7};
			break;
		case(B1x1DR1):
			asset = MallocAsset(8);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, RM_ORANGE4};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, RM_ORANGE4};
			asset->tris[2] = (Tri){3*BU, 1*BU, 7*BU, 3*BU, 5*BU, 1*BU, RM_ORANGE5};
			asset->tris[3] = (Tri){7*BU, 3*BU, 3*BU, 1*BU, 1*BU, 3*BU, RM_ORANGE5};
			asset->tris[4] = (Tri){1*BU, 3*BU, 1*BU, 5*BU, 7*BU, 3*BU, RM_ORANGE5};
			asset->tris[5] = (Tri){7*BU, 3*BU, 1*BU, 5*BU, 7*BU, 5*BU, RM_ORANGE5};
			asset->tris[6] = (Tri){1*BU, 5*BU, 5*BU, 7*BU, 7*BU, 5*BU, RM_ORANGE5};
			asset->tris[7] = (Tri){5*BU, 7*BU, 1*BU, 5*BU, 3*BU, 7*BU, RM_ORANGE5};
			break;
		case(B1x1GR0):
			asset = MallocAsset(9);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, RM_GREEN5};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, RM_GREEN5};
			asset->tris[2] = (Tri){4*BU, 5.5*BU, 2.125*BU, 3.625*BU, 3*BU, 5.5*BU, RM_GREEN4};
			asset->tris[3] = (Tri){5.125*BU, 5.5*BU, 6*BU, 3.375*BU, 4*BU, 5.5*BU, RM_GREEN4};
			asset->tris[4] = (Tri){5*BU, 4.5*BU, 3*BU, 4.5*BU, 4*BU, 5.5*BU, RM_GREEN4};
			asset->tris[5] = (Tri){3.5*BU, 4.5*BU, 4.5*BU, 4.5*BU, 3.75*BU, 2.125*BU, RM_GREEN4};
			asset->tris[6] = (Tri){3.125*BU, 4.5*BU, 3.625*BU, 4.5*BU, 2.625*BU, 2.75*BU, RM_GREEN4};
			asset->tris[7] = (Tri){4.5*BU, 4.5*BU, 5*BU, 4.5*BU, 5.125*BU, 3*BU, RM_GREEN4};
			asset->tris[8] = (Tri){5.5*BU, 5.5*BU, 2.5*BU, 5.5*BU, 4*BU, 5.875*BU, RM_GREEN4};
			break;
		case(B1x1WR0):
			anim_time = (int)Wrap((float)game_time, 0.0, 4.0);
			color1 = RM_TEAL0;
			color2 = RM_TEAL1;
			color3 = RM_BLUE2;
			color4 = RM_BLUE1;
			if(anim_time == 1)
			{
				color1 = RM_BLUE1;
				color2 = RM_TEAL0;
				color3 = RM_TEAL1;
				color4 = RM_BLUE2;
			}
			else if(anim_time == 2)
			{
				color1 = RM_BLUE2;
				color2 = RM_BLUE1;
				color3 = RM_TEAL0;
				color4 = RM_TEAL1;
			}
			else if(anim_time == 3)
			{
				color1 = RM_TEAL1;
				color2 = RM_BLUE2;
				color3 = RM_BLUE1;
				color4 = RM_TEAL0;
			}
			asset = MallocAsset(36);
			asset->tris[0] = (Tri){1*BU, 2*BU, 2*BU, 0*BU, 0*BU, 0*BU, color4};
			asset->tris[1] = (Tri){3*BU, 2*BU, 4*BU, 0*BU, 2*BU, 0*BU, color3};
			asset->tris[2] = (Tri){5*BU, 2*BU, 6*BU, 0*BU, 4*BU, 0*BU, color2};
			asset->tris[3] = (Tri){7*BU, 2*BU, 8*BU, 0*BU, 6*BU, 0*BU, color1};
			asset->tris[4] = (Tri){6*BU, 4*BU, 7*BU, 2*BU, 5*BU, 2*BU, color2};
			asset->tris[5] = (Tri){4*BU, 4*BU, 5*BU, 2*BU, 3*BU, 2*BU, color3};
			asset->tris[6] = (Tri){2*BU, 4*BU, 3*BU, 2*BU, 1*BU, 2*BU, color4};
			asset->tris[7] = (Tri){1*BU, 6*BU, 2*BU, 4*BU, 0*BU, 4*BU, color1};
			asset->tris[8] = (Tri){3*BU, 6*BU, 4*BU, 4*BU, 2*BU, 4*BU, color4};
			asset->tris[9] = (Tri){5*BU, 6*BU, 6*BU, 4*BU, 4*BU, 4*BU, color3};
			asset->tris[10] = (Tri){7*BU, 6*BU, 8*BU, 4*BU, 6*BU, 4*BU, color2};
			asset->tris[11] = (Tri){6*BU, 8*BU, 7*BU, 6*BU, 5*BU, 6*BU, color3};
			asset->tris[12] = (Tri){4*BU, 8*BU, 5*BU, 6*BU, 3*BU, 6*BU, color4};
			asset->tris[13] = (Tri){2*BU, 8*BU, 3*BU, 6*BU, 1*BU, 6*BU, color1};
			asset->tris[14] = (Tri){3*BU, 2*BU, 2*BU, 0*BU, 1*BU, 2*BU, color1};
			asset->tris[15] = (Tri){5*BU, 2*BU, 4*BU, 0*BU, 3*BU, 2*BU, color4};
			asset->tris[16] = (Tri){7*BU, 2*BU, 6*BU, 0*BU, 5*BU, 2*BU, color3};
			asset->tris[17] = (Tri){8*BU, 4*BU, 7*BU, 2*BU, 6*BU, 4*BU, color3};
			asset->tris[18] = (Tri){6*BU, 4*BU, 5*BU, 2*BU, 4*BU, 4*BU, color4};
			asset->tris[19] = (Tri){4*BU, 4*BU, 3*BU, 2*BU, 2*BU, 4*BU, color1};
			asset->tris[20] = (Tri){2*BU, 4*BU, 1*BU, 2*BU, 0*BU, 4*BU, color2};
			asset->tris[21] = (Tri){3*BU, 6*BU, 2*BU, 4*BU, 1*BU, 6*BU, color2};
			asset->tris[22] = (Tri){5*BU, 6*BU, 4*BU, 4*BU, 3*BU, 6*BU, color1};
			asset->tris[23] = (Tri){7*BU, 6*BU, 6*BU, 4*BU, 5*BU, 6*BU, color4};
			asset->tris[24] = (Tri){8*BU, 8*BU, 7*BU, 6*BU, 6*BU, 8*BU, color4};
			asset->tris[25] = (Tri){6*BU, 8*BU, 5*BU, 6*BU, 4*BU, 8*BU, color1};
			asset->tris[26] = (Tri){4*BU, 8*BU, 3*BU, 6*BU, 2*BU, 8*BU, color2};
			asset->tris[27] = (Tri){2*BU, 8*BU, 1*BU, 6*BU, 0*BU, 8*BU, color3};
			asset->tris[28] = (Tri){1*BU, 6*BU, 0*BU, 4*BU, 0*BU, 6*BU, color3};
			asset->tris[29] = (Tri){1*BU, 2*BU, 0*BU, 0*BU, 0*BU, 2*BU, color2};
			asset->tris[30] = (Tri){8*BU, 0*BU, 7*BU, 2*BU, 8*BU, 2*BU, color2};
			asset->tris[31] = (Tri){8*BU, 4*BU, 7*BU, 6*BU, 8*BU, 6*BU, color3};
			asset->tris[32] = (Tri){8*BU, 6*BU, 7*BU, 6*BU, 8*BU, 8*BU, color2};
			asset->tris[33] = (Tri){8*BU, 2*BU, 7*BU, 2*BU, 8*BU, 4*BU, color1};
			asset->tris[34] = (Tri){1*BU, 2*BU, 0*BU, 2*BU, 0*BU, 4*BU, color1};
			asset->tris[35] = (Tri){1*BU, 6*BU, 0*BU, 6*BU, 0*BU, 8*BU, color2};
			break;
		case(B1x1AR1):
			asset = MallocAsset(2);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, RM_WHITE4};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, RM_WHITE4};
			break;
		case(B1x1AT1):
			asset = MallocAsset(11);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, RM_WHITE4};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, RM_WHITE4};
			asset->tris[2] = (Tri){7*BU, 8*BU, 7.5*BU, 8*BU, 6.5*BU, 4.5*BU, RM_WHITE7};
			asset->tris[3] = (Tri){0*BU, 0.5*BU, 0*BU, 1*BU, 3.5*BU, 1.5*BU, RM_WHITE7};
			asset->tris[4] = (Tri){3.5*BU, 1.5*BU, 6.5*BU, 4.5*BU, 5.375*BU, 2.625*BU, RM_WHITE7};
			asset->tris[5] = (Tri){0*BU, 0.5*BU, 3.5*BU, 1.5*BU, 3.75*BU, 1*BU, RM_WHITE7};
			asset->tris[6] = (Tri){5.375*BU, 2.625*BU, 3.75*BU, 1*BU, 3.5*BU, 1.5*BU, RM_WHITE7};
			asset->tris[7] = (Tri){7.5*BU, 8*BU, 7*BU, 4.25*BU, 6.5*BU, 4.5*BU, RM_WHITE7};
			asset->tris[8] = (Tri){7*BU, 4.25*BU, 5.375*BU, 2.625*BU, 6.5*BU, 4.5*BU, RM_WHITE7};
			asset->tris[9] = (Tri){0*BU, 1*BU, 0*BU, 0.5*BU, -0.25*BU, 0.75*BU, RM_WHITE7};
			asset->tris[10] = (Tri){7*BU, 8*BU, 7.25*BU, 8.25*BU, 7.5*BU, 8*BU, RM_WHITE7};
			break;
		case(B1x1AR2):
			asset = MallocAsset(6);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, RM_WHITE4};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, RM_WHITE4};
			asset->tris[2] = (Tri){0.5*BU, 0*BU, 1*BU, 0*BU, 0.75*BU, -0.25*BU, RM_WHITE7};
			asset->tris[3] = (Tri){0.5*BU, 0*BU, 1*BU, 8*BU, 1*BU, 0*BU, RM_WHITE7};
			asset->tris[4] = (Tri){0.5*BU, 0*BU, 0.5*BU, 8*BU, 1*BU, 8*BU, RM_WHITE7};
			asset->tris[5] = (Tri){0.5*BU, 8*BU, 0.75*BU, 8.25*BU, 1*BU, 8*BU, RM_WHITE7};
			break;
		case(B2x2AT1):
			size = (Vector2){BLOCK_SIZE * 2, BLOCK_SIZE * 2};
			asset = MallocAsset(10);
			asset->tris[0] = (Tri){0*BU, 16*BU, 16*BU, 16*BU, 16*BU, 0*BU, RM_WHITE4};
			asset->tris[1] = (Tri){0*BU, 0*BU, 0*BU, 16*BU, 16*BU, 0*BU, RM_WHITE4};
			asset->tris[2] = (Tri){15*BU, 16*BU, 15.5*BU, 16*BU, 14*BU, 8.75*BU, RM_WHITE7};
			asset->tris[3] = (Tri){15*BU, 16*BU, 14*BU, 8.75*BU, 13.5*BU, 9*BU, RM_WHITE7};
			asset->tris[4] = (Tri){14*BU, 8.75*BU, 7.25*BU, 2*BU, 7*BU, 2.5*BU, RM_WHITE7};
			asset->tris[5] = (Tri){13.5*BU, 9*BU, 14*BU, 8.75*BU, 7*BU, 2.5*BU, RM_WHITE7};
			asset->tris[6] = (Tri){7.25*BU, 2*BU, 0*BU, 0.5*BU, 0*BU, 1*BU, RM_WHITE7};
			asset->tris[7] = (Tri){0*BU, 1*BU, 7*BU, 2.5*BU, 7.25*BU, 2*BU, RM_WHITE7};
			asset->tris[8] = (Tri){0*BU, 1*BU, 0*BU, 0.5*BU, -0.25*BU, 0.75*BU, RM_WHITE7};
			asset->tris[9] = (Tri){15*BU, 16*BU, 15.25*BU, 16.25*BU, 15.5*BU, 16*BU, RM_WHITE7};
			break;
		case(B1x1AT2):
			asset = MallocAsset(4);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, RM_WHITE4};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, RM_WHITE4};
			asset->tris[2] = (Tri){-0.25*BU, 7.25*BU, 0.75*BU, 8.25*BU, 1*BU, 8*BU, RM_WHITE7};
			asset->tris[3] = (Tri){-0.25*BU, 7.25*BU, 1*BU, 8*BU, 0*BU, 7*BU, RM_WHITE7};
			break;
		case(B1x1AR3):
			asset = MallocAsset(6);
			asset->tris[0] = (Tri){0*BU, 0*BU, 0*BU, 8*BU, 8*BU, 8*BU, RM_WHITE4};
			asset->tris[1] = (Tri){0*BU, 0*BU, 8*BU, 8*BU, 8*BU, 0*BU, RM_WHITE4};
			asset->tris[2] = (Tri){-0.25*BU, 7.25*BU, 0.75*BU, 8.25*BU, 1*BU, 8*BU, RM_WHITE7};
			asset->tris[3] = (Tri){-0.25*BU, 7.25*BU, 1*BU, 8*BU, 0*BU, 7*BU, RM_WHITE7};
			asset->tris[4] = (Tri){-0.25*BU, 0.75*BU, 1*BU, 0*BU, 0.75*BU, -0.25*BU, RM_WHITE7};
			asset->tris[5] = (Tri){-0.25*BU, 0.75*BU, 0*BU, 1*BU, 1*BU, 0*BU, RM_WHITE7};
			break;
		case(B1x1BR1):
			anim_time = (int)Wrap((float)game_time, 0.0, 3.0);
			color1 = RM_CYAN0;
			color2 = RM_BLUE1;
			color3 = RM_MAGENTA2;
			if(anim_time == 1)
			{
				color1 = RM_MAGENTA2;
				color2 = RM_CYAN0;
				color3 = RM_BLUE1;
			}
			if(anim_time == 2)
			{
				color1 = RM_BLUE1;
				color2 = RM_MAGENTA2;
				color3 = RM_CYAN0;
			}
			asset = MallocAsset(16);
			asset->tris[0] = (Tri){0*BU, 8*BU, 8*BU, 8*BU, 8*BU, 7*BU, color1};
			asset->tris[1] = (Tri){0*BU, 7*BU, 0*BU, 8*BU, 8*BU, 7*BU, color2};
			asset->tris[2] = (Tri){0*BU, 7*BU, 8*BU, 7*BU, 8*BU, 6*BU, color2};
			asset->tris[3] = (Tri){0*BU, 7*BU, 8*BU, 6*BU, 0*BU, 6*BU, color3};
			asset->tris[4] = (Tri){0*BU, 6*BU, 8*BU, 6*BU, 8*BU, 5*BU, color3};
			asset->tris[5] = (Tri){0*BU, 6*BU, 8*BU, 5*BU, 0*BU, 5*BU, color1};
			asset->tris[6] = (Tri){0*BU, 5*BU, 8*BU, 5*BU, 8*BU, 4*BU, color1};
			asset->tris[7] = (Tri){8*BU, 4*BU, 0*BU, 4*BU, 0*BU, 5*BU, color2};
			asset->tris[8] = (Tri){0*BU, 4*BU, 8*BU, 3*BU, 0*BU, 3*BU, color3};
			asset->tris[9] = (Tri){0*BU, 4*BU, 8*BU, 4*BU, 8*BU, 3*BU, color2};
			asset->tris[10] = (Tri){0*BU, 3*BU, 8*BU, 3*BU, 8*BU, 2*BU, color3};
			asset->tris[11] = (Tri){0*BU, 3*BU, 8*BU, 2*BU, 0*BU, 2*BU, color1};
			asset->tris[12] = (Tri){0*BU, 2*BU, 8*BU, 2*BU, 8*BU, 1*BU, color1};
			asset->tris[13] = (Tri){8*BU, 1*BU, 0*BU, 1*BU, 0*BU, 2*BU, color2};
			asset->tris[14] = (Tri){0*BU, 1*BU, 8*BU, 1*BU, 8*BU, 0*BU, color2};
			asset->tris[15] = (Tri){0*BU, 1*BU, 8*BU, 0*BU, 0*BU, 0*BU, color3};
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

void PaintAsset(Asset* asset, Palette palette)
{
	for(int i = 0; i < asset->tri_amount; i++)
	{
		Tri tri = asset->tris[i];
		if(tri.color < CAR_COLOR_AMOUNT)
		{
			tri.color = palette.colors[tri.color];
		}
		asset->tris[i] = tri;
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

Color WaterColor(Vector2 pos, double game_time, Color wave, Color water)
{
	float color_step = AirQuotesNoise(pos.x * 0.005 + pos.y * 0.001 + game_time * 0.3, false) * 0.7 + pos.x * 0.001 - pos.y * 0.003 + game_time * 0.25;
	color_step = Wrap(color_step, 0.0, 2.0);
	if(color_step > 1.0)
	{
		color_step = 2.0 - color_step;
	}
	return ColorLerp(water, wave, color_step);
	/*
	(Color){
		Lerp(water.r, wave.r, color_step),
		Lerp(water.g, wave.g, color_step),
		Lerp(water.b, wave.b, color_step),
		255
	};
	*/
}

void DrawBackgroundWater(Vector2 position, float zoom, double game_time)
{
	DrawBackgroundTris(position, zoom, game_time, RM_TEAL0, RM_BLUE2);
}

void DrawBackgroundTris(Vector2 position, float zoom, double game_time, char wave, char base)
{
	Vector2 offset = (Vector2){Wrap(position.x, 0, 64), Wrap(position.y, 0, 128)};
	Vector2 points = (Vector2){17 / zoom * .5 + 2, 11 / zoom * .5 + 2};
	for(int y = -points.y; y < points.y; y++)
	{
		for(int x = -points.x; x < points.x; x++)
		{
			Vector2 pos_a = (Vector2){x * 64, y * 64 + 64};
			if(y & 1)
			{
				pos_a.x += 32;
			}
			pos_a = Vector2Subtract(pos_a, offset);

			Vector2 pos_b = pos_a;
			pos_b.x += 64;
			Vector2 pos_c_top = pos_a;
			pos_c_top.x += 32;
			pos_c_top.y -= 64;
			Vector2 pos_c_bot = pos_a;
			pos_c_bot.x += 32;
			pos_c_bot.y += 64;

			Color color_top = WaterColor(Vector2Add(pos_a, position), game_time, rmc(wave), rmc(base));
			Color color_bot = WaterColor(Vector2Add(pos_b, position), game_time, rmc(wave), rmc(base));

			pos_a = Vector2Scale(pos_a, zoom);
			pos_a = Vector2Add(pos_a, SCREEN_CENTER);

			pos_b = Vector2Scale(pos_b, zoom);
			pos_b = Vector2Add(pos_b, SCREEN_CENTER);

			pos_c_top = Vector2Scale(pos_c_top, zoom);
			pos_c_top = Vector2Add(pos_c_top, SCREEN_CENTER);

			pos_c_bot = Vector2Scale(pos_c_bot, zoom);
			pos_c_bot = Vector2Add(pos_c_bot, SCREEN_CENTER);

			DrawTriangle(pos_a, pos_b, pos_c_top, color_top);
			DrawTriangle(pos_a, pos_c_bot, pos_b, color_bot);
			//DrawPixel(pos_a.x, pos_a.y, BLACK);
		}
	}
}

void DrawBackgroundVoid(Vector2 position, float zoom, double game_time)
{
	ClearBackground(rmc(RM_WHITE8));
	Tri tris[18];
	float wgt = Wrap(game_time * 0.1, 0.0, 1.0);
	position = Vector2Scale(position, 0.1);
	for(int i = 0; i < 9; i++)
	{
		int i1 = i * 2;
		int i2 = i1 + 1;
		float size = 8.0 - i + wgt;
		Vector2 pos = Vector2Subtract(position, Vector2Scale(position, .125 * (1.0 - wgt)));

		tris[i1].a = (Vector2){0, 320 * size};
		tris[i1].b = (Vector2){320 * size, 0};
		tris[i1].c = (Vector2){-320 * size, 0};
		tris[i1].a = Vector2Subtract(tris[i1].a, pos);
		tris[i1].b = Vector2Subtract(tris[i1].b, pos);
		tris[i1].c = Vector2Subtract(tris[i1].c, pos);
		tris[i1].a = Vector2Scale(tris[i1].a, zoom);
		tris[i1].b = Vector2Scale(tris[i1].b, zoom);
		tris[i1].c = Vector2Scale(tris[i1].c, zoom);
		tris[i1].a = Vector2Add(tris[i1].a, SCREEN_CENTER);
		tris[i1].b = Vector2Add(tris[i1].b, SCREEN_CENTER);
		tris[i1].c = Vector2Add(tris[i1].c, SCREEN_CENTER);
		tris[i1].color = RM_WHITE8 - i;

		tris[i2].a = (Vector2){0, -320 * size};
		tris[i2].b = (Vector2){-320 * size, 0};
		tris[i2].c = (Vector2){320 * size, 0};
		tris[i2].a = Vector2Subtract(tris[i2].a, pos);
		tris[i2].b = Vector2Subtract(tris[i2].b, pos);
		tris[i2].c = Vector2Subtract(tris[i2].c, pos);
		tris[i2].a = Vector2Scale(tris[i2].a, zoom);
		tris[i2].b = Vector2Scale(tris[i2].b, zoom);
		tris[i2].c = Vector2Scale(tris[i2].c, zoom);
		tris[i2].a = Vector2Add(tris[i2].a, SCREEN_CENTER);
		tris[i2].b = Vector2Add(tris[i2].b, SCREEN_CENTER);
		tris[i2].c = Vector2Add(tris[i2].c, SCREEN_CENTER);
		tris[i2].color = RM_WHITE8 - i;

		position = Vector2Subtract(position, Vector2Scale(position, .125));
	}
	for(int i = 0; i < 16; i++)
	{
		DrawTriangle(tris[i].a, tris[i].b, tris[i].c, ColorLerp(rmc(tris[i].color - 1), rmc(tris[i].color), wgt));
	}
	for(int i = 16; i < 18; i++)
	{
		DrawTriangle(tris[i].a, tris[i].b, tris[i].c, rmc(tris[i].color));
	}
}
