#ifndef COLOR_H
#define COLOR_H

#include "globals.h"

#define NK_TO_SDL_COL(c) c.r * 255, c.g * 255, c.b * 255, c.a * 255

typedef struct nk_colorf color_t;

// Function to get the linear difference between two colors. color ramp
color_t linear_color(color_t c1, color_t c2, float n);

#endif //COLOR_H