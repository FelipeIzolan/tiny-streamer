#define KIT_IMPL
#include "kit.h"

int main(void) {
    kit_Context *ctx = kit_create("hi", 128, 128, KIT_SCALE2X);
    double delta;

    while (kit_step(ctx, &delta)) {
        if (kit_key_pressed(ctx, VK_ESCAPE)) { break; }
        kit_draw_rect(ctx,kit_rgb(0,255,0),KIT_BIG_RECT);
        kit_draw_text(ctx, KIT_WHITE, "Hello world!", 10, 10);
    }

    kit_destroy(ctx);
    return 0;
}
