#define KIT_IMPL
#include "kit.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

extern int update_history_index();
extern int array_int_has(int arr[], int value);
extern int get_length_in_directory(const char *p);
extern void read_files_in_directory(char ** addr, const char *p);

int is_speaking = 0;
int is_speaking_history[4] = {};


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
  is_speaking_history[update_history_index()] = *((float*)pInput) <= -0.009 ? 1 : 0;
  is_speaking=array_int_has(is_speaking_history,1);
}

int main(void) {
  // ----- assets
  int sprite_idle_frames = get_length_in_directory(".//assets//idle");
  double sprite_idle_current_frame = 0;
  
  char *sprite_idle_file[sprite_idle_frames];
  kit_Image *sprite_idle_kit[sprite_idle_frames];

  read_files_in_directory(sprite_idle_file, ".//assets//idle");

  for (int i = 0; i<sprite_idle_frames; i++) {
    char src[] = ".//assets//idle//";
    strcat(src,sprite_idle_file[i]);

    sprite_idle_kit[i] = kit_load_image_file(src);
  }
  // ------

  // ------ voice detector
  ma_device device;

  ma_device_config config  = ma_device_config_init(ma_device_type_capture);
  config.playback.format   = ma_format_f32;
  config.playback.channels = 0;
  config.sampleRate        = 0;
  config.dataCallback      = data_callback;

  if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) return -1;
  ma_device_start(&device);
  // ------

  // ------ ui
  kit_Context *ctx = kit_create(
      "Tiny Streamer",
      sprite_idle_kit[0]->w * 4,
      sprite_idle_kit[0]->h * 4,
      KIT_SCALE2X, 
      "icon.ico"
  );

  printf("%i", ctx->win_w);

  double delta;
  int x = (ctx->win_w/2-sprite_idle_kit[0]->w)/2;
  int y = (ctx->win_h/2-sprite_idle_kit[0]->h)/2;

  while (kit_step(ctx, &delta)) {
      if (kit_key_pressed(ctx, VK_ESCAPE)) { break; }

      kit_clear(ctx, KIT_BLACK);
      
      if (!is_speaking) {
        if (abs(sprite_idle_current_frame)>sprite_idle_frames-1) sprite_idle_current_frame = 0;
        kit_draw_image(ctx, sprite_idle_kit[abs(sprite_idle_current_frame)], x, y);
        sprite_idle_current_frame += delta / 0.1;
      } else {
        // continue....
      }
  }
  // ------

  kit_destroy(ctx);
  ma_device_uninit(&device);

  return 0;
}
