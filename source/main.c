
#include <gba_base.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include "gba_sprites.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sprites.h"
#include "map.h"

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	irqInit();
	irqEnable(IRQ_VBLANK);

	REG_DISPCNT = MODE_0 | OBJ_ON| BG0_ON | OBJ_1D_MAP;

	REG_BG0CNT = BG_16_COLOR | BG_SIZE(0) | CHAR_BASE(0) | SCREEN_BASE(30);
	REG_BG0HOFS = 8;

	memcpy((void*) 0x05000000, mapPal, mapPalLen);
	memcpy(CHAR_BASE_BLOCK(0), mapTiles, mapTilesLen);
	memcpy(SCREEN_BASE_BLOCK(30), mapMap, mapMapLen);

	memcpy(OBJ_BASE_ADR, spritesTiles, spritesTilesLen);
	memcpy((void*) 0x05000200, spritesPal, spritesPalLen);

	Sprite sprite;

	sprite.Y = 25;
	sprite.RotationScaling = 0;
	sprite.Disable = 0;
	sprite.Mode = 0;
	sprite.Mosaic = 0;
	sprite.ColorMode = 0;
	sprite.Shape = 0;

	sprite.X = 25;
	sprite.NotUsed = 0;
	sprite.HFlip = 0;
	sprite.VFlip = 0;
	sprite.Size = 1;

	sprite.Character = 0;
	sprite.Priority = 0;
	sprite.Palette = 0;
	sprite.dummy = 0;

	((Sprite*)OAM)[0] = sprite;

    while(1) {
		VBlankIntrWait();

		scanKeys();

		if (keysHeld() & KEY_LEFT) {
			sprite.X -= 1;
			((Sprite*)OAM)[0] = sprite;
		}

		if (keysHeld() & KEY_RIGHT) {
			sprite.X += 1;
			((Sprite*)OAM)[0] = sprite;
		}
	}

    return 0;
}


