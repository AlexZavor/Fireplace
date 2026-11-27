#include "color.h"

// Function to get linear position between two floats
float lin(float f1, float f2, float n){
    return ((f2-f1)*n) + f1;
}

// Function to get the linear difference between two colors. color ramp
color_t linear_color(color_t c1, color_t c2, float n){
    color_t color;

    color.r = lin(c1.r, c2.r, n);
    color.g = lin(c1.g, c2.g, n);
    color.b = lin(c1.b, c2.b, n);
    color.a = lin(c1.a, c2.a, n);

    return color;
}
