#pragma once

#include <string>
#include <unordered_map>

#include <nnmcpp/stream.hpp>

namespace nnmcpp::parsing {

  enum class LexemType { EMPTY, TEXT, COLON, NEWLINE };

  static const std::unordered_map<LexemType, std::string> kLexemTypeNaming
      = {{LexemType::EMPTY, "EMPTY"},
         {LexemType::TEXT, "TEXT"},
         {LexemType::COLON, "COLON"},
         {LexemType::NEWLINE, "NEWLINE"}};

  struct Lexem {
    Lexem();
    Lexem(LexemType t);
    Lexem(LexemType t, const std::string& v);

    LexemType type;
    std::string value;
  };

  using LexemStream = Stream<Lexem>;

  class Lexer {
  public:
    LexemStream Tokenize(std::istream& in);

  private:
    Lexem parseLexem(std::istream& in);
  };

}  // namespace nnmcpp::parsing
