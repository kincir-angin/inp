#include <stdio.h>
#include <string.h>
#include <create-package.h>

void usage() {
  printf("Usage: \n"
         "  install  To install package\n");
}

int main(int argc, char const *argv[]) {
  if (create_package("./contoh") == RET_OK) {
    fprintf(stdout, "BERHASIL\n");
    exit(0);
  }
  // for (int i = 0; i < argc; i++) {
  //   if (strcmp(argv[i], "create-package") == 0) {
  //     /* Fungsi untuk Buat package */
  //     create_package(argv[i + 1]);
  //   } else if (strcmp(argv[i], "install") == 0) {
  //     /* Fungsi untuk install package */
  //   } else if (strcmp(argv[i], "uninstall") == 0) {
  //     /* Fungsi untuk uninstall package */

  //   } else if (strcmp(argv[i], "list-packages") == 0) {

  //   } else if (strcmp(argv[i], "help") == 0) {
  //     usage();
  //   } else {
  //     printf("In Package Manager (inp) v0.1\n");
  //   }
  // }

  return 0;
}
