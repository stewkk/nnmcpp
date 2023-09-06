#pragma once

#include <nnmcpp/stream.hpp>
#include <string>
#include <unordered_map>
#include <utility>

#define LABELED_PAIR(x) \
  { #x, x }

namespace nnmcpp::parsing {

  using ParserStream = Stream<std::pair<std::string, std::string>>;

  struct Info {
    std::string title;
    std::string ru_title;
    std::string en_title;
    std::string year;

    std::string video;
    std::string codec;
    std::string video_w;
    std::string video_h;

    std::string translation;
    std::string director;
    std::string country;
    std::string production;
    std::string quality;
    std::string genre;
    std::string audio;
    std::string actor;
    std::string duration;
    std::string subtitles;

    const std::string& get(const std::string& k);
    void set(const std::string& k, const std::string& v);

    std::unordered_map<std::string, std::string&> fields
        = {LABELED_PAIR(title),    LABELED_PAIR(translation), LABELED_PAIR(year),
           LABELED_PAIR(director), LABELED_PAIR(country),     LABELED_PAIR(production),
           LABELED_PAIR(video),    LABELED_PAIR(genre),       LABELED_PAIR(audio),
           LABELED_PAIR(actor),    LABELED_PAIR(duration),    LABELED_PAIR(subtitles),
           LABELED_PAIR(ru_title), LABELED_PAIR(en_title), LABELED_PAIR(codec),
           LABELED_PAIR(video_w),  LABELED_PAIR(video_h),  LABELED_PAIR(quality)};

    void parse_title(const std::string& title);
    void parse_video(const std::string& video);
  };

  class Parser {
  public:
    Info parse(std::istream& in);

  private:
  };

}  // namespace nnmcpp::parsing
