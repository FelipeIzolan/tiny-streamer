#define KIT_IMPL
#include "kit.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_FRAMES 16
#define MIN_FREQUENCY -0.010

typedef struct {
  int fmax;
  double fcurrent;
  char *files[MAX_FRAMES];
  kit_Image *sprites[MAX_FRAMES];
} tiny_Frame;

extern int update_int_index(int arr[], int length);
extern int array_int_has(int arr[], int value, int length);
extern int get_length_in_directory(const char *p);
extern void read_files_in_directory(char ** addr, const char *p);

int is_speaking = 0;
int is_speaking_history[24] = {};
int is_speaking_history_length = sizeof(is_speaking_history)/sizeof(int);

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
  is_speaking_history[update_int_index(is_speaking_history, is_speaking_history_length)] = *((float*)pInput) <= MIN_FREQUENCY ? 1 : 0;
  is_speaking=array_int_has(is_speaking_history, 1, is_speaking_history_length);
}

int main(void) {
  // ----- assets 
  tiny_Frame idle;
  idle.fmax = get_length_in_directory(".//assets//idle");
  idle.fcurrent = 0;
  read_files_in_directory(idle.files, ".//assets//idle");
  for (int i = 0; i<idle.fmax; i++) { 
    char src[] = ".//assets//idle//";
    strcat(src,idle.files[i]); 
    idle.sprites[i] = kit_load_image_file(src);
  }
  // ------
  tiny_Frame speak;
  speak.fmax = get_length_in_directory(".//assets//speak");
  speak.fcurrent = 0;
  read_files_in_directory(speak.files, ".//assets//speak");
  for (int i = 0; i<speak.fmax; i++) { 
    char src[] = ".//assets//speak//";
    strcat(src,speak.files[i]); 
    speak.sprites[i] = kit_load_image_file(src);
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
      idle.sprites[0]->w * 4,
      idle.sprites[0]->h * 4,
      KIT_SCALE2X, 
      "icon.ico"
  );

  double delta;
  int x = (ctx->win_w/2-idle.sprites[0]->w)/2;
  int y = (ctx->win_h/2-idle.sprites[0]->h)/2;

  while (kit_step(ctx, &delta)) {
      if (kit_key_pressed(ctx, VK_ESCAPE)) { break; }

      kit_clear(ctx, KIT_BLACK);
      
      if (!is_speaking) {
        if ((int)idle.fcurrent > idle.fmax-1) idle.fcurrent = 0;
        kit_draw_image(ctx, idle.sprites[(int) idle.fcurrent], x, y);
        idle.fcurrent += delta/0.1;
      } else {
        if ((int)speak.fcurrent > speak.fmax-1) speak.fcurrent = 0;
        kit_draw_image(ctx, speak.sprites[(int) speak.fcurrent], x, y);
        speak.fcurrent += delta/0.1;
      }
  }
  // ------

  kit_destroy(ctx);
  ma_device_uninit(&device);

  return 0;
}
