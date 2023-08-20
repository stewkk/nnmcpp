#include <fmt/format.h>

#include <nnmcpp/serializer.hpp>

namespace nnmcpp {

  std::string Serialize(const Info& info) {
    // return fmt::format("{} \\{} \\{}\\{} / {} / {}\\{}\\{}\\{}\\{}\\{}\\{}\\{}\\{}\\{}\\");
    return info.title;
  }

}  // namespace nnmcpp
