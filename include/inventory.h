#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "gfc_types.h"

typedef enum
{
    I_Weapon,
    I_Key,
    I_Ammo,
    I_Healing
}ItemType;

typedef struct Item_S
{
    Uint8 _inuse;

    int id;
    char * name;
    ItemType type;

    void *data;
}Item;

void inventory_init(Uint32 item_max);

void inventory_update();

Item* inventory_item_new();

void inventory_remove_item(Item *item);

void inventory_free();

void inventory_free_item(Item *item);

void inventory_close();

Item* inventory_item_create(int id, char* name, ItemType type);




#endif
