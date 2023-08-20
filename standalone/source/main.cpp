#include <nnmcpp/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <nnmcpp/nnmcpp.hpp>
#include <string>
#include <unordered_map>

auto main(int argc, char** argv) -> int {
  cxxopts::Options options(*argv, ".info file parser");

  std::string path;

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
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

  std::cout << path << std::endl;

  return 0;
}
