#ifndef RM_ASSETS
#define RM_ASSETS

#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"

#define ASSET_AMOUNT 64


typedef struct Tri
{
	Vector2 a, b, c;
	short unsigned int color;
} Tri;

typedef struct Asset
{
	unsigned int tri_amount;
	Tri tris[];
} Asset;


void RaylibLogo(void);
void TabinLogo(void);

Tri MoveTri(Tri tri, Vector2 position);
Color TriColor(Tri tri);
void PrintTri(Tri tri);

Asset* RotateAsset(Asset* asset, BlockRotation rot, float size);
Asset* MallocAsset(int tri_count);
Asset* AllocAsset(int asset_id, BlockRotation rot, double game_time);
void FreeAsset(Asset* asset);

void DrawAsset(Asset* asset, float scale, Vector2 position);

#endif
