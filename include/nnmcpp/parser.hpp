#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include <nnmcpp/stream.hpp>

#define LABELED_PAIR(x) \
  { #x, x }

namespace nnmcpp::parsing {

  using ParserStream = Stream<std::pair<std::string, std::string>>;

  struct Info {
    std::string title;
    std::string translation;
    std::string year;
    std::string director;
    std::string country;
    std::string production;
    std::string video;
    std::string quality;
    std::string genre;
    std::string audio;
    std::string actor;
    std::string duration;
    std::string subtitles;

    const std::string& get(const std::string& k);
    void set(const std::string& k, const std::string& v);

    std::unordered_map<std::string, std::string&> fields
        = {LABELED_PAIR(title),      LABELED_PAIR(translation), LABELED_PAIR(year),
           LABELED_PAIR(director),   LABELED_PAIR(country),     LABELED_PAIR(production),
           LABELED_PAIR(video), LABELED_PAIR(genre),       LABELED_PAIR(audio),
           LABELED_PAIR(actor), LABELED_PAIR(quality), LABELED_PAIR(duration), LABELED_PAIR(subtitles)};
  };

  class Parser {
  public:
    Info parse(std::istream& in);

  private:
  };

}  // namespace nnmcpp::parsing
