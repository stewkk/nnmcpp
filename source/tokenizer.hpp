#include <istream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "keys.hpp"
#include "lexer.hpp"
#include "stream.hpp"

namespace nnmcpp::parsing {

  enum class TokenType {
    EMPTY,
    KEY,
    TEXT,
    TITLE,
  };

  static const std::unordered_map<TokenType, std::string> kTokenTypeNaming
      = {{TokenType::EMPTY, "EMPTY"},
         {TokenType::KEY, "KEY"},
         {TokenType::TEXT, "TEXT"},
         {TokenType::TITLE, "TITLE"}};

  enum class TokenizerState { WT, TT, P, PCH, LT, KT, CH, T, MT, F, B };

  static const std::unordered_map<TokenizerState, std::string> kTokenizerStateNaming
      = {{TokenizerState::WT, "WAITING_TITLE"}, {TokenizerState::TT, "TITLE"},
         {TokenizerState::P, "PARAGRAPH"},      {TokenizerState::PCH, "PARAGRAPH_CHECK"},
         {TokenizerState::LT, "LINE_TEXT"},     {TokenizerState::KT, "KEY_TEXT"},
         {TokenizerState::CH, "CHECK"},         {TokenizerState::T, "TEXT"},
         {TokenizerState::MT, "MULTI_TEXT"},    {TokenizerState::F, "FINAL"},
         {TokenizerState::B, "BROKEN"}};

  struct Token {
    Token();
    Token(TokenType t);
    Token(TokenType t, const std::string& v);

    TokenType type;
    std::string value;
  };

  using TokenStream = Stream<Token>;

  class Tokenizer {
  public:
    Tokenizer();
    TokenStream Tokenize(std::istream& in);

  private:
    Token key;
    Token text;
    Token title;

    TokenizerState state;
    TokenStream stream;

    void transit(Lexem lex);
  };

}  // namespace nnmcpp::parsing
