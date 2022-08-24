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
#include <string>
#include <array>
#include <Utils.h>
#include <archive.h>
#include <archive_entry.h>
#include <libtar.h>
#include <ncurses.h>
#include <dirent.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define PREINSTALL  ".tmp/preinstall.sh" 
#define POSTINSTALL ".tmp/postinstall.sh" 

/* App operation */
enum OP {
  INSTALL = 0,
  REMOVE = 1,
  RECREATE = 2,
  UPDATE = 3,
  CHANGE = 4,
  CREATE = 5
};

class App {
  public:
    App(int operation, std::string filename);
    ~App();

    void CountTotal();
    void Extract();
    static int CopyData(struct archive *ar, struct archive *aw);
    int CreatePackage();
    int CreateMetaData();
    int CreateContents(int indent);

  private:
    std::string m_filename;
    std::vector<std::string> m_files;
    int m_total;
};

#endif