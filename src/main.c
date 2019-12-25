#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tonc.h>
#include "sprites.h"
#include "bg.h"
#include "object.h"

int main(void) {

	REG_DISPCNT =  DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;

	REG_BG0CNT = BG_4BPP | BG_SIZE(0) | BG_CBB(0) | BG_SBB(24);
	REG_BG0HOFS = 8;

	memcpy((void*)MEM_PAL_BG, bgPal, bgPalLen);
	memcpy(&tile_mem[0][0], bgTiles, bgTilesLen);
	memcpy(&tile_mem[3][0], bgMap, bgMapLen);

	memcpy((void*)MEM_PAL_OBJ, spritesPal, spritesPalLen);
	memcpy(tile_mem_obj, spritesTiles, spritesTilesLen);

    while(1) {
		vid_vsync();
		key_poll();
		draw_objects();
	}
}
