#ifndef RMLIB
#define RMLIB

#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>

#define DEBUG true

#define SCREEN_SIZE (Vector2){1024, 640}
#define SCREEN_CENTER (Vector2){512, 320}

#define FRAME 0.016
#define INV_PI 1.0/PI
#define UP_VECTOR (Vector2){0.0, -1.0}
#define PNULL (void*)0

#if defined(_WIN32) || defined(WIN32)
#define WINDOWS
#endif

#ifdef __unix__
#define LINUX
#endif

#define INPUT_BLOCK 0.05
#define INPUT_BLOCK_START 0.4

#define BLOCK_SIZE 256
#define INV_BLOCK_SIZE 1/BLOCK_SIZE
#define BU 0.125*BLOCK_SIZE


typedef enum InputKeys {INPUT_LEFT, INPUT_RIGHT, INPUT_UP, INPUT_DOWN, INPUT_BACK, INPUT_ENTER, INPUT_ESC, INPUT_R} InputKeys;

typedef enum BlockRotation {ROT_NORTH = 0, ROT_EAST, ROT_SOUTH, ROT_WEST} BlockRotation;

typedef enum RMColors {
	RL_GRAY0, RL_GRAY1, RL_GRAY2, RL_YELLOW0, RL_YELLOW1, RL_YELLOW2, RL_RED0, RL_RED1, RL_RED2,
	RL_GREEN0, RL_GREEN1, RL_GREEN2, RL_BLUE0, RL_BLUE1, RL_BLUE2, RL_PURPLE0, RL_PURPLE1, RL_PURPLE2,
	RL_BROWN0, RL_BROWN1, RL_BROWN2, RL_WHITE, RL_BLACK, RL_MAGENTA, RL_RAYWHITE,
	RM_WHITE0, RM_WHITE1, RM_WHITE2, RM_WHITE3, RM_WHITE4, RM_WHITE5, RM_WHITE6, RM_WHITE7, RM_WHITE8,
	RM_RED0, RM_RED1, RM_RED2, RM_RED3, RM_RED4, RM_RED5, RM_RED6,
	RM_YELLOW0, RM_YELLOW1, RM_YELLOW2, RM_YELLOW3, RM_YELLOW4, RM_YELLOW5, RM_YELLOW6,
	RM_GREEN0, RM_GREEN1, RM_GREEN2, RM_GREEN3, RM_GREEN4, RM_GREEN5, RM_GREEN6,
	RM_CYAN0, RM_CYAN1, RM_CYAN2, RM_CYAN3, RM_CYAN4, RM_CYAN5, RM_CYAN6,
	RM_NAVY0, RM_NAVY1, RM_NAVY2, RM_NAVY3, RM_NAVY4, RM_NAVY5, RM_NAVY6,
	RM_PINK0, RM_PINK1, RM_PINK2, RM_PINK3, RM_PINK4, RM_PINK5, RM_PINK6,
	RM_ORANGE0, RM_ORANGE1, RM_ORANGE2, RM_ORANGE3, RM_ORANGE4, RM_ORANGE5, RM_ORANGE6,
	RM_LIME0, RM_LIME1, RM_LIME2, RM_LIME3, RM_LIME4, RM_LIME5, RM_LIME6,
	RM_TEAL0, RM_TEAL1, RM_TEAL2, RM_TEAL3, RM_TEAL4, RM_TEAL5, RM_TEAL6,
	RM_BLUE0, RM_BLUE1, RM_BLUE2, RM_BLUE3, RM_BLUE4, RM_BLUE5, RM_BLUE6,
	RM_PURPLE0, RM_PURPLE1, RM_PURPLE2, RM_PURPLE3, RM_PURPLE4, RM_PURPLE5, RM_PURPLE6,
	RM_MAGENTA0, RM_MAGENTA1, RM_MAGENTA2, RM_MAGENTA3, RM_MAGENTA4, RM_MAGENTA5, RM_MAGENTA6,
} RMColors;

typedef struct Vector2int
{
	int x, y;
} Vector2int;

typedef struct RMInput
{
	unsigned char current, past;
} RMInput;


extern void* _malloc(size_t size);
extern void* _realloc(void* ptr, size_t size);
extern void _free(void* ptr);

bool zero(float x);
int absi(int x);
int sign(float x);
float absf(float x);
float min(float x, float y);
float max(float x, float y);

void ClearString(char* string, unsigned int n);

float AirQuotesNoise(float point, bool flipped);

Vector2 Vector2Sign(Vector2 vector);
Vector2 Vector2Swap(Vector2 vector);
Vector2 InvertAroundPoint(Vector2 position, Vector2 point);
Vector2 ClosestPoint(Vector2 a, Vector2 b, Vector2 p);

bool Vector2intEqual(Vector2int a, Vector2int b);
Vector2int Vector2intSubtract(Vector2int a, Vector2int b);

Color ColorLerp(Color color_start, Color color_end, float amount);

void CheckKeyboardInput(RMInput* input);
bool InputHeld(RMInput input, int key);
bool InputPressed(RMInput input, int key);
void CheckMenuInput(RMInput* menu, RMInput input, float* block);

void DrawUnitVector(Vector2 position, Vector2 magnitude, Color color);

unsigned int TextFindLastChar(const char* text, unsigned char ch);

float AudioVolume(char value);

Color rmc(char i);

#endif
