/*
 * Author: Naufal Adriansyah
 * Copyright (C) Kincir Angin's Open Source License 
 */
#include <App.h>

App::App(int operation, std::string filename)
  : m_filename(filename) {
  if (operation == OP::INSTALL) {
    /* 1. Count total, needed for ncurses progress bar */
    CountTotal();
    /* 2. Read and Execute preinstall.sh */
    Utils::ReadAndExecute(PREINSTALL);
    /* 3. Extract the tarbal to temp folder */
    Extract();
    /* 4. Read and Execute postinstall.sh */
    Utils::ReadAndExecute(POSTINSTALL);
  } else if (operation == OP::CREATE) {
    int status = CreatePackage();

    if (status == 0) {
      std::cout << "created packed for " << filename << std::endl;
    } else {
      std::cout << "failed" << std::endl;
    }
  }
  
}

App::~App() {

}

void App::CountTotal() {
  int r;
  struct archive* arc;
  struct archive_entry* arc_e;

  arc = archive_read_new();
  archive_read_support_filter_all(arc);
  archive_read_support_format_all(arc);

  if ((r = archive_read_open_filename(arc, m_filename.c_str(), 10240))) {
    std::cout << "archive_read_open_filename() " << archive_error_string(arc) << std::endl;
    exit(1);
  }

  /* Counting All files */
  while (archive_read_next_header(arc, &arc_e) == ARCHIVE_OK) {
    m_total++;
    archive_read_data_skip(arc);
  }

  r = archive_read_free(arc);
}

void App::Extract() {
  char buff[512];
  ssize_t size;

  struct archive* arc;
  struct archive* ext;
  struct archive_entry* arc_e;
  int r, flags;

  flags  = ARCHIVE_EXTRACT_TIME;
  flags |= ARCHIVE_EXTRACT_PERM;
  flags |= ARCHIVE_EXTRACT_ACL;
  flags |= ARCHIVE_EXTRACT_FFLAGS;

  arc = archive_read_new();
  archive_read_support_filter_all(arc);
  archive_read_support_format_all(arc);
  ext = archive_write_disk_new();
  archive_write_disk_set_options(ext, flags);
  archive_write_disk_set_standard_lookup(ext);

  if ((r = archive_read_open_filename(arc, m_filename.c_str(), 10240))) {
    std::cout << "archive_read_open_filename() " << archive_error_string(arc) << std::endl;
    exit(1);
  }

  std::cout << "Total files of package: " << m_total << std::endl;

  for (;;) {
    r = archive_read_next_header(arc, &arc_e);

    if (r == ARCHIVE_EOF) 
      break;
    if (r != ARCHIVE_OK) {
			std::cout << "archive_read_next_header()" << archive_error_string(arc) << std::endl;
      exit(1);
    }

    r = archive_write_header(ext, arc_e);

    if (r < ARCHIVE_OK)
      fprintf(stderr, "%s\n", archive_error_string(arc));
    else if (archive_entry_size(arc_e) > 0) {
      r = CopyData(arc, ext);

      if (r < ARCHIVE_OK)
        fprintf(stderr, "%s\n", archive_error_string(arc));
      if (r < ARCHIVE_WARN)
        exit(1);
    }

    r = archive_write_finish_entry(ext);
    if (r < ARCHIVE_OK)
      fprintf(stderr, "%s\n", archive_error_string(arc));
    if (r < ARCHIVE_WARN)
      exit(1);
  }

  archive_read_close(arc);
  archive_read_free(arc);
  archive_write_close(ext);
  archive_write_free(ext);

  /* Create directory for tmp */
  Utils::CreateDirectory(".tmp", 0755);
  Utils::ExecLog("mv packages .tmp");
  Utils::ExecLog("mv postinstall.sh .tmp");
  Utils::ExecLog("mv preinstall.sh .tmp");
}

int App::CopyData(struct archive *ar, struct archive *aw) {
	int r;
	const void *buff;
	size_t size;
	int64_t offset;

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r != ARCHIVE_OK)
			return (r);
		r = archive_write_data_block(aw, buff, size, offset);
		if (r != ARCHIVE_OK) {
			std::cout << "archive_write_data_block()" << archive_error_string(aw);
			return (r);
		}
	}
}

int App::CreatePackage() {
  /* Create meta-data.json */
  CreateMetaData();
  CreateContents(1);
  
  std::ofstream file("CONTENTS");
  for (std::string line : m_files) {
    file << line << std::endl;
  }


  /* Check pre and post script */
  /* Create package */

  return 0;
}

int App::CreateMetaData() {
  std::fstream stream("meta-data.json");

  if (stream.fail()) {
    std::string answer;
    json config;

    std::cout << "missing meta-data.json, do you wan't to create it? (Y/n)" << std::endl;
    std::cin >> answer;

    if (std::tolower(answer.c_str()[0]) == 'y') {
      /* Create meta-data.json file */
      std::cout << "creating meta-data.json" << std::endl;

      std::array<std::string, 5> project_config = {
        "name", "version", "description", "author", "organization"
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

      config["project"]["content"] = "CONTENTS";

      for (auto i : license_config) {
        std::string s;
        
        std::cout << "[LICENSE] Enter " << i << " detail: ";
        std::getline(std::cin >> std::ws, s);

        config["license"][i] = s;
      }

      std::ofstream file("meta-data.json");
      file << config;
    } else {
      std::cout << "exiting..." << std::endl;
    }
  }

  return 0;
}

int App::CreateContents(int indent) {
  struct dirent *files;
  DIR *dir = opendir(m_filename.c_str());

  if (dir == NULL){
    std::cout << "failed" << std::endl;
    return -1;
  }

  while ((files = readdir(dir)) != NULL) {
    if (files->d_type == DT_DIR) {
      if (strcmp(files->d_name, ".") == 0 || strcmp(files->d_name, "..")) {
        continue;
      } else {
        char *new_path = (char *) malloc(strlen(m_filename.c_str()) + strlen(files->d_name) + 1);
        
        m_files.push_back(files->d_name);
        CreateContents(indent + 1);

        free(new_path);
      }
    }
  }
  
  closedir(dir);
}