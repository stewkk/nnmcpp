#include <iostream>
#include <istream>
#include <nnmcpp/parser.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <regex>

#include "tokenizer.hpp"
#include "langs.hpp"

using namespace nnmcpp::parsing;

void Title::parse(const std::string& target) {
  std::regex pattern(R"((.*) / (.*) \(((?:19|20)\d{2})\))");
  std::smatch match;
  std::regex_search(target, match, pattern);

  if (match.empty()) {
    throw std::runtime_error("Wrong title format");
  }

  raw = target;

  ru_title = match.str(1);
  en_title = match.str(2);
  year = match.str(3);
}

void Video::parse(const std::string& target) {
  std::regex pattern(R"(\s*.*, (?:\d+x\d+@)*(\d+)x(\d+), ~\d+ Kbps)");
  std::smatch match;
  std::regex_search(target, match, pattern);

  if (match.empty()) {
    throw std::runtime_error("Wrong resolution format");
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

void Subtitles::parse(const std::string& target) {
  raw = target;
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

void StringField::parse(const std::string& target) {
  raw = target;
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
