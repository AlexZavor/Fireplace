#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION

#include "stdio.h"
#include "globals.h"
#include "SDL2/SDL.h"
#include "nuklear_sdl_renderer.h"
#include "menu.h"
#include "color.h"



// The window we'll be rendering to
SDL_Window* window = NULL;
// The window renderer
static SDL_Renderer* renderer = NULL;
// Event handler
SDL_Event e;
// Nuklear context
struct nk_context *ctx;

struct nk_colorf bg = {0.1, 0.18, 0.24, 1.0};

bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer){
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}
	//Create window
	*window = SDL_CreateWindow( PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*SCALE, SCREEN_HEIGHT*SCALE, SDL_WINDOW_SHOWN );
	if( *window == NULL ) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}
	//Create v-synced renderer for window
	*renderer = SDL_CreateRenderer( *window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ );
	if( *renderer == NULL ) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
	}

	SDL_RenderSetScale(*renderer, SCALE, SCALE);

	return true;
}

bool closeSDL(SDL_Window* window, SDL_Renderer* renderer){
	//Destroy renderer
	if( renderer != NULL ) {
		SDL_DestroyRenderer(renderer);
	}
	//Destroy window
	if( window != NULL ) {
		SDL_DestroyWindow( window );
	}
	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

bool initializeNK(struct nk_context** ctx){
	*ctx = nk_sdl_init(window, renderer);

	// Load font
	{
        struct nk_font_atlas *atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;

        /* set up the font atlas and add desired font; note that font sizes are
         * multiplied by font_scale to produce better results at higher DPIs */
        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13 * SCALE, &config);
        nk_sdl_font_stash_end();

        /* this hack makes the font appear to be scaled down to the desired
         * size and is only necessary when font_scale > 1 */
        font->handle.height /= SCALE;
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(*ctx, &font->handle);
    }

    return true;
}

int main(void) {
	// Initalize
	bool result = initializeSDL(&window, &renderer);
	if(!result){
		return 1;
	}
	initializeNK(&ctx);

	// Frame loop
	while(true){
		// Input
        nk_input_begin(ctx);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) goto cleanup;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q && SDL_GetModState() & KMOD_CTRL) goto cleanup;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) goto cleanup;
            nk_sdl_handle_event(&e);
        }
        nk_sdl_handle_grab(); /* optional grabbing behavior */
        nk_input_end(ctx);


        /* GUI */
        draw_menu(ctx);

		// Draw
        // BG
        SDL_SetRenderDrawColor(renderer, bg.r * 255, bg.g * 255, bg.b * 255, bg.a * 255);
        SDL_RenderClear(renderer);


        struct SDL_Rect rect;
        rect.y = 400;
        rect.w = pixel_size-1;
        rect.h = pixel_size-1;
        for(int i = 0; i < base_width; i++){
            rect.x = (300-((base_width*pixel_size)/2)) + (i*pixel_size);
            SDL_SetRenderDrawColor(renderer, NK_TO_SDL_COL(linear_color(c1,c2,(float)i/(float)base_width)));
            SDL_RenderFillRect(renderer, &rect);
        }

		// Render
        nk_sdl_render(NK_ANTI_ALIASING_ON);
        SDL_RenderPresent(renderer);
	}

	// Cleanup for NK and SDL2
cleanup:
    nk_sdl_shutdown();
	closeSDL(window, renderer);
	return 0;
}
