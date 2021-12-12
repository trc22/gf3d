#include <math.h>

#include "gfc_matrix.h"
#include "simple_logger.h"
#include "gf3d_vgraphics.h"
#include "gf3d_camera.h"

#include "player.h"
#include "interactable.h"
#include "room.h"
#include "weapon.h"

UniformBufferObject ubo;

Item *current_item = NULL;

int input_timer = 250;

void player_update(Entity *ent);
void player_think(Entity *ent);
void player_touch(Entity* self, Entity* other);

Entity * player_spawn(Vector3D position)
{
    Entity *ent = gf3d_entity_create("misc");

    ent->update = player_update;
    ent->think = player_think;
    ent->touch = player_touch;

    ent->camera_mode = 0;


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
    Vector3D newPos = vector3d(0, 0, 0);


        if(ent->camera_mode == 0)
        {
            if(keys[SDL_SCANCODE_W])
            {
                newPos.x = ent->position.x + 0.1 * (sin(ent->rotation.z));
                newPos.y = ent->position.y - 0.1 * (cos(ent->rotation.z));
                if(room_check_bounds(newPos)) return;

                ent->position.x += 0.1 * (sin(ent->rotation.z));
                ent->position.y -= 0.1 * (cos(ent->rotation.z));
            }
            if(keys[SDL_SCANCODE_S])
            {
                newPos.x = ent->position.x - 0.1 * (sin(ent->rotation.z));
                newPos.y = ent->position.y + 0.1 * (cos(ent->rotation.z));
                if(room_check_bounds(newPos)) return;

                ent->position.x -= 0.1 * (sin(ent->rotation.z));
                ent->position.y += 0.1 * (cos(ent->rotation.z));
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

            if(keys[SDL_SCANCODE_TAB])
            {
                inventory_display();
                input_timer = 0;
            }

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
    if(pos == -1)
    {
        current_item = NULL;
        return;
    }
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
            if(item->quantity <= 0)
            {
                Item* ammo = inventory_get_item_by_id(5);
                if(ammo == NULL) break;
                player_use_item(self, ammo);
                if(item->quantity <= 0)
                {
                    slog("No ammo");
                    break;
                }

            }

            slog("Using pistol");
            item->quantity--;
            weapon_use_pistol(item, self);
            slog("Pistol fired, %i shots remaining", item->quantity);
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
            if(item->quantity <= 0)
                inventory_free_item(item);

            Item* pistol = inventory_get_item_by_id(1);
            if(pistol == NULL) break;
            int temp = 12 - pistol->quantity;
            pistol->quantity += item->quantity;
            if(pistol->quantity > 12) pistol->quantity = 12;
            item->quantity = item->quantity - temp;


            slog("Reloaded: pistol has %i shots, ammo remaining is %i", pistol->quantity, item->quantity);

            if(item->quantity <= 0)
                inventory_free_item(item);

            current_item = NULL;
            break;
    }
}

