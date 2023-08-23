#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "stream.hpp"

#define LABELED_PAIR(x) \
  { #x, x }

namespace nnmcpp::parsing {

  using ParserStream = Stream<std::pair<std::string, std::string>>;

  struct Info {
    const std::string& get(const std::string& k);
    void set(const std::string& k, const std::string& v);

  private:
    const std::unordered_map<std::string, std::string&> fields
        = {LABELED_PAIR(title),      LABELED_PAIR(translaiton), LABELED_PAIR(year),
           LABELED_PAIR(director),   LABELED_PAIR(country),     LABELED_PAIR(production),
           LABELED_PAIR(resolution), LABELED_PAIR(genre),       LABELED_PAIR(audio),
           LABELED_PAIR(actor)};

    std::string title;
    std::string translaiton;
    std::string year;
    std::string director;
    std::string country;
    std::string production;
    std::string resolution;
    std::string genre;
    std::string audio;
    std::string actor;
  };

  class Parser {
  public:
    Info parse(std::istream& in);

  private:
  };

}  // namespace nnmcpp::parsing
