#ifndef RM_OBJECTS
#define RM_OBJECTS

#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"
#include "asset.h"


#define BLOCK_DEBUG false

#define MAX_BLOCK_AMOUNT 2048
#define MAX_LOADED_BLOCKS 128 // LOAD_DIST * BLOCKS_PER_PIECE
#define MAX_LOADED_BLOCK_WALLS 18 // BLOCKS_PER_PIECE * 9
#define MAX_BLOCK_WALL_AMOUNT 32

#define PLAYER_LAYER 2
#define Z_LAYERS 4
#define LOAD_DISTANCE (Vector2){4, 3}


typedef enum Surface {SURFACE_ASPHALT = 0, SURFACE_GRASS, SURFACE_DIRT, SURFACE_ICE, SURFACE_WATER, SURFACE_AMOUNT} Surface;

typedef enum AreaType {
TYPE_VOID,
TYPE_ASPHALT, TYPE_GRASS, TYPE_DIRT, TYPE_ICE, TYPE_WATER,
TYPE_CHECKPOINT, TYPE_FINISH, TYPE_START, TYPE_BOOSTER
} AreaType;

typedef struct Area 
{
int type;
Rectangle rect;
} Area;

typedef struct Wall 
{
	Vector2 pos_x;
	Vector2 pos_y;
} Wall;

typedef struct Block
{
	int id, z, rot;
	Vector2int pos, size;
	Area area;
} Block;

typedef struct BlockWallArray
{
	int block_id;
	int wall_amount;
	Wall walls[MAX_BLOCK_WALL_AMOUNT];
} BlockWallArray;

Wall ShiftWall(Wall wall, Vector2 shift);
bool CheckWallCollision(Wall wall1, Wall wall2, Vector2* collision_point);

void DrawWallDebug(Wall wall, Color color);

Block MakeBlock(int id, Vector2int pos, int rot);
void ClearBlocks(int block_amount, Block blocks[]);
void ClearPlacedBlocks(Block blocks[MAX_BLOCK_AMOUNT]);
void LoadNearbyBlocks(Block blocks[MAX_BLOCK_AMOUNT], int layers[Z_LAYERS][MAX_LOADED_BLOCKS], Vector2int placement);

void DrawBlock(Block block, double game_time);
void DrawBlockAdv(Block block, float scale, Vector2 position, double game_time);
void DrawLoadedBlocks(Block blocks[MAX_BLOCK_AMOUNT], int layer[MAX_LOADED_BLOCKS], double game_time);
void DrawBlockDebug(Block block);
void DrawBlockDebugAdv(Block block, float scale, Vector2 position);
void DrawPlacedBlocksDebug(Block blocks[MAX_BLOCK_AMOUNT], int layer[MAX_LOADED_BLOCKS]);

BlockWallArray MakeBlockWalls(int block_id, Block block);
void ClearLoadedBlockWalls(BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS]);
void LoadNearbyBlockWalls(Block blocks[MAX_BLOCK_AMOUNT], BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], Vector2int placement);

void DrawBlockWallsDebug(BlockWallArray block_walls);
void DrawLoadedBlockWallsDebug(BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS]);

Vector2 CheckPosition(Vector2int placement);
Vector2 CheckRotation(int rot);
Vector2int PositionToPlacement(Vector2 pos);

#endif
