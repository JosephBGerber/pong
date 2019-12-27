#ifndef OBJECT_H
#define OBJECT_H

#include <tonc.h>

typedef enum Type_E {
    Nothing,
    Player,
    Punch,
    Platform,
} Type;

typedef struct PlayerState_U {
    u32 jumps:1;
    u32 hflip:1;
} PlayerState;

typedef struct PunchState_U {
    u32 timer: 8;
    u32 hflip: 1;
} PunchState;

typedef union State_U {
    u32 value;
    PlayerState player;
    PunchState punch;
} State;

/// Data structure representing an object in the world
typedef struct Object_S {
    Type type;
    u8 health;
    FIXED width;
    FIXED height;
    FIXED x;
    FIXED y;
    FIXED dx;
    FIXED dy;
    State state;
} Object;

static inline bool collide (Object* obj1, Object* obj2) {
    if (
        obj1->x + obj1->width  > obj2->x &&
        obj1->y + obj1->height > obj2->y &&
        obj1->x < obj2->x + obj2->width &&
        obj1->y < obj2->y + obj2->height
    ) {
        return 1;
    }
    return 0;
}

typedef struct Collision_S {
    FIXED distance;
    enum Direction_E{
        Top,
        Right,
        Bottom,
        Left,
    } direction;
} Collision;

static inline Collision collision_direction(Object* obj1, Object* obj2) {
    Collision collision;

    u32 top = (obj2->y + obj2->height) - obj1->y;          
    u32 right = (obj2->x + obj2->width) - obj1->x;
    u32 bottom = (obj1->y + obj1->height) - obj2->y;
    u32 left = (obj1->x + obj1->width) - obj2->x;
    
    collision.distance = __INT_MAX__;

    if ((top > 0) && (top < collision.distance)) {
        collision.distance = top;
        collision.direction = Top;
    }
    if ((right > 0) && (right < collision.distance)) {
        collision.distance = right;
        collision.direction = Right;
    }
    if ((bottom > 0) && (bottom < collision.distance)) {
        collision.distance = bottom;
        collision.direction = Bottom;
    }
    if ((left > 0) && (left < collision.distance)) {
        collision.distance = left;
        collision.direction = Left;
    }

    return collision;
}
 
void init_punch(FIXED x, FIXED y, u32 hflip);
void init_platform(FIXED width, FIXED height, FIXED x, FIXED y);
void draw_objects();

#endif