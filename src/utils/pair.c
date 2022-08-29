#include <utils/pair.h>

char* make_pair(char* key, char* value) {
  char* pair = (char*) malloc(512);
  strcat(pair, key);
  strcat(pair, " = ");
  strcat(pair, value);
  strcat(pair, "\n");

  return pair;
}