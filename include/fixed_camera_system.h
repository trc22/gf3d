#ifndef __FIXED_CAMERA_SYSTEM_H__
#define __FIXED_CAMERA_SYSTEM_H__

typedef struct FixedCamera_S
{
    Uint32 _inuse;
    int id;

    bool isCurrent;

    Vector3D scale;
    Vector3D rotation;
    Vector3D position;

}FixedCamera;

void fixed_camera_system_init(uint32 numCameras);

void fixed_camera_system_update();

void fixed_camera_new();

void fixed_camera_free(FixedCamera *camera);

void fixed_camera_free_all();

void fixed_camera_system_free();


#endif
