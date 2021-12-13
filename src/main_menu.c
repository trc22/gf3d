#include "main_menu.h"
#include "menu.h"
#include "room.h"
#include "player.h"
#include "editor.h"


int mode = 0;

void start_game(Window* self);
void level_editor(Window* self);
void quit_game();

Window *start, *level_edit;

void main_menu()
{

    start = window_create("images/main_menu/start.png", vector2d(300,200), 128, 256);
    start->on_click = start_game;
    start->active = 1;

    level_edit = window_create("images/main_menu/level_edit.png", vector2d(300,300), 128, 256);
    level_edit->on_click = level_editor;
    level_edit->active = 1;
}

void start_game(Window* self)
{
    window_free(level_edit);

    room_load("test_room");

    room_set_camera(vector3d(1, 10, 1),vector3d(3.3, 0, 3.14));

    inventory_init(8);
    inventory_load_item("pistol");//->icon= window_create("images/items/test_2.png", vector2d(100,100), 128, 128);
    inventory_load_item("key");//->icon= window_create("images/items/test_3.png", vector2d(100,100), 128, 128);
    inventory_load_item("ammo_pistol")->combine = 2;
    inventory_load_item("knife")->combine = 5;
    inventory_load_item("shotgun");
    inventory_load_item("rifle");


    set_loading(0);

    window_free(self);
}

void level_editor(Window* self)
{
    window_free(start);

    room_load("test_edit");
    gf3d_camera_set_rotation(vector3d(-1.57, 0, 0));
    gf3d_camera_set_position(vector3d(0, -10, 100));
    set_loading(0);

    level_editor_init();
    window_free(self);
}
