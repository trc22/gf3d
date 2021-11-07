#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "simple_json.h"

#include "room.h"
#include "inventory.h"

#include "interactable.h"

void use_door(Interactable *door);
void use_inspect(Interactable *inspect);
void use_pickup(Interactable *pickup);
void use_button(Interactable *button);
void use_box(Interactable *box);

typedef struct
{
    Interactable *interactable_list;
    Uint32  interact_max;
}Interactable_manager;

static Interactable_manager interactable_manager = {0};

void interactable_init(Uint32 interact_max)
{
    interactable_manager.interactable_list = (Interactable*)gfc_allocate_array(sizeof(Interactable),interact_max);
    interactable_manager.interact_max = interact_max;
    if (!interactable_manager.interactable_list)
    {
        slog("failed to allocate interactable list");
        return;
    }
    for(int i = 0; i < interactable_manager.interact_max; i++)
    {
        interactable_manager.interactable_list[i]._inuse = 0;
    }
    slog("Interactable system initialized");
    atexit(interactable_close);
}

Interactable* interactable_new()
{
    int i;
    for (i = 0; i < interactable_manager.interact_max; i++)
    {
        if (interactable_manager.interactable_list[i]._inuse == 1)continue;
        //. found a free entity
        memset(&interactable_manager.interactable_list[i],0,sizeof(Interactable));
        interactable_manager.interactable_list[i]._inuse = 1;
        return &interactable_manager.interactable_list[i];
    }
    slog("request for interactable failed: all full up");
    return NULL;
}

void interactable_free(Interactable *interact)
{
    if (!interact)
    {
        slog("item pointer is not valid");
        return;
    }
    interact->_inuse = 0;
    if (interact->data != NULL)
    {
        slog("warning: data not freed at interactable free!");
    }
}
void interactable_free_all()
{
    for(int i = 0; i < interactable_manager.interact_max; i++)
    {
        interactable_free(&interactable_manager.interactable_list[i]);
    }
}

void interactable_interact(Interactable *interact)
{
    if(!interact) return;
    if(interact->_inuse == 0) return;
    switch(interact->type)
    {
        case IT_Inspect:
            slog("It's a: %s", interact->name);
            break;
        case IT_Door:
            slog("Switching rooms.");
            use_door(interact);
            break;
        case IT_Pickup:
            slog("Picking up item");
            use_pickup(interact);
            break;
        case IT_Button:
            slog("You push a button.");
            use_button(interact);
            break;
        case IT_Box:
            slog("Opening item box.");
            use_box(interact);
            break;
    }
}

void interactable_close()
{
    if(interactable_manager.interactable_list != NULL)
    {
        free(interactable_manager.interactable_list);
    }
    memset(&interactable_manager,0,sizeof(Interactable_manager));
}

void use_door(Interactable* door)
{
    if(!door) return;
    if(door->dest == NULL) return;

    if(door->locked)
    {
        slog("Door is locked");
        return;
    }
    interactable_free_all();
    room_change(door->dest);
}

void use_inspect(Interactable *inspect)
{
    if(!inspect) return;
    if(inspect->inspectText == NULL) return;

    slog("%s", inspect->inspectText);
}

void use_pickup(Interactable *pickup)
{
    if(!pickup) return;
    if(pickup->itemName == NULL) return;

    if(inventory_load_item(pickup->itemName) == NULL)
    {
        slog("Inventory is full!");
        return;
    }

    slog("Item %s was picked up", pickup->itemName);

    interactable_free(pickup);
}

void use_button(Interactable *button)
{
    int i;

    if(!button) return;
    if(button->dest == NULL) return;

    slog("You press the button");

    for (i = 0; i < interactable_manager.interact_max; i++)
    {
        if (interactable_manager.interactable_list[i]._inuse == 0)continue;

        if (strcmp(interactable_manager.interactable_list[i].name, button->dest) == 0)
        {
            interactable_manager.interactable_list[i].locked = 0;
            slog("A door is now unlocked.");
            interactable_free(button);
            return;
        }
    }

    slog("Nothing happened.");

}
void use_box(Interactable *box)
{
    inventory_item_box();
    interactable_free(box);
}
