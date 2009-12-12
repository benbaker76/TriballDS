#include <nds.h>
#include "Globals.h"

void DrawString(const char* string, int x, int y, bool sub)
{
	u16* pMap = (sub ? BG_MAP_RAM_SUB(BG0_MAP_BASE_SUB) : BG_MAP_RAM(BG0_MAP_BASE)) + (x + y * 32);
	
	for(u32 i=0; i<strlen(string); i++)
		*pMap++ = string[i];
}
