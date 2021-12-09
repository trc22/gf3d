#include "menu.h"

#include "simple_logger.h"

typedef struct
{
    Window *window_list;
    Uint32  window_max;
}WindowManager;

static WindowManager window_manager = {0};

void window_manager_close()
{
    slog("Freeing window system");
    if(window_manager.window_list != NULL)
    {
        free(window_manager.window_list);
    }

    memset(&window_manager,0,sizeof(WindowManager));

    slog("Window system freed");
}


void menu_init(Uint32 window_max)
{
    window_manager.window_list = (Window*)gfc_allocate_array(sizeof(Window*),window_max);
    window_manager.window_max = window_max;
    if (!window_manager.window_list)
    {
        slog("failed to allocate window list");
        return;
    }
    for(int i = 0; i < window_manager.window_max; i++)
    {
        window_manager.window_list[i]._inuse = 0;
    }
    slog("Window system initialized");
    atexit(window_manager_close);
}

Window* window_new()
{
    int i;

    for(int i = 0; i < window_manager.window_max; i++)
    {
        if(window_manager.window_list[i]._inuse == 1) continue;

        memset(&window_manager.window_list[i],0,sizeof(Window));
        window_manager.window_list[i]._inuse = 1;

        slog("New window created");
        return &window_manager.window_list[i];
    }
    return NULL;
}

void menu_update()
{
    Uint32 mouse;
    int x, y;

    mouse = SDL_GetMouseState(&x, &y);
    if ((mouse & SDL_BUTTON_LMASK) != 0)
    {
        slog("Mouse cursor is at %i, %i", x, y);
        window_check_all(vector2d(x, y));
    }
}

void menu_draw_all()
{
    int i;

    for(int i = 0; i < window_manager.window_max; i++)
    {
        if(window_manager.window_list[i]._inuse == 0) continue;
        window_draw(&window_manager.window_list[i]);
    }

}

void window_draw(Window *window)
{
    if(!window) return;

    if(window->_inuse != 1) return;

    if(!window->sprite) {slog("No valid sprite");return;}

    gf3d_sprite_draw(window->sprite, vector2d(window->position.x - window->boundingBox.w * 0.5, window->position.y - window->boundingBox.h * 0.5), vector2d(1, 1), 1);
}

int window_check(Window *window, Vector2D mousePos)
{
    int left, right, top, bot;
    if(!window) return 0;

    left = window->boundingBox.x + window->position.x;
    right = window->boundingBox.x + window->boundingBox.w + window->position.x;
    top = window->boundingBox.y + window->position.y;
    bot = window->boundingBox.y + window->boundingBox.h + window->position.y;

    //slog("%i, %i, %i, %i", left, right, top, bot);

    if (mousePos.x < left)
        return 0;
    if(mousePos.x > right)
        return 0;
    if(mousePos.y > bot)
        return 0;
    if(mousePos.y < top)
        return 0;
    slog("window clicked!");
    return 1;

}

void window_check_all(Vector2D mousePos)
{
    for(int i = 0; i < window_manager.window_max; i++)
    {
        if(window_manager.window_list[i]._inuse != 1) continue;
        window_check(&window_manager.window_list[i],mousePos);
    }
}

void menu_free()
{
    for(int i = 0; i < window_manager.window_max; i++)
    {
        if(window_manager.window_list[i]._inuse == 0) continue;
        window_free(&window_manager.window_list[i]);
    }
}

Window *window_create(char* image, Vector2D position, int frame_height, int frame_width)
{
    Window* window;
    window = window_new();

    slog("Creating new window");

    if(!window) return NULL;


    window->sprite = gf3d_sprite_load(image, frame_width, frame_height, 1);

    vector2d_copy(window->position, position);

    window->boundingBox.x = 0;
    window->boundingBox.y = 0;
    window->boundingBox.w = frame_width / 2;
    window->boundingBox.h = frame_height / 2;

    slog("window created");
    return window;
}


void window_free(Window* self)
{
    if (!self)
    {
        slog("self pointer is not valid");
        return;
    }
    self->_inuse = 0;
    if(self->sprite)
        gf3d_sprite_free(self->sprite);
    if (self->data != NULL)
    {
        slog("warning: data not freed at entity free!");
    }
}
