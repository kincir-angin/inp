/*
 * Author: Naufal Adriansyah
 * Copyright (C) Kincir Angin's Open Source License 
 */
#ifndef INSTALL_H
#define INSTALL_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <utils.h>
#include <archive.h>
#include <archive_entry.h>
#include <libtar.h>
#include <ncurses.h>

#define PREINSTALL  ".tmp/preinstall.sh" 
#define POSTINSTALL ".tmp/postinstall.sh" 

class Install {
  public:
    Install(std::string filename);
    ~Install();

    void CountTotal();
    void Extract();
    static int CopyData(struct archive *ar, struct archive *aw);
    int ReadAndExecute(const char* file);
    static std::string exec(const char* cmd);

  private:
    std::string m_filename;
    int m_total;
};

#endif