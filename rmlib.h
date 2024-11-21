#ifndef RMLIB
#define RMLIB

#include <raylib.h>
#include <raymath.h>

#define DEBUG false

#define SCREEN_SIZE (Vector2){1024, 640}
#define SCREEN_CENTER (Vector2){512, 320}

#define FRAME 0.016
#define INV_PI 1.0/PI
#define UP_VECTOR (Vector2){0.0, -1.0}
#define PNULL (void*)0

#define INPUT_BLOCK 0.05
#define INPUT_BLOCK_START 0.4

#define FILE_LIST_PAGE_ITEMS 11

typedef enum InputKeys {INPUT_LEFT, INPUT_RIGHT, INPUT_UP, INPUT_DOWN, INPUT_BACK, INPUT_ENTER, INPUT_ESC, INPUT_R} InputKeys;

typedef struct Vector2int
{
	int x, y;
} Vector2int;

typedef struct RMInput
{
	unsigned char current, past;	
} RMInput;


void RaylibLogo(void);
void TabinLogo(void);

int sign(float x);
float absf(float x);
float min(float x, float y);
float max(float x, float y);
float AirQuotesNoise(float point, bool flipped);

Vector2 Vector2Sign(Vector2 vector);
Vector2 InvertAroundPoint(Vector2 position, Vector2 point);

bool Vector2intEqual(Vector2int a, Vector2int b);

void CheckKeyboardInput(RMInput* input);
bool InputHeld(RMInput input, int key);
bool InputPressed(RMInput input, int key);
void CheckMenuInput(RMInput* menu, RMInput input, float* block);

void DrawUnitVector(Vector2 position, Vector2 magnitude, Color color);

unsigned int TextFindLastChar(const char* text, unsigned char ch);

void MoveFileListCursor(unsigned int count, int* current, int move);
void DrawFileList(FilePathList fpl, int current, Color bg1, Color bg2);
void ChangeToDirectory(unsigned char* dir, unsigned char* new_dir, bool overwrite);
void ReturnToParentDirectory(unsigned char* dir);

#endif
