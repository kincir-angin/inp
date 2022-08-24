/*
 * Author: Naufal Adriansyah
 * Copyright (C) Kincir Angin's Open Source License 
 */
#include <install.h>

Install::Install(std::string filename)
  : m_filename(filename) {
  /* 1. Count total, needed for ncurses progress bar */
  CountTotal();
  /* 2. Read and Execute preinstall.sh */
  ReadAndExecute(PREINSTALL);
  /* 3. Extract the tarbal to temp folder */
  Extract();
  /* 4. Read and Execute postinstall.sh */
  ReadAndExecute(POSTINSTALL);
}

Install::~Install() {

}

void Install::CountTotal() {
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

void Install::Extract() {
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
  Install::exec("mv packages .tmp");
  Install::exec("mv postinstall.sh .tmp");
  Install::exec("mv preinstall.sh .tmp");
}

int Install::CopyData(struct archive *ar, struct archive *aw) {
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

int Install::ReadAndExecute(const char* file) {
  std::fstream stream(file);
  std::string line;

  while (getline(stream, line)) {
    std::cout << Install::exec(line.c_str()) << std::endl;
  }

  return 0;
}

std::string Install::exec(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}