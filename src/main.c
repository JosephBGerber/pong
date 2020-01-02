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

	memcpy((void*)MEM_PAL_BG, bgPal, bgPalLen);
	memcpy(&tile_mem[0][0], bgTiles, bgTilesLen);
	memcpy(&tile_mem[3][0], bgMap, bgMapLen);

	memcpy((void*)MEM_PAL_OBJ, spritesPal, spritesPalLen);
	memcpy(tile_mem_obj, spritesTiles, spritesTilesLen);

	// TOP
	init_platform(240 << 8, 64 << 8, 0 << 8, -(64 << 8));
	// RIGHT
	init_platform(64 << 8, 160 << 8, 240 << 8, 0 << 8);
	// BOTTOM
	init_platform(240 << 8, 64 << 8, 0 << 8, 160 << 8);
	// LEFT
	init_platform(64 << 8, 160 << 8, -(64 << 8), 0 << 8);
	
	// LEFT WALL
	init_platform(16 << 8, 160 << 8, 0 << 8, 0 << 8);
	// RIGHT WALL
	init_platform(16 << 8, 160 << 8, 224 << 8, 0 << 8);

	// TOP LEFT PLATFORM
	init_platform(40 << 8, 8 << 8, 24 << 8, 88 << 8);
	// TOP RIGHT PLATFORM
	init_platform(40 << 8, 8 << 8, 176 << 8, 88 << 8);
	// MIDDLE PLATFORM
	init_platform(64 << 8, 8 << 8, 88 << 8, 120 << 8);

	init_ball(70 << 8, 130 << 8);

    while(1) {
		vid_vsync();
		key_poll();
		draw_objects();
	}
}