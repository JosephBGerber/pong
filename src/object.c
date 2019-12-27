#include <tonc.h>
#include "object.h"

static u32 NUM_OBJECTS = 1;
static Object OBJECT_ARRAY[128] = 
{
    {
		Player,
		10,
		8 << 8,
		8 << 8,
		100 << 8,
		100 << 8,
		0 << 8,
		0 << 8,
		{0}
    }
};

inline static void init_object(Object obj) {
    for (int i = 0; i < NUM_OBJECTS; i++){
        if (OBJECT_ARRAY[i].type == Nothing) {
            OBJECT_ARRAY[i] = obj;
            return;
        }
    }

    NUM_OBJECTS += 1;
    OBJECT_ARRAY[NUM_OBJECTS - 1] = obj;
}

inline static void draw_nothing(OBJ_ATTR* attributes) {
    obj_hide(attributes);
}

inline static void update_player(Object* player) {
    PlayerState* state = &player->state.player;
    player->x = player->x + player->dx;
    player->y = player->y + player->dy;

    if (key_is_down(KEY_UP)) {
        if (state->jumps > 0) {
            state->jumps--;
            player->dy = player->dy + int2fx(-5);
        }
    }

    player->dy = player->dy + float2fx(0.20);

    if (key_is_down(KEY_LEFT)) {
        player->dx = player->dx + float2fx(-0.08);
        state->hflip = 1;
    }
    if (key_is_down(KEY_RIGHT)) {
        player->dx = player->dx + float2fx(0.08);
        state->hflip = 0;
    }
    if (key_is_down(KEY_A) && key_was_up(KEY_A)){
        init_punch(player->x, player->y, state->hflip);
    }
}



inline static void collide_player(Object* player, Object* obj) {
    PlayerState* state = &player->state.player;

    if (obj->type == Platform) {                    
        Collision collision = collision_direction(player, obj);

        if (collision.direction == Top) {
            player->dy = 0;
            player->y = player->y + collision.distance;

        } else if (collision.direction == Right) {
            player->dx = 0;
            player->x = player->x + collision.distance;

        } else if (collision.direction == Bottom) {
            player->dy = 0;
            player->y = player->y - collision.distance;

            if (!(key_is_down(KEY_LEFT | KEY_RIGHT))) {
                // Apply friction to player
                player->dx = 0;
            }

            // Reset the jump counter for the player
            state->jumps = 1;

        } else {
            player->dx = 0;
            player->x = player->x - collision.distance;
        }
    }
}

inline static void draw_player(OBJ_ATTR* attributes, Object* player) {
    PlayerState* state = &player->state.player;
    obj_set_attr(
		attributes,
		ATTR0_SQUARE,
		ATTR1_SIZE_8 | (state->hflip << 12),
		ATTR2_ID(1) | ATTR2_PALBANK(0)
	);

    obj_set_pos(attributes, fx2int(player->x), fx2int(player->y));
}

void init_punch(FIXED x, FIXED y, u32 hflip) {
    PunchState punch_state = {
        30,
        hflip
    };

    State state;
    state.punch = punch_state;

    Object obj = {
        Punch,
        0,
        int2fx(8),
        int2fx(8),
        x,
        y,
        0,
        0,
        state,
    };

    init_object(obj);
}

inline static void update_punch(Object* punch) {
    PunchState* state = &punch->state.punch;
    
    if (state->hflip) {
        punch->x = punch->x - int2fx(2);
    } else {
        punch->x = punch->x + int2fx(2);
    }
    
    state->timer--;

    if (state->timer == 0) {
        punch->type = Nothing;
    }
}

inline static void collide_punch(Object* platform, Object* obj) {}

inline static void draw_punch(OBJ_ATTR* attributes, Object* punch) {
    PunchState* state = &punch->state.punch;
    obj_set_attr(
		attributes,
		ATTR0_SQUARE,
		ATTR1_SIZE_8 | (state->hflip << 12),
		ATTR2_ID(2) | ATTR2_PALBANK(0)
	);

    obj_set_pos(attributes, fx2int(punch->x), fx2int(punch->y));
}

void init_platform(FIXED width, FIXED height, FIXED x, FIXED y) {
    Object obj = {
        Platform,
        0,
        width,
        height,
        x,
        y,
        0,
        0,
        {0}
    };

    init_object(obj);
}



inline static void update_platform(Object* player) {}

inline static void collide_platform(Object* platform, Object* obj) {}

inline static void draw_platform(OBJ_ATTR* attributes, Object* platform) {}

void draw_objects() {
    for (int i = 0; i < NUM_OBJECTS; i++) {
        switch (OBJECT_ARRAY[i].type) {
            case Nothing:
                break;
            case Player:
                update_player(&OBJECT_ARRAY[i]);
                break;
            case Punch:
                update_punch(&OBJECT_ARRAY[i]);
                break;
            case Platform:
                update_platform(&OBJECT_ARRAY[i]);
                break;
        }
    }

    for (int i = 0; i < NUM_OBJECTS; i++) {
        for (int u = 0; u < NUM_OBJECTS; u++) {
            if ((OBJECT_ARRAY[u].type == Nothing) | (i == u)) continue;

            bool direction = collide(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u]);

            if (direction) {
                switch(OBJECT_ARRAY[i].type) {
                    case Nothing:
                        break;
                    case Player:
                        collide_player(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u]);
                        break;
                    case Punch:
                        collide_punch(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u]);
                        break;
                    case Platform:
                        collide_platform(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u]);
                        break;
                }
            }
        }
    }

    for (int i = 0; i < NUM_OBJECTS; i++) {
        switch (OBJECT_ARRAY[i].type) {
            case Nothing:
                draw_nothing(&oam_mem[i]);
                break;
            case Player:
                draw_player(&oam_mem[i], &OBJECT_ARRAY[i]);
                break;
            case Punch:
                draw_punch(&oam_mem[i], &OBJECT_ARRAY[i]);
                break;
            case Platform:
                draw_platform(&oam_mem[i], &OBJECT_ARRAY[i]);
                break;
        }
    }
}
