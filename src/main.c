#include <stdio.h>
#include <string.h>
#include <create-package.h>
#include <getopt.h>
#include <config.h>
#define true 1

void usage(char* argv) {
  printf("InOS Package Manager %d"
        "Usage: %s [OPTION] [FILE OR PACKAGES] \n"
         "  OPTIONS: \n" 
         "    -i --install To install package\n"
         "    -u --uninstall To remove or uninstall package\n"
         "    -r --remove To remove or uninstall package\n"
         "    -l --list-packages To list installed package\n"
         "    -c --create-package To create package from directory\n",
         argv, PACKAGE_VERSION);
}

int main(int argc, char **argv) {
  int c; 

  while (true) {
    int longindex = 0;

    static struct option long_options[] = {
      { "install", required_argument, 0, 'i' },
      { "uninstall", required_argument, 0, 'u' },
      { "remove", required_argument, 0, 'r' },
      { "create-package", required_argument, 0, 'c' },
      { "list-packages", required_argument, 0, 'l' }
    };

    c = getopt_long(argc, argv, "i:u:r:c:l", long_options, &longindex);

    switch (c) {
      case 'i':
      case 'u':
      case 'r':
      case 'l':
        fprintf(stdout, "PASS\n"); // TODO
        break;
      case 'c':
        create_package(optarg);
        break;
      case '?':
        usage(argv[0]);
    }

    if (c == -1) break;
  }

  exit(EXIT_SUCCESS);
}
