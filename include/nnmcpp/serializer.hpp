#pragma once

#include <optional>
#include <string>

namespace nnmcpp {

  struct Info {
    std::string title;
    std::optional<std::string> translation;
  };

  std::string Serialize(const Info& info);

}  // namespace nnmcpp
