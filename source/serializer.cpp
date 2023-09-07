#include "nnmcpp/serializer.hpp"

#include <boost/algorithm/string/join.hpp>
#include <fmt/format.h>

namespace nnmcpp {

  std::string Serialize(const Info& info) {
    std::vector<std::string> fields = {
	    boost::algorithm::join(std::vector<std::string>{info.title.ru_title, info.title.en_title}, " / "),
        info.translation.raw, /* TODO: strange encoding of translation type */
        info.title.year,
        boost::algorithm::join(
            std::vector<std::string>{info.director.raw, info.country.raw, info.production.raw}, " / "),
        "", /* TODO: place on hard drive */
        fmt::format("{}x{}", info.video.width, info.video.height),
        info.quality.raw,
        info.duration.raw,
        info.genre.raw,
        "", /* empty field */
        "", /* TODO: extra content file */
        info.audio.raw,
        info.subtitles.raw,
        info.actor.raw,
        "" /* empty field */
    };
    return boost::algorithm::join(fields, " \\ ");
  }

}  // namespace nnmcpp
