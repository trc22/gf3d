#ifndef __MENU_H__
#define __MENU_H__

#include "gf3d_sprite.h"
#include "bounding_box.h"

typedef struct Window_S
{
    Uint8 _inuse;
    Sprite *sprite;

    Vector2D position;
    BoundingBox boundingBox;

    void (*on_click) (struct Window_S* self);

    void *data;
}Window;

void menu_init(Uint32 window_max);

Window * window_new();

void menu_update();

void menu_draw_all();

void window_draw(Window *window);

int window_check(Window *window, Vector2D mousePos);

void window_check_all(Vector2D mousePos);

void menu_free();

void window_free(Window* self);

Window *window_create(char* image, Vector2D position, int frame_height, int frame_width);

#endif
