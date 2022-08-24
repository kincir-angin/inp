/*
 * Author: Naufal Adriansyah
 * Copyright (C) Kincir Angin's Open Source License 
 */
#include <iostream>
#include <cstring>
#include <install.h>
#include <utils.h>

/* DEBUG 1 */
#include <fstream>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cctype>
#include <string>
#include <array>

using json = nlohmann::json;

int main(int argc, char const *argv[]) {
  // std::ios::sync_with_stdio(0);
  
  // for (int i = 0; i < argc; i++) {
  //   if (strcmp(argv[i], "install") == 0) {
      // Install install("../../linux-headers.tar.xz");
  //   }
  // }
  std::fstream stream("meta-data.json");

  if (stream.fail()) {
    std::string answer;
    json config;

    std::cout << "missing meta-info.json, do you wan't to create it? (Y/n)" << std::endl;
    std::cin >> answer;

    if (std::tolower(answer.c_str()[0]) == 'y') {
      std::cout << "creating meta-info.json" << std::endl;

      std::array<std::string, 6> project_config = {
        "name", "version", "description", "author", "organization", "contents"
      };

      std::array<std::string, 3> license_config = {
        "name", "version", "content"
      };

      for (auto i : project_config) {
        std::string s;
        
        std::cout << "[PROJECT] Enter " << i << " detail: ";
        std::getline(std::cin >> std::ws, s);

        config["project"][i] = s;
      }

      for (auto i : license_config) {
        std::string s;
        
        std::cout << "[LICENSE] Enter " << i << " detail: ";
        std::getline(std::cin >> std::ws, s);

        config["license"][i] = s;
      }

      std::ofstream file("meta-info.json");
      file << config;

      std::cout << "created meta-info.conf..." << std::endl;
    } else {
      std::cout << "exiting..." << std::endl;
    }
  }

  json data = json::parse(stream);
  std::cout << data << std::endl;

  return 0;
}
