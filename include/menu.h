#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "color.h"

extern int pixel_size;
extern int base_width;
extern color_t c1;
extern color_t c2;

void draw_menu(struct nk_context *ctx);

#endif //MENU_H