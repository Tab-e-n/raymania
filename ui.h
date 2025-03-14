#ifndef RM_UI
#define RM_UI

#include <raylib.h>
#include "rmlib.h"
#include "profiles.h"
#include "asset.h"

#define FILE_LIST_PAGE_ITEMS 11

#define MAX_PARTY_PROFILES 10
#define MAX_PARTY_TIMER 20
#define MIN_PARTY_TIMER 1


typedef enum OptionPages
{
	OPTPAGE_MAIN,
	OPTPAGE_PROFILES,
	OPTPAGE_GAMEPLAY,
	OPTPAGE_AUDIO,
	OPTPAGE_EDITOR,
	OPTPAGE_PARTY,
} OptionPages;


void MoveFileListCursor(unsigned int count, int* current, int move);
void DrawFileList(FilePathList fpl, int current, Color bg1, Color bg2);
void ChangeToDirectory(unsigned char* dir, unsigned char* new_dir, bool overwrite);
void ReturnToParentDirectory(unsigned char* dir);

void DrawCursor(Vector2 cursor_pos, float size, Color color);
void DrawGuideArrow(Vector2 start, Vector2 target);

void DrawPartyMenu(int current_opt, unsigned int party_count, Profile* profile);

void DrawOptions(int current, int page, int max, Profile* profile);
void DrawOptionsCustomization(int car, int current, int max, int set);
void DrawOptionsCars(int current, int max, unsigned char palette);
void DrawOptionsPalettes(int current, int max);


#endif
