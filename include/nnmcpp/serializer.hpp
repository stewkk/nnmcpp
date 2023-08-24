#pragma once

#include <optional>
#include <string>

#include <nnmcpp/parser.hpp>

namespace nnmcpp {

  using Info = nnmcpp::parsing::Info;

  std::string Serialize(const Info& info);

}  // namespace nnmcpp
