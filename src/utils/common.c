#include <utils/common.h>

int exec(const char* cmd) {
  FILE* fp;

  fp = popen(cmd, "r");

  if (fp == NULL) {
    fprintf(stderr, "[FAILED]: cannot run command: %s", cmd);
    return RET_ERR;
  }

  pclose(fp);
  return RET_OK;
}

int exec_with_log(const char* cmd) {
  FILE* fp;

  fp = popen(cmd, "r");

  if (fp == NULL) {
    fprintf(stderr, "[FAILED]: cannot run command: %s", cmd);
    return RET_ERR;
  }

  /* 1024 bytes per line should be enough, i thought. */
  char line[1024];
  while (fgets(line, sizeof(line), fp) != NULL) {
    printf("%s", line);
  }

  pclose(fp);
  return RET_OK;
}

void clean_stdin(void) {
  int c;
  while((c = getchar()) != '\n' && c != EOF);
}


void mkdir_(char* pathname, int mode) {
  char* p;
  int r;

  /* Menghilangkan akhiran '/' */
  if (pathname[strlen(pathname - 1)] == '/')
    pathname[strlen(pathname - 1)] = '\0';

  r = mkdir(pathname, mode);

  if (r != 0) {
    p = strrchr(pathname, '/');
    
    if (p != NULL) {
      *p = '\0';
      mkdir_(pathname, 0755);
      *p = '/';
      r = mkdir(pathname, mode);
    }
  }

  if (r != 0) {
    fprintf(stderr, "cannot create directory");
  }
}

FILE* mkfile_(char* pathname, int mode) {
  FILE *f;

	f = fopen(pathname, "wb+");
	if (f == NULL) {
		/* Try creating parent dir and then creating file. */
		char *p = strrchr(pathname, '/');
		if (p != NULL) {
			*p = '\0';
			mkdir_(pathname, 0755);
			*p = '/';
			f = fopen(pathname, "wb+");
		}
	}

	return (f);
}

int is_end_of_archive(const char *p) {
	for (int i = 511; i >= 0; --i) {
    if (p[i] != '\0')
			return (0);
  }

	return (1);
}

int parse_octal(const char* p, size_t n) {
  int i = 0;

	while ((*p < '0' || *p > '7') && n > 0) {
		++p;
		--n;
	}

	while (*p >= '0' && *p <= '7' && n > 0) {
		i *= 8;
		i += *p - '0';
		++p;
		--n;
	}

	return (i);
}

char* project_directory(char* root, char* path) {
  char* dir = (char*) malloc(100);
  strcat(dir, root);
  strcat(dir, path);

  return dir;
}

char *str_replace(char *orig, char *rep, char *with) {
  char *result; // the return string
  char *ins;    // the next insert point
  char *tmp;    // varies
  int len_rep;  // length of rep (the string to remove)
  int len_with; // length of with (the string to replace rep with)
  int len_front; // distance between rep and end of last rep
  int count;    // number of replacements

  // sanity checks and initialization
  if (!orig || !rep)
    return NULL;
  len_rep = strlen(rep);
  if (len_rep == 0)
    return NULL; // empty rep causes infinite loop during count
  if (!with)
    with = "";
  len_with = strlen(with);

  // count the number of replacements needed
  ins = orig;
  for (count = 0; tmp = strstr(ins, rep); ++count) {
    ins = tmp + len_rep;
  }

  tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

  if (!result)
    return NULL;

  // first time through the loop, all the variable are set correctly
  // from here on,
  //    tmp points to the end of the result string
  //    ins points to the next occurrence of rep in orig
  //    orig points to the remainder of orig after "end of rep"
  while (count--) {
    ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep; // move to next "end of rep"
  }

  strcpy(tmp, orig);
  return result;
}