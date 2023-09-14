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
  expected_info.title.raw = "Сон в летнюю ночь / A Midsummer Night's Dream (1999)";
  expected_info.title.ru_title = "Сон в летнюю ночь";
  expected_info.title.en_title = "A Midsummer Night's Dream";
  expected_info.title.year = "1999";

  expected_info.video.raw = " MPEG-2, 720x576@1024x576, ~5500 Kbps";
  expected_info.video.width = "1024";
  expected_info.video.height = "576";

  expected_info.subtitles.raw = " английский, французский, испанский, немецкий";
  expected_info.subtitles.langs = {
    "en", "fr", "es", "de"
  };

  expected_info.translation.raw = "";
  expected_info.director.raw = " Майкл Хоффман";
  expected_info.country.raw = "";
  expected_info.production.raw
      = " Италия, Великобритания, США (Fox Searchlight Pictures, Regency Enterprises, Taurus Film)";
  expected_info.quality.raw = " DVDRemux";
  expected_info.genre.raw = " комедия, фэнтези";
  expected_info.audio.raw = " AC3, 2 ch, 192 Kbps, русский";
  expected_info.actors.raw = " Софи Марсо, Кевин Клайн, Мишель Пфайффер, Стэнли Туччи, Руперт Эверетт, Калиста "
        "Флокхарт, Доминик Уэст, Кристиан Бэйл, Анна Фрил, Дэвид Стрэтэйрн, Роджер Рис, Сэм"
        " Рокуэлл, Грегори Джбара, Билл Ирвин, Макс Райт и др.";
  expected_info.actors.actors = {
    " Софи Марсо",
    " Кевин Клайн",
    " Мишель Пфайффер",
    " Стэнли Туччи",
    " Руперт Эверетт",
    " Калиста Флокхарт",
    " Доминик Уэст",
    " Кристиан Бэйл",
    " Анна Фрил",
    " Дэвид Стрэтэйрн",
    " Роджер Рис",
    " Сэм Рокуэлл",
    " Грегори Джбара",
    " Билл Ирвин",
    " Макс Райт",
  };
  expected_info.duration.raw = " 01:55:37";
  expected_info.duration.minutes = 115;

  std::pair<std::string, std::string> got, expected;
  std::pair<std::string, std::string> empty;

  ASSERT_EQ(expected_info.title.raw, got_info.title.raw);
  ASSERT_EQ(expected_info.title.ru_title, got_info.title.ru_title);
  ASSERT_EQ(expected_info.title.en_title, got_info.title.en_title);
  ASSERT_EQ(expected_info.title.year, got_info.title.year);

  ASSERT_EQ(expected_info.video.raw, got_info.video.raw);
  ASSERT_EQ(expected_info.video.width, got_info.video.width);
  ASSERT_EQ(expected_info.video.height, got_info.video.height);

  ASSERT_EQ(expected_info.subtitles.raw, got_info.subtitles.raw);

  ASSERT_EQ(expected_info.translation.raw, got_info.translation.raw);
  ASSERT_EQ(expected_info.director.raw, got_info.director.raw);
  ASSERT_EQ(expected_info.country.raw, got_info.country.raw);
  ASSERT_EQ(expected_info.production.raw, got_info.production.raw);
  ASSERT_EQ(expected_info.quality.raw, got_info.quality.raw);
  ASSERT_EQ(expected_info.genre.raw, got_info.genre.raw);
  ASSERT_EQ(expected_info.audio.raw, got_info.audio.raw);

  // ASSERT_EQ(expected_info.actors.raw, got_info.actors.raw);
  size_t c = 0;

  auto expected_elem = expected_info.actors.actors.begin();
  auto expected_end = expected_info.actors.actors.end();

  auto got_elem = got_info.actors.actors.begin();
  auto got_end = got_info.actors.actors.end();

  while (expected_elem != expected_end && got_elem != got_end) {
    ASSERT_EQ(*expected_elem, *got_elem);

    c++;
    got_elem++;
    expected_elem++;
  }

  ASSERT_EQ(c, 15);

  c = 0;

  auto expected_sub_elem = expected_info.subtitles.langs.begin();
  auto expected_sub_end = expected_info.subtitles.langs.end();

  auto got_sub_elem = got_info.subtitles.langs.begin();
  auto got_sub_end = got_info.subtitles.langs.end();

  while (expected_sub_elem != expected_sub_end && got_sub_elem != got_sub_end) {
    ASSERT_EQ(*expected_sub_elem, *got_sub_elem);

    c++;
    got_sub_elem++;
    expected_sub_elem++;
  }

  ASSERT_EQ(c, 4);

  ASSERT_EQ(expected_info.duration.raw, got_info.duration.raw);
  ASSERT_EQ(expected_info.duration.minutes, got_info.duration.minutes);
}
