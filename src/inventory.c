#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "simple_json.h"

#include "inventory.h"
#include "player.h"

typedef struct
{
    Item *item_list;
    Uint32  item_max;
    Uint8 is_open;
}Inventory;

void item_click(Window *window);

static Inventory inventory = {0};

void inventory_init(Uint32 item_max)
{
    inventory.item_list = (Item*)gfc_allocate_array(sizeof(Item),item_max);
    inventory.item_max = item_max;
    inventory.is_open = 0;
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
    if(item->icon)
        window_free(item->icon);
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

    Item* item, *item_check;
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

    item_check = inventory_get_item_by_id(id);
    if(item_check)
    {
        slog("%s already in inventory, updating quantity", item_check->name);
        item_check->quantity += quantity;
        return item_check;
    }

    item = inventory_item_create(id, name, type, quantity);
    item->icon = window_create("images/items/test.png", vector2d(100,100), 128, 128);
    item->icon->on_click = item_click;
    item->icon->item_id = id;
    item->combine_result = "test_combine";


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

void inventory_display()
{
    int i;
    if(inventory.is_open)
        inventory.is_open = 0;
    else
        inventory.is_open = 1;

    for(i = 0; i < inventory.item_max; i++)
    {
        if(inventory.item_list[i]._inuse != 1) continue;
        if(!inventory.item_list[i].icon) continue;

        inventory.item_list[i].icon->position.x = 100 + 80 * i;
        inventory.item_list[i].icon->inventory_pos = i;
        if(!inventory.item_list[i].icon->selected)
            inventory.item_list[i].icon->active = inventory.is_open;
        else if(inventory.is_open)
        {
            inventory.item_list[i].icon->selected = 0;
            player_set_current_item(-1);
        }
    }

}

void item_click(Window *window)
{
    Item *item, *combine;
    Window* selection;

    if(!window) return;

    item = inventory_get_item_by_id(window->item_id);

    selection = window_get_selected();
    if(selection)
    {
        combine = inventory_get_item_by_id(selection->item_id);
        item_combine(item, combine);
        selection->selected = 0;
        return;
    }
    slog("Item %s selected", item->name);
    player_set_current_item(window->inventory_pos);
    window->selected = 1;
}

int item_combine(Item *self, Item *other)
{
    char* result;
    slog("combining %s and %s", self->name, other->name);
    if(self->combine == other->id)
    {
        slog("combined");
        result = strdup(self->combine_result);
        inventory_free_item(self);
        inventory_free_item(other);
        inventory_load_item(result);
        return 1;
    }
    slog("cannot combine");
    return 0;

}

Uint8 inventory_get_open()
{
    return inventory.is_open;
}
