#include "menu.h"
#include "stdio.h"

int pixel_size = 7;
int base_width = 9;
color_t c1 = {0.2, 0.2, 0.9, 1.0};
color_t c2 = {0.5, 0.5, 0.6, 1.0};

void draw_menu(struct nk_context *ctx) {
    if (nk_begin(ctx, PROJECT_NAME, nk_rect(10, 10, 200, 300),
        NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {

        // nk_layout_row_dynamic(ctx, 30, 1);
        // if (nk_button_label(ctx, "button"))
        // //     fprintf(stdout, "button pressed\n");
        // nk_layout_row_dynamic(ctx, 30, 2);
        // if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
        // if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;

        nk_layout_row_dynamic(ctx, 25, 1);
        nk_property_int(ctx, "Width:", 1, &base_width, 40, 1, 1);

        nk_layout_row_dynamic(ctx, 25, 1);
        nk_property_int(ctx, "Pixel size:", 1, &pixel_size, 15, 1, 1);

        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "C1:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 20, 1);
        if (nk_combo_begin_color(ctx, nk_rgb_cf(c1), nk_vec2(nk_widget_width(ctx),400))) {
            nk_layout_row_dynamic(ctx, 120, 1);
            c1 = nk_color_picker(ctx, c1, NK_RGBA);
            nk_layout_row_dynamic(ctx, 25, 1);
            c1.r = nk_propertyf(ctx, "#R:", 0, c1.r, 1.0f, 0.01f,0.005f);
            c1.g = nk_propertyf(ctx, "#G:", 0, c1.g, 1.0f, 0.01f,0.005f);
            c1.b = nk_propertyf(ctx, "#B:", 0, c1.b, 1.0f, 0.01f,0.005f);
            c1.a = nk_propertyf(ctx, "#A:", 0, c1.a, 1.0f, 0.01f,0.005f);
            nk_combo_end(ctx);
        }

        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "C2:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 20, 1);
        if (nk_combo_begin_color(ctx, nk_rgb_cf(c2), nk_vec2(nk_widget_width(ctx),400))) {
            nk_layout_row_dynamic(ctx, 120, 1);
            c2 = nk_color_picker(ctx, c2, NK_RGBA);
            nk_layout_row_dynamic(ctx, 25, 1);
            c2.r = nk_propertyf(ctx, "#R:", 0, c2.r, 1.0f, 0.01f,0.005f);
            c2.g = nk_propertyf(ctx, "#G:", 0, c2.g, 1.0f, 0.01f,0.005f);
            c2.b = nk_propertyf(ctx, "#B:", 0, c2.b, 1.0f, 0.01f,0.005f);
            c2.a = nk_propertyf(ctx, "#A:", 0, c2.a, 1.0f, 0.01f,0.005f);
            nk_combo_end(ctx);
        }
    }
    nk_end(ctx);
}
