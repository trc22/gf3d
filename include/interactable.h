#ifndef __INTERACTABLE_H__
#define __INTERACTABLE_H__

#include "gfc_types.h"

typedef enum {
    IT_Pickup,
    IT_Button,
    IT_Inspect,
    IT_Door,
    IT_Box

}InteractType;

typedef struct Interactable_S
{
    Uint32 _inuse;

    char* name;
    InteractType type;

    char *dest;
    int locked;

    char* inspectText;

    char* itemName;

    void (*interact) ();   /**<Custom interact function if needed*/

    void* data;

}Interactable;

void interactable_init(Uint32 interact_max);

Interactable* interactable_new();

void interactable_free(Interactable *interact);

void interactable_free_all();

void interactable_interact(Interactable *interact);

void interactable_close();

#endif
