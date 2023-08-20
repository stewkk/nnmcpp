#include <istream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "keys.hpp"

namespace nnmcpp {
namespace parsing {

enum class LexemType {
    EMPTY,
    TEXT,
    COLON,
    NEWLINE
};

static const std::unordered_map<LexemType, std::string> kLexemTypeNaming = {
    {LexemType::EMPTY, "EMPTY"},
    {LexemType::TEXT, "TEXT"},
    {LexemType::COLON, "COLON"},
    {LexemType::NEWLINE, "NEWLINE"}
};

enum class TokenType {
    EMPTY,
    KEY,
    TEXT,
    TITLE,
};

static const std::unordered_map<TokenType, std::string> kTokenTypeNaming = {
    {TokenType::EMPTY, "EMPTY"},
    {TokenType::KEY, "KEY"},
    {TokenType::TEXT, "TEXT"},
    {TokenType::TITLE, "TITLE"}
};

enum class TokenizerState {
    WT, TT, P, PCH, LT, KT, CH, T, MT, F, B
};

static const std::unordered_map<TokenizerState, std::string> kTokenizerStateNaming = {
    {TokenizerState::WT, "WAITING_TITLE"},
    {TokenizerState::TT, "TITLE"},
    {TokenizerState::P, "PARAGRAPH"},
    {TokenizerState::PCH, "PARAGRAPH_CHECK"},
    {TokenizerState::LT, "LINE_TEXT"},
    {TokenizerState::KT, "KEY_TEXT"},
    {TokenizerState::CH, "CHECK"},
    {TokenizerState::T, "TEXT"},
    {TokenizerState::MT, "MULTI_TEXT"},
    {TokenizerState::F, "FINAL"},
    {TokenizerState::B, "BROKEN"}
};

struct Lexem {
    Lexem();
    Lexem(LexemType t);
    Lexem(LexemType t, const std::string& v);

    LexemType type;
    std::string value;
};

struct Token {
    Token();
    Token(TokenType t);
    Token(TokenType t, const std::string& v);

    TokenType type;
    std::string value;
};

template <typename T>
struct Stream {
public:
    using type_t = decltype(T::type);

    Stream();
    Stream(const std::vector<T>& elements);

    T Peek() const;
    T Read();
    void Put(const T& elem);

    bool IsBroken();
    void Break();

private:
    std::queue<T> buf;
    bool broken_;

};

using TokenStream = Stream<Token>;
using LexemStream = Stream<Lexem>;

class Lexer {
public:
    LexemStream Tokenize(std::istream& in);

private:
    Lexem parseLexem(std::istream& in);

};

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

}  // namespace lexer
}  // namespace nnmcpp

