#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"
#include "blocks.h"


Wall ShiftWall(Wall wall, Vector2 shift)
{
	wall.pos_x.x += shift.x;
	wall.pos_x.y += shift.y;
	wall.pos_y.x += shift.x;
	wall.pos_y.y += shift.y;
	return wall;
}

bool CheckWallCollision(Wall wall1, Wall wall2, Vector2* collision_point)
{

	return CheckCollisionLines(wall1.pos_x, wall1.pos_y, wall2.pos_x, wall2.pos_y, collision_point);
}

void DrawWallDebug(Wall wall, Color color)
{
	DrawLine(wall.pos_x.x, wall.pos_x.y, wall.pos_y.x, wall.pos_y.y, color);
}

Block MakeBlock(int id, Vector2int pos, int rot)
{
	Block block;

	block.id = id;
	block.pos = pos;
	block.rot = rot;

	block.z = 0;

	switch(id)
	{
		case(1):
		case(4):
			block.area = (Area){TYPE_GRASS, 0, 0, BLOCK_SIZE, BLOCK_SIZE};
			block.size = (Vector2int){1, 1};
			break;
		case(2):
			block.area = (Area){TYPE_ASPHALT, 0, 0, BLOCK_SIZE * 2, BLOCK_SIZE};
			block.size = (Vector2int){2, 1};
			break;
		case(3):
			block.area = (Area){TYPE_DIRT, 0, 0, BLOCK_SIZE * 2, BLOCK_SIZE * 2};
			block.size = (Vector2int){2, 2};
			break;
		case(5):
			block.area = (Area){TYPE_ICE, 0, 0, BLOCK_SIZE, BLOCK_SIZE};
			block.size = (Vector2int){1, 1};
			break;
		case(6):
			block.area = (Area){TYPE_CHECKPOINT, 0, BU * 3.5, BLOCK_SIZE, BU};
			block.size = (Vector2int){1, 1};
			break;
		case(7):
			block.area = (Area){TYPE_FINISH, 0, BU * 3.5, BLOCK_SIZE, BU};
			block.size = (Vector2int){1, 1};
			break;
		case(8):
			block.area = (Area){TYPE_START, 0, BU * 3.5, BLOCK_SIZE, BU};
			block.size = (Vector2int){1, 1};
			block.z = 1;
			break;
		default:
			block.id = 0;
			block.area = (Area){TYPE_VOID, 0, 0, 0, 0};
			block.size = (Vector2int){1, 1};
			break;
	}

	if(rot == ROT_EAST || rot == ROT_WEST)
	{
		double t = block.area.rect.width;
		block.area.rect.width = block.area.rect.height;
		block.area.rect.height = t;
		t = block.area.rect.x;
		block.area.rect.x = block.area.rect.y;
		block.area.rect.y = t;
	}

	block.area.rect.x += (double)pos.x * BLOCK_SIZE;
       	block.area.rect.y += (double)pos.y * BLOCK_SIZE;

	return block;
}

void ClearPlacedBlocks(Block blocks[MAX_BLOCK_AMOUNT])
{
	Vector2int pos = (Vector2int){0, 0};
	for(int i = 0; i < MAX_BLOCK_AMOUNT; i++)
	{
		blocks[i] = MakeBlock(0, pos, ROT_NORTH);
	}
}

void DrawBlockDebug(Block block)
{
	Vector2 pos = (Vector2){0, 0};
	DrawBlockDebugAdv(block, 1.0, pos);
}

void DrawBlockDebugAdv(Block block, float scale, Vector2 position)
{
	if(block.id == 0)
	{
		return;
	}

	Color c_area = RAYWHITE;

	switch(block.area.type)
	{
		case(TYPE_VOID):
			c_area = RAYWHITE;
			break;
		case(TYPE_ASPHALT):
			c_area = GRAY;
			break;
		case(TYPE_GRASS):
			c_area = LIME;
			break;
		case(TYPE_DIRT):
			c_area = ORANGE;
			break;
		case(TYPE_ICE):
			c_area = SKYBLUE;
			break;
		case(TYPE_WATER):
			c_area = DARKBLUE;
			break;
		case(TYPE_CHECKPOINT):
			c_area = BLUE;
			break;
		case(TYPE_FINISH):
			c_area = RED;
			break;
		case(TYPE_START):
			c_area = GREEN;
			break;
	}

	Rectangle rect = block.area.rect;
	if(scale != 1.0)
	{
		rect.width *= scale;
		rect.height *= scale;
		rect.x *= scale;
		rect.y *= scale;
	}
	rect.x += position.x;
	rect.y += position.y;

	DrawRectangleRec(rect, c_area);
}

void DrawPlacedBlocksDebug(Block blocks[MAX_BLOCK_AMOUNT], int z)
{
	for(int i = 0; i < MAX_BLOCK_AMOUNT; i++)
	{
		if(blocks[i].z == z)
		{
			DrawBlockDebug(blocks[i]);
		}
	}
}

BlockWallArray MakeBlockWalls(int block_id, Block block)
{
	BlockWallArray block_walls;
	block_walls.block_id = block_id;
	switch(block.id)
	{
		case(1):
			block_walls.wall_amount = 1;
			block_walls.walls[0] = (Wall){0, 0, 0, BU * 8};
			break;
		case(2):
		case(3):
			block_walls.wall_amount = 5;
			block_walls.walls[0] = (Wall){BU * 10, BU * 2, BU * 14, BU * 2};
			block_walls.walls[1] = (Wall){BU * 10, BU * 2, BU * 10, BU * 6};
			block_walls.walls[2] = (Wall){BU * 10, BU * 6, BU * 14, BU * 6};
			block_walls.walls[3] = (Wall){BU * 14, BU * 2, BU * 14, BU * 6};
			block_walls.walls[4] = (Wall){BU * 4, BU * 2, BU * 4, BU * 6};
			break;
		case(4):
			block_walls.wall_amount = 1;
			block_walls.walls[0] = (Wall){0, 0, BU * 8, BU * 8};
			break;
		default:
			block_walls.wall_amount = 0;
			break;
	}

	if(block.rot == ROT_SOUTH)
	{
		for(int i = 0; i < block_walls.wall_amount; i++)
		{
			Wall wall = block_walls.walls[i];
			wall.pos_x.x = BLOCK_SIZE - wall.pos_x.x + BLOCK_SIZE * (block.size.x - 1);
			wall.pos_x.y = BLOCK_SIZE - wall.pos_x.y + BLOCK_SIZE * (block.size.y - 1);
			wall.pos_y.x = BLOCK_SIZE - wall.pos_y.x + BLOCK_SIZE * (block.size.x - 1);
			wall.pos_y.y = BLOCK_SIZE - wall.pos_y.y + BLOCK_SIZE * (block.size.y - 1);
			block_walls.walls[i] = wall;
		}
	}
	if(block.rot == ROT_EAST)
	{
		for(int i = 0; i < block_walls.wall_amount; i++)
		{
			Wall wall = block_walls.walls[i];
			wall.pos_x.y = block_walls.walls[i].pos_x.x;
			wall.pos_x.x = BLOCK_SIZE - block_walls.walls[i].pos_x.y + BLOCK_SIZE * (block.size.y - 1);
			wall.pos_y.y = block_walls.walls[i].pos_y.x;
			wall.pos_y.x = BLOCK_SIZE - block_walls.walls[i].pos_y.y + BLOCK_SIZE * (block.size.y - 1);
			block_walls.walls[i] = wall;
		}
	}
	if(block.rot == ROT_WEST)
	{
		for(int i = 0; i < block_walls.wall_amount; i++)
		{
			Wall wall = block_walls.walls[i];
			wall.pos_x.x = block_walls.walls[i].pos_x.y;
			wall.pos_x.y = BLOCK_SIZE - block_walls.walls[i].pos_x.x + BLOCK_SIZE * (block.size.x - 1);
			wall.pos_y.x = block_walls.walls[i].pos_y.y;
			wall.pos_y.y = BLOCK_SIZE - block_walls.walls[i].pos_y.x + BLOCK_SIZE * (block.size.x - 1);
			block_walls.walls[i] = wall;
		}
	}

	Vector2 pos = (Vector2){block.pos.x * BLOCK_SIZE, block.pos.y * BLOCK_SIZE};
	for(int i = 0; i < block_walls.wall_amount; i++)
	{
		Wall wall = block_walls.walls[i];
		wall.pos_x.x += pos.x;
		wall.pos_x.y += pos.y;
		wall.pos_y.x += pos.x;
		wall.pos_y.y += pos.y;
		block_walls.walls[i] = wall;
	}
	
	return block_walls;
}

void ClearLoadedBlockWalls(BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS])
{
	for(int i = 0; i < MAX_LOADED_BLOCK_WALLS; i++)
	{
		block_walls[i].wall_amount = 0;
	}
}

void LoadNearbyBlockWalls(Block blocks[MAX_BLOCK_AMOUNT], BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], Vector2int placement)
{
	ClearLoadedBlockWalls(block_walls);
	int i_bw = 0;
	for(int i = 0; i < MAX_BLOCK_AMOUNT; i++)
	{
		if(blocks[i].id == 0)
		{
			continue;
		}
		Vector2int dist;
		int max_dist_x = blocks[i].size.x;
		int max_dist_y = blocks[i].size.y;
		if(blocks[i].rot == ROT_EAST || blocks[i].rot == ROT_WEST)
		{
			max_dist_x = blocks[i].size.y;
			max_dist_y = blocks[i].size.x;
		}
		dist.x = blocks[i].pos.x - placement.x;
		if(dist.x > 1 || dist.x < -max_dist_x)
		{
			continue;
		}
		dist.y = blocks[i].pos.y - placement.y;
		if(dist.y > 1 || dist.y < -max_dist_y)
		{
			continue;
		}
		BlockWallArray walls = MakeBlockWalls(i, blocks[i]);
		//if(walls.wall_amount == 0)
		//{
		//	continue;
		//}
		block_walls[i_bw] = walls;
		i_bw++;
		if(i_bw == MAX_LOADED_BLOCK_WALLS)
		{
			break;
		}
	}
}

void DrawBlockWallsDebug(BlockWallArray block_walls)
{
	for(int i = 0; i < block_walls.wall_amount; i++)
	{
		DrawWallDebug(block_walls.walls[i], BLACK);
	}
	
}

void DrawLoadedBlockWallsDebug(BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS])
{
	for(int i = 0; i < MAX_LOADED_BLOCK_WALLS; i++)
	{
		DrawBlockWallsDebug(block_walls[i]);
	}
}

Vector2 CheckPosition(Vector2int placement)
{
	return (Vector2){((float)placement.x + 0.5) * BLOCK_SIZE, ((float)placement.y + 0.5) * BLOCK_SIZE};
}

Vector2 CheckRotation(int rot)
{
	switch(rot)
	{
		case(ROT_NORTH):
			return (Vector2){0.0, -1.0};
			break;
		case(ROT_EAST):
			return (Vector2){1.0, 0.0};
			break;
		case(ROT_SOUTH):
			return (Vector2){0.0, 1.0};
			break;
		case(ROT_WEST):
			return (Vector2){-1.0, 0.0};
			break;
	}
}

Vector2int PositionToPlacement(Vector2 pos)
{
	Vector2int vec = (Vector2int){(int)(pos.x) * INV_BLOCK_SIZE, (int)(pos.y) * INV_BLOCK_SIZE};
	if(pos.x < 0)
	{
		vec.x -= 1;
	}
	if(pos.y < 0)
	{
		vec.y -= 1;
	}
	return vec;
}
