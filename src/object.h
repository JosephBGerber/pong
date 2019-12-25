#ifndef OBJECT_H
#define OBJECT_H

#include <tonc.h>

typedef enum Type_E {
    Player,
    Platform,
} Type;

/// Data structure representing an object in the world
typedef struct Object_S {
    bool enabled;
    Type type;
    u8 health;
    FIXED width;
    FIXED height;
    FIXED x;
    FIXED y;
    FIXED dx;
    FIXED dy;
    u32 state;
} Object;

static inline u32 collide (Object* obj1, Object* obj2) {
    if (obj1->x + obj1->width  > obj2->x) {
        return 1;
    }
    if (obj1->y + obj1->height > obj2->y) {
        return 2;
    }
    if (obj1->x < obj2->x + obj2->width) {
        return 3;
    }
    if (obj1->y < obj2->y + obj2->height) {
        return 4;
    }
    return 0;
} 

void draw_objects();

#endif