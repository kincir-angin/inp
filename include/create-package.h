#ifndef CREATE_PACKAGE_H
#define CREATE_PACKAGE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <return.h>
#include <utils/common.h>
#include <utils/pair.h>
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

struct package_t {
  FILE* fp;
  char* preinstall;
  char* postinstall;
  char* meta_data;
  char* license;
};

int create_metadata(const char* path);
int create_package(const char* _project_name);
int create_contents(int c, const char* root, const char* path);
static void create(const char* file_name);

#endif