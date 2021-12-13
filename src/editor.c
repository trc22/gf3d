#include "simple_logger.h"
#include "simple_json.h"
#include "editor.h"
#include "menu.h"
#include "gf3d_entity.h"

void save_level(Window* self);
void spawn_ent(Window* self);
void clear_ent(Window* self);

void editor_close();
void editor_json_insert(int type);

typedef struct
{
    Uint8 enabled;
    Entity* selected_ent;
    int door_count, pickup_count, ent_type, timer;
    SJson *sj_pickups, *sj_doors;
}EditorInfo;

static EditorInfo editor = {0};

void level_editor_init()
{
    Window *save, *spawn;

    editor.enabled = 1;
    editor.selected_ent = NULL;
    editor.timer = 0;

    save = window_create("images/editor/save.png", vector2d(300,0), 128, 128);
    save->on_click = save_level;
    save->active = 1;

    spawn = window_create("images/editor/pickup.png", vector2d(400,0), 128, 128);
    spawn->on_click = spawn_ent;
    spawn->active = 1;

    editor.sj_doors = sj_object_new();
    editor.sj_pickups = sj_object_new();
    editor.door_count = 0;
    editor.pickup_count = 0;

    atexit(editor_close);
}

void level_editor_click();

void level_editor_update()
{
    Uint32 mouse;
    int x, y;

    if(editor.enabled != 1)
        return;

    mouse = SDL_GetMouseState(&x, &y);

    if(editor.selected_ent != NULL)
    {
        editor.selected_ent->position.x =  x - 400;
        editor.selected_ent->position.y = -y + 300;
    }

    if(editor.timer != 100)
    {
        editor.timer++;
        return;
    }

    if((mouse & SDL_BUTTON_RMASK) != 0)
    {
        if(editor.selected_ent != NULL)
        {
            editor.selected_ent = NULL;
            slog("Placing ent");
        }
        editor.timer = 0;

    }

}

void level_editor_save()
{
    SJson *json, *sj_room;
    SJson *room_name, *room_model, *room_position, *room_scale;


    room_name = sj_new_str("test room");
    room_model = sj_new_str("room");

    room_scale = sj_array_new();
    sj_array_append(room_scale, sj_new_int(5));
    sj_array_append(room_scale, sj_new_int(10));
    sj_array_append(room_scale, sj_new_int(5));

    room_position = sj_array_new();
    sj_array_append(room_position, sj_new_int(1));
    sj_array_append(room_position, sj_new_int(1));
    sj_array_append(room_position, sj_new_int(-12));


    sj_room = sj_object_new();
    sj_object_insert(sj_room, "name", room_name);
    sj_object_insert(sj_room, "model", room_model);
    sj_object_insert(sj_room, "scale", room_scale);
    sj_object_insert(sj_room, "position", room_position);


    json = sj_object_new();
    sj_object_insert(json,"test_edit", sj_room);
    sj_save(json, "rooms/test_edit.json");

    sj_free(json);


}

void save_level(Window* self)
{
    level_editor_save();
}

void spawn_ent(Window* self)
{
    editor.selected_ent = NULL;
    editor.selected_ent = gf3d_entity_create("cube");
    editor.ent_type = 0;
    editor.pickup_count ++;
    slog("Spawning ent");
}
void clear_ent(Window* self);

void editor_close()
{
    memset(&editor,0,sizeof(EditorInfo));
}
