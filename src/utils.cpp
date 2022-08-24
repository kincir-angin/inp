#include <utils.h>

void Utils::CreateDirectory(char* pathname, int mode) {
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
      CreateDirectory(pathname, 0755);
      *p = '/';
      r = mkdir(pathname, mode);
    }
  }

  if (r != 0) {
    std::cout << "Cannot create directory" << std::endl;
  }
}

FILE* Utils::CreateFile(char* pathname, int mode) {
  FILE *f;

	f = fopen(pathname, "wb+");
	if (f == NULL) {
		/* Try creating parent dir and then creating file. */
		char *p = strrchr(pathname, '/');
		if (p != NULL) {
			*p = '\0';
			CreateDirectory(pathname, 0755);
			*p = '/';
			f = fopen(pathname, "wb+");
		}
	}

	return (f);
}

int Utils::IsEndOfArchive(const char *p) {
	for (int i = 511; i >= 0; --i) {
    if (p[i] != '\0')
			return (0);
  }

	return (1);
}

int Utils::ParseOctal(const char* p, size_t n) {
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

int Utils::VerifyChecksum(const char* p) {
	int n, u = 0;
	for (n = 0; n < 512; ++n) {
		if (n < 148 || n > 155)
			/* Standard tar checksum adds unsigned bytes. */
			u += ((unsigned char *)p)[n];
		else
			u += 0x20;
	}
  
	return (u == Utils::ParseOctal(p + 148, 8));
}
