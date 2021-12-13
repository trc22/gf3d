#include <stdlib.h>
#include <string.h>
#include "simple_json.h"
#include "simple_logger.h"

#include "gf3d_entity.h"
#include "player.h"
#include "room.h"
#include "enemy.h"

Room *room;

void room_new()
{
    room = (Room*)gfc_allocate_array(sizeof(Room),1);
    if(!room)
    {
        slog("ERROR: Cannot allocate memory for room");
    }
    room->currentCamera = (Camera*)gfc_allocate_array(sizeof(Camera),1);
}

void room_load(char* room_name)
{
    TextLine assetname;

    SJson* json, *sj_room, *sj_model, *sj_temp, *sj_extents, *sj_position, *sj_scale;
    Entity*_player, *button;
    const char *model_name;

    snprintf(assetname,GFCLINELEN,"rooms/%s.json",room_name);
    slog("%s", assetname);

    room_new();


    if(!room)
    {
        slog("Room loading failed");
        return;
    }

    json = sj_load(assetname);

    if(!json) return;

    room->name = strdup(room_name);

    sj_room = sj_object_get_value(json, room->name);

    slog("%s", room->name);

    gfc_matrix_identity(room->modelMat);

    //Load room model
    sj_model = sj_object_get_value(sj_room, "model");
    model_name = sj_get_string_value(sj_model);
    slog("%s", model_name);
    room->model = gf3d_model_load(strdup(model_name));

    //Set room boundaries
    sj_extents = sj_object_get_value(sj_room, "boundaries");
    sj_temp = sj_array_get_nth(sj_extents, 0);
    sj_get_float_value(sj_temp, &room->extents.x);
    slog("%f", room->extents.x);
    sj_temp = sj_array_get_nth(sj_extents, 1);
    sj_get_float_value(sj_temp, &room->extents.y);
    slog("%f", room->extents.y);
    sj_temp = sj_array_get_nth(sj_extents, 2);
    sj_get_float_value(sj_temp, &room->extents.z);
    slog("%f", room->extents.z);
    sj_temp = sj_array_get_nth(sj_extents, 3);
    sj_get_float_value(sj_temp, &room->extents.w);
    slog("%f", room->extents.w);


    vector3d_dup(room->position);

    sj_scale = sj_object_get_value(sj_room, "scale");

    sj_temp = sj_array_get_nth(sj_scale, 0);
    sj_get_float_value(sj_temp, &room->scale.x);

    sj_temp = sj_array_get_nth(sj_scale, 1);
    sj_get_float_value(sj_temp, &room->scale.y);

    sj_temp = sj_array_get_nth(sj_scale, 2);
    sj_get_float_value(sj_temp, &room->scale.z);


    sj_position = sj_object_get_value(sj_room, "position");

    sj_temp = sj_array_get_nth(sj_position, 0);
    sj_get_float_value(sj_temp, &room->position.x);

    sj_temp = sj_array_get_nth(sj_position, 1);
    sj_get_float_value(sj_temp, &room->position.y);

    sj_temp = sj_array_get_nth(sj_position, 2);
    sj_get_float_value(sj_temp, &room->position.z);


    room->rotation = vector3d(0, 0, 0);

    room_load_entities(sj_room);

    if(strcmp(room_name, "test_edit"))
        _player = player_spawn(vector3d(0, 0, 0));

    if(!strcmp(room_name, "boss_room"))
    {
        enemy_spawn_boss(vector3d(0,-10,10));
        gf3d_camera_set_rotation(vector3d(-1.7, 0, 0));
        gf3d_camera_set_position(vector3d(0, -5, 80));
        vector3d_copy(room->currentCamera->rotation, vector3d(-1.7, 0, 0));
        vector3d_copy(room->currentCamera->position, vector3d(0, -5, 80));
        vector3d_copy(_player->position, vector3d(0, 20, 0));

        button = gf3d_entity_create_interactable("cube", IT_Button, "button");
        button->interactable->dest = "";
        gf3d_entity_set_bounding_box(button, 2, 2, 1, 1);
        vector3d_copy(button->position, vector3d(0, 30, -10));

        button = gf3d_entity_create_interactable("cube", IT_Button, "button");
        button->interactable->dest = "";
        gf3d_entity_set_bounding_box(button, 2, 2, 1, 1);
        vector3d_copy(button->position, vector3d(40, -15, -10));

        button = gf3d_entity_create_interactable("cube", IT_Button, "button");
        button->interactable->dest = "";
        gf3d_entity_set_bounding_box(button, 2, 2, 1, 1);
        vector3d_copy(button->position, vector3d(-40, -15, -10));

        enemy_set_player(_player);

    }

    sj_free(json);

}

void room_load_entities(SJson *sj_room)
{
    SJson* sj_temp, *sj_entities, *sj_doors, *sj_door, *sj_pickups, *sj_pickup;
    SJson* sj_ent_name, *sj_ent_pos, *sj_ent_dest;

    Entity* ent;

    int count, i;


    char num;

    Vector3D ent_pos = vector3d(0, 0, 0);


    if(!sj_room) return;

    slog("Loading entities!");

    sj_entities = sj_object_get_value(sj_room, "entities");

    sj_doors = sj_object_get_value(sj_entities, "doors");
    sj_temp = sj_object_get_value(sj_doors, "count");
    sj_get_integer_value(sj_temp, &count);

    slog("door count: %i", count);
    for(i = 0; i < count; i++)
    {
        num = i + '0';
        sj_door = sj_object_get_value(sj_doors,&num);

        sj_ent_pos = sj_object_get_value(sj_door, "position");
        sj_temp = sj_array_get_nth(sj_ent_pos, 0);

        sj_get_float_value(sj_temp, &ent_pos.x);
        slog("ent pos x: %f", ent_pos.x);

        sj_temp = sj_array_get_nth(sj_ent_pos, 1);
        sj_get_float_value(sj_temp, &ent_pos.y);
        slog("ent pos y: %f", ent_pos.y);

        sj_temp = sj_array_get_nth(sj_ent_pos, 2);
        sj_get_float_value(sj_temp, &ent_pos.z);
        slog("ent pos z: %f", ent_pos.z);

        sj_ent_name = sj_object_get_value(sj_door, "name");

        sj_ent_dest = sj_object_get_value(sj_door, "dest");

        ent = gf3d_entity_create_interactable("door", 3, strdup(sj_get_string_value(sj_ent_name)));
        slog("%s", ent->interactable->name);


        vector3d_copy(ent->position, ent_pos);

        gf3d_entity_set_bounding_box(ent, -2, 1, 10, 10);

        vector3d_copy(ent->scale, vector3d(2, 2, 2));

        ent->interactable->dest = strdup(sj_get_string_value(sj_ent_dest));
        slog("door dest: %s", ent->interactable->dest);

    }
    sj_pickups = sj_object_get_value(sj_entities, "pickups");
    sj_temp = sj_object_get_value(sj_pickups, "count");
    sj_get_integer_value(sj_temp, &count);

    slog("pickup count: %i", count);
    for(i = 0; i < count; i++)
    {
        num = i + '0';
        sj_pickup = sj_object_get_value(sj_pickups,&num);

        sj_ent_pos = sj_object_get_value(sj_pickup, "position");
        sj_temp = sj_array_get_nth(sj_ent_pos, 0);

        sj_get_float_value(sj_temp, &ent_pos.x);
        slog("ent pos x: %f", ent_pos.x);

        sj_temp = sj_array_get_nth(sj_ent_pos, 1);
        sj_get_float_value(sj_temp, &ent_pos.y);
        slog("ent pos y: %f", ent_pos.y);

        sj_temp = sj_array_get_nth(sj_ent_pos, 2);
        sj_get_float_value(sj_temp, &ent_pos.z);
        slog("ent pos z: %f", ent_pos.z);

        sj_ent_name = sj_object_get_value(sj_pickup, "name");

        sj_ent_dest = sj_object_get_value(sj_pickup, "item");

        ent = gf3d_entity_create_interactable("cube", 0, strdup(sj_get_string_value(sj_ent_name)));

        //vector3d_copy(ent->scale, vector3d(2, 2, 2));
        vector3d_copy(ent->position, ent_pos);

        gf3d_entity_set_bounding_box(ent, 2, 2, 1, 1);

        ent->interactable->itemName = strdup(sj_get_string_value(sj_ent_dest));
    }
}

void room_draw()
{
    if(!room)
        return;
    gf3d_model_draw(room->model, room->modelMat);
}

void room_free()
{
    if(room != NULL) return;

    memset(room,0,sizeof(Room));
}

void room_set_camera(Vector3D pos, Vector3D rot)
{
    vector3d_copy(room->currentCamera->position, pos);
    vector3d_copy(room->currentCamera->rotation, rot);
    slog("current camera rotation.z = %f", room->currentCamera->rotation.z);
}
void room_camera_enable()
{
    gf3d_camera_set_position(room->currentCamera->position);
    gf3d_camera_set_rotation(room->currentCamera->rotation);
}

void room_change(char* room_name)
{
    room_free();
    gf3d_entity_free_all();
    room_load(room_name);
}

int room_check_bounds(Vector3D position)
{
    if(!room) return 0;

    if(position.x >= room->extents.x) return 1;
    if(position.x <= room->extents.y) return 1;
    if(position.y >= room->extents.z) return 1;
    if(position.y <= room->extents.w) return 1;

    return 0;
}

void room_update()
{
    if (!room)return;

    gfc_matrix_identity(room->modelMat);
    gfc_matrix_scale(room->modelMat, room->scale);

    gfc_matrix_rotate(room->modelMat, room->modelMat, room->rotation.z, vector3d(0, 0, 1));
    gfc_matrix_rotate(room->modelMat, room->modelMat, room->rotation.y, vector3d(0, 1, 0));
    gfc_matrix_rotate(room->modelMat, room->modelMat, room->rotation.x, vector3d(1, 0, 0));

    gfc_matrix_translate(room->modelMat, room->position);

}
