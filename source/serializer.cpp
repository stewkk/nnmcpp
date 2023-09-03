#include "nnmcpp/serializer.hpp"

#include <boost/algorithm/string/join.hpp>

namespace nnmcpp {

  std::string Serialize(const Info& info) {
    std::vector<std::string> fields = {
        info.title,
        info.translation, /* TODO: strange encoding of translation type */
        info.year,
        boost::algorithm::join(
            std::vector<std::string>{info.director, info.country, info.production}, "/"),
        "", /* TODO: place on hard drive */
        info.video,
        info.quality,
        info.duration,
        info.genre,
        "", /* empty field */
        "", /* TODO: extra content file */
        info.audio,
        info.subtitles,
        info.actor,
        "" /* empty field */
    };
    return boost::algorithm::join(fields, "\\");
  }

}  // namespace nnmcpp
