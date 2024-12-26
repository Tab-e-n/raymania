#ifndef RM_ASSETS
#define RM_ASSETS

#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"


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

Asset* AllocAsset(int asset_id, float scale, BlockRotation rot, double game_time);
void FreeAsset(Asset* asset);

void DrawAsset(Asset* asset, Vector2 position);

#endif
