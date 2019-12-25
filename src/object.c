#include <tonc.h>
#include "object.h"

static u32 NUM_OBJECTS = 1;
static Object OBJECT_ARRAY[128] = {
    {
        1,
		Player,
		10,
		16 << 8,
		16 << 8,
		100 << 8,
		100 << 8,
		0 << 8,
		0 << 8,
		0
    },

};

void collide_player(Object* player, Object* obj, u32 direction) {
    if (obj->type == Player) {
        //unreachable?!
    }
    if (obj->type == Platform) {
        if (direction == 1) {
            player->x = player->x - ((player->x + player->width) - obj->x);
            player->dx = 0;
        }
        if (direction == 2) {
            player->y = player->y - ((player->y + player->height) - obj->y);
            player->dy = 0;
        }
        if (direction == 3) {
            player->x = player->x - (player->x - (obj->x + obj->width));
            player->dx = 0;
        }
        if (direction == 4) {
            player->y = player->y - (player->y - (obj->y + obj->height));
            player->dy = 0;

        }
    }
}

void draw_player(OBJ_ATTR* attributes, Object* player) {
    obj_set_attr(
		attributes,
		ATTR0_SQUARE,
		ATTR1_SIZE_16,
		ATTR2_ID(4) | ATTR2_PALBANK(0)
	);

    obj_set_pos(attributes, fx2int(player->x), fx2int(player->y));

    player->x = player->x + player->dx;
    player->y = player->y + player->dy;

    player->dy = player->dy + 50;

    if (key_is_down(1 << KI_LEFT)) {
        player->dx = player->dx - 16;
    }
    if (key_is_down(1 << KI_RIGHT)) {
        player->dx = player->dx + 16;
    }

    if (
        (player->y < int2fx(0)) |
        (player->y + player->height > int2fx(160))
    ) {
        player->dy = -player->dy;
    }

    if (
        (player->x < int2fx(0)) |
        (player->x + player->width > int2fx(240))
    ) {
        player->dx = -player->dx;
    }
}

void collide_platform(Object* platform, Object* obj, u32 direction);
void draw_platform(OBJ_ATTR* attribute, Object* platform);

void draw_objects() {
    for (int i = 0; i < NUM_OBJECTS; i++) {
        for (int u = 0; u < NUM_OBJECTS; u++) {
            u32 direction = collide(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u]);
            if (direction && i != u ) {
                if (OBJECT_ARRAY[i].type == Player) {
                    collide_player(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u], direction);
                }
                if (OBJECT_ARRAY[i].type == Platform) {
                    collide_platform(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u], direction);
                }
            }
        }
    }

    for (int i = 0; i < NUM_OBJECTS; i ++) {
        if (OBJECT_ARRAY[i].type == Player) {
            draw_player(&oam_mem[i], &OBJECT_ARRAY[i]);
        }
        if (OBJECT_ARRAY[i].type == Platform) {
            //draw_platform(&oam_mem[i], &OBJECT_ARRAY[i]);
        }
    }
}
