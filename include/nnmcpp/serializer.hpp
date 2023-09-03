#pragma once

#include <nnmcpp/parser.hpp>
#include <optional>
#include <string>

namespace nnmcpp {

  using Info = nnmcpp::parsing::Info;

  std::string Serialize(const Info& info);

}  // namespace nnmcpp
