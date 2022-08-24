#ifndef UTILS_H
#define UTILS_H
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/stat.h>

class Utils {
  public:
    /*
     * Membuat directory. Digunakan untuk membuat directory 
     * packages.
     */
    static void CreateDirectory(char* pathname, int mode);
    
    /*
     * Membuat file dari file yang sudah ada. 
     */
    static FILE* CreateFile(char* pathname, int mode);

    static int IsEndOfArchive(const char* p);
    static int ParseOctal(const char* p, size_t n);
    static int VerifyChecksum(const char* p);
};

#endif