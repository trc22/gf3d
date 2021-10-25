#include "simple_logger.h"

#include "gf3d_camera.h"

#include "fixed_camera_system.h"

typedef struct
{
    FixedCamera *fixed_camera_list;
    Uint32  fixed_camera_max;
}FixedCameraManager;

static FixedCameraManager fixed_camera_manager = {0};

void fixed_camera_system_close()
{
    if(fixed_camera_manager.fixed_camera_list != NULL)
    {
        fixed_camera_free_all();
        free(fixed_camera_manager.fixed_camera_list);
    }
    memset(&fixed_camera_manager,0,sizeof(FixedCameraManager));
}

void fixed_camera_system_init(Uint32 num_cameras)
{
    fixed_camera_manager.fixed_camera_list = (FixedCamera*)gfc_allocate_array(sizeof(FixedCamera),num_cameras);
    fixed_camera_manager.fixed_camera_max = num_cameras;
    if (!fixed_camera_manager.fixed_camera_list)
    {
        slog("failed to allocate fixed camera list");
        return;
    }
    for(int i = 0; i < fixed_camera_manager.fixed_camera_max; i++)
    {
        fixed_camera_manager.fixed_camera_list[i]._inuse = 0;
    }
    slog("Fixed camera system initialized");
    atexit(fixed_camera_system_close);
}

void fixed_camera_switch(int id);

FixedCamera* fixed_camera_new()
{
    int i;
    for (i = 0; i < fixed_camera_manager.fixed_camera_max; i++)
    {
        if (fixed_camera_manager.fixed_camera_list[i]._inuse == 1)continue;
        //. found a free entity
        memset(&fixed_camera_manager.fixed_camera_list[i],0,sizeof(FixedCamera));
        fixed_camera_manager.fixed_camera_list[i]._inuse = 1;

        return &fixed_camera_manager.fixed_camera_list[i];
    }
    slog("request for fixed camera failed: all full up");
    return NULL;
}

void fixed_camera_free(FixedCamera *camera)
{
    if (!camera)
    {
        slog("fixed camera pointer is not valid");
        return;
    }
    camera->_inuse = 0;
    if (camera->data != NULL)
    {
        slog("warning: data not freed at fixed camera free!");
    }
}

void fixed_camera_free_all()
{
    int i;
    for (i = 0; i < fixed_camera_manager.fixed_camera_max; i++)
    {
        if (fixed_camera_manager.fixed_camera_list[i]._inuse == 0)continue;

        fixed_camera_free(&fixed_camera_manager.fixed_camera_list[i]);
    }
}
