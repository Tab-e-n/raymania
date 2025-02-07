#ifndef RM_ASSETS
#define RM_ASSETS

#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"

#define ASSET_AMOUNT 64


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
 */
typedef enum BlockID {
	DVOID,
	D1x1GWN, D2x1AIO, D2x2DIO, D1x1GWS, D1x1I, D1x1C, D1x1F, D1x1S,
	D1x1A, D2, D3, D4, D5, D6, D7, D8, D9,
	B1x1AR0
} BlockID;

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

void DrawAsset(Asset* asset, float scale, Vector2 position);
void DrawAssetPixels(Asset* asset, float scale, Vector2 position, Color color);

#endif
