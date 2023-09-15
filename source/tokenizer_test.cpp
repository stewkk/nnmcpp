#include "tokenizer.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <istream>
#include <queue>
#include <sstream>

#include "keys.hpp"

using namespace nnmcpp::parsing;

static std::filesystem::path AbsolutePath(const std::filesystem::path& path) {
  return std::filesystem::path(__FILE__).replace_filename(path);
}

static const std::string kInfoPath = "static/00_info.txt";
static const std::string kOtherInfoPath = "static/01_info.txt";

TEST(TokenizerTest, SimpleTokenize) {
  Tokenizer tokenizer;

  std::fstream info_file(AbsolutePath(kInfoPath));
  TokenStream got_stream = tokenizer.Tokenize(info_file);
  info_file.close();

  TokenStream expected_stream(
      {Token(TokenType::TITLE, "Сон в летнюю ночь / A Midsummer Night's Dream (1999)"),
       Token(TokenType::TEXT, "pic"),
       Token(TokenType::KEY, "production"),
       Token(TokenType::TEXT,
             " Италия, Великобритания, США (Fox Searchlight Pictures, Regency Enterprises, Taurus"
             " Film)"),
       Token(TokenType::KEY, "genre"),
       Token(TokenType::TEXT, " комедия, фэнтези"),
       Token(TokenType::TEXT, ""),
       Token(TokenType::KEY, "director"),
       Token(TokenType::TEXT, " Майкл Хоффман"),
       Token(TokenType::KEY, "actors"),
       Token(TokenType::TEXT,
             " Софи Марсо, Кевин Клайн, Мишель Пфайффер, Стэнли Туччи, Руперт Эверетт, Калиста "
             "Флокхарт, Доминик Уэст, Кристиан Бэйл, Анна Фрил, Дэвид Стрэтэйрн, Роджер Рис, Сэм"
             " Рокуэлл, Грегори Джбара, Билл Ирвин, Макс Райт и др."),
       Token(TokenType::TEXT, ""),
       Token(TokenType::KEY, "description"),
       Token(TokenType::TEXT,
             " По мотивам одноимённой пьесы Шекспира.В чувственном и прекрасном мире низких "
             "холмов и бесподобных кулинарных кушаний персонажи Шекспира получают новое право "
             "на жизнь, исследуя свой мир на новомодном изобретении — велосипеде. Пролетая по "
             "лесным дорогам на двух колёсах, дворяне, любовники и актёры Тосканы оказываются "
             "во власти озорных духов, которые правят миром…"),
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
       Token(TokenType::TEXT, " английский, французский, испанский, немецкий")});

  Token got_token, expected_token;
  while ((got_token = got_stream.Read()).type != TokenType::EMPTY
         && (expected_token = expected_stream.Read()).type != TokenType::EMPTY) {
    ASSERT_EQ(got_token.value, expected_token.value);
    ASSERT_EQ(kTokenTypeNaming.find(got_token.type)->second,
              kTokenTypeNaming.find(expected_token.type)->second);
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
      Token(TokenType::KEY, "production"),
      Token(TokenType::TEXT, " США, Канада, Великобритания / Warner Bros. Pictures"),
      Token(TokenType::KEY, "genre"),
      Token(TokenType::TEXT, " фантастика, боевик, приключения"),
      Token(TokenType::TEXT, ""),
      Token(TokenType::KEY, "director"),
      Token(TokenType::TEXT, " Зак Снайдер"),
      Token(TokenType::KEY, "actors"),
      Token(TokenType::TEXT,
            " Генри Кавилл, Эми Адамс, Майкл Шеннон (II), Кевин Костнер, Дайан Лэйн, Лоуренс "
            "Фишборн, Антье Трауэ, Айелет Зурер, Расселл Кроу, Гарри Дж. Ленникс"),
      Token(TokenType::TEXT, ""),
      Token(TokenType::KEY, "description"),
      Token(TokenType::TEXT,
            "Кларк Кент (Кал-Эл) - молодой человек, который чувствует себя чужаком из-за своей "
            "невероятной силы. Много лет назад он был отправлен на Землю с развитой планеты "
            "Криптон, и теперь задается вопросом: зачем?Воспитанный приемными родителями Мартой и "
            "Джонатаном Кентами, Кларк знает: обладать сверхспособностями - значит принимать "
            "сложные решения. Но когда человечество более всего нуждается в стабильности, оно "
            "подвергается нападению. Сможет ли герой восстановить мир или воспользуется своей "
            "силой для того, чтобы окончательно его разрушить?"),
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
  while ((got_token = got_stream.Read()).type != TokenType::EMPTY
         && (expected_token = expected_stream.Read()).type != TokenType::EMPTY) {
    ASSERT_EQ(got_token.value, expected_token.value);
    ASSERT_EQ(kTokenTypeNaming.find(got_token.type)->second,
              kTokenTypeNaming.find(expected_token.type)->second);
  }
}
