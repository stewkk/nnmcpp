#include <nnmcpp/serializer.hpp>

namespace nnmcpp {

std::string Serialize(const Info& info) {
    return info.title;
}

}  // namespace nnmcpp
