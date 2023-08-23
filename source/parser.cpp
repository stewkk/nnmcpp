#include "parser.hpp"

#include <istream>
#include <map>
#include <stdexcept>
#include <utility>

#include "tokenizer.hpp"

using namespace nnmcpp::parsing;

const std::string& Info::get(const std::string& k) {
  auto field = fields.find(k);

  if (field == fields.end()) throw std::logic_error("Not such field in structure");

  return field->second;
}

void Info::set(const std::string& k, const std::string& v) {
  auto field = fields.find(k);

  if (field == fields.end()) throw std::logic_error("Not such field in structure");

  field->second = v;
}

Info Parser::parse(std::istream& in) {
  Info info;

  Tokenizer tokenizer;
  TokenStream token_stream = tokenizer.Tokenize(in);

  Token title = token_stream.Read();

  if (title.type != TokenType::TITLE) throw std::logic_error("There is no title in parsing text");

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
        info.set(key, value);
        state = 0;
        break;
    }
  }

  return info;
}
