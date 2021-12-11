#include "main_menu.h"
#include "menu.h"
#include "room.h"

int mode = 0;

void start_game(Window* self);
void quit_game();

void main_menu()
{
    Window *start;
    start = window_create("images/main_menu/start.png", vector2d(400,300), 128, 256);
    start->on_click = start_game;
}

void start_game(Window* self)
{
    room_load("test_room");
    room_set_camera(vector3d(1, 10, 1),vector3d(3.3, 0, 3.14));

    window_free(self);

}
