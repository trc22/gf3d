#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "simple_json.h"

#include "inventory.h"

typedef struct
{
    Item *item_list;
    Uint32  item_max;
}Inventory;

static Inventory inventory = {0};

void inventory_init(Uint32 item_max)
{
    inventory.item_list = (Item*)gfc_allocate_array(sizeof(Item),item_max);
    inventory.item_max = item_max;
    if (!inventory.item_list)
    {
        slog("failed to allocate item list");
        return;
    }
    for(int i = 0; i < inventory.item_max; i++)
    {
        inventory.item_list[i]._inuse = 0;
    }
    slog("Inventory initialized");
    atexit(inventory_close);
}

void inventory_update()
{
    for(int i = 0; i < inventory.item_max; i++)
    {

    }
}

Item* inventory_item_new()
{
    int i;
    for (i = 0; i < inventory.item_max; i++)
    {
        if (inventory.item_list[i]._inuse == 1)continue;
        //. found a free entity
        memset(&inventory.item_list[i],0,sizeof(Item));
        inventory.item_list[i]._inuse = 1;
        return &inventory.item_list[i];
    }
    slog("request for item failed: inventory is full");
    return NULL;
}

void inventory_remove_item(Item *item);

void inventory_free()
{
    for(int i = 0; i < inventory.item_max; i++)
    {
        inventory_free_item(&inventory.item_list[i]);
    }
}

void inventory_free_item(Item *item)
{
    if (!item)
    {
        slog("item pointer is not valid");
        return;
    }
    item->_inuse = 0;
    if (item->data != NULL)
    {
        slog("warning: data not freed at item free!");
    }
}

void inventory_close()
{
    inventory_free();
    if(inventory.item_list != NULL)
    {
        free(inventory.item_list);
    }
    memset(&inventory,0,sizeof(Inventory));
}

Item* inventory_item_create(int id, char* name, ItemType type)
{
    Item* item;
    item = inventory_item_new();

    if(!item)
    {
        slog("Error! Item not loaded properly");
    }

    item->id = id;
    item->name = name;
    item->type = type;

    slog("Item %s created and added to inventory", item->name);

    return item;
}

Item* inventory_load_item(char* item_name)
{
    SJson* json, *sj_item;
    SJson *sj_id, *sj_name, *sj_type;

    Item* item;
    int id;
    char* name;
    int type;

    json = sj_load("items/items.json");

    if(!json)
    {
        slog("Error! Items Json file not loaded");
        return NULL;
    }

    sj_item = sj_object_get_value(json, item_name);

    if(!sj_item || sj_item == NULL)
    {
        slog("Error! Item not loaded");
        return NULL;
    }

    sj_id = sj_object_get_value(sj_item, "id");
    sj_get_integer_value(sj_id, &id);
    slog("Item id: %i", id);

    sj_name = sj_object_get_value(sj_item, "name");
    name = (char*)sj_get_string_value(sj_name);
    slog("Item name: %s", name);

    sj_type = sj_object_get_value(sj_item, "type");
    sj_get_integer_value(sj_type, &type);
    slog("Item type: %i", type);

    item = inventory_item_create(id, name, type);

    sj_free(json);

    return item;
}
