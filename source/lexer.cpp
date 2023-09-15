#include "lexer.hpp"

#include <istream>

using namespace nnmcpp::parsing;

Lexem::Lexem() : type(LexemType::EMPTY) {}

Lexem::Lexem(LexemType t) : type(t) {}

Lexem::Lexem(LexemType t, const std::string& v) : type(t), value(v) {}

LexemStream Lexer::Tokenize(std::istream& in) {
  LexemStream stream;
  Lexem lexem;

  while (in.good() && (lexem = parseLexem(in)).type != LexemType::EMPTY) stream.Put(lexem);

  return stream;
}

Lexem Lexer::parseLexem(std::istream& in) {
  char c;
  std::string s;

  while (in.good()) {
    c = static_cast<char>(in.peek());
    switch (c) {
      case ':':
      case '\n':
        if (!s.empty()) return Lexem(LexemType::TEXT, s);

        in.get();
        return Lexem((c == '\n' ? LexemType::NEWLINE : LexemType::COLON), (c == '\n' ? "\n" : ":"));

      case '\r':
        in.get();
        break;

      default:
        in.get();
        s.push_back(c);
        break;
    }
  }

  if (!s.empty()) return Lexem(LexemType::TEXT, s);

  return Lexem(LexemType::EMPTY, "");
}
