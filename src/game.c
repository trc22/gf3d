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

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    Model *model;
    Matrix4 modelMat;

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

    // main game loop
    slog("gf3d main loop begin");
	slog_sync();
	model = gf3d_model_load("dino");
	gfc_matrix_identity(modelMat);

    gf3d_vgraphics_rotate_camera(0.25, vector3d(0, 1, 0));

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here

       // gf3d_vgraphics_rotate_camera(0.001);
      /*  gfc_matrix_rotate(
            modelMat,
            modelMat,
            0.002,
            vector3d(1,0,0));*/


        if(keys[SDL_SCANCODE_W])
        {
            gfc_matrix_translate(modelMat, (vector3d(modelMat[0][1] * 0.1, modelMat[0][0] * -0.1, 0)));
        }
        if(keys[SDL_SCANCODE_S])
            gfc_matrix_translate(modelMat, (vector3d(modelMat[0][1] * -0.1, modelMat[0][0] * 0.1, 0)));
        if(keys[SDL_SCANCODE_A])
            gfc_matrix_rotate(
                modelMat,
                modelMat,
                0.01,
                vector3d(0,0,1));
        if(keys[SDL_SCANCODE_D])
            gfc_matrix_rotate(
                modelMat,
                modelMat,
                -0.01,
                vector3d(0,0,1));


        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

                gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat);

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
