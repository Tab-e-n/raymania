#include <raylib.h>
#include "rmlib.h"
#include "blocks.h"
#include "racecar.h"
#include "profiles.h"
#include "track.h"

bool BlockOverlap(Block block, Vector2int info_placement, Block info_block)
{
	Vector2int block_size = block.size;
	if(block.rot & 1 > 0)
	{
		block_size = (Vector2int){block_size.y, block_size.x};
	}
	Vector2int info_size = info_block.size;
	if(info_block.rot & 1 > 0)
	{
		info_size = (Vector2int){info_size.y, info_size.x};
	}
	Vector2int difference = Vector2intSubtract(info_placement, block.pos);
	return difference.x > -info_size.x && difference.x < block_size.x && difference.y > -info_size.y && difference.y < block_size.y;
}

void SetStart(Track* track, Vector2int placement, BlockRotation rot)
{
	track->start_pos = CheckPosition(placement);
	track->start_rot = CheckRotation(rot);
}

void TrackBlockmixed(Track* track, bool blockmixed)
{
	if(blockmixed)
	{
		track->blockmixed = true;
	}
}

unsigned int GetPieceAdv(unsigned int piece_id, Vector2int placement, BlockRotation rot, Block piece[BLOCKS_PER_PIECE])
{
	unsigned int id[BLOCKS_PER_PIECE], blocks_used;

	// PIECE DEFINITIONS

	for(int i = 0; i < BLOCKS_PER_PIECE; i++)
	{
		id[i] = 0;
	}

	switch(piece_id)
	{
		case(1):
			blocks_used = 1;
			id[0] = D1x1GWN;
			id[1] = DVOID;
			break;
		case(2):
			blocks_used = 1;
			id[0] = D2x1AIO;
			id[1] = DVOID;
			break;
		case(3):
			blocks_used = 1;
			id[0] = D2x2DIO;
			id[1] = DVOID;
			break;
		case(4):
			blocks_used = 1;
			id[0] = D1x1GWS;
			id[1] = DVOID;
			break;
		case(5):
			blocks_used = 1;
			id[0] = D1x1I;
			id[1] = DVOID;
			break;
		case(6):
			blocks_used = 1;
			id[0] = D1x1C;
			id[1] = DVOID;
			break;
		case(7):
			blocks_used = 1;
			id[0] = D1x1F;
			id[1] = DVOID;
			break;
		case(8):
			blocks_used = 1;
			id[0] = D1x1S;
			id[1] = DVOID;
			break;
		case(9):
			blocks_used = 2;
			id[0] = D1x1I;
			id[1] = D1x1C;
			break;
		case(10):
			blocks_used = 1;
			id[0] = D1x1A;
			id[1] = DVOID;
			break;
		case(11): // Basic Straight
			blocks_used = 1;
			id[0] = B1x1AR0;
			id[1] = DVOID;
			break;
		case(12): // Basic Turn
			blocks_used = 1;
			id[0] = B1x1AT0;
			id[1] = DVOID;
			break;
		case(13): // Checkpoint
			blocks_used = 2;
			id[0] = B1x1AR0;
			id[1] = B1x1CR0;
			break;
		case(14): // Start
			blocks_used = 2;
			id[0] = B1x1AR0;
			id[1] = B1x1SR0;
			break;
		case(15): // Finish
			blocks_used = 2;
			id[0] = B1x1AR0;
			id[1] = B1x1FR0;
			break;
		case(16): // Straight with center obstacle
			blocks_used = 2;
			id[0] = B1x1AR0;
			id[1] = B1x1VB0;
			break;
		case(17): // Turn with out obstacle
			blocks_used = 2;
			id[0] = B1x1AT0;
			id[1] = B1x1VB0;
			break;
		case(18): // Turn with inner obstacle
			blocks_used = 2;
			id[0] = B1x1AT0;
			id[1] = B1x1VB1;
			break;
		case(19): // Straight with two diagonal obstacles
			blocks_used = 2;
			id[0] = B1x1AR0;
			id[1] = B1x1VB2;
			break;
		case(20): // Straight with two diagonal obstacles (mirror)
			blocks_used = 2;
			id[0] = B1x1AR0;
			id[1] = B1x1VB3;
			break;
		case(21): // Big turn
			blocks_used = 1;
			id[0] = B2x2AT0;
			id[1] = DVOID;
			break;
		case(22): // T-Junction
			blocks_used = 1;
			id[0] = B1x1AJ0;
			id[1] = DVOID;
			break;
		case(23): // Crossroad
			blocks_used = 1;
			id[0] = B1x1AJ1;
			id[1] = DVOID;
			break;
		case(24): // Booster
			blocks_used = 2;
			id[0] = B1x1AR0;
			id[1] = B1x1BR0;
			break;
		case(25): // Big T-Junction
			blocks_used = 1;
			id[0] = B3x2AJ0;
			id[1] = DVOID;
			break;
		case(26): // Wiggle left
			blocks_used = 1;
			id[0] = B2x2AR0;
			id[1] = DVOID;
			break;
		case(27): // Wiggle right
			blocks_used = 1;
			id[0] = B2x2AR1;
			id[1] = DVOID;
			break;
		case(28): // Big Y-Junction
			blocks_used = 1;
			id[0] = B3x2AJ1;
			id[1] = DVOID;
			break;
		case(29): // Dirt Start
			blocks_used = 2;
			id[0] = B1x1DR0;
			id[1] = B1x1SR0;
			break;
		case(30): // Dirt Checkpoint
			blocks_used = 2;
			id[0] = B1x1DR0;
			id[1] = B1x1CR0;
			break;
		case(31): // Dirt Finish
			blocks_used = 2;
			id[0] = B1x1DR0;
			id[1] = B1x1FR0;
			break;
		case(32): // Dirt Basic Straight
			blocks_used = 1;
			id[0] = B1x1DR0;
			id[1] = DVOID;
			break;
		case(33): // Dirt Basic Turn
			blocks_used = 1;
			id[0] = B1x1DT0;
			id[1] = DVOID;
			break;
		case(34): // Dirt Big turn
			blocks_used = 1;
			id[0] = B2x2DT0;
			id[1] = DVOID;
			break;
		case(35): // Dirt Wiggle left
			blocks_used = 1;
			id[0] = B2x2DR0;
			id[1] = DVOID;
			break;
		case(36): // Dirt Wiggle right
			blocks_used = 1;
			id[0] = B2x2DR1;
			id[1] = DVOID;
			break;
		case(37): // Dirt
			blocks_used = 1;
			id[0] = B1x1DR1;
			id[1] = DVOID;
			break;
		case(38): // Dirt T-Junction
			blocks_used = 1;
			id[0] = B1x1DJ0;
			id[1] = DVOID;
			break;
		case(39): // Dirt Crossroad
			blocks_used = 1;
			id[0] = B1x1DJ1;
			id[1] = DVOID;
			break;
		case(40): // Dirt Big Y-Junction
			blocks_used = 1;
			id[0] = B3x2DJ1;
			id[1] = DVOID;
			break;
		case(41): // Dirt Big T-Junction
			blocks_used = 1;
			id[0] = B3x2DJ0;
			id[1] = DVOID;
			break;
		case(42): // Dirt Straight with center obstacle
			blocks_used = 2;
			id[0] = B1x1DR0;
			id[1] = B1x1VB0;
			break;
		case(43): // Dirt Straight with two diagonal obstacles
			blocks_used = 2;
			id[0] = B1x1DR0;
			id[1] = B1x1VB2;
			break;
		case(44): // Dirt Straight with two diagonal obstacles (mirror)
			blocks_used = 2;
			id[0] = B1x1DR0;
			id[1] = B1x1VB3;
			break;
		case(45): // Dirt Turn with out obstacle
			blocks_used = 2;
			id[0] = B1x1DT0;
			id[1] = B1x1VB0;
			break;
		case(46): // Dirt Turn with inner obstacle
			blocks_used = 2;
			id[0] = B1x1DT0;
			id[1] = B1x1VB1;
			break;
		case(47): // Platform
			blocks_used = 1;
			id[0] = B1x1AR1;
			id[1] = DVOID;
			break;
		case(48): // Platform edge
			blocks_used = 1;
			id[0] = B1x1AR2;
			id[1] = DVOID;
			break;
		case(49): // Platform turn
			blocks_used = 1;
			id[0] = B1x1AT1;
			id[1] = DVOID;
			break;
		case(50): // Platform big turn
			blocks_used = 1;
			id[0] = B2x2AT1;
			id[1] = DVOID;
			break;
		case(51): // Platform corner .
			blocks_used = 1;
			id[0] = B1x1AT2;
			id[1] = DVOID;
			break;
		case(52): // Platform enter :
			blocks_used = 1;
			id[0] = B1x1AR3;
			id[1] = DVOID;
			break;
		case(53): // Platform booster
			blocks_used = 1;
			id[0] = B1x1BR1;
			id[1] = DVOID;
			break;
		case(54): // Grass
			blocks_used = 1;
			id[0] = B1x1GR0;
			id[1] = DVOID;
			break;
		case(55): // Water
			blocks_used = 1;
			id[0] = B1x1WR0;
			id[1] = DVOID;
			break;
		case(56): // Controls
			blocks_used = 1;
			id[0] = DCONTROLS;
			id[1] = DVOID;
			break;
		default:
			blocks_used = 0;
			break;
	}

	for(int i = 0; i < BLOCKS_PER_PIECE; i++)
	{
		piece[i] = MakeBlock(id[i], placement, rot);
	}

	return blocks_used;
}

unsigned int GetPiece(PieceInfo* info, Block piece[BLOCKS_PER_PIECE])
{
	return GetPieceAdv(info->id, info->placement, info->rot, piece);
}

bool AddPiece(Track* track, Block blocks[MAX_BLOCK_AMOUNT], PieceInfo* info)
{
	unsigned int empty_piece = MAX_PIECES;
	for(int i = 0; i < MAX_PIECES; i++)
	{
		if(track->pieces[i].id == 0)
		{
			empty_piece = i;
			break;
		}
	}

	Block piece[BLOCKS_PER_PIECE];
	unsigned int blocks_used = GetPiece(info, piece);

	if(blocks_used == 0)
	{
		if(DEBUG) TraceLog(LOG_INFO, "BLOCK: Is placed because it is empty");
		return true;
	}

	if(track->has_start)
	{
		for(int i = 0; i < blocks_used; i++)
		{
			if(piece[i].area.type == TYPE_START)
			{
				if(DEBUG) TraceLog(LOG_INFO, "BLOCK: NOT placed because a start already exists");
				return false;
			}
		}
	}
	if(track->checkpoint_amount >= MAX_CHECKPOINTS)
	{
		for(int i = 0; i < blocks_used; i++)
		{
			if(piece[i].area.type == TYPE_CHECKPOINT)
			{
				if(DEBUG) TraceLog(LOG_INFO, "BLOCK: NOT placed because the checkpoint limit has been reached");
				return false;
			}
		}
	}

	unsigned int spots[BLOCKS_PER_PIECE], spots_found = 0;

	for(int i = 0; i < MAX_BLOCK_AMOUNT; i++)
	{
		if(blocks[i].id == 0)
		{
			if(spots_found < blocks_used)
			{
				spots[spots_found] = i;
				spots_found++;
			}
		}
		else if(!track->blockmixed)
		{
			if(BlockOverlap(blocks[i], info->placement, piece[0]))
			{
				if(DEBUG) TraceLog(LOG_INFO, "BLOCK: NOT placed because it overlaps with another block");
				//if(DEBUG) TraceLog(LOG_INFO, "%i: %i, pos %i %i, size %i %i", i, blocks[i].id, blocks[i].pos.x, blocks[i].pos.y, blocks[i].size.x, blocks[i].size.y);
				return false;
			}
		}
	}

	if(spots_found < blocks_used)
	{
		if(DEBUG) TraceLog(LOG_INFO, "BLOCK: NOT placed because the block limit has been reached");
		return false;
	}

	for(int i = 0; i < spots_found; i++)
	{
		blocks[spots[i]] = piece[i];
		if(piece[i].area.type == TYPE_START)
		{
			SetStart(track, info->placement, info->rot);
			track->has_start = true;
		}
		if(piece[i].area.type == TYPE_CHECKPOINT)
		{
			track->checkpoint_amount++;
		}
	}
	track->pieces[empty_piece] = *info;

	if(DEBUG) TraceLog(LOG_INFO, "BLOCK: Is placed successfully");
	return true;
}

bool DeletePiece(Track* track, Block blocks[MAX_BLOCK_AMOUNT], Vector2int placement)
{
	bool removed = false;
	for(int i = 0; i < MAX_PIECES; i++)
	{
		if(track->pieces[i].id == 0)
		{
			// pass
		}
		else if(Vector2intEqual(track->pieces[i].placement, placement))
		{
			track->pieces[i].id = 0;
			removed = true;
		}
	}
	for(int i = 0; i < MAX_BLOCK_AMOUNT; i++)
	{
		if(Vector2intEqual(blocks[i].pos, placement))
		{
			if(blocks[i].area.type == TYPE_START)
			{
				track->has_start = false;
			}
			if(blocks[i].area.type == TYPE_CHECKPOINT)
			{
				track->checkpoint_amount--;
			}
			blocks[i] = MakeBlock(0, placement, ROT_NORTH);
		}
	}
	return removed;
}

unsigned int OverlapingPieces(Track* track, Vector2int placement)
{
	if(!track->blockmixed)
	{
		return 0;
	}
	unsigned int count = 0;
	for(int i = 0; i < MAX_PIECES; i++)
	{
		if(Vector2intEqual(track->pieces[i].placement, placement))
		{
			count++;
		}
	}
	return count;
}

void ClearTrack(Track* track)
{
	//ClearPlacedBlocks(track->blocks);
	track->validated = false;
	track->blockmixed = false;
	track->has_start = false;
	track->medal_bronz = 0.0;
	track->medal_silver = 0.0;
	track->medal_gold = 0.0;
	track->medal_author = 0.0;
	track->checkpoint_amount = 0;
	track->car = CAR_ROAD;
	track->env = ENV_VOID;
	Vector2int pos = (Vector2int){0};
	SetStart(track, pos, ROT_NORTH);
	for(int i = 0; i < MAX_PIECES; i++)
	{
		track->pieces[i].id = 0;
	}
}

void MakeTrackBlocks(Track* track, Block blocks[MAX_BLOCK_AMOUNT])
{
	ClearPlacedBlocks(blocks);
	unsigned int current_block = 0;
	for(int i = 0; i < MAX_PIECES; i++)
	{
		Block piece[BLOCKS_PER_PIECE];
		unsigned int blocks_used = GetPiece(&track->pieces[i], piece);

		if(blocks_used == 0)
		{
			continue;
		}
		if(current_block + blocks_used > MAX_BLOCK_AMOUNT)
		{
			break;
		}

		for(int j = 0; j < blocks_used; j++)
		{
			blocks[current_block] = piece[j];
			current_block++;
		}
	}
}

bool CheckTrackFile(unsigned int size, unsigned int size_block, unsigned int size_max_block, unsigned int size_other)
{
	return size != sizeof(TrackSave) ||
	   size_block != TRACK_SIZE_BLOCK ||
	   size_max_block != TRACK_SIZE_MAX_BLOCK ||
	   size_other != TRACK_SIZE_OTHER;
}

Track LoadTrack(const char* filename)
{
	int data_size;
	unsigned char* file_data = LoadFileData(filename, &data_size);
	Track track = {0};

	if(file_data != (void*)0)
	{
		int* size_check = (int*)file_data;
		if(CheckTrackFile(data_size, size_check[0], size_check[1], size_check[2]))
		{
			// File doesn't match requirements
			// File conversion code here
			TraceLog(LOG_WARNING, "FILEIO: [%s] File is not a Track File.", filename);
		}
		else
		{
			TrackSave* savefile = (TrackSave*)file_data;
			track = savefile->track;

			TraceLog(LOG_INFO, "FILEIO: [%s] Track loaded successfully.", filename);
		}

		UnloadFileData(file_data);
	}

	TraceLog(LOG_INFO, "Loading track ended.");
	return track;
}

bool SaveTrack(Track* track, const char* filename, unsigned char author[PROFILE_NAME_LENGHT])
{
	for(int i = 0; i < PROFILE_NAME_LENGHT; i++)
	{
		track->author[i] = author[i];
	}
	bool success = false;
	int data_size;
	int save_data_size;
	unsigned char* file_data = LoadFileData(filename, &data_size);
	unsigned char* save_file_data = (void*)0;

	if(file_data != (void*)0)
	{
		int* size_check = (int*)file_data;
		if(CheckTrackFile(data_size, size_check[0], size_check[1], size_check[2]))
		{
			save_data_size = sizeof(TrackSave);
			TraceLog(LOG_INFO, "REALLOC: save track");
			save_file_data = (unsigned char*)_realloc(file_data, save_data_size);

			if(save_file_data != (void*)0)
			{
				TrackSave* savefile = (TrackSave*)save_file_data;
				savefile->track = *track;
				savefile->size_block = TRACK_SIZE_BLOCK;
				savefile->size_max_block = TRACK_SIZE_MAX_BLOCK;
				savefile->size_other = TRACK_SIZE_OTHER;
			}
			else
			{
				TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data.", filename);

				save_file_data = file_data;
				save_data_size = data_size;
			}
		}
		else
		{
			save_file_data = file_data;
			save_data_size = data_size;

			TrackSave* savefile = (TrackSave*)save_file_data;
			savefile->track = *track;
			savefile->size_block = TRACK_SIZE_BLOCK;
			savefile->size_max_block = TRACK_SIZE_MAX_BLOCK;
			savefile->size_other = TRACK_SIZE_OTHER;
		}

		success = SaveFileData(filename, file_data, data_size);
		TraceLog(LOG_INFO, "FREE: save track");
		_free(save_file_data);
	}
	else
	{
		data_size = sizeof(TrackSave);
		TraceLog(LOG_INFO, "MALLOC: save track");
		file_data = (unsigned char*)_malloc(data_size);
		TrackSave* savefile = (TrackSave*)file_data;

		savefile->track = *track;
		savefile->size_block = TRACK_SIZE_BLOCK;
		savefile->size_max_block = TRACK_SIZE_MAX_BLOCK;
		savefile->size_other = TRACK_SIZE_OTHER;

		success = SaveFileData(filename, file_data, data_size);
		UnloadFileData(file_data);
	}

	TraceLog(LOG_INFO, "Saving track ended.");
	return success;
}

const char* TrackFileName(unsigned char* track_dir, unsigned char name[TRACK_NAME_LENGHT])
{
	unsigned int size = 0;
	unsigned int spaces = 0;
	for(int i = TRACK_NAME_LENGHT - 1; i >= 0; i--)
	{
		if(name[i] == 0)
		{
			spaces++;
		}
		else
		{
			break;
		}
	}
	size = TRACK_NAME_LENGHT - spaces;
	if(size == 0)
	{
		name[0] = 'U';
		name[1] = 'n';
		name[2] = 'n';
		name[3] = 'a';
		name[4] = 'm';
		name[5] = 'e';
		name[6] = 'd';
		name[7] = '\0';
		size = 8;
	}

	unsigned int track_dir_len = TextLength(track_dir);
	TraceLog(LOG_INFO, "MALLOC: track name");
	unsigned char* filename = (unsigned char*)_malloc(track_dir_len + size + 6);
	for(int i = 0; i < track_dir_len; i++)
	{
		filename[i] = track_dir[i];
	}
#ifdef WINDOWS
	filename[track_dir_len] = '\\';
#else
	filename[track_dir_len] = '/';
#endif
	for(int i = 0; i < size; i++)
	{
		filename[i + track_dir_len + 1] = name[i];
	}
	filename[track_dir_len + size + 1] = '.';
	filename[track_dir_len + size + 2] = 't';
	filename[track_dir_len + size + 3] = 'r';
	filename[track_dir_len + size + 4] = 'k';
	filename[track_dir_len + size + 5] = '\0';
	//const char* FILENAME = (const char*)_realloc(filename, size);
	const char* FILENAME = (const char*)filename;
	TraceLog(LOG_INFO, "Track Filename: %s", FILENAME);
	return FILENAME;
}

void TrackNameFromFilename(const char* filename, unsigned char name[TRACK_NAME_LENGHT])
{
	ClearString(name, TRACK_NAME_LENGHT);
	TraceLog(LOG_INFO, "|%s|", filename);
#ifdef WINDOWS
	unsigned int len = TextFindLastChar(filename, '\\') + 1;
#else
	unsigned int len = TextFindLastChar(filename, '/') + 1;
#endif // WINDOWS
	if(TextLength(filename) - len < 4)
	{
		return;
	}
	for(int i = 0; i < TRACK_NAME_LENGHT; i++)
	{
		if(filename[i + len + 4] == 0)
		{
			break;
		}
		name[i] = filename[i + len];
	}
}

void LoadTrackDirectory(FilePathList* fpl, unsigned char* track_dir)
{
	if(fpl->count != 0) UnloadDirectoryFiles(*fpl);
	*fpl = LoadDirectoryFiles(TextFormat("%s", track_dir));
}

void CalculateMedalTimes(Track* track)
{
	if(track->medal_gold < track->medal_author)
	{
		track->medal_gold = track->medal_author * MEDAL_GOLD_MULT;
	}
	if(track->medal_silver < track->medal_gold)
	{
		track->medal_silver = track->medal_author * MEDAL_SILVER_MULT;
	}
	if(track->medal_bronz < track->medal_silver)
	{
		track->medal_bronz = track->medal_author * MEDAL_BRONZ_MULT;
	}
}

void ResetMedalTimes(Track* track)
{
	track->medal_bronz = 0.0;
	track->medal_silver = 0.0;
	track->medal_gold = 0.0;
	track->medal_author = 0.0;
	track->validated = false;
}

void MoveEditorCursor(PieceInfo* info, int x, int y)
{
	info->placement.x += x;
	info->placement.y += y;

	if(info->placement.x < 0) info->placement.x = 0;
	if(info->placement.y < 0) info->placement.y = 0;
	if(info->placement.x >= TRACK_GRID_SIZE) info->placement.x = TRACK_GRID_SIZE - 1;
	if(info->placement.y >= TRACK_GRID_SIZE) info->placement.y = TRACK_GRID_SIZE - 1;
}

float CataloguePosition(float initial, unsigned int piece_n)
{
	initial += piece_n * BLOCK_SIZE * CATALOGUE_BLOCK_SCALE;
	initial += piece_n * 16;
	return initial;
}

void PiecesInPage(unsigned int page[9], unsigned int page_num)
{
	// PIECE CATALOGUE DEFINITIONS
	switch(page_num)
	{
		case(PAGE_DEBUG):
			for(int i = 0; i < 9; i++)
			{
				page[i] = i + 1;
			}
			break;
		case(PAGE_ROAD_BASE):
			page[0] = 14;
			page[1] = 13;
			page[2] = 15;
			page[3] = 11;
			page[4] = 12;
			page[5] = 21;
			page[6] = 26;
			page[7] = 27;
			page[8] = 24;
			break;
		case(PAGE_ROAD_OBSTACLE):
			page[0] = 22;
			page[1] = 23;
			page[2] = 28;
			page[3] = 25;
			page[4] = 16;
			page[5] = 19;
			page[6] = 20;
			page[7] = 17;
			page[8] = 18;
			break;
		case(PAGE_DIRT_BASE):
			page[0] = 29;
			page[1] = 30;
			page[2] = 31;
			page[3] = 32;
			page[4] = 33;
			page[5] = 34;
			page[6] = 35;
			page[7] = 36;
			page[8] = 37;
			break;
		case(PAGE_DIRT_OBSTACLE):
			page[0] = 38;
			page[1] = 39;
			page[2] = 40;
			page[3] = 41;
			page[4] = 42;
			page[5] = 43;
			page[6] = 44;
			page[7] = 45;
			page[8] = 46;
			break;
		case(PAGE_PLATFORM_BASE):
			page[0] = 47;
			page[1] = 48;
			page[2] = 49;
			page[3] = 50;
			page[4] = 51;
			page[5] = 52;
			page[6] = 53;
			page[7] = 54;
			page[8] = 55;
			break;
		case(PAGE_BONUS):
			page[0] = 56;
			page[1] = 0;
			page[2] = 0;
			page[3] = 0;
			page[4] = 0;
			page[5] = 0;
			page[6] = 0;
			page[7] = 0;
			page[8] = 0;
			break;
	}
}

void PutPieceIntoCatalogue(Block piece_catalogue[PIECE_CATALOGUE_LENGHT], unsigned int piece_id, unsigned int piece_count)
{
	Vector2int placement = (Vector2int){0, 0};
	Block piece[BLOCKS_PER_PIECE];
	unsigned int block_amount = GetPieceAdv(piece_id, placement, ROT_NORTH, piece);
	for(int i = 0; i < block_amount; i++)
	{
		piece[i].pos = (Vector2int){0, 0};
		piece_catalogue[piece_count * BLOCKS_PER_PIECE + i] = piece[i];
	}
}

void LoadPieceCataloguePage(Block piece_catalogue[PIECE_CATALOGUE_LENGHT], unsigned int page[9])
{
	for(int i = 0; i < PIECE_CATALOGUE_LENGHT; i++)
	{
		piece_catalogue[i] = (Block){0};
	}
	for(int i = 0; i < 9; i++)
	{
		PutPieceIntoCatalogue(piece_catalogue, page[i], i);
	}
}

void DrawPieceCatalogue(Block piece_catalogue[PIECE_CATALOGUE_LENGHT], Vector2 position)
{
	for(int i = 0; i < PIECE_CATALOGUE_LENGHT; i++)
	{
		Block block = piece_catalogue[i];
		float size = block.size.x;
		if(block.size.x < block.size.y) size = block.size.y;
		size = CATALOGUE_BLOCK_SCALE / size;

		float piece_n = i / BLOCKS_PER_PIECE;
		Vector2 pos = position;
		pos.x = CataloguePosition(position.x, piece_n);
		DrawBlockAdv(block, size, pos, 0);
	}
}
