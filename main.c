#ifndef _WIN32
#error "Windows OS is required for this software."
#endif

#define KIT_IMPL
#include "kit.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_FRAMES 16
float MIN_FREQUENCY = -0.012;

typedef struct {
  int fmax;
  double fcurrent;
  char *files[MAX_FRAMES];
  kit_Image *sprites[MAX_FRAMES];
} tiny_Frame;

extern int update_int_index(int arr[], int length);
extern int array_int_has(int arr[], int value, int length);
extern int get_length_in_directory(const char *p);
extern void get_files_in_directory(char ** addr, const char *p);

int is_speaking = 0;
int is_speaking_history[24] = {};
int is_speaking_history_length = sizeof(is_speaking_history)/sizeof(int);

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
  is_speaking_history[update_int_index(is_speaking_history, is_speaking_history_length)] = *((float*)pInput) <= MIN_FREQUENCY ? 1 : 0;
  is_speaking=array_int_has(is_speaking_history, 1, is_speaking_history_length);
}

int main(void) {
  // ----- Idle Assets 
  tiny_Frame idle;
  idle.fmax = get_length_in_directory(".//assets//idle");
  idle.fcurrent = 0;
  
  get_files_in_directory(idle.files, ".//assets//idle");
  
  for (int i = 0; i < idle.fmax; i++) { 
    char src[] = ".//assets//idle//";
    strcat(src,idle.files[i]); 
    idle.sprites[i] = kit_load_image_file(src);
  }
  // ------ Speak Assets
  tiny_Frame speak;
  speak.fmax = get_length_in_directory(".//assets//speak");
  speak.fcurrent = 0;
  
  get_files_in_directory(speak.files, ".//assets//speak");

  for (int i = 0; i < speak.fmax; i++) { 
    char src[] = ".//assets//speak//";
    strcat(src,speak.files[i]); 
    speak.sprites[i] = kit_load_image_file(src);
  }
  // ------ Emojis Assets
  tiny_Frame emojis;
  emojis.fmax = get_length_in_directory(".//assets//emojis");
  emojis.fcurrent = -1;

  get_files_in_directory(emojis.files, ".//assets//emojis");
  
  for (int i = 0; i < emojis.fmax; i++) { 
    char src[] = ".//assets//emojis//";
    strcat(src,emojis.files[i]); 
    emojis.sprites[i] = kit_load_image_file(src);
  }
  // ------

  // ------ Voice Detector
  ma_device device;

  ma_device_config config  = ma_device_config_init(ma_device_type_capture);
  config.playback.format   = ma_format_f32;
  config.playback.channels = 0;
  config.sampleRate        = 0;
  config.dataCallback      = data_callback;

  if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) return -1;
  ma_device_start(&device);
  // ------

  // ------ UI
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
      kit_clear(ctx, KIT_GREEN);
      if (kit_key_pressed(ctx, VK_ESCAPE)) { break; }

      if (GetAsyncKeyState(VK_F1) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 0;
      if (GetAsyncKeyState(VK_F2) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 1;
      if (GetAsyncKeyState(VK_F3) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 2;
      if (GetAsyncKeyState(VK_F4) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 3;
      if (GetAsyncKeyState(VK_F5) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 4;
      if (GetAsyncKeyState(VK_F6) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 5;
      if (GetAsyncKeyState(VK_F7) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 6;
      if (GetAsyncKeyState(VK_F8) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 7;
      if (GetAsyncKeyState(VK_F9) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 8;
      if (GetAsyncKeyState(VK_F10) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 9;
      if (GetAsyncKeyState(VK_F11) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 10;
      if (GetAsyncKeyState(VK_F12) && GetAsyncKeyState(VK_CONTROL)) emojis.fcurrent = 10;
      
      if (kit_key_down(ctx, VK_CONTROL)) {
        char f[10];
        sprintf(f, "%f", MIN_FREQUENCY);

        kit_draw_text(ctx, KIT_WHITE, f, 2, 2);

        if (kit_key_pressed(ctx, VK_OEM_PLUS)) MIN_FREQUENCY += 0.001;
        if (kit_key_pressed(ctx, VK_OEM_MINUS)) MIN_FREQUENCY -= 0.001;
      }
 
      if (is_speaking && emojis.fcurrent == -1) {
        if ((int)speak.fcurrent >= speak.fmax) speak.fcurrent = 0;
        kit_draw_image(ctx, speak.sprites[(int) speak.fcurrent], x, y);
        speak.fcurrent += delta/0.1;
      }

      if (!is_speaking && emojis.fcurrent == -1) {
        if ((int)idle.fcurrent >= idle.fmax) idle.fcurrent = 0;
        kit_draw_image(ctx, idle.sprites[(int) idle.fcurrent], x, y);
        idle.fcurrent += delta/0.1;
      }

      if (emojis.fcurrent != -1) {
        emojis.fcurrent = emojis.fcurrent >= emojis.fmax ? emojis.fmax-1 : emojis.fcurrent;
        kit_draw_image(ctx, emojis.sprites[(int)emojis.fcurrent], x, y);
      }

      emojis.fcurrent = -1;
  }
  // ------

  kit_destroy(ctx);
  ma_device_uninit(&device);

  return 0;
}
