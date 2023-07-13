#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int array_int_has(int arr[], int value, int length) {
  for (int i=0; i < length; i++) {
    if (arr[i] == value) return 1;
  }

  return 0;
}

int update_int_index(int arr[], int length) {
  static int x = -1;
  x++;
  if (x > length-1) x=-1;
  return x;
}

int get_length_in_directory(const char *p) {
  DIR *x = opendir(p);  
  struct dirent *f;
  int count = 0;

  while((f = readdir(x)) != NULL) {
    if (strcmp(f->d_name, ".") && strcmp(f->d_name, "..")) {
      count++;
    }
  }

  closedir(x);

  return count;
}

void get_files_in_directory(char ** addr, const char *p) {
  DIR *x = opendir(p);
  struct dirent *f;
  int i = 0;

  while ((f=readdir(x)) != NULL) {
    if (strcmp(f->d_name, ".") && strcmp(f->d_name, "..")) {
      char *filename = malloc(256);
      strcpy(filename,f->d_name);
      addr[i] = filename;
      i++;
    }
  }

  closedir(x);
}
