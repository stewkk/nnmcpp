#include <istream>
#include <nnmcpp/parser.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <regex>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include "tokenizer.hpp"
#include "langs.hpp"

using namespace nnmcpp::parsing;

static std::string strip(const std::string& str) {
  size_t fs = 0;
  int ls = 0;

  for (auto s = str.begin(); s != str.end(); s++) {
    if (*s != ' ') {
      fs = std::distance(str.begin(), s);
      break;
    }
  }

  for (auto s = str.rbegin(); s != str.rend(); s++) {
    if (*s != ' ') {
      ls = str.size() - std::distance(str.rbegin(), s) - fs;
      break;
    }
  }

  if (ls < 0)
    return "";

  return str.substr(fs, ls);
}

void Title::parse(const std::string& target) {
  std::regex pattern(R"((.*?)( ?\/ ?(.*))? ?\(((?:18|19|20)\d{2})\).*)");
  std::smatch match;
  std::regex_search(target, match, pattern);

  if (match.empty()) {
    throw std::runtime_error("Wrong title format");
  }

  raw = target;

  ru_title = match.str(1);
  en_title = match.str(3);
  year = match.str(4);
}

void Video::parse(const std::string& target) {
  std::regex pattern(R"(\s*.*, (?:\d+x\d+@)*(\d+)x(\d+), ~\d+ Kbps)");
  std::smatch match;
  std::regex_search(target, match, pattern);

  if (match.empty()) { throw std::runtime_error("Wrong resolution format");
  }

  raw = target;

  width = match.str(1);
  height = match.str(2);
}

void Actors::parse(const std::string& target) {
  std::regex pattern(R"((^.*?)( и др\.)?$)");
  std::smatch match;
  std::regex_search(target, match, pattern);

  if (match.empty()) {
    throw std::runtime_error("Wrong actors format");
  }


  raw = target;

  std::stringstream stream_target(match.str(1));
  std::string actor;

  while (stream_target.good()) {
    std::getline(stream_target, actor, ',');
    actors.push_back(actor);
  }
}

std::string Normalized(const std::string& s) {
  return strip(s);
}

void Subtitles::parse(const std::string& target) {
  raw = target;

  std::stringstream stream_target(target);
  std::string lang;

  while (stream_target.good()) {
    std::getline(stream_target, lang, ',');
    if (auto it = kNormalizedLangs.find(Normalized(lang)); it != kNormalizedLangs.end()) {
      langs.push_back(it->second);
    }
  }
}

void Audio::parse(const std::string& target) {
  AudioUnit au;
  au.parse(target);
  units.push_back(au);
}

void Audio::AudioUnit::parse(const std::string& target) {
  std::regex pattern(R"(^\s*.*?, \d+ ch, \d+ Kbps(?: -|,) (.*)$)");
  std::smatch match;
  std::regex_search(target, match, pattern);

  if (match.empty()) {
    throw std::runtime_error("Wrong audio format");
  }

  raw = target;
  if (auto it = kNormalizedLangs.find(Normalized(match.str(1))); it != kNormalizedLangs.end()) {
    lang = it->second;
  }
}

void Duration::parse(const std::string& target) {
  std::regex pattern(R"(\s*(\d{2}):(\d{2}):(\d{2}))");
  std::smatch match;
  std::regex_search(target, match, pattern);

  if (match.empty()) {
    throw std::runtime_error("Wrong time format");
  }

  raw = target;

  minutes = std::stoi(match.str(1)) * 60 + std::stoi(match.str(2));
}

void Production::parse(const std::string& target) {
  raw = target;

  std::vector<std::string> fields;
  boost::algorithm::split(fields, target, boost::algorithm::is_any_of("/"));
  for (auto& el : fields) {
    el = strip(el);
  }
  if (!fields.empty()) {
    country = fields[0];
  }
  if (fields.size() > 1) {
    producer_company = fields[1];
  }
}

void StringField::parse(const std::string& target) {
  raw = strip(target);
}

const std::string& Info::get(const std::string& k) {
  auto field = fields.find(k);

  if (field == fields.end()) {
    throw std::logic_error("Not such field in structure");
  }

  return field->second.raw;
}

void Info::set(const std::string& k, const std::string& v) {
  auto field = fields.find(k);

  if (field == fields.end()) {
    throw std::logic_error("Not such field in structure");
  }

  field->second.parse(v);
}

Info Parser::parse(std::istream& in) {
  Info info;

  Tokenizer tokenizer;
  TokenStream token_stream = tokenizer.Tokenize(in);

  Token title = token_stream.Read();

  if (title.type != TokenType::TITLE) {
    throw std::logic_error("There is no title in parsing text");
  }

  info.set("title", title.value);

  std::string key, value;
  int state = 0;

  for (auto token = token_stream.Read(); token.type != TokenType::EMPTY;
       token = token_stream.Read()) {

    switch (state) {
      case 0:
        if (token.type != TokenType::KEY) continue;

        key = token.value;
        state = 1;
        break;
      case 1:
        if (token.type != TokenType::TEXT) continue;

        value = token.value;

        try {
          info.set(key, value);
        } catch (...) {}

        state = 0;
        break;
    }
  }

  return info;
}
