#include "resource_manager.h"
#include "memory"
#include "filesystem"

namespace resource_manager {

std::string Read(const std::string &path) {
  auto close_file = [](FILE *f) { fclose(f); };

  auto holder = std::unique_ptr<FILE, decltype(close_file)>(fopen(path.c_str(), "rb"), close_file);
  if (!holder)
    return "";
  FILE *f = holder.get();
  auto size = std::filesystem::file_size(path);
  std::string res;
  res.resize(size);
  fread(const_cast<char *>(res.data()), 1, size, f);
  return res;
}

}