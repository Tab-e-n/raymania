#include <raylib.h>
#include "rmlib.h"
#include "blocks.h"
#include "racecar.h"
#include "profiles.h"
#include "track.h"

void SetStart(Track* track, Vector2int placement, BlockRotation rot)
{
	track->start_pos = CheckPosition(placement);
	track->start_rot = CheckRotation(rot);
}

unsigned int GetPieceAdv(unsigned int piece_id, Vector2int placement, BlockRotation rot, Block piece[BLOCKS_PER_PIECE])
{
	unsigned int id[BLOCKS_PER_PIECE], blocks_used;

	for(int i = 0; i < BLOCKS_PER_PIECE; i++)
	{
		id[i] = 0;
	}

	switch(piece_id)
	{
		case(1):
			blocks_used = 1;
			id[0] = 1;
			id[1] = 0;
			break;
		case(2):
			blocks_used = 1;
			id[0] = 2;
			id[1] = 0;
			break;
		case(3):
			blocks_used = 1;
			id[0] = 3;
			id[1] = 0;
			break;
		case(4):
			blocks_used = 1;
			id[0] = 4;
			id[1] = 0;
			break;
		case(5):
			blocks_used = 1;
			id[0] = 5;
			id[1] = 0;
			break;
		case(6):
			blocks_used = 1;
			id[0] = 6;
			id[1] = 0;
			break;
		case(7):
			blocks_used = 1;
			id[0] = 7;
			id[1] = 0;
			break;
		case(8):
			blocks_used = 1;
			id[0] = 8;
			id[1] = 0;
			break;
		case(9):
			blocks_used = 2;
			id[0] = 5;
			id[1] = 6;
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
		if(track->pieces[i].id != 0)
		{
			if(Vector2intEqual(track->pieces[i].placement, info->placement))
			{
				return false;
			}
		}
		else if(empty_piece == MAX_PIECES)
		{
			empty_piece = i;
		}
	}

	Block piece[BLOCKS_PER_PIECE];
	unsigned int blocks_used = GetPiece(info, piece);

	if(blocks_used == 0)
	{
		return true;
	}
	
	if(track->has_start)
	{
		for(int i = 0; i < blocks_used; i++)
		{
			if(piece[i].area.type == TYPE_START)
			{
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
				return false;
			}
		}
	}

	unsigned int spots[BLOCKS_PER_PIECE], spots_found = 0;

	for(int i = 0; i < MAX_BLOCK_AMOUNT; i++)
	{
		if(blocks[i].id == 0)
		{
			spots[spots_found] = i;
			spots_found++;
			if(spots_found >= blocks_used)
			{
				break;
			}
		}
	}

	if(spots_found < blocks_used)
	{
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

	return true;
}

bool DeletePiece(Track* track, Block blocks[MAX_BLOCK_AMOUNT], Vector2int placement)
{
	for(int i = 0; i < MAX_PIECES; i++)
	{
		if(Vector2intEqual(track->pieces[i].placement, placement))
		{
			track->pieces[i].id = 0;
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
}

void ClearTrack(Track* track)
{
	//ClearPlacedBlocks(track->blocks);
	track->has_start = false;
	track->medal_bronz = 0.0;
	track->medal_silver = 0.0;
	track->medal_gold = 0.0;
	track->medal_author = 0.0;
	track->checkpoint_amount = 0;
	track->car = CAR_ROAD;
	track->env = ENV_STADIUM;
	Vector2int pos = (Vector2int){0};
	SetStart(track, pos, ROT_NORTH);
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
			TraceLog(LOG_INFO, "RL_REALLOC: save track");
			save_file_data = (unsigned char*)RL_REALLOC(file_data, save_data_size);

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
		TraceLog(LOG_INFO, "RL_FREE: save track");
		RL_FREE(save_file_data);
	}
	else
	{
		data_size = sizeof(TrackSave);
		TraceLog(LOG_INFO, "RL_MALLOC: save track");
		file_data = (unsigned char*)RL_MALLOC(data_size);
		TrackSave* savefile = (TrackSave*)file_data;
	
		savefile->track = *track;
		savefile->size_block = TRACK_SIZE_BLOCK;
		savefile->size_max_block = TRACK_SIZE_MAX_BLOCK;
		savefile->size_other = TRACK_SIZE_OTHER;

		success = SaveFileData(filename, file_data, data_size);
		UnloadFileData(file_data);
	}

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
	TraceLog(LOG_INFO, "RL_MALLOC: track name");
	unsigned char* filename = (unsigned char*)RL_MALLOC(track_dir_len + size + 6);
	for(int i = 0; i < track_dir_len; i++)
	{
		filename[i] = track_dir[i];
	}
	filename[track_dir_len] = '/';
	for(int i = 0; i < size; i++)
	{
		filename[i + track_dir_len + 1] = name[i];
	}
	filename[track_dir_len + size + 1] = '.';
	filename[track_dir_len + size + 2] = 't';
	filename[track_dir_len + size + 3] = 'r';
	filename[track_dir_len + size + 4] = 'k';
	filename[track_dir_len + size + 5] = '\0';
	//const char* FILENAME = (const char*)RL_REALLOC(filename, size);
	const char* FILENAME = (const char*)filename;
	return FILENAME;
}

void TrackNameFromFilename(const char* filename, unsigned char name[TRACK_NAME_LENGHT])
{
	unsigned int len = TextFindLastChar(filename, '/') + 1;
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
	switch(page_num)
	{
		case(PAGE_DEFAULT):
			for(int i = 0; i < 9; i++)
			{
				page[i] = i + 1;
			}
			break;
		case(1):
			for(int i = 0; i < 9; i++)
			{
				page[i] = 9 - i;
			}
			break;
		case(2):
			for(int i = 0; i < 9; i++)
			{
				page[i] = i + 1;
			}
			break;
		case(3):
			for(int i = 0; i < 9; i++)
			{
				page[i] = 9 - i;
			}
			break;
		case(4):
			for(int i = 0; i < 9; i++)
			{
				page[i] = i + 1;
			}
			break;
		case(5):
			for(int i = 0; i < 9; i++)
			{
				page[i] = 9 - i;
			}
			break;
		case(6):
			for(int i = 0; i < 9; i++)
			{
				page[i] = i + 1;
			}
			break;
		case(7):
			for(int i = 0; i < 9; i++)
			{
				page[i] = 9 - i;
			}
			break;
		case(8):
			for(int i = 0; i < 9; i++)
			{
				page[i] = i + 1;
			}
			break;
		case(9):
			for(int i = 0; i < 9; i++)
			{
				page[i] = 9 - i;
			}
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
		piece[i].pos = (Vector2int){piece_count, 0};
		piece_catalogue[piece_count * BLOCKS_PER_PIECE + i] = piece[i];
	}
}

void LoadPieceCataloguePage(Block piece_catalogue[PIECE_CATALOGUE_LENGHT], unsigned int page[9])
{
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
		DrawBlockDebugAdv(block, size, pos);
	}
}