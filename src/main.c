#include "stdio.h"
#include "globals.h"
#include "SDL2/SDL.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"


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
            nk_sdl_handle_event(&e);
        }
        nk_sdl_handle_grab(); /* optional grabbing behavior */
        nk_input_end(ctx);


        /* GUI */
        if (nk_begin(ctx, PROJECT_NAME, nk_rect(10, 10, 200, 300),
            NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_button_label(ctx, "button"))
                fprintf(stdout, "button pressed\n");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 40, 1);
            if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx),400))) {
                nk_layout_row_dynamic(ctx, 120, 1);
                bg = nk_color_picker(ctx, bg, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
                bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
                bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
                bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
                nk_combo_end(ctx);
            }
        }
        nk_end(ctx);

		// Draw
        SDL_SetRenderDrawColor(renderer, bg.r * 255, bg.g * 255, bg.b * 255, bg.a * 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 100,100,50,50);

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
