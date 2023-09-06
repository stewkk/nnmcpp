#include "nnmcpp/serializer.hpp"

#include <boost/algorithm/string/join.hpp>

namespace nnmcpp {

  std::string Serialize(const Info& info) {
    std::vector<std::string> fields = {
        info.title.raw,
        info.translation.raw, /* TODO: strange encoding of translation type */
        info.title.year,
        boost::algorithm::join(
            std::vector<std::string>{info.director.raw, info.country.raw, info.production.raw}, "/"),
        "", /* TODO: place on hard drive */
        info.video.raw,
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
    return boost::algorithm::join(fields, "\\");
  }

}  // namespace nnmcpp
