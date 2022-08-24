/*
 * Author: Naufal Adriansyah
 * Copyright (C) Kincir Angin's Open Source License 
 */
#include <iostream>
#include <cstring>
#include <App.h>
#include <Utils.h>

/* DEBUG 1 */
#include <fstream>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cctype>
#include <string>
#include <array>

using json = nlohmann::json;

void usage() {
  
}

int main(int argc, char const *argv[]) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "install") == 0) {
      App app(OP::INSTALL, argv[i + 1]);
    } else if (strcmp(argv[i], "create-package") == 0) {
      App app(OP::CREATE, argv[i + 1]);
    }
  }

  return 0;
}
