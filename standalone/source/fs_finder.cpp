#include "fs_finder.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace nnmcpp::standalone {

  std::vector<std::string> FindInfoFiles(const std::string& path) {
    try {
      for (const auto& dir_entry : fs::recursive_directory_iterator(path)) {
        std::cout << dir_entry << '\n';
      }
    } catch (const std::exception&) {
      std::cerr << "Unhandled exception in fs_finder\n";
    }

    return {path};
  }

}  // namespace nnmcpp::standalone
