#include <tonc.h>
#include "object.h"

static u32 NUM_OBJECTS = 6;
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
    // TOP
    {
        1,
        Platform,
        0,
        240 << 8,
        64 << 8,
        0 << 8,
        -(64 << 8),
        0,
        0,
        0
    },
    // RIGHT
    {
        1,
        Platform,
        0,
        64 << 8,
        160 << 8,
        240 << 8,
        0 << 8,
        0,
        0,
        0
    },
    // BOTTOM
    {
        1,
        Platform,
        0,
        240 << 8,
        64 << 8,
        0 << 8,
        160 << 8,
        0,
        0,
        0
    },
    // LEFT
    {
        1,
        Platform,
        0,
        64 << 8,
        160 << 8,
        -(64 << 8),
        0 << 8,
        0,
        0,
        0
    },
    // MIDDLE
    {
        1,
        Platform,
        0,
        48 << 8,
        8 << 8,
        96 << 8,
        112 << 8,
        0,
        0,
        0
    },
    

};

inline static void update_player(Object* player) {
    player->x = player->x + player->dx;
    player->y = player->y + player->dy;

    if (key_is_down(BIT(KI_UP))) {
        if (player->dy == 0) {
            player->dy = player->dy + int2fx(-5);
        }
    }

    player->dy = player->dy + 50;

    if (key_is_down(BIT(KI_LEFT))) {
        player->dx = player->dx - 16;
    }
    if (key_is_down(BIT(KI_RIGHT))) {
        player->dx = player->dx + 16;
    }
}

enum Direction {
    Top,
    Right,
    Left,
    Bottom,
};

inline static void collide_player(Object* player, Object* obj, bool direction) {
    if (obj->type == Platform) {                    
        u32 top = (obj->y + obj->height) - player->y;          
        u32 right = (obj->x + obj->width) - player->x;
        u32 bottom = (player->y + player->height) - obj->y;
        u32 left = (player->x + player->width) - obj->x;
        

        u32 smallest = __INT_MAX__;
        enum Direction direction;

        if ((top > 0) && (top < smallest)) {
            smallest = top;
            direction = Top;
        }
        if ((right > 0) && (right < smallest)) {
            smallest = right;
            direction = Right;
        }
        if ((bottom > 0) && (bottom < smallest)) {
            smallest = bottom;
            direction = Bottom;
        }
        if ((left > 0) && (left < smallest)) {
            smallest = left;
            direction = Left;
        }

        if (direction == Top) {
            player->dy = 0;
            player->y = player->y + smallest;
        } else if (direction == Right) {
            player->dx = 0;
            player->x = player->x + smallest;
        } else if (direction == Bottom) {
            player->dy = 0;
            player->y = player->y - smallest;
        } else {
            player->dx = 0;
            player->x = player->x - smallest;
        }
    }
}

inline static void draw_player(OBJ_ATTR* attributes, Object* player) {
    obj_set_attr(
		attributes,
		ATTR0_SQUARE,
		ATTR1_SIZE_16,
		ATTR2_ID(4) | ATTR2_PALBANK(0)
	);

    obj_set_pos(attributes, fx2int(player->x), fx2int(player->y));
}

inline static void update_platform(Object* player) {}

inline static void collide_platform(Object* platform, Object* obj, bool direction) {}

inline static void draw_platform(OBJ_ATTR* attributes, Object* platform) {}

void draw_objects() {
    for (int i = 0; i < NUM_OBJECTS; i++) {
        switch (OBJECT_ARRAY[i].type) {
            case Player:
                update_player(&OBJECT_ARRAY[i]);
                break;
            case Platform:
                update_platform(&OBJECT_ARRAY[i]);
                break;
        }
    }

    for (int i = 0; i < NUM_OBJECTS; i++) {
        for (int u = 0; u < NUM_OBJECTS; u++) {
            if ((!OBJECT_ARRAY[u].enabled) | (i == u)) continue;

            bool direction = collide(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u]);

            if (direction) {
                switch(OBJECT_ARRAY[i].type) {
                    case Player:
                        collide_player(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u], direction);
                        break;
                    case Platform:
                        collide_platform(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u], direction);
                        break;
                }
            }
        }
    }

    for (int i = 0; i < NUM_OBJECTS; i++) {
        switch (OBJECT_ARRAY[i].type) {
            case Player:
                draw_player(&oam_mem[i], &OBJECT_ARRAY[i]);
                break;
            case Platform:
                draw_platform(&oam_mem[i], &OBJECT_ARRAY[i]);
                break;
        }
    }
}
