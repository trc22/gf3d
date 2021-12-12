#include <SDL.h>


#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gfc_audio.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_sprite.h"

#include "gf3d_entity.h"
#include "interactable.h"
#include "player.h"
#include "room.h"
#include "enemy.h"
#include "menu.h"
#include "main_menu.h"


int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 * keys;

    Sprite *mouse = NULL;
    Sprite *loading = NULL;

    int mousex,mousey;

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

	//gfc_audio_init(6, 2, 1, 2, 1, 0);
    mouse = gf3d_sprite_load("images/mouse.png",64,64, 1);

    menu_init(12);

    main_menu();


    gf3d_entity_manager_init(10);
    interactable_init(8);
    loading = gf3d_sprite_load("images/loading.png", 1600, 1200, 1);

    //gfc_sound_play(gfc_sound_load("sounds/but_first_a_dance.mp3", 0.5, 1), 4, 0.5, 1, 1);


    //enemy_spawn(vector3d(-10, -10, 0));

    //bounding_box_update(ent->boundingBox, ent->position);

    //enemy_set_player(player_spawn(vector3d(1, 1, 1)));

    // main game loop
    slog("gf3d main loop begin");
	slog_sync();

    gf3d_camera_set_scale(vector3d(1,1,1));
    gf3d_camera_set_position(vector3d(1, 10, 1));
    gf3d_camera_set_rotation(vector3d(3.14, 0, 3.14));

    inventory_init(8);
    inventory_load_item("test item")->icon= window_create("images/items/test_1.png", vector2d(100,100), 128, 128);
    inventory_load_item("pistol")->icon= window_create("images/items/test_2.png", vector2d(100,100), 128, 128);
    //inventory_load_item("key")->icon= window_create("images/items/test_3.png", vector2d(100,100), 128, 128);
    inventory_load_item("health pack")->icon= window_create("images/items/test_4.png", vector2d(100,100), 128, 128);
    inventory_load_item("ammo_pistol")->combine = 2;
    inventory_load_item("knife")->combine = 5;

    set_loading(1);

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        SDL_GetMouseState(&mousex,&mousey);

        //update game things here

        gf3d_entity_think_all();
        gf3d_entity_update_all();

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        room_update();
        menu_update();
        gf3d_camera_update_view();
        gf3d_camera_get_view(gf3d_vgraphics_get_view_matrix());


        gf3d_vgraphics_render_start();

                room_draw();
                gf3d_entity_draw_all();
                if(get_loading())
                    gf3d_sprite_draw(loading, vector2d(0,0), vector2d(1,1), 0);
                menu_draw_all();

        gf3d_sprite_draw(mouse,vector2d(mousex - 32,mousey - 32),vector2d(1,1),0);


        gf3d_vgraphics_render_end();

        gf3d_entity_overlap_all();

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }

    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
