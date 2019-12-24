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

	obj_set_attr(
		&oam_mem[0],
		ATTR0_SQUARE,
		ATTR1_SIZE_16,
		ATTR2_PALBANK(0) | 4
	);

	obj_set_pos(&oam_mem[0], 15, 30);

	FIXED y = int2fx(0);
	FIXED x = int2fx(0);

	FIXED dy = int2fx(2);
	FIXED dx = int2fx(2);

    while(1) {
		vid_vsync();

		y += dy;
		x += dx;

		obj_set_pos(&oam_mem[0], fx2int(x), fx2int(y));

		if ((y > int2fx(160 - 16)) | (y < int2fx(0))) {
			dy = -(dy + (5 - (rand() % 10)));
		}

		if ((x > int2fx(240 - 16)) | (x < int2fx(0))) {
			dx = -(dx + (5 - (rand() % 10)));
		}
	}

    return 0;
}
