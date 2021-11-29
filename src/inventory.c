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

Item* inventory_item_create(int id, const char* name, ItemType type, int quantity)
{
    Item* item;
    item = inventory_item_new();

    if(!item)
    {
        slog("Error! Item not loaded properly");
    }

    item->id = id;
    item->name = strdup(name);
    item->type = type;
    item->quantity = quantity;

    slog("Item %s loaded successfully", item->name);

    return item;
}

Item* inventory_load_item(char* item_name)
{
    SJson* json, *sj_item;
    SJson *sj_id, *sj_name, *sj_type, *sj_quantity;

    Item* item;
    int id;
    const char* name;
    int type;
    int quantity;

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
    name = sj_get_string_value(sj_name);
    slog("Item name: %s", name);

    sj_type = sj_object_get_value(sj_item, "type");
    sj_get_integer_value(sj_type, &type);
    slog("Item type: %i", type);

    sj_quantity = sj_object_get_value(sj_item, "quantity");
    sj_get_integer_value(sj_quantity, &quantity);
    slog("Item quantity: %i", quantity);

    item = inventory_item_create(id, name, type, quantity);

    sj_free(json);

    slog("%s succesfully added to inventory.",item->name);
    return item;
}

void inventory_item_box()
{
    SJson* json, *item, *item_name, *item_quantity;

    json = sj_object_new();

    for(int i = 0; i < inventory.item_max; i++)
    {
        if(!inventory.item_list[i]._inuse)
            continue;

        if(inventory.item_list[i].name == NULL)
            continue;

        slog("inserting: %s into box", inventory.item_list[i].name);

        item = sj_object_new();

        item_name = sj_new_str(inventory.item_list[i].name);
        item_quantity = sj_new_int(inventory.item_list[i].quantity);

        sj_object_insert(item, "name", item_name);
        sj_object_insert(item, "quantity", item_quantity);


        sj_object_insert(json, inventory.item_list[i].name, item);

        inventory_free_item(&inventory.item_list[i]);
    }
    sj_save(json,"items/item_box.json");
    sj_free(json);


}

Item* inventory_get_item(int pos)
{
    if(inventory.item_list[pos]._inuse == 0) return NULL;

    return &inventory.item_list[pos];
}

Item* inventory_get_item_by_id(int id)
{
    for(int i = 0; i < inventory.item_max; i++)
    {
        if(inventory.item_list[i]._inuse != 1) continue;

        if(inventory.item_list[i].id == id)
            return &inventory.item_list[i];
    }
    return NULL;
}

void inventory_display_()
{
    int i;
    slog("Inventory:");
    for(int i = 0; i < inventory.item_max; i++)
    {
        if(inventory.item_list[i]._inuse != 1)
        {
            slog("%i: Empty", (i + 1));
            continue;
        }

        slog("%i: %s", (i + 1), inventory.item_list[i].name);
    }
    slog("Inventory end");

}
