#include "simple_logger.h"
#include "simple_json.h"
#include "editor.h"
#include "menu.h"
#include "gf3d_entity.h"

void save_level(Window* self);
void spawn_ent(Window* self);
void clear_ent(Window* self);

int timer_input = 100;

Entity * selected_ent = NULL;

void level_editor_init()
{
    Window *save, *spawn;

    save = window_create("images/editor/save.png", vector2d(300,0), 128, 128);
    save->on_click = save_level;
    save->active = 1;

    spawn = window_create("images/editor/pickup.png", vector2d(400,0), 128, 128);
    spawn->on_click = spawn_ent;
    spawn->active = 1;
}

void level_editor_click();

void level_editor_update()
{
    Uint32 mouse;
    int x, y;

    mouse = SDL_GetMouseState(&x, &y);

    if(selected_ent != NULL)
    {
        selected_ent->position.x = x - 400;
        selected_ent->position.y = -y + 300;
    }

    if(timer_input != 100)
    {
        timer_input++;
        return;
    }

    if((mouse & SDL_BUTTON_RMASK) != 0)
    {
        if(selected_ent != NULL)
        {
            selected_ent = NULL;
            slog("Placing ent");
        }
        timer_input = 0;

    }

}

void save_level(Window* self)
{
    window_free(self);
}

void spawn_ent(Window* self)
{
    selected_ent = NULL;
    selected_ent = gf3d_entity_create("cube");
    slog("Spawning ent");
}
void clear_ent(Window* self);
