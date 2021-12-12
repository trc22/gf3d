#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gf3d_model.h"
#include "interactable.h"
#include "bounding_box.h"

typedef enum
{
    Default,
    Pause
}EntityState;

typedef struct Entity_S
{
    Uint8           _inuse;         /**<flag to keep track if this isntance is in use and should not be reassigned*/
    Model          *model;          /**<the 3d model for this entity*/
    Matrix4         modelMat;       /**<orientation matrix for the model*/
    Vector3D        position;       /**<position of the entity in 3d space*/
    Vector3D        velocity;       /**<velocity of the entity in 3d space*/
    Vector3D        acceleration;   /**<acceleration of the entity in 3d space*/
    Vector3D        rotation;       /**<yaw, pitch, and roll of the entity*/
    Vector3D        scale;          /**<*please default to 1,1,1*/
    int             isEnemy;
    int             id;
    EntityState     state;          /**<current state of the entity*/
    void (*think) (struct Entity_S* self);   /**<function called on entity think*/
    void (*update)(struct Entity_S* self);   /**<function called on entity update*/
    void (*touch) (struct Entity_S* self,struct Entity_S* other);   /**<function called on entity think*/
    Interactable*   interactable;
    BoundingBox* boundingBox;
    float           health;
    float           healthmax;
    int camera_mode;
    void *data;                     /**<additional entity specific data*/

}Entity;

/**
 * @brief initializes the entity subsystem
 * @param entity_max maximum number of simultaneous entities you wish to support
 */
void gf3d_entity_manager_init(Uint32 entity_max);

/**
 * @brief get an empty entity from the system
 * @return NULL on out of space or a pointer to an entity otherwise
 */
Entity *gf3d_entity_new();

/**
 * @brief free an active entity
 * @param self the entity to free
 */
void gf3d_entity_free(Entity *self);

void gf3d_entity_free_all();

void gf3d_entity_update(Entity *ent);

void gf3d_entity_update_all();

void gf3d_entity_think(Entity *ent);

void gf3d_entity_think_all();

void gf3d_entity_draw(Entity *ent);

void gf3d_entity_draw_all();

Entity* gf3d_entity_create(char* modelName);

void gf3d_entity_set_bounding_box(Entity* ent, int x, int y, int h, int w);

Entity* gf3d_entity_create_interactable(char* modelName, InteractType type, char* name);

void gf3d_entity_overlap_all();

void gf3d_entity_check_enemies_distance(Entity* ent);

Entity* gf3d_entity_get_by_id(int id);

void gf3d_entity_weapon_check_all(Vector2D ray_start, Vector2D ray_end);

#endif
