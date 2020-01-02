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


/// Player
inline static void update_player(Object* player) {
    PlayerState* state = &player->state.player;
    player->x = player->x + player->dx;
    player->y = player->y + player->dy;

    if (key_is_down(KEY_A)) {
        if (state->jump == 12) {
            player->dy = int2fx(-2);
        }

        if (state->jump > 0) {
            state->jump--;
            player->dy = player->dy - float2fx(0.3);
        }
    }

    if (key_released(KEY_A)) {
        state->jump = 0;
    }

    player->dy = player->dy + float2fx(0.2);

    if (key_hit(KEY_B)){
        init_punch(player->x, player->y, state->hflip);
    }
    
    if (key_is_down(KEY_LEFT)) {
        player->dx = player->dx - float2fx(0.08);
        state->hflip = 1;
    }
    if (key_is_down(KEY_RIGHT)) {
        player->dx = player->dx + float2fx(0.08);
        state->hflip = 0;
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

            if (
                (key_is_up(KEY_LEFT) && player->dx < 0) ||
                (key_is_up(KEY_RIGHT) && player->dx > 0)
            ) {
                // Apply friction to player
                player->dx = fxmul(player->dx, float2fx(0.70));
            }

            // Reset the jump value for the player
            state->jump = 12;

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


/// Punch
void init_punch(FIXED x, FIXED y, u32 hflip) {
    State state = {
        .punch = {
            30,
            hflip
        }
    };

    FIXED dx;
    if (hflip) {
        dx = int2fx(-2);
    } else {
        dx = int2fx(2);
    }

    Object obj = {
        Punch,
        0,
        int2fx(8),
        int2fx(8),
        x,
        y,
        dx,
        0,
        state,
    };

    init_object(obj);
}

inline static void update_punch(Object* punch) {
    PunchState* state = &punch->state.punch;

    punch->x = punch->x + punch->dx;
    
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


/// Ball
void init_ball(FIXED x, FIXED y) {
    Object obj = {
        Ball,
        0,
        int2fx(16),
        int2fx(16),
        x,
        y,
        0,
        0,
        {0},
    };

    init_object(obj);
}

static inline void update_ball(Object* ball) {
    ball->x = ball->x + ball->dx;
    ball->y = ball->y + ball->dy;

    ball->dy = ball->dy + float2fx(0.2);
}

static inline void collide_ball(Object* ball, Object* obj) {
    
    if (obj->type == Platform) {
        Collision collision = collision_direction(ball, obj);

        switch (collision.direction) {
            case Top:
                ball->y = ball->y + collision.distance;
                break;
            case Right:
                ball->x = ball->x + collision.distance;
                break;
            case Bottom:
                ball->y = ball->y - collision.distance;
                break;
            case Left:
                ball->x = ball->x - collision.distance;
                break;
        }

        if ((collision.direction == Top) | (collision.direction == Bottom)) {
            ball->dy = fxmul(-ball->dy, float2fx(0.95));
        } else {
            ball->dx = fxmul(-ball->dx, float2fx(0.95));
        }
    }

    if (obj->type == Punch) {
        ball->dx = ball->dx + obj->dx;
        ball->dy = ball->dy + obj->dy + float2fx(0.3);
    }
}

static inline void draw_ball(OBJ_ATTR* attributes, Object* ball) {
    obj_set_attr(
		attributes,
		ATTR0_SQUARE,
		ATTR1_SIZE_16,
		ATTR2_ID(4) | ATTR2_PALBANK(0)
	);

    obj_set_pos(attributes, fx2int(ball->x), fx2int(ball->y));
}


/// Platform
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
            case Ball:
                update_ball(&OBJECT_ARRAY[i]);
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
                    case Ball:
                        collide_ball(&OBJECT_ARRAY[i], &OBJECT_ARRAY[u]);
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
            case Ball:
                draw_ball(&oam_mem[i], &OBJECT_ARRAY[i]);
                break;
        }
    }
}
