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

    int quantity;

    void *data;
}Item;

/**
 * @brief initializes the inventory
 * @param item_max the maximum amount of items the inventory can hold
 */
void inventory_init(Uint32 item_max);

void inventory_update();

/**
 * @brief creates a new item and adds it the inventory
 * @param item_max the maximum amount of items the inventory can hold
 * @return item which was created, null on failure.
 */
Item* inventory_item_new();

/**
 * @brief removes an item from the inventory
 * @param item to remove
 */
void inventory_remove_item(Item *item);

/**
 * @brief frees all items in the inventory
 */
void inventory_free();

/**
 * @brief frees an item in the inventory
 * @param item to free
 */
void inventory_free_item(Item *item);

/**
 * @brief frees all items in the inventory and then frees the inventory itself
 */
void inventory_close();

/**
 * @brief creates an item with specific paramters and adds it to inventory
 * @param id the unique item identification number
 * @param name the item's name
 * @param type one of the item types specified in ItemType enumeration
 * @return item that was created
 */
Item* inventory_item_create(int id, const char* name, ItemType type);

/**
 * @brief loads an item from items/items.json and adds it to the inventory
 * @param item_name the key for the item in the json file
 * @return the item that was created
 */
Item* inventory_load_item(char* item_name);

void inventory_item_box();

Item* inventory_get_item(int pos);

void inventory_display_();


#endif
