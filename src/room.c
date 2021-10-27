#include "simple_logger.h"
#include "room.h"

Room *room;

void room_new()
{
    room = (Room*)gfc_allocate_array(sizeof(Room),1);
    if(!room)
    {
        slog("ERROR: Cannot allocate memory for room");
    }
    room->currentCamera = (Camera*)gfc_allocate_array(sizeof(Camera),1);
}

void room_load(char* room_name)
{
    room_new();

    if(!room)
    {
        slog("Room loading failed");
        return;
    }

    room->name = room_name;

    slog("%s", room->name);

    gfc_matrix_identity(room->mat);

    room->model = gf3d_model_load(room_name);

}

void room_draw(VkCommandBuffer commandBuffer, Uint32 bufferFrame)
{
    gf3d_model_draw(room->model, bufferFrame, commandBuffer, room->mat);
}

void room_free()
{
    if(room != NULL) return;

    memset(room,0,sizeof(Room));
}

void room_set_camera(Vector3D pos, Vector3D rot)
{
    vector3d_copy(room->currentCamera->position, pos);
    vector3d_copy(room->currentCamera->rotation, rot);
    slog("current camera rotation.z = %f", room->currentCamera->rotation.z);
}
void room_camera_enable()
{
    gf3d_camera_set_position(room->currentCamera->position);
    gf3d_camera_set_rotation(room->currentCamera->rotation);
}
