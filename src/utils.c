// check if int array have a specific int value.
int array_has(int arr[], int value) {
  for (int i=0; i < sizeof(*arr); i++) {
    if (arr[i] == value) return 1;
  }

  return 0;
}

int update_history_index() {
  static int x = -1;
  x++;
  if (x > 7) x=-1;
  return x;
}
