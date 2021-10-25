#include <SDL.h>

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"

#include "gf3d_entity.h"
#include "interactable.h"
#include "player.h"
#include "room.h"
#include "fixed_camera_system.h"


int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }

    init_logger("gf3d.log");
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        800,                   //screen width
        600,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();

    gf3d_entity_manager_init(10);
    interactable_init(5);

    room_load("room");

    fixed_camera_system_init(3);

    player_spawn(vector3d(1, 1, 1));

    gf3d_entity_create_interactable("cube", 1, "test interact");


    // main game loop
    slog("gf3d main loop begin");
	slog_sync();

    gf3d_camera_set_scale(vector3d(1,1,1));
    gf3d_camera_set_position(vector3d(1, 10, 1));
    gf3d_camera_set_rotation(vector3d(3.14, 0, 3.14));


    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here


        gf3d_entity_update_all();

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool


       gf3d_camera_update_view();
       gf3d_camera_get_view(gf3d_vgraphics_get_view_matrix());

        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

                room_draw(commandBuffer, bufferFrame);
                gf3d_entity_draw_all(commandBuffer, bufferFrame);

            gf3d_command_rendering_end(commandBuffer);

        gf3d_vgraphics_render_end(bufferFrame);


        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }

    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
