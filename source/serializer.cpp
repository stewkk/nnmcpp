#include "nnmcpp/serializer.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <fmt/format.h>

namespace nnmcpp {

  std::string Serialize(const Info& info) {
    std::vector<std::string> fields = {
        info.title.ru_title,
        info.title.en_title,
        info.title.year,
        info.translation.raw,
        info.director.raw,
        info.production.country.value_or(""),
        info.production.producer_company.value_or(""),
        fmt::format("{}x{}", info.video.width, info.video.height),
        info.quality.raw,
        fmt::format("{}min", info.duration.minutes),
        info.genre.raw,
        boost::algorithm::join(info.audio.units | boost::adaptors::transformed([](const auto& u){return u.lang;}), ","),
        boost::algorithm::join(info.subtitles.langs, ","),
        info.actors.raw,
    };
    return boost::algorithm::join(fields, "\\");
  }

}  // namespace nnmcpp
