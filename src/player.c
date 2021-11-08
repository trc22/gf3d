#include <math.h>

#include "gfc_matrix.h"
#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"

#include "player.h"
#include "interactable.h"
#include "room.h"

UniformBufferObject ubo;

Item *current_item = NULL;

int input_timer = 250;

void player_update(Entity *ent);
void player_think(Entity *ent);
void player_touch(Entity* self, Entity* other);

void player_inventory_inputs(const Uint8 *keys);

Entity * player_spawn(Vector3D position)
{
    Entity *ent = gf3d_entity_create("misc");

    ent->update = player_update;
    ent->think = player_think;
    ent->touch = player_touch;

    ent->camera_mode = 0;

    inventory_init(8);

    inventory_load_item("test item");
    inventory_load_item("pistol");
    inventory_load_item("key");
    inventory_load_item("health pack");
    inventory_load_item("ammo_pistol");
    inventory_load_item("knife");



    gf3d_entity_set_bounding_box(ent, 1, 1, 5, 5);

    ent->healthmax = 100;
    ent->health = 100;

    current_item = NULL;

    //slog("Bounding box for player: %f to %f", ent->boundingBox->minExtent.x, ent->boundingBox->maxExtent.x);

    vector3d_dup(ent->position);

    return ent;
}

void player_think(Entity *ent)
{
    int x, y;
    const Uint8 * keys;
    Uint32 mouse;

    SDL_PumpEvents();

    keys = SDL_GetKeyboardState(NULL);
    mouse = SDL_GetMouseState(&x, &y);

        if(ent->camera_mode == 0)
        {
            if(keys[SDL_SCANCODE_W])
            {
                //gfc_matrix_translate(ent->modelMat, (vector3d(ent->modelMat[0][1] * 0.075, ent->modelMat[0][0] * -0.075, 0)));    ent->position = vector3d(ent->modelMat[3][0],ent->modelMat[3][1], ent->modelMat[3][2]);
                ent->position.x += 0.1 * (sin(ent->rotation.z));
                ent->position.y -= 0.1 * (cos(ent->rotation.z));
                bounding_box_update(ent->boundingBox, ent->position);
                //slog("%f, %f, %f", ent->boundingBox->minExtentPos.x, ent->boundingBox->minExtentPos.y, ent->boundingBox->minExtentPos.z);
            }
            if(keys[SDL_SCANCODE_S])
            {
                ent->position.x -= 0.1 * (sin(ent->rotation.z));
                ent->position.y += 0.1 * (cos(ent->rotation.z));
                bounding_box_update(ent->boundingBox, ent->position);
            }
        }
        else if(input_timer == 250)
        {
            if ((mouse & SDL_BUTTON_LMASK) != 0)
            {
                /*Vector3D temp;
                ubo = gf3d_vgraphics_get_uniform_buffer_object();

                slog("Mouse cursor is at %i, %i", x, y);

                vector3d_copy(temp,gfc_unproject(vector3d(x, y, 0), ubo.view, ubo.proj, vector4d(200, 300, 800, 600)));

                slog("%f, %f, %f", temp.x, temp.y, temp.z);

                */
                player_use_item(ent, current_item);
                input_timer = 0;
            }
        }
        if(keys[SDL_SCANCODE_A])
        {
            if(ent->camera_mode == 1)
            {
                ent->rotation.z += 0.0025;
                gf3d_camera_set_rotation(vector3d(3.14, 0, 3.14 + ent->rotation.z));
            }
            else
                ent->rotation.z += 0.0075;

        }
        if(keys[SDL_SCANCODE_D])
        {
            if(ent->camera_mode == 1)
            {
                ent->rotation.z -= 0.0025;
                gf3d_camera_set_rotation(vector3d(3.14, 0, 3.14 + ent->rotation.z));
            }
            else
                ent->rotation.z -= 0.0075;
        }


        if(input_timer == 250)
        {   if(keys[SDL_SCANCODE_LSHIFT])
            {
                player_camera_fps(ent);

                input_timer = 0;
            }
            if(keys[SDL_SCANCODE_E])
            {
                ent->interactable = NULL;
                gf3d_entity_overlap_all();

                if(ent->interactable)
                {
                    interactable_interact(ent->interactable);
                    ent->interactable = NULL;
                }
                input_timer = 0;
            }

            player_inventory_inputs(keys);

        }
        else
            input_timer += 1;
}

void player_update(Entity* ent)
{

}

void player_camera_fps(Entity* ent)
{

    if(ent->camera_mode == 1)
    {
        ent->camera_mode = 0;
        room_camera_enable();
        return;
    }

    gf3d_camera_set_position(ent->position);
    gf3d_camera_set_rotation(vector3d(3.14, 0, 3.14 + ent->rotation.z));

    ent->camera_mode = 1;
}

void player_touch(Entity* self, Entity* other)
{
    if(!self || !other)
        return;
    //slog("collision");
    if(other->interactable)
        self->interactable = other->interactable;
    else
        self->interactable = NULL;
}

void player_set_current_item(int pos)
{
    current_item = inventory_get_item(pos);
    if(current_item == NULL)
    {
        slog("Current item: nothing");
        return;
    }

    slog("Current item: %s", current_item->name);

}

void player_use_item(Entity* self, Item *item)
{
    if(!self) return;
    if(!item) return;
    if(item == NULL) return;
    if(item->_inuse != 1);

    switch(item->id)
    {
        case 0:
            break;
        case 1:
            slog("Using pistol");
            Item* ammo = inventory_get_item_by_id(5);
            if(ammo == NULL)
            {
                slog("No ammo");
                break;
            }
            ammo->quantity--;
            slog("Pistol fired, %i bullets remaining", ammo->quantity);
            if(ammo->quantity <= 0)
                inventory_free_item(ammo);
            break;
        case 2:
            slog("Using knife");
            gf3d_entity_check_enemies_distance(self);
            break;
        case 3:
            slog("Using health kit");
            self->health += 25;
            if(self->health > self->healthmax)
                self->health = self->healthmax;
            slog("New health: %f", self->health);
            inventory_free_item(item);
            current_item = NULL;
            break;
        case 4:
            slog("Using key");
            if(self->interactable != NULL)
            {
                self->interactable->locked = 0;
                inventory_free_item(item);
                current_item = NULL;
            }
            break;
        case 5:
            break;
    }
}

void player_inventory_inputs(const Uint8* keys)
{
    if(keys[SDL_SCANCODE_TAB])
    {
        inventory_display_();
        input_timer = 0;
    }
    else if(keys[SDL_SCANCODE_1])
    {
        player_set_current_item(0);
        input_timer = 0;
    }
    else if(keys[SDL_SCANCODE_2])
    {
        player_set_current_item(1);
        input_timer = 0;
    }
    else if(keys[SDL_SCANCODE_3])
    {
        player_set_current_item(2);
        input_timer = 0;
    }
    else if(keys[SDL_SCANCODE_4])
    {
        player_set_current_item(3);
        input_timer = 0;
    }
    else if(keys[SDL_SCANCODE_5])
    {
        player_set_current_item(4);
        input_timer = 0;
    }
    else if(keys[SDL_SCANCODE_6])
    {
        player_set_current_item(5);
        input_timer = 0;
    }
    else if(keys[SDL_SCANCODE_7])
    {
        player_set_current_item(6);
        input_timer = 0;
    }
    else if(keys[SDL_SCANCODE_8])
    {
        player_set_current_item(7);
        input_timer = 0;
    }

}

