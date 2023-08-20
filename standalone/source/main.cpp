#include <nnmcpp/version.h>

#include <cxxopts.hpp>
#include <fs_finder.hpp>
#include <fstream>
#include <iostream>
#include <nnmcpp/serializer.hpp>
#include <string>
#include <unordered_map>

using nnmcpp::standalone::FindInfoFiles;

auto main(int argc, char** argv) -> int {
  cxxopts::Options options(*argv, ".info file parser");

  std::string path;

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("d,dry", "Only print paths to .info files")
    ("path", "Path to search .info files", cxxopts::value(path)->default_value("."))
  ;
  // clang-format on

  options.parse_positional({"path"});

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (result["version"].as<bool>()) {
    std::cout << "Nnmcpp, version " << NNMCPP_VERSION << std::endl;
    return 0;
  }

  const auto paths = nnmcpp::standalone::FindInfoFiles(path);

  if (result["dry"].as<bool>()) {
    for (auto info_file_path : paths) {
      std::cout << info_file_path << '\n';
    }
    return 0;
  }

  std::cerr << "FIXME: parsing is not yet implemented";
  for (auto info_file_path : paths) {
    std::ifstream f(info_file_path);
    std::stringstream buf;
    buf << f.rdbuf();
    // NOTE: get file contents with buf.str()

    const auto info = nnmcpp::Info{"test", "translation"};  // TODO: call parser here
    std::cout << nnmcpp::Serialize(info) << '\n';
  }

  return 0;
}
