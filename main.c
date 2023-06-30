#define KIT_IMPL
#include "kit.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdlib.h>
#include <stdio.h>

int is_speaking = 0;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
  is_speaking = *((float*)pInput) <= -0.01 ? 1 : 0;
}

int main(void) {
  ma_device device;

  ma_device_config config  = ma_device_config_init(ma_device_type_capture);
  config.playback.format   = ma_format_f32;
  config.playback.channels = 0;
  config.sampleRate        = 0;
  config.dataCallback      = data_callback;

  if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) return -1;
  ma_device_start(&device);

  kit_Context *ctx = kit_create("Tiny Streamer", 128, 128, KIT_SCALE2X);
  double delta;

  while (kit_step(ctx, &delta)) {
      if (kit_key_pressed(ctx, VK_ESCAPE)) { break; }
      kit_draw_rect(ctx, is_speaking ? kit_rgb(0,255,0) : kit_rgb(255,0,0),KIT_BIG_RECT);
  }

  kit_destroy(ctx);
  ma_device_uninit(&device);
  return 0;
}
