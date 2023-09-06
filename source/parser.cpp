#include <iostream>
#include <istream>
#include <nnmcpp/parser.hpp>
#include <stdexcept>
#include <utility>
#include <regex>

#include "tokenizer.hpp"

using namespace nnmcpp::parsing;

const std::string& Info::get(const std::string& k) {
  auto field = fields.find(k);

  if (field == fields.end()) {
    throw std::logic_error("Not such field in structure");
  }

  return field->second;
}

void Info::set(const std::string& k, const std::string& v) {
  auto field = fields.find(k);

  if (field == fields.end()) {
    throw std::logic_error("Not such field in structure");
  }

  if (field->first == "title") {
    parse_title(v);
  } else if (field->first == "video") {
    parse_video(v);
  }

  field->second = v;
}


void Info::parse_title(const std::string& title) {
  std::regex title_pattern(R"((.*) / (.*) \(((?:19|20)\d{2})\))");
  std::smatch match;
  std::regex_match(title, match, title_pattern);

  if (match.empty()) {
    throw std::runtime_error("Wrong title format");
  }

  fields.find("ru_title")->second = match.str(1);
  fields.find("en_title")->second = match.str(2);
  fields.find("year")->second = match.str(3);
}

void Info::parse_video(const std::string& video) {
  std::regex resolution_pattern(R"(\s*(.*), (?:\d+x\d+@)*((\d+)x(\d+)), ~(\d+) (Kbps))");
  std::smatch match;
  std::regex_match(video, match, resolution_pattern);

  if (match.empty()) {
    throw std::runtime_error("Wrong resolution format");
  }

  fields.find("codec")->second = match.str(1);
  fields.find("video_w")->second = match.str(3);
  fields.find("video_h")->second = match.str(4);
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
