#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "interactable.h"

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
    switch(interact->type)
    {
        case IT_Inspect:
            break;
        case IT_Door:
            break;
        case IT_Pickup:
            break;
        case IT_Button:
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


