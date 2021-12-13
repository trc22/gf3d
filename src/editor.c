#include "simple_logger.h"
#include "simple_json.h"
#include "editor.h"
#include "menu.h"
#include "gf3d_entity.h"

void save_level(Window* self);
void spawn_ent_door(Window* self);
void spawn_ent_pickup(Window* self);
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
    Window *save, *spawn_door, *spawn_pickup, *clear;

    editor.enabled = 1;
    editor.selected_ent = NULL;
    editor.timer = 0;

    save = window_create("images/editor/save.png", vector2d(300,0), 128, 128);
    save->on_click = save_level;
    save->active = 1;

    spawn_door = window_create("images/editor/door.png", vector2d(400,0), 128, 128);
    spawn_door->on_click = spawn_ent_door;
    spawn_door->active = 1;

    spawn_pickup = window_create("images/editor/pickup.png", vector2d(500,0), 128, 128);
    spawn_pickup->on_click = spawn_ent_pickup;
    spawn_pickup->active = 1;

    clear = window_create("images/editor/clear.png", vector2d(600,0), 128, 128);
    clear->on_click = clear_ent;
    clear->active = 1;

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
            level_editor_write_ent();
            editor.selected_ent = NULL;
            slog("Placing ent");

        }
        editor.timer = 0;

    }

}

void level_editor_save()
{
    SJson *json, *sj_room, *sj_entities;
    SJson *room_name, *room_model, *room_position, *room_scale, *room_boundaries;


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

    room_boundaries = sj_array_new();
    sj_array_append(room_boundaries, sj_new_int(20));
    sj_array_append(room_boundaries, sj_new_int(-20));
    sj_array_append(room_boundaries, sj_new_int(5));
    sj_array_append(room_boundaries, sj_new_int(-45));


    sj_room = sj_object_new();
    sj_object_insert(sj_room, "name", room_name);
    sj_object_insert(sj_room, "model", room_model);
    sj_object_insert(sj_room, "scale", room_scale);
    sj_object_insert(sj_room, "position", room_position);
    sj_object_insert(sj_room, "boundaries", room_boundaries);


    sj_entities = sj_object_new();
    sj_object_insert(editor.sj_doors, "count", sj_new_int(editor.door_count));
    sj_object_insert(editor.sj_pickups, "count", sj_new_int(editor.pickup_count));
    sj_object_insert(sj_entities, "doors",editor.sj_doors);
    sj_object_insert(sj_entities, "pickups",editor.sj_pickups);

    sj_object_insert(sj_room, "entities:", sj_entities);

    json = sj_object_new();
    sj_object_insert(json,"test_edit", sj_room);
    sj_save(json, "rooms/test_edit.json");

    sj_free(json);


}

void save_level(Window* self)
{
    level_editor_save();
}

void spawn_ent_door(Window* self)
{
    editor.selected_ent = NULL;
    editor.selected_ent = gf3d_entity_create("door");
    editor.ent_type = 0;
    slog("Spawning ent");
}
void spawn_ent_pickup(Window* self)
{
    editor.selected_ent = NULL;
    editor.selected_ent = gf3d_entity_create("cube");
    editor.ent_type = 1;
    slog("Spawning ent");
}
void clear_ent(Window* self)
{
    editor.selected_ent = NULL;
    gf3d_entity_free_all();

    editor.pickup_count = 0;
    editor.door_count = 0;

    editor.sj_doors = sj_object_new();
    editor.sj_pickups = sj_object_new();
}

void editor_close()
{
    memset(&editor,0,sizeof(EditorInfo));
}

void level_editor_write_ent()
{
    SJson* sj_ent, *sj_name, *sj_pos, *sj_dest, *sj_locked;

    sj_ent = sj_object_new();
    sj_name = sj_object_new();

    sj_pos = sj_array_new();
    sj_array_append(sj_pos, sj_new_int(editor.selected_ent->position.x));
    sj_array_append(sj_pos, sj_new_int(editor.selected_ent->position.y));
    sj_array_append(sj_pos, sj_new_int(editor.selected_ent->position.z));

    if(editor.ent_type == 0)
    {
        editor.door_count ++;

        sj_name = sj_new_str("test door");
        sj_object_insert(sj_ent, "name", sj_name);

        sj_object_insert(sj_ent, "position", sj_pos);

        sj_dest = sj_new_str("test_room");
        sj_object_insert(sj_ent, "dest", sj_dest);



        sj_object_insert(editor.sj_doors, "0", sj_ent);
    }
    else if(editor.ent_type == 1)
    {
        editor.pickup_count ++;

        sj_name = sj_new_str("test pickup");
        sj_object_insert(sj_ent, "name", sj_name);

        sj_object_insert(sj_ent, "position", sj_pos);

        sj_dest = sj_new_str("key");
        sj_object_insert(sj_ent, "item", sj_dest);

        sj_object_insert(editor.sj_pickups, "0", sj_ent);
    }
}
