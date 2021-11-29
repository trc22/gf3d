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
        gf3d_entity_manager.entity_list[i].id = i;

        gfc_matrix_identity(gf3d_entity_manager.entity_list[i].modelMat);
        gf3d_entity_manager.entity_list[i].scale = vector3d(1, 1, 1);

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

void gf3d_entity_draw(Entity *ent)
{
        if(!ent)return;
        if(!ent->model)return;

        gf3d_model_draw(ent->model, ent->modelMat);
}

void gf3d_entity_draw_all()
{
    for(int i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        if(!gf3d_entity_manager.entity_list[i]._inuse) continue;
        if(gf3d_entity_manager.entity_list[i].camera_mode == 1)
            continue;

        gf3d_entity_draw(&gf3d_entity_manager.entity_list[i]);
    }
}

void gf3d_entity_update(Entity *ent)
{
    if (!ent)return;
    if (!ent->_inuse)return;

    vector3d_add(ent->position, ent->position, ent->velocity);
    vector3d_add(ent->velocity, ent->velocity, ent->acceleration);


    gfc_matrix_identity(ent->modelMat);
    gfc_matrix_scale(ent->modelMat, ent->scale);

    gfc_matrix_rotate(ent->modelMat, ent->modelMat, ent->rotation.z, vector3d(0, 0, 1));
    gfc_matrix_rotate(ent->modelMat, ent->modelMat, ent->rotation.y, vector3d(0, 1, 0));
    gfc_matrix_rotate(ent->modelMat, ent->modelMat, ent->rotation.x, vector3d(1, 0, 0));

    gfc_matrix_translate(ent->modelMat, ent->position);


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


void gf3d_entity_think(Entity *ent)
{
    if (!ent)return;
    if (ent->think)ent->think(ent);
}

void gf3d_entity_think_all()
{
    for(int i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        if(!gf3d_entity_manager.entity_list[i]._inuse) continue;
        gf3d_entity_think(&gf3d_entity_manager.entity_list[i]);
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

Entity* gf3d_entity_create_interactable(char* modelName, InteractType type, char* name)
{
    Entity *ent = gf3d_entity_create(modelName);
    if(!ent)
    {
        slog("Failed to create entity %s", modelName);
        return NULL;
    }
    ent->interactable = interactable_new();
    ent->interactable->name = name;
    ent->interactable->type = type;
    ent->interactable->parentEnt = ent->id;

    slog("Interactable name: %s", ent->interactable->name);
    return ent;

}

void gf3d_entity_set_bounding_box(Entity* ent, int x, int y, int h, int w)
{
    ent->boundingBox = (BoundingBox*)gfc_allocate_array(sizeof(BoundingBox),1);
    ent->boundingBox->x = x;
    ent->boundingBox->y = y;
    ent->boundingBox->h = h;
    ent->boundingBox->w = w;


    /*ent->boundingBox = (BoundingBox*)gfc_allocate_array(sizeof(BoundingBox),1);
    vector3d_copy(ent->boundingBox->minExtent, minExtent);
    vector3d_copy(ent->boundingBox->maxExtent, maxExtent);
    vector3d_add(ent->boundingBox->minExtentPos,ent->boundingBox->minExtent,ent->position);
    vector3d_add(ent->boundingBox->minExtentPos,ent->boundingBox->maxExtent,ent->position);*/
}

void gf3d_entity_overlap_all()
{
    int i, j;
    for(int i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        if(!gf3d_entity_manager.entity_list[i]._inuse) continue;
        if(gf3d_entity_manager.entity_list[i].boundingBox == NULL) continue;

        for(int j = 1; j < gf3d_entity_manager.entity_max; j++)
        {
            if(i == j) continue;
            if(!gf3d_entity_manager.entity_list[j]._inuse) continue;
            if(gf3d_entity_manager.entity_list[j].boundingBox == NULL) continue;

            if(bounding_box_overlap(gf3d_entity_manager.entity_list[i].boundingBox, gf3d_entity_manager.entity_list[j].boundingBox, gf3d_entity_manager.entity_list[i].position, gf3d_entity_manager.entity_list[j].position))
            {
                    if (gf3d_entity_manager.entity_list[i].touch)
                        gf3d_entity_manager.entity_list[i].touch(&gf3d_entity_manager.entity_list[i], &gf3d_entity_manager.entity_list[j]);
                    if (gf3d_entity_manager.entity_list[j].touch)
                        gf3d_entity_manager.entity_list[j].touch(&gf3d_entity_manager.entity_list[j], &gf3d_entity_manager.entity_list[i]);
            }
        }
    }
}

void gf3d_entity_free_all()
{
    for(int i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        gf3d_entity_manager.entity_list[i]._inuse = 0;
    }
}

void gf3d_entity_check_enemies_distance(Entity* ent)
{
    for(int i = 0; i < gf3d_entity_manager.entity_max; i++)
    {
        if(gf3d_entity_manager.entity_list[i]._inuse != 1) continue;
        if(gf3d_entity_manager.entity_list[i].isEnemy != 1) continue;
        if(vector3d_distance_between_less_than(ent->position, gf3d_entity_manager.entity_list[i].position, 8))
            gf3d_entity_manager.entity_list[i]._inuse = 0;
    }
}

Entity* gf3d_entity_get_by_id(int id)
{
    if(gf3d_entity_manager.entity_list[id]._inuse != 1) return NULL;
    return &gf3d_entity_manager.entity_list[id];
}

/*eol@eof*/
