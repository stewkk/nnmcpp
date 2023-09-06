#pragma once

#include <nnmcpp/stream.hpp>
#include <string>
#include <unordered_map>
#include <utility>

#define LABELED_PAIR(x) \
  { #x, x }

namespace nnmcpp::parsing {

  using ParserStream = Stream<std::pair<std::string, std::string>>;

  struct Field {
    virtual void parse(const std::string& target) = 0;

    std::string raw;
  };

  struct Title : public Field {
    void parse(const std::string& target) override;

    std::string ru_title;
    std::string en_title;
    std::string year;
  };

  struct Video : public Field {
    void parse(const std::string& target) override;

    std::string height;
    std::string width;
  };
  
  struct Subtitles : public Field {
    void parse(const std::string& target) override;

    std::vector<std::string> langs;
  }; 

  struct Audio : public Field {
    struct AudioUnit : public Field {
      void parse(const std::string& target) override;
      
      std::string lang;
    };

    void parse(const std::string& target) override;

    std::vector<AudioUnit> units;
  };

  struct StringField : public Field {
    void parse(const std::string& target) override;
  };

  struct Info {
    Title title;
    Video video;
    StringField subtitles;
    StringField audio;

    StringField translation;

    StringField director;
    StringField country;
    StringField production;
    StringField quality;
    StringField genre;
    StringField actor;
    StringField duration;

    const std::string& get(const std::string& k);
    void set(const std::string& k, const std::string& v);

    std::unordered_map<std::string, Field&> fields
        = {LABELED_PAIR(title),    LABELED_PAIR(translation), LABELED_PAIR(quality),
           LABELED_PAIR(director), LABELED_PAIR(country),     LABELED_PAIR(production),
           LABELED_PAIR(video),    LABELED_PAIR(genre),       LABELED_PAIR(audio),
           LABELED_PAIR(actor),    LABELED_PAIR(duration),    LABELED_PAIR(subtitles)};

    void parse_title(const std::string& title);
    void parse_video(const std::string& video);
  };

  class Parser {
  public:
    Info parse(std::istream& in);

  private:
  };

}  // namespace nnmcpp::parsing
