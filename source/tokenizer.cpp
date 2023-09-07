#include "tokenizer.hpp"

#include <iostream>

#include "lexer.hpp"

using namespace nnmcpp::parsing;

Token::Token() : type(TokenType::EMPTY) {}

Token::Token(TokenType t) : type(t) {}

Token::Token(TokenType t, const std::string& v) : type(t), value(v) {}

Tokenizer::Tokenizer()
    : key(TokenType::KEY),
      text(TokenType::TEXT),
      title(TokenType::TITLE),
      state(TokenizerState::WT),
      stream() {}

TokenStream Tokenizer::Tokenize(std::istream& in) {
  LexemStream lexem_stream = Lexer().Tokenize(in);

  for (Lexem lexem = lexem_stream.Read(); lexem.type != LexemType::EMPTY;
       lexem = lexem_stream.Read()) {
    // std::cout << kLexemTypeNaming.find(lexem.type)->second << ": " << lexem.value << std::endl;
    transit(lexem);
  }

  transit(lexem_stream.Read());

  return stream;
}

void Tokenizer::transit(Lexem lex) {
  switch (state) {
    case TokenizerState::WT:
      switch (lex.type) {
        case LexemType::EMPTY:
          state = TokenizerState::F;
          return;
        case LexemType::TEXT:
        case LexemType::COLON:
          state = TokenizerState::TT;
          title.value += lex.value;
          return;
        case LexemType::NEWLINE:
          state = TokenizerState::WT;
          return;
      }

      break;

    case TokenizerState::TT:
      switch (lex.type) {
        case LexemType::EMPTY:
          state = TokenizerState::F;
          return;
        case LexemType::TEXT:
        case LexemType::COLON:
          state = TokenizerState::TT;
          title.value += lex.value;
          return;
        case LexemType::NEWLINE:
          state = TokenizerState::P;
          stream.Put(title);
          return;
      }

      break;

    case TokenizerState::P:
      switch (lex.type) {
        case LexemType::EMPTY:
          state = TokenizerState::F;
          return;
        case LexemType::TEXT:
          state = TokenizerState::PCH;
          key.value += lex.value;
          return;
        case LexemType::COLON:
          state = TokenizerState::LT;
          text.value += lex.value;
          return;
        case LexemType::NEWLINE:
          state = TokenizerState::P;
          return;
      }

      break;

    case TokenizerState::PCH:
      switch (lex.type) {
        case LexemType::EMPTY:
          state = TokenizerState::F;
          return;
        case LexemType::TEXT:
          state = TokenizerState::PCH;
          key.value += lex.value;
          return;
        case LexemType::COLON:
          state = TokenizerState::LT;
          if (kNormalizedKeys.find(key.value) != kNormalizedKeys.end()) {
            stream.Put(Token(TokenType::KEY, kNormalizedKeys.find(key.value)->second));
            key.value.clear();
            return;
          }

          text.value += key.value;
          key.value.clear();
          return;
        case LexemType::NEWLINE:
          state = TokenizerState::KT;
          text.value += key.value;
          key.value.clear();
          return;
      }

      break;

    case TokenizerState::LT:
      switch (lex.type) {
        case LexemType::EMPTY:
          state = TokenizerState::F;
          stream.Put(Token(TokenType::TEXT, text.value));
          text.value = "";
          return;
        case LexemType::TEXT:
        case LexemType::COLON:
          state = TokenizerState::LT;
          text.value += lex.value;
          return;
        case LexemType::NEWLINE:
          state = TokenizerState::KT;
          return;
      }
      break;

    case TokenizerState::KT:
      switch (lex.type) {
        case LexemType::EMPTY:
          state = TokenizerState::F;
          stream.Put(Token(TokenType::TEXT, text.value));
          text.value = "";
          return;
        case LexemType::TEXT:
          state = TokenizerState::CH;
          key.value += lex.value;
          return;
        case LexemType::COLON:
          state = TokenizerState::T;
          text.value += lex.value;
          return;
        case LexemType::NEWLINE:
          state = TokenizerState::KT;
          stream.Put(text);
          text.value.clear();
          return;
      }
      break;

    case TokenizerState::CH:
      switch (lex.type) {
        case LexemType::EMPTY:
          state = TokenizerState::F;
          stream.Put(Token(TokenType::TEXT, text.value));
          return;
        case LexemType::TEXT:
          state = TokenizerState::CH;
          text.value += lex.value;
          return;
        case LexemType::COLON:
          if (kNormalizedKeys.find(key.value) != kNormalizedKeys.end()) {
            state = TokenizerState::LT;
            stream.Put(text);
            text.value.clear();
            stream.Put(Token(TokenType::KEY, kNormalizedKeys.find(key.value)->second));
            key.value.clear();
            return;
          }

          state = TokenizerState::T;
          text.value += key.value;
          key.value.clear();
          text.value += lex.value;
          return;

        case LexemType::NEWLINE:
          state = TokenizerState::MT;
          text.value += key.value;
          key.value.clear();
          return;
      }
      break;

    case TokenizerState::T:
      switch (lex.type) {
        case LexemType::EMPTY:
          state = TokenizerState::F;
          return;
        case LexemType::TEXT:
        case LexemType::COLON:
          state = TokenizerState::T;
          text.value += lex.value;
          return;
        case LexemType::NEWLINE:
          state = TokenizerState::MT;
          return;
      }
      break;

    case TokenizerState::MT:
      switch (lex.type) {
        case LexemType::EMPTY:
          state = TokenizerState::F;
          return;
        case LexemType::TEXT:
        case LexemType::COLON:
          state = TokenizerState::T;
          text.value += lex.value;
          return;
        case LexemType::NEWLINE:
          state = TokenizerState::P;
          stream.Put(text);
          text.value.clear();
          return;
      }
      break;

    case TokenizerState::F:
    case TokenizerState::B:
      return;
  }
}
