#include <create-package.h>

char* data_project[] = { "name", "version", "author", "organization", "license" };
char* data_license[] = { "name", "version", "content" };

int create_metadata(const char* path) {
  FILE* fo = fopen(path, "w");

  /* Loop through project data */
  fputs("[project]\n", fo);
  for (int i = 0; i < 5; i++) {
    clean_stdin();

    /* 512 bytes should be enough */
    char* answer = (char*) malloc(512);
    char key[13];
    strcpy(key, data_project[i]);

    fprintf(stdout, "[PROJECT] Enter %s details: ", data_project[i]);
    scanf("%[^\n]s", answer);

    /* Create a pair data (key = value) */
    char* pair = make_pair(data_project[i], answer);

    /* Write to file */
    fputs(pair, fo);

    free(answer);
  }

  /* Loop through license data */
  fputs("\n[license]\n", fo);
  for (int i = 0; i < 3; i++) {
    clean_stdin();

    /* 512 bytes should be enough */
    char* answer = (char*) malloc(512);
    char key[13];
    strcpy(key, data_license[i]);

    fprintf(stdout, "[LICENSE] Enter %s details: ", data_license[i]);
    scanf("%[^\n]s", answer);

    /* Create a pair data (key = value) */
    char* pair = make_pair(data_license[i], answer);

    /* Write to file */
    fputs(pair, fo);

    free(answer);
  }

  /* Loop through license data */
  fclose(fo);

  return 0;
}

int create_package(const char* _project_name) {
  /* Project directories */
  char* PROJ_DIR = (char*) _project_name;
  char* META_DIR = project_directory(PROJ_DIR, "/META_DATA");
  char* PREINSTALL_FILE  = project_directory(PROJ_DIR, "/preinstall.sh");
  char* POSTINSTALL_FILE = project_directory(PROJ_DIR, "/postinstall.sh");

  /* Check META_DATA */
  FILE* fp = fopen(META_DIR, "r");
  if (fp == NULL) {
    char answer;
   
    fprintf(stdout, "missing META_DATA. do you want to create it? (Y/n)\n");
    scanf("%c", &answer);

    if (answer == 'Y' || answer == 'y') {
      if (create_metadata(META_DIR) == RET_OK) {
        fprintf(stdout, "META_DATA is created.\n");
      } else {
        fprintf(stderr, "error while creating META_DATA\n");
        exit(RET_ERR);
      }
    } else {
      fprintf(stderr, "exiting...\n");
      exit(RET_ERR);
    }
  }

  fclose(fp);

  /* Check Preinstall script */
  FILE* fppre = fopen(PREINSTALL_FILE, "r");

  if (fppre == NULL) {
    fprintf(stderr, "preinstall.sh is missing, skipping...\n");
  } else {
    fprintf(stdout, "pre-install script found\n");
    fclose(fppre);
  }

  /* Check Postinstall script */
  FILE* fppost = fopen(POSTINSTALL_FILE, "r");

  if (fppost == NULL) {
    fprintf(stderr, "postinstall.sh is missing, skipping...\n");
  } else {
    fprintf(stdout, "post-install script found\n");
    fclose(fppost);
  }

  /* Create CONTENTS file */
  if (create_contents(0, _project_name, _project_name) != RET_OK) {
    fprintf(stdout, "cannot create CONTENTS\n");
    exit(RET_ERR);
  }

  /* Create tarball */
  // create(PROJ_DIR);

  return RET_OK;
}

int create_contents(int c, const char* root, const char* path) {
  // fprintf(stdout, "BEING CALLED: %d\n", c);
  char* package_dir;
  char* contents = project_directory(root, "/CONTENTS");
  FILE* fp;

  if (c == 0) {
    package_dir = project_directory(path, "/package");
    fp = fopen(contents, "w+");
  } else {
    package_dir = (char*) path;
    fp = fopen(contents, "r+");
  }
  
  DIR* dir = opendir(package_dir);

  if (dir != NULL && fp != NULL) {
    struct dirent *dp;

    while ((dp = readdir(dir)) != NULL) {
      if (dp->d_name[0] != '.') {
        char *fullpath = malloc(strlen(package_dir) + strlen(dp->d_name) + 2);
        strcpy(fullpath, package_dir);
        strcat(fullpath, "/");
        strcat(fullpath, dp->d_name);

        if (dp->d_type == DT_DIR) {
          fprintf(stdout, "%s\n", fullpath);
          create_contents(1, root, (const char*) fullpath);
        }

        char* file = malloc(strlen(fullpath) + 256);
        strcat(file, strdup(fullpath));
        strcat(file, "\n");
        fputs(file, fp);
        free(file);
      }
    }
    
    closedir(dir);
  }

  free(package_dir);
  free(fp);
  return 0;
}

static char buff[16384];

static void create(const char* file_name) {
  char* filename = (char*) malloc(512);
  char* proj_dir = (char*) malloc(512);

  strcat(filename, file_name);
  strcat(filename, ".tar");
  strcat(proj_dir, file_name);
  strcat(proj_dir, "/");

  struct archive* a;
  struct archive_entry* entry;
  ssize_t len;
  int fd, r;

  a = archive_write_new();
  
  /* filter gzip */
  archive_write_add_filter_gzip(a);
  archive_write_set_format_ustar(a);

	if (proj_dir != NULL && strcmp(proj_dir, "-") == 0)
		proj_dir = NULL;
	archive_write_open_filename(a, filename);

  /* Start reading disk */
  struct archive* disk = archive_read_disk_new();
  archive_read_disk_set_standard_lookup(disk);
  r = archive_read_disk_open(disk, proj_dir);

  if (r != ARCHIVE_OK) {
    fprintf(stderr, "archive error: %s\n", archive_error_string(disk));
    exit(1);
  }

  for (;;) {
    int needcr = 0;

    entry = archive_entry_new();
    r = archive_read_next_header2(disk, entry);

    if (r == ARCHIVE_EOF)
      break;

    if (r != ARCHIVE_OK) {
      fprintf(stderr, "archive error: %s\n", archive_error_string(disk));
      exit(1);
    }

    archive_read_disk_descend(disk);

    /* verbose by default */
    fprintf(stdout, "a %s\n", archive_entry_pathname(entry));
    needcr = 1;

    char* entry_name = (char*) archive_entry_pathname(entry);
    char* tmp = str_replace(entry_name, "./contoh", ".");
    archive_entry_set_pathname(entry, tmp);

    r = archive_write_header(a, entry);
    
    if (r < ARCHIVE_OK) {
      fprintf(stderr, "archive error: %s\n", archive_error_string(a));
      needcr = 1;
    }

    if (r == ARCHIVE_FATAL)
      exit(1);

    if (r > ARCHIVE_FAILED) {
      fd = open(archive_entry_sourcepath(entry), O_RDONLY);
      len = read(fd, buff, sizeof(buff));

      while (len > 0) {
        archive_write_data(a, buff, len);
        len = read(fd, buff, sizeof(buff));
      }

      close(fd);
    }

    archive_entry_free(entry);
  }

  archive_read_close(disk);
  archive_read_free(disk);
  archive_write_close(a);
	archive_write_free(a);
}