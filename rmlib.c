#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"

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

}

extern void* _malloc(size_t size)
{
	return malloc(size);
}

extern void* _realloc(void* ptr, size_t size)
{
	return realloc(ptr, size);
}

extern void _free(void* ptr)
{
	return free(ptr);
}

int sign(float x)
{
	return (x > 0) - (x < 0);
}

float absf(float x)
{
	return (x < 0 ? -x : x);
}

float min(float x, float y)
{
	return (x < y ? x : y);
}

float max(float x, float y)
{
	return (x > y ? x : y);
}

float AirQuotesNoise(float point, bool flipped)
{
	float noise[40] = {
		0.0233, 0.9872, 0.2257, 0.7326, 0.4527, 0.6693, 0.8916, 0.1858, 0.3675, 0.7854, 0.2912, 0.5524, 0.1424, 0.9506, 0.3139, 0.5030, 0.4038, 0.8412, 0.0680, 0.6278,
		0.8749, 0.5969, 0.4746, 0.9650, 0.5380, 0.2637, 0.1428, 0.3597, 0.7221, 0.0592, 0.6037, 0.1910, 0.9128, 0.2235, 0.7610, 0.0137, 0.6427, 0.8439, 0.3824, 0.4225,
	};
	int min_x = (int)point;
	float t = point - min_x;
	int max_x = (min_x + 1) % 40;
	min_x %= 40;
	if(flipped)
	{
		if(min_x % 2 == 1)
		{
			min_x -= 1;
			max_x += 1;
		}
		else
		{
			min_x += 1;
			max_x -= 1;
		}
	}
	float result = Lerp(noise[min_x], noise[max_x], t);
	if(flipped) result = 1 - result;
	return result;
}

Vector2 Vector2Sign(Vector2 vector)
{
	return (Vector2){sign(vector.x), sign(vector.y)};
}

Vector2 InvertAroundPoint(Vector2 position, Vector2 point)
{
	Vector2 dif = Vector2Subtract(point, position);
	return Vector2Add(point, dif);
}

bool Vector2intEqual(Vector2int a, Vector2int b)
{
	return a.x == b.x && a.y == b.y;
}

void CheckKeyboardInput(RMInput* input)
{
	input->past = input->current;
	input->current = 0;

	input->current += (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) << INPUT_LEFT;
	input->current += (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) << INPUT_RIGHT;
	input->current += (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) << INPUT_UP;
	input->current += (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) << INPUT_DOWN;
	input->current += IsKeyDown(KEY_BACKSPACE) << INPUT_BACK;
	input->current += IsKeyDown(KEY_ENTER) << INPUT_ENTER;
	input->current += IsKeyDown(KEY_ESCAPE) << INPUT_ESC;
	input->current += IsKeyDown(KEY_R) << INPUT_R;
}

bool InputOn(unsigned char input, int key)
{
	return input & 0b1 << key;
}

bool InputHeld(RMInput input, int key)
{
	return InputOn(input.current, key);
}

bool InputPressed(RMInput input, int key)
{
	return InputOn(input.current, key) && !InputOn(input.past, key);
}

bool DirectionMenuInput(RMInput* menu, RMInput input, float* block, int key)
{
	bool success = false;
	if(InputPressed(input, key)) 
	{
		menu->current += (0b1 << key);
		*block = INPUT_BLOCK_START;
		success = true;
	}
	else if(InputHeld(input, key) && *block <= 0.0)
	{
		menu->current += (0b1 << key);
		*block = INPUT_BLOCK;
		success = true;
	}
	return success;
}

void ActionMenuInput(RMInput* menu, RMInput input, bool direction_pressed, int key)
{
	if(InputPressed(input, key) || (InputHeld(input, key) && direction_pressed))
	{
		menu->current += (0b1 << key);
	}
}

void CheckMenuInput(RMInput* menu, RMInput input, float* block)
{
	if(*block > 0)
	{
		*block -= FRAME;
	}

	bool dir_pressed = false;

	dir_pressed = dir_pressed || DirectionMenuInput(menu, input, block, INPUT_LEFT);
	dir_pressed = dir_pressed || DirectionMenuInput(menu, input, block, INPUT_RIGHT);
	dir_pressed = dir_pressed || DirectionMenuInput(menu, input, block, INPUT_UP);
	dir_pressed = dir_pressed || DirectionMenuInput(menu, input, block, INPUT_DOWN);

	ActionMenuInput(menu, input, dir_pressed, INPUT_ENTER);
	ActionMenuInput(menu, input, dir_pressed, INPUT_BACK);

	menu->current += (InputPressed(input, INPUT_ESC) << INPUT_ESC);
	menu->current += (InputPressed(input, INPUT_R) << INPUT_R);
}

void DrawUnitVector(Vector2 position, Vector2 magnitude, Color color)
{
	DrawLine(position.x, position.y, position.x + magnitude.x * 10, position.y + magnitude.y * 10, color);
}

unsigned int TextFindLastChar(const char* text, unsigned char ch)
{
	unsigned int i = 0, pos = 0;
	while(text[i] != '\0')
	{
		if(text[i] == ch)
		{
			pos = i;
		}
		i++;
	}
	return pos;
}

void MoveFileListCursor(unsigned int count, int* current, int move)
{
	*current = *current + move;
	if(*current < 0) 
	{
		*current = 0;
	}
	if(*current >= count)
	{
		*current = count - 1;
	}
}

void DrawFileList(FilePathList fpl, int current, Color bg1, Color bg2)
{
	const Vector2 SIZE = (Vector2){512, 448};
	const Vector2 POSITION = (Vector2){256, 160};

	DrawRectangle(POSITION.x - 4, POSITION.y - 4, SIZE.x + 8, SIZE.y + 8, BLACK);
	DrawRectangle(POSITION.x, POSITION.y, SIZE.x, SIZE.y, bg1); // GREEN
	DrawRectangle(POSITION.x + 24, POSITION.y + 24, SIZE.x - 48, SIZE.y - 80, bg2); // LIME

	int page = current / FILE_LIST_PAGE_ITEMS;

	for(int i = 0; i < FILE_LIST_PAGE_ITEMS; i++)
	{
		int item = i + page * FILE_LIST_PAGE_ITEMS;
		if(item >= fpl.count)
		{
			break;
		}
		unsigned char* filepath = fpl.paths[item];
		unsigned int pos = TextFindLastChar(filepath, '/'), len = TextLength(filepath);
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

void ChangeToDirectory(unsigned char* dir, unsigned char* new_dir, bool overwrite)
{
	if(overwrite)
	{
		TraceLog(LOG_INFO, "RL_FREE: change dir");
		RL_FREE(dir);
		TraceLog(LOG_INFO, "RL_MALLOC: change dir");
		dir = (unsigned char*)RL_MALLOC(TextLength(new_dir));
		TextCopy(dir, new_dir);
	}
	else
	{
		if(!DirectoryExists(TextFormat("%s/%s", dir, new_dir)))
		{
			return;
		}
		TextCopy(dir, TextFormat("%s/%s", dir, new_dir));
	}
}

void ReturnToParentDirectory(unsigned char* dir)
{
	unsigned int pos = TextFindLastChar(dir, '/');
	if(pos == 0)
	{
		return;
	}
	TraceLog(LOG_INFO, "RL_REALLOC: return dir");
	dir = (unsigned char*)RL_REALLOC(dir, pos + 1);
	dir[pos] = '\0';
}
