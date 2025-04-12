#ifndef RM_ASSETS
#define RM_ASSETS

#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"

#define PALETTE_AMOUNT 10 
#define MODEL_AMOUNT 1


typedef enum RacecarColors {
	CAR_WHEEL0, CAR_WHEEL1, CAR_WHEEL2, CAR_BASE0, CAR_BASE1, CAR_BASE2, CAR_ACCENT0, CAR_ACCENT1, CAR_ACCENT2,
	CAR_DRIVER, CAR_SEAT, CAR_HULL, CAR_WINDOW0, CAR_WINDOW1, CAR_WINDOW2, CAR_LIGHT_FRONT, CAR_LIGHT_BACK, CAR_LIGHT_OTHER,
	CAR_NUMPLATE_BASE, CAR_NUMPLATE_TEXT0, CAR_NUMPLATE_TEXT1, CAR_EXTRA0, CAR_EXTRA1, CAR_EXTRA2, CAR_TRAIL,
	CAR_COLOR_AMOUNT
} RacecarColors;

/*
 * [Set][Size XxY][Surface][Other info]
 * Set
 *  - D, Debug blocks
 *  - B, Normal Blocks
 * Size
 *  - 1x1, Size 1 by 1
 *  - 2x1, 2 wide 1 tall
 * Surface
 *  - V, Void
 *  - A, Asphalt
 *  - G, Grass
 *  - D, Dirt
 *  - I, Ice
 *  - W, Water
 *  - C, Checkpoint
 *  - F, Finish
 *  - S, Start
 *  - B, Booster
 *  Other info
 *  - R, Roads
 *  - T, Turns
 *  - B, Obstacles
 *  - J, Junctions, Splits, Crossings
 */
typedef enum BlockID {
	DVOID, D1x1GWN, D2x1AIO, D2x2DIO, D1x1GWS, D1x1I, D1x1C, D1x1F, D1x1S,
	D1x1A, DRACECAR, DCONTROLS, D4, D5, D6, D7, D8, D9,
	B1x1AR0, B1x1AT0, B1x1CR0, B1x1SR0, B1x1FR0, B1x1VB0, B1x1VB1, B1x1VB2, B1x1VB3,
	B2x2AT0, B1x1AJ0, B1x1AJ1, B1x1BR0, B3x2AJ0, B2x2AR0, B2x2AR1, B3x2AJ1,
	B1x1DR0, B1x1DT0, B2x2DT0, B1x1DJ0, B1x1DJ1, B3x2DJ0, B2x2DR0, B2x2DR1, B3x2DJ1,
	B1x1DR1,
	B1x1GR0, B1x1WR0, B1x1AR1, B1x1AT1, B1x1AR2, B2x2AT1, B1x1AT2, B1x1AR3, B1x1BR1,
} BlockID;

typedef struct Palette
{
	char colors[CAR_COLOR_AMOUNT];
} Palette;

typedef struct Tri
{
	Vector2 a, b, c;
	char color;
} Tri;

typedef struct Asset
{
	unsigned int tri_amount;
	Tri tris[];
} Asset;


void RaylibLogo(void);
void TabinLogo(void);

Palette GetPalette(unsigned char id);

Tri MoveTri(Tri tri, Vector2 position);
Tri ScaleTri(Tri tri, float scale);
bool TriIsPoint(Tri tri);
Color TriColor(Tri tri);
void PrintTri(Tri tri);

int SizeOfAsset(int tri_count);
Asset* RotateAsset(Asset* asset, BlockRotation rot, Vector2 size);
Asset* MallocAsset(int tri_count);
Asset* AllocAsset(int asset_id, BlockRotation rot, double game_time);
void FreeAsset(Asset* asset);
void PaintAsset(Asset* asset, Palette palette);

void DrawAsset(Asset* asset, float scale, Vector2 position);
void DrawAssetPixels(Asset* asset, float scale, Vector2 position, Color color);

Color WaterColor(Vector2 pos, double game_time, Color wave, Color water);
void DrawBackgroundWater(Vector2 position, float zoom, double game_time);
void DrawBackgroundTris(Vector2 position, float zoom, double game_time, char wave, char base);
void DrawBackgroundVoid(Vector2 position, float zoom, double game_time);

#endif
