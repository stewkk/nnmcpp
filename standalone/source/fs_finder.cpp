#include "fs_finder.hpp"

#include <fmt/format.h>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace nnmcpp::standalone {

  namespace {

    static const std::string kInfoName = "info.txt";

  }  // namespace

  std::vector<std::string> FindInfoFiles(const std::string& path) {
    std::vector<std::string> res;

    try {
      for (const auto& dir_entry : fs::recursive_directory_iterator(path)) {
        if (dir_entry.is_regular_file() && dir_entry.path().filename() == kInfoName) {
          res.push_back(dir_entry.path());
        }
      }
    } catch (const std::exception& e) {
      std::cerr << fmt::format("unhandled exception in fs_finder: {}\n", e.what());
    }

    return res;
  }

}  // namespace nnmcpp::standalone
