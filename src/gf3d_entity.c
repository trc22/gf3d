#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "gf3d_entity.h"

typedef struct
{
    Entity *entity_list;
    Uint32  entity_max;
}EntityManager;

static EntityManager gf3d_entity_manager = {0};

void gf3d_entity_manager_close()
{
    if(gf3d_entity_manager.entity_list != NULL)
    {
        free(gf3d_entity_manager.entity_list);
    }
    memset(&gf3d_entity_manager,0,sizeof(EntityManager));
}

void gf3d_entity_manager_init(Uint32 entity_max)
{
    gf3d_entity_manager.entity_list = (Entity*)gfc_allocate_array(sizeof(Entity),entity_max);
    gf3d_entity_manager.entity_max = entity_max;
    if (!gf3d_entity_manager.entity_list)
    {
        slog("failed to allocate entity list");
        return;
    }
    for(int i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        gf3d_entity_manager.entity_list[i]._inuse = 0;
    }
    slog("Entity system initialized");
    atexit(gf3d_entity_manager_close);
}

Entity *gf3d_entity_new()
{
    int i;
    for (i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        if (gf3d_entity_manager.entity_list[i]._inuse == 1)continue;
        //. found a free entity
        memset(&gf3d_entity_manager.entity_list[i],0,sizeof(Entity));
        gf3d_entity_manager.entity_list[i]._inuse = 1;
        gfc_matrix_identity(gf3d_entity_manager.entity_list[i].modelMat);
        return &gf3d_entity_manager.entity_list[i];
    }
    slog("request for entity failed: all full up");
    return NULL;
}

void gf3d_entity_free(Entity *self)
{
    if (!self)
    {
        slog("self pointer is not valid");
        return;
    }
    self->_inuse = 0;
    gf3d_model_free(self->model);
    if (self->data != NULL)
    {
        slog("warning: data not freed at entity free!");
    }
}

void gf3d_entity_draw(Entity *ent, VkCommandBuffer commandBuffer, Uint32 bufferFrame)
{
        if(!ent)return;
        if(!ent->model)return;

        gf3d_model_draw(ent->model, bufferFrame, commandBuffer, ent->modelMat);
}

void gf3d_entity_draw_all(VkCommandBuffer commandBuffer, Uint32 bufferFrame)
{
    for(int i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        if(!gf3d_entity_manager.entity_list[i]._inuse) continue;

        gf3d_entity_draw(&gf3d_entity_manager.entity_list[i], commandBuffer, bufferFrame);
    }
}

void gf3d_entity_update(Entity *ent)
{
    if (!ent)return;
    if (!ent->_inuse)return;


    if (ent->update != NULL)
    {
        ent->update(ent);
    }
}

void gf3d_entity_update_all()
{
    for(int i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        if(!gf3d_entity_manager.entity_list[i]._inuse) continue;
        gf3d_entity_update(&gf3d_entity_manager.entity_list[i]);
    }
}

Entity* gf3d_entity_create(char* modelName)
{
    Entity *ent = gf3d_entity_new();
    if(!ent)
    {
        slog("Failed to create entity %s", modelName);
        return NULL;
    }
    ent->model = gf3d_model_load(modelName);

    return ent;

}


/*eol@eof*/
