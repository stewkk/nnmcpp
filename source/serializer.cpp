#include "nnmcpp/serializer.hpp"

#include <boost/algorithm/string/join.hpp>
#include <fmt/format.h>

namespace nnmcpp {

  std::string Serialize(const Info& info) {
    std::vector<std::string> fields = {
	    boost::algorithm::join(std::vector<std::string>{info.title.ru_title, info.title.en_title}, " / "),
        info.translation.raw,
        info.title.year,
        boost::algorithm::join(
            std::vector<std::string>{info.director.raw, info.country.raw, info.production.raw}, " / "),
        fmt::format("{}x{}", info.video.width, info.video.height),
        info.quality.raw,
        fmt::format("{}min", info.duration.minutes),
        info.genre.raw,
        info.audio.raw,
        info.subtitles.raw,
        info.actors.raw,
    };
    return boost::algorithm::join(fields, " \\ ");
  }

}  // namespace nnmcpp
