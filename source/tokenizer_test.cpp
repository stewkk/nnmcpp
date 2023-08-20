#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <istream>
#include <queue>
#include <sstream>
#include <fstream>

#include "keys.hpp"
#include "tokenizer.hpp"

using namespace nnmcpp::parsing;

Lexem::Lexem() :
    type(LexemType::EMPTY) {}

Lexem::Lexem(LexemType t) :
    type(t) {}

Lexem::Lexem(LexemType t, const std::string& v) :
    type(t),
    value(v) {}

Token::Token() :
    type(TokenType::EMPTY) {}

Token::Token(TokenType t) :
    type(t) {}

Token::Token(TokenType t, const std::string& v) :
    type(t),
    value(v) {}

template <typename T>
Stream<T>::Stream() :
    buf(),
    broken_(false) {}

template <typename T>
Stream<T>::Stream(const std::vector<T>& elements) :
    Stream() {

    for (auto elem : elements) {
        Put(elem);
    }
}

template <typename T>
T Stream<T>::Peek() const {
    if (buf.empty())
        return T();

    return buf.front();
}

template <typename T>
T Stream<T>::Read() {
    if (buf.empty())
        return T();

    T elem = buf.front();
    buf.pop();
    return elem;
}

template <typename T>
void Stream<T>::Put(const T& elem) {
    buf.push(elem);
}

template <typename T>
void Stream<T>::Break() {
    broken_ = true;
}

template <typename T>
bool Stream<T>::IsBroken() {
    return broken_;
}

LexemStream Lexer::Tokenize(std::istream& in) {
    LexemStream stream;
    Lexem lexem;

    while (in.good() && (lexem = parseLexem(in)).type != LexemType::EMPTY)
        stream.Put(lexem);

    return stream;
}

Lexem Lexer::parseLexem(std::istream& in) {
    char c;
    std::string s;

    while (in.good()) {
        c = in.peek();
        switch (c) {
            case ':':
            case '\n':
                if (!s.empty())
                    return Lexem(LexemType::TEXT, s);

                in.get();
                return Lexem((c == '\n' ? LexemType::NEWLINE : LexemType::COLON), (c == '\n' ? "\n" : ":"));

            default:
                in.get();
                s.push_back(c);
        }
    }

    if (!s.empty())
        return Lexem(LexemType::TEXT, s);

    return Lexem(LexemType::EMPTY, "");
}

Tokenizer::Tokenizer() :
    key(TokenType::KEY),
    text(TokenType::TEXT),
    title(TokenType::TITLE),
    state(TokenizerState::WT),
    stream() {}

TokenStream Tokenizer::Tokenize(std::istream& in) {
    LexemStream lexem_stream = Lexer().Tokenize(in);

    for (Lexem lexem = lexem_stream.Read(); lexem.type != LexemType::EMPTY; lexem = lexem_stream.Read()) {
        std::cout << kTokenizerStateNaming.find(state)->second << " -> ";
        transit(lexem);
        std::cout << kTokenizerStateNaming.find(state)->second << std::endl;
    }

    return stream;
}

void Tokenizer::transit(Lexem lex) {
    switch (state) {
        case TokenizerState::WT:
            switch (lex.type) {
                case LexemType::EMPTY:
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

std::filesystem::path AbsolutePath(const std::filesystem::path& path) {
    return std::filesystem::path(__FILE__).replace_filename(path);
}

static const std::string kInfoPath = "static/00_info.txt";
static const std::string kOtherInfoPath = "static/01_info.txt";

TEST(LexerTest, SimpleTokenize) {
    Lexer lexer;

    std::fstream info_file(AbsolutePath(kInfoPath));
    LexemStream got_stream = lexer.Tokenize(info_file);
    info_file.close();

    Lexem newline = Lexem(LexemType::NEWLINE, "\n");
    Lexem colon = Lexem(LexemType::COLON, ":");

    LexemStream expected_stream({
        Lexem(LexemType::TEXT, "Сон в летнюю ночь / A Midsummer Night's Dream (1999)"), newline,
        Lexem(LexemType::TEXT, "pic"), newline,
        Lexem(LexemType::TEXT, "Производство"), colon,
        Lexem(LexemType::TEXT, " Италия, Великобритания, США (Fox Searchlight Pictures, Regency Enterprises, Taurus Film)"), newline,
        Lexem(LexemType::TEXT, "Жанр"), colon,
        Lexem(LexemType::TEXT, " комедия, фэнтези"), newline, newline,
        Lexem(LexemType::TEXT, "Режиссер"), colon,
        Lexem(LexemType::TEXT, " Майкл Хоффман"), newline,
        Lexem(LexemType::TEXT, "Актеры"), colon,
        Lexem(LexemType::TEXT, " Софи Марсо, Кевин Клайн, Мишель Пфайффер, Стэнли Туччи, Руперт Эверетт, Калиста Флокхарт, Доминик Уэст, Кристиан Бэйл, Анна Фрил, Дэвид Стрэтэйрн, Роджер Рис, Сэм Рокуэлл, Грегори Джбара, Билл Ирвин, Макс Райт и др."), newline, newline,
        Lexem(LexemType::TEXT, "Описание"), colon,
        Lexem(LexemType::TEXT, " По мотивам одноимённой пьесы Шекспира."), newline,
        Lexem(LexemType::TEXT, "В чувственном и прекрасном мире низких холмов и бесподобных кулинарных кушаний персонажи Шекспира получают новое право на жизнь, исследуя свой мир на новомодном изобретении — велосипеде. Пролетая по лесным дорогам на двух колёсах, дворяне, любовники и актёры Тосканы оказываются во власти озорных духов, которые правят миром…"), newline, newline,
        Lexem(LexemType::TEXT, "pic"), newline,
        Lexem(LexemType::TEXT, "6.4/1025,731"), newline, newline,
        Lexem(LexemType::TEXT, "Рейтинг MPAA"), colon,
        Lexem(LexemType::TEXT, " G (нет возрастных ограничений)"), newline, newline,
        Lexem(LexemType::TEXT, "Продолжительность"), colon,
        Lexem(LexemType::TEXT, " 01"), colon,
        Lexem(LexemType::TEXT, "55"), colon,
        Lexem(LexemType::TEXT, "37"), newline,
        Lexem(LexemType::TEXT, "Качество видео"), colon,
        Lexem(LexemType::TEXT, " DVDRemux"), newline, newline,
        Lexem(LexemType::TEXT, "Видео"), colon,
        Lexem(LexemType::TEXT, " MPEG-2, 720x576@1024x576, ~5500 Kbps"), newline,
        Lexem(LexemType::TEXT, "Аудио #1"), colon,
        Lexem(LexemType::TEXT, " AC3, 6 ch, 384 Kbps, английский"), newline,
        Lexem(LexemType::TEXT, "Аудио #2"), colon,
        Lexem(LexemType::TEXT, " AC3, 6 ch, 384 Kbps, французский"), newline,
        Lexem(LexemType::TEXT, "Аудио #3"), colon,
        Lexem(LexemType::TEXT, " AC3, 6 ch, 384 Kbps, английский"), newline,
        Lexem(LexemType::TEXT, "Аудио #4"), colon,
        Lexem(LexemType::TEXT, " AC3, 6 ch, 384 Kbps, испанский"), newline,
        Lexem(LexemType::TEXT, "Аудио #5"), colon,
        Lexem(LexemType::TEXT, " AC3, 2 ch, 192 Kbps, русский"), newline, newline,
        Lexem(LexemType::TEXT, "Субтитры"), colon,
        Lexem(LexemType::TEXT, " английский, французский, испанский, немецкий")
    });

    Lexem got_lexem, expected_lexem;
    
    while ((got_lexem = got_stream.Read()).type != LexemType::EMPTY && 
            (expected_lexem = expected_stream.Read()).type != LexemType::EMPTY) {
        ASSERT_EQ(kLexemTypeNaming.find(got_lexem.type)->second, kLexemTypeNaming.find(expected_lexem.type)->second);
        ASSERT_EQ(got_lexem.value, expected_lexem.value);
    }
}

TEST(LexerTest, OtherSimpleTokenize) {
    Lexer lexer;

    std::fstream info_file(AbsolutePath(kOtherInfoPath));
    LexemStream got_stream = lexer.Tokenize(info_file);
    info_file.close();

    Lexem newline = Lexem(LexemType::NEWLINE, "\n");
    Lexem colon = Lexem(LexemType::COLON, ":");

    LexemStream expected_stream({
        Lexem(LexemType::TEXT, "Человек из стали / Man of Steel (2013) BDRip"), newline,
        Lexem(LexemType::TEXT, "pic"), newline,
        Lexem(LexemType::TEXT, "Производство"), colon,
        Lexem(LexemType::TEXT, " США, Канада, Великобритания / Warner Bros. Pictures"), newline,
        Lexem(LexemType::TEXT, "Жанр"), colon,
        Lexem(LexemType::TEXT, " фантастика, боевик, приключения"), newline, newline,
        Lexem(LexemType::TEXT, "Режиссер"), colon,
        Lexem(LexemType::TEXT, " Зак Снайдер"), newline,
        Lexem(LexemType::TEXT, "Актеры"), colon,
        Lexem(LexemType::TEXT, " Генри Кавилл, Эми Адамс, Майкл Шеннон (II), Кевин Костнер, Дайан Лэйн, Лоуренс Фишборн, Антье Трауэ, Айелет Зурер, Расселл Кроу, Гарри Дж. Ленникс"), newline, newline,
        Lexem(LexemType::TEXT, "Описание"), colon, newline,
        Lexem(LexemType::TEXT, "Кларк Кент (Кал-Эл) - молодой человек, который чувствует себя чужаком из-за своей невероятной силы. Много лет назад он был отправлен на Землю с развитой планеты Криптон, и теперь задается вопросом"), colon,
        Lexem(LexemType::TEXT, " зачем?"), newline,
        Lexem(LexemType::TEXT, "Воспитанный приемными родителями Мартой и Джонатаном Кентами, Кларк знает"), colon,
        Lexem(LexemType::TEXT, " обладать сверхспособностями - значит принимать сложные решения. Но когда человечество более всего нуждается в стабильности, оно подвергается нападению. Сможет ли герой восстановить мир или воспользуется своей силой для того, чтобы окончательно его разрушить?"), newline, newline,
        Lexem(LexemType::TEXT, "pic"), newline,
        Lexem(LexemType::TEXT, "7.1/10753,350"), newline, newline,
        Lexem(LexemType::TEXT, "Возраст"), colon,
        Lexem(LexemType::TEXT, " 12+ (зрителям, достигшим 12 лет)"), newline,
        Lexem(LexemType::TEXT, "Рейтинг MPAA"), colon,
        Lexem(LexemType::TEXT, " PG-13 (детям до 13 лет просмотр нежелателен)"), newline, newline,
        Lexem(LexemType::TEXT, "Продолжительность"), colon,
        Lexem(LexemType::TEXT, " 02"), colon,
        Lexem(LexemType::TEXT, "23"), colon,
        Lexem(LexemType::TEXT, "13"), newline,
        Lexem(LexemType::TEXT, "Качество видео"), colon,
        Lexem(LexemType::TEXT, " BDRip"), newline,
        Lexem(LexemType::TEXT, "Перевод"), colon,
        Lexem(LexemType::TEXT, " Дублированный (BD CEE)"), newline,
        Lexem(LexemType::TEXT, "Субтитры"), colon,
        Lexem(LexemType::TEXT, " Английские"), newline, newline,
        Lexem(LexemType::TEXT, "Видео"), colon,
        Lexem(LexemType::TEXT, " AVC/H.264, 1024x426, ~2800 Kbps"), newline,
        Lexem(LexemType::TEXT, "Аудио #1"), colon,
        Lexem(LexemType::TEXT, " AC3, 6 ch, 448 Kbps - Русский"), newline,
        Lexem(LexemType::TEXT, "Аудио #2"), colon,
        Lexem(LexemType::TEXT, " AC3, 6 ch, 448 Kbps - Английский")
    });

    Lexem got_lexem, expected_lexem;
    
    while ((got_lexem = got_stream.Read()).type != LexemType::EMPTY && 
            (expected_lexem = expected_stream.Read()).type != LexemType::EMPTY) {
        ASSERT_EQ(got_lexem.value, expected_lexem.value);
        ASSERT_EQ(kLexemTypeNaming.find(got_lexem.type)->second, kLexemTypeNaming.find(expected_lexem.type)->second);
    }
}

TEST(TokenizerTest, SimpleTokenize) {
    Tokenizer tokenizer;

    std::fstream info_file(AbsolutePath(kInfoPath));
    TokenStream got_stream = tokenizer.Tokenize(info_file);
    info_file.close();

    TokenStream expected_stream({
        Token(TokenType::TITLE, "Сон в летнюю ночь / A Midsummer Night's Dream (1999)"),
        Token(TokenType::TEXT, "pic"),
        Token(TokenType::KEY, "country"),
        Token(TokenType::TEXT, " Италия, Великобритания, США (Fox Searchlight Pictures, Regency Enterprises, Taurus Film)"),
        Token(TokenType::KEY, "genre"),
        Token(TokenType::TEXT, " комедия, фэнтези"),
        Token(TokenType::TEXT, ""),
        Token(TokenType::KEY, "director"),
        Token(TokenType::TEXT, " Майкл Хоффман"),
        Token(TokenType::KEY, "actor"),
        Token(TokenType::TEXT, " Софи Марсо, Кевин Клайн, Мишель Пфайффер, Стэнли Туччи, Руперт Эверетт, Калиста Флокхарт, Доминик Уэст, Кристиан Бэйл, Анна Фрил, Дэвид Стрэтэйрн, Роджер Рис, Сэм Рокуэлл, Грегори Джбара, Билл Ирвин, Макс Райт и др."),
        Token(TokenType::TEXT, ""),
        Token(TokenType::KEY, "description"),
        Token(TokenType::TEXT, " По мотивам одноимённой пьесы Шекспира.В чувственном и прекрасном мире низких холмов и бесподобных кулинарных кушаний персонажи Шекспира получают новое право на жизнь, исследуя свой мир на новомодном изобретении — велосипеде. Пролетая по лесным дорогам на двух колёсах, дворяне, любовники и актёры Тосканы оказываются во власти озорных духов, которые правят миром…"),
        Token(TokenType::TEXT, "pic6.4/1025,731"),
        Token(TokenType::KEY, "mpaa"),
        Token(TokenType::TEXT, " G (нет возрастных ограничений)"),
        Token(TokenType::TEXT, ""),
        Token(TokenType::KEY, "duration"),
        Token(TokenType::TEXT, " 01:55:37"),
        Token(TokenType::KEY, "quality"),
        Token(TokenType::TEXT, " DVDRemux"),
        Token(TokenType::TEXT, ""),
        Token(TokenType::KEY, "video"),
        Token(TokenType::TEXT, " MPEG-2, 720x576@1024x576, ~5500 Kbps"),
        Token(TokenType::KEY, "audio"),
        Token(TokenType::TEXT, " AC3, 6 ch, 384 Kbps, английский"),
        Token(TokenType::KEY, "audio"),
        Token(TokenType::TEXT, " AC3, 6 ch, 384 Kbps, французский"),
        Token(TokenType::KEY, "audio"),
        Token(TokenType::TEXT, " AC3, 6 ch, 384 Kbps, английский"),
        Token(TokenType::KEY, "audio"),
        Token(TokenType::TEXT, " AC3, 6 ch, 384 Kbps, испанский"),
        Token(TokenType::KEY, "audio"),
        Token(TokenType::TEXT, " AC3, 2 ch, 192 Kbps, русский"),
        Token(TokenType::TEXT, ""),
        Token(TokenType::KEY, "subtitles"),
        Token(TokenType::TEXT, " английский, французский, испанский, немецкий")
    });

    Token got_token, expected_token;
    while ((got_token = got_stream.Read()).type != TokenType::EMPTY &&
            (expected_token = expected_stream.Read()).type != TokenType::EMPTY) {
        ASSERT_EQ(got_token.value, expected_token.value);
        ASSERT_EQ(kTokenTypeNaming.find(got_token.type)->second, kTokenTypeNaming.find(expected_token.type)->second);
    }
}

TEST(TokenizerTest, OtherSimpleTokenize) {
    Tokenizer tokenizer;

    std::fstream info_file(AbsolutePath(kOtherInfoPath));
    TokenStream got_stream = tokenizer.Tokenize(info_file);
    info_file.close();

    TokenStream expected_stream({
        Token(TokenType::TITLE, "Человек из стали / Man of Steel (2013) BDRip"),
        Token(TokenType::TEXT, "pic"),
        Token(TokenType::KEY, "country"),
        Token(TokenType::TEXT, " США, Канада, Великобритания / Warner Bros. Pictures"),
        Token(TokenType::KEY, "genre"),
        Token(TokenType::TEXT, " фантастика, боевик, приключения"),
        Token(TokenType::TEXT, ""),
        Token(TokenType::KEY, "director"),
        Token(TokenType::TEXT, " Зак Снайдер"),
        Token(TokenType::KEY, "actor"),
        Token(TokenType::TEXT, " Генри Кавилл, Эми Адамс, Майкл Шеннон (II), Кевин Костнер, Дайан Лэйн, Лоуренс Фишборн, Антье Трауэ, Айелет Зурер, Расселл Кроу, Гарри Дж. Ленникс"),
        Token(TokenType::TEXT, ""),
        Token(TokenType::KEY, "description"),
        Token(TokenType::TEXT, "Кларк Кент (Кал-Эл) - молодой человек, который чувствует себя чужаком из-за своей невероятной силы. Много лет назад он был отправлен на Землю с развитой планеты Криптон, и теперь задается вопросом: зачем?Воспитанный приемными родителями Мартой и Джонатаном Кентами, Кларк знает: обладать сверхспособностями - значит принимать сложные решения. Но когда человечество более всего нуждается в стабильности, оно подвергается нападению. Сможет ли герой восстановить мир или воспользуется своей силой для того, чтобы окончательно его разрушить?"),
        Token(TokenType::TEXT, "pic7.1/10753,350"),
        Token(TokenType::KEY, "age"),
        Token(TokenType::TEXT, " 12+ (зрителям, достигшим 12 лет)"),
        Token(TokenType::KEY, "mpaa"),
        Token(TokenType::TEXT, " PG-13 (детям до 13 лет просмотр нежелателен)"),
        Token(TokenType::TEXT, ""),
        Token(TokenType::KEY, "duration"),
        Token(TokenType::TEXT, " 02:23:13"),
        Token(TokenType::KEY, "quality"),
        Token(TokenType::TEXT, " BDRip"),
        Token(TokenType::KEY, "translation"),
        Token(TokenType::TEXT, " Дублированный (BD CEE)"),
        Token(TokenType::KEY, "subtitles"),
        Token(TokenType::TEXT, " Английские"),
        Token(TokenType::TEXT, ""),
        Token(TokenType::KEY, "video"),
        Token(TokenType::TEXT, " AVC/H.264, 1024x426, ~2800 Kbps"),
        Token(TokenType::KEY, "audio"),
        Token(TokenType::TEXT, " AC3, 6 ch, 448 Kbps - Русский"),
        Token(TokenType::KEY, "audio"),
        Token(TokenType::TEXT, " AC3, 6 ch, 448 Kbps - Английский"),
    });

    Token got_token, expected_token;
    while ((got_token = got_stream.Read()).type != TokenType::EMPTY &&
            (expected_token = expected_stream.Read()).type != TokenType::EMPTY) {
        ASSERT_EQ(got_token.value, expected_token.value);
        ASSERT_EQ(kTokenTypeNaming.find(got_token.type)->second, kTokenTypeNaming.find(expected_token.type)->second);
    }

}

