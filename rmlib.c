#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"

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

bool zero(float x)
{
	return x == .0 || x == -.0;
}

int absi(int x)
{
	return (x < 0 ? -x : x);
}

int sign(float x)
{
	return (x > 0) - (x < 0);
}

float absf(float x)
{
	return (x < 0.0 ? -x : x);
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

Vector2 Vector2Swap(Vector2 vector)
{
	float t = vector.x;
	vector.x = vector.y;
	vector.y = t;
	return vector;
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

Vector2int Vector2intSubtract(Vector2int a, Vector2int b)
{
	return (Vector2int){a.x - b.x, a.y - b.y};
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

Color rmc(char i)
{
	// TODO: Replace with custom colors
	switch(i)
	{
		case(0):
			return LIGHTGRAY;
		case(1):
			return GRAY;
		case(2):
			return DARKGRAY;
		case(3):
			return YELLOW;
		case(4):
			return GOLD;
		case(5):
			return ORANGE;
		case(6):
			return PINK;
		case(7):
			return RED;
		case(8):
			return MAROON;
		case(9):
			return GREEN;
		case(10):
			return LIME;
		case(11):
			return DARKGREEN;
		case(12):
			return SKYBLUE;
		case(13):
			return BLUE;
		case(14):
			return DARKBLUE;
		case(15):
			return PURPLE;
		case(16):
			return VIOLET;
		case(17):
			return DARKPURPLE;
		case(18):
			return BEIGE;
		case(19):
			return BROWN;
		case(20):
			return DARKBROWN;
		case(21):
			return WHITE;
		case(22):
			return BLACK;
		case(23):
			return MAGENTA;
		case(24):
			return RAYWHITE;
		// WHITE
		case(25): return (Color){255, 249, 243, 255};
		case(26): return (Color){220, 214, 209, 255};
		case(27): return (Color){188, 181, 176, 255};
		case(28): return (Color){155, 149, 144, 255};
		case(29): return (Color){125, 118, 113, 255};
		case(30): return (Color){98, 89, 84, 255};
		case(31): return (Color){74, 62, 56, 255};
		case(32): return (Color){58, 42, 33, 255};
		case(33): return (Color){39, 26, 18, 255};
		// RED
		case(34): return (Color){230, 185, 184, 255};
		case(35): return (Color){237, 140, 138, 255};
		case(36): return (Color){246, 76, 76, 255};
		case(37): return (Color){248, 18, 18, 255};
		case(38): return (Color){182, 20, 12, 255};
		case(39): return (Color){122, 21, 7, 255};
		case(40): return (Color){73, 22, 5, 255};
		// YELLOW
		case(41): return (Color){228, 233, 184, 255};
		case(42): return (Color){233, 225, 138, 255};
		case(43): return (Color){242, 217, 75, 255};
		case(44): return (Color){244, 214, 15, 255};
		case(45): return (Color){180, 156, 10, 255};
		case(46): return (Color){120, 101, 6, 255};
		case(47): return (Color){72, 54, 5, 255};
		// GREEN
		case(48): return (Color){161, 247, 184, 255};
		case(49): return (Color){124, 247, 138, 255};
		case(50): return (Color){91, 244, 76, 255};
		case(51): return (Color){71, 244, 18, 255};
		case(52): return (Color){60, 178, 12, 255};
		case(53): return (Color){52, 116, 7, 255};
		case(54): return (Color){47, 61, 5, 255};
		// CYAN
		case(55): return (Color){157, 250, 227, 255};
		case(56): return (Color){116, 250, 216, 255};
		case(57): return (Color){71, 249, 206, 255};
		case(58): return (Color){38, 249, 202, 255};
		case(59): return (Color){42, 181, 146, 255};
		case(60): return (Color){45, 118, 93, 255};
		case(61): return (Color){46, 62, 45, 255};
		// NAVY
		case(62): return (Color){159, 185, 248, 255};
		case(63): return (Color){119, 140, 248, 255};
		case(64): return (Color){80, 75, 247, 255};
		case(65): return (Color){55, 13, 247, 255};
		case(66): return (Color){51, 18, 179, 255};
		case(67): return (Color){48, 21, 115, 255};
		case(68): return (Color){46, 22, 57, 255};
		// PINK
		case(69): return (Color){228, 185, 225, 255};
		case(70): return (Color){235, 140, 212, 255};
		case(71): return (Color){243, 77, 201, 255};
		case(72): return (Color){245, 20, 197, 255};
		case(73): return (Color){180, 21, 142, 255};
		case(74): return (Color){121, 22, 91, 255};
		case(75): return (Color){72, 22, 44, 255};
		// ORANGE
		case(76): return (Color){227, 203, 184, 255};
		case(77): return (Color){232, 174, 139, 255};
		case(78): return (Color){241, 146, 77, 255};
		case(79): return (Color){243, 133, 23, 255};
		case(80): return (Color){179, 96, 15, 255};
		case(81): return (Color){120, 63, 8, 255};
		case(82): return (Color){72, 36, 5, 255};
		// LIME
		case(83): return (Color){190, 244, 184, 255};
		case(84): return (Color){174, 242, 139, 255};
		case(85): return (Color){171, 239, 80, 255};
		case(86): return (Color){167, 238, 32, 255};
		case(87): return (Color){124, 173, 21, 255};
		case(88): return (Color){86, 113, 12, 255};
		case(89): return (Color){58, 60, 6, 255};
		// TEAL
		case(90): return (Color){155, 252, 201, 255};
		case(91): return (Color){112, 255, 171, 255};
		case(92): return (Color){60, 255, 141, 255};
		case(93): return (Color){0, 255, 128, 255};
		case(94): return (Color){31, 186, 91, 255};
		case(95): return (Color){41, 121, 57, 255};
		case(96): return (Color){45, 64, 26, 255};
		// BLUE
		case(97): return (Color){157, 207, 247, 255};
		case(98): return (Color){118, 181, 247, 255};
		case(99): return (Color){74, 156, 245, 255};
		case(100): return (Color){44, 145, 245, 255};
		case(101): return (Color){45, 105, 178, 255};
		case(102): return (Color){46, 69, 114, 255};
		case(103): return (Color){46, 39, 57, 255};
		// PURPLE
		case(104): return (Color){190, 185, 246, 255};
		case(105): return (Color){173, 139, 246, 255};
		case(106): return (Color){170, 77, 243, 255};
		case(107): return (Color){166, 20, 243, 255};
		case(108): return (Color){123, 21, 176, 255};
		case(109): return (Color){86, 22, 113, 255};
		case(110): return (Color){58, 22, 56, 255};
		// MAGENTA
		case(111): return (Color){224, 186, 196, 255};
		case(112): return (Color){228, 141, 162, 255};
		case(113): return (Color){235, 78, 125, 255};
		case(114): return (Color){237, 24, 108, 255};
		case(115): return (Color){174, 23, 77, 255};
		case(116): return (Color){117, 22, 47, 255};
		case(117): return (Color){71, 22, 21, 255};
	}
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
	
	//DrawText("Exit - Backspace", POSITION.x + SIZE.x - 182, POSITION.y + SIZE.y - 44, 32, BLACK);
	int page = current / FILE_LIST_PAGE_ITEMS;
	DrawText(TextFormat("%i/%i", page + 1, fpl.count / FILE_LIST_PAGE_ITEMS + 1), POSITION.x + 24, POSITION.y + SIZE.y - 44, 32, BLACK);

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
