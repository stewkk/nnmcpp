#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <nnmcpp/parser.hpp>
#include <utility>

using namespace nnmcpp::parsing;

static std::filesystem::path AbsolutePath(const std::filesystem::path& path) {
  return std::filesystem::path(__FILE__).replace_filename(path);
}

static const std::string kInfoPath = "static/00_info.txt";
static const std::string kOtherInfoPath = "static/01_info.txt";

TEST(ParserTest, SimpleParse) {
  Parser parser;

  std::fstream info_file(AbsolutePath(kInfoPath));
  Info got_info = parser.parse(info_file);
  info_file.close();

  Info expected_info;
  expected_info.title = "Сон в летнюю ночь / A Midsummer Night's Dream (1999)";
  expected_info.ru_title = "Сон в летнюю ночь";
  expected_info.en_title = "A Midsummer Night's Dream";
  expected_info.year = "1999";
  expected_info.translation = "";
  expected_info.director = " Майкл Хоффман";
  expected_info.country = "";
  expected_info.production
      = " Италия, Великобритания, США (Fox Searchlight Pictures, Regency Enterprises, Taurus Film)";
  expected_info.video = " MPEG-2, 720x576@1024x576, ~5500 Kbps";
  expected_info.codec = "MPEG-2";
  expected_info.video_w = "1024";
  expected_info.video_h = "576";
  expected_info.quality = " DVDRemux";
  expected_info.genre = " комедия, фэнтези";
  expected_info.audio = " AC3, 2 ch, 192 Kbps, русский";
  expected_info.actor
      = " Софи Марсо, Кевин Клайн, Мишель Пфайффер, Стэнли Туччи, Руперт Эверетт, Калиста "
        "Флокхарт, Доминик Уэст, Кристиан Бэйл, Анна Фрил, Дэвид Стрэтэйрн, Роджер Рис, Сэм"
        " Рокуэлл, Грегори Джбара, Билл Ирвин, Макс Райт и др.";
  expected_info.duration = " 01:55:37";
  expected_info.subtitles = " английский, французский, испанский, немецкий";

  std::pair<std::string, std::string> got, expected;
  std::pair<std::string, std::string> empty;

  ASSERT_EQ(expected_info.title, got_info.title);
  ASSERT_EQ(expected_info.ru_title, got_info.ru_title);
  ASSERT_EQ(expected_info.en_title, got_info.en_title);
  ASSERT_EQ(expected_info.year, got_info.year);

  ASSERT_EQ(expected_info.video, got_info.video);
  ASSERT_EQ(expected_info.codec, got_info.codec);
  ASSERT_EQ(expected_info.video_w, got_info.video_w);
  ASSERT_EQ(expected_info.video_h, got_info.video_h);

  ASSERT_EQ(expected_info.translation, got_info.translation);
  ASSERT_EQ(expected_info.director, got_info.director);
  ASSERT_EQ(expected_info.country, got_info.country);
  ASSERT_EQ(expected_info.production, got_info.production);
  ASSERT_EQ(expected_info.quality, got_info.quality);
  ASSERT_EQ(expected_info.genre, got_info.genre);
  ASSERT_EQ(expected_info.audio, got_info.audio);
  ASSERT_EQ(expected_info.actor, got_info.actor);
  ASSERT_EQ(expected_info.duration, got_info.duration);
  ASSERT_EQ(expected_info.subtitles, got_info.subtitles);
}
