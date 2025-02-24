#ifndef RM_TRACK_MANAGER
#define RM_TRACK_MANAGER

#include <raylib.h>
#include "rmlib.h"
#include "blocks.h"
#include "racecar.h"
#include "profiles.h"

#define BLOCKS_PER_PIECE 2
#define MAX_PIECES (unsigned int)MAX_BLOCK_AMOUNT/BLOCKS_PER_PIECE

#define TRACK_NAME_LENGHT 32
#define TRACK_GRID_SIZE 32

#define TRACK_DIRECTORY "Tracks\0"

#define TRACK_SIZE_BLOCK sizeof(PieceInfo)
#define TRACK_SIZE_MAX_BLOCK sizeof(PieceInfo)*MAX_PIECES
#define TRACK_SIZE_OTHER sizeof(TrackSave)-TRACK_SIZE_MAX_BLOCK

#define PIECE_CATALOGUE_LENGHT 9*BLOCKS_PER_PIECE
#define CATALOGUE_BLOCK_SCALE 80.0/BLOCK_SIZE
#define PIECE_CATALOGUE_PAGE_AMOUNT 2
//#define UNDO_LIST_LENGHT 10*BLOCKS_PER_PIECE

#define MAX_CHECKPOINTS 9
#define MEDAL_BRONZ_MULT 1.6
#define MEDAL_SILVER_MULT 1.3
#define MEDAL_GOLD_MULT 1.15


typedef enum PieceCataloguePage {PAGE_DEBUG, PAGE_DEBUG_2} PieceCataloguePage; 

typedef struct PieceInfo
{
	unsigned int id;
	Vector2int placement;
	BlockRotation rot;
} PieceInfo;

typedef struct Track
{
	unsigned char author[PROFILE_NAME_LENGHT];
	bool validated,
	     has_start,
	     blockmixed;
	Vector2 start_pos,
		start_rot;
	unsigned int checkpoint_amount;
	DefaultCar car;
	DefaultEnviroment env;
	double medal_bronz,
	       medal_silver,
	       medal_gold,
	       medal_author;
	PieceInfo pieces[MAX_PIECES];
} Track;

typedef struct TrackSave
{
	unsigned int size_block,
		     size_max_block,
		     size_other;
	Track track;
} TrackSave;


bool BlockOverlap(Block block, Vector2int info_placement, Block info_block);
void SetStart(Track* track, Vector2int placement, BlockRotation rot);
void TrackBlockmixed(Track* track, bool blockmixing);

unsigned int GetPieceAdv(unsigned int piece_id, Vector2int placement, BlockRotation rot, Block piece[BLOCKS_PER_PIECE]);
unsigned int GetPiece(PieceInfo* info, Block piece[BLOCKS_PER_PIECE]);

bool AddPiece(Track* track, Block blocks[MAX_BLOCK_AMOUNT], PieceInfo* info);
bool DeletePiece(Track* track, Block blocks[MAX_BLOCK_AMOUNT], Vector2int placement);
unsigned int OverlapingPieces(Track* track, Vector2int placement);

void ClearTrack(Track* track);
void MakeTrackBlocks(Track* track, Block blocks[MAX_BLOCK_AMOUNT]);

bool CheckTrackFile(unsigned int size, unsigned int size_block, unsigned int size_max_block, unsigned int size_other);
Track LoadTrack(const char *filename);
bool SaveTrack(Track* track, const char *filename, unsigned char author[PROFILE_NAME_LENGHT]);

const char* TrackFileName(unsigned char* track_dir, unsigned char name[TRACK_NAME_LENGHT]);
void TrackNameFromFilename(const char* filename, unsigned char name[TRACK_NAME_LENGHT]);

void LoadTrackDirectory(FilePathList* fpl, unsigned char* track_dir);

void CalculateMedalTimes(Track* track);
void ResetMedalTimes(Track* track);

void MoveEditorCursor(PieceInfo* info, int x, int y);

float CataloguePosition(float initial, unsigned int piece_n);
void PiecesInPage(unsigned int page[9], unsigned int page_num);

void PutPieceIntoCatalogue(Block piece_catalogue[PIECE_CATALOGUE_LENGHT], unsigned int piece_id, unsigned int piece_count);
void LoadPieceCataloguePage(Block piece_catalogue[PIECE_CATALOGUE_LENGHT], unsigned int page[9]);

void DrawPieceCatalogue(Block piece_catalogue[PIECE_CATALOGUE_LENGHT], Vector2 position);

#endif
