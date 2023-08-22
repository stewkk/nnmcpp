#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <utility>

#include "parser.hpp"

using namespace nnmcpp::parsing;

static std::filesystem::path AbsolutePath(const std::filesystem::path& path) {
    return std::filesystem::path(__FILE__).replace_filename(path);
}

static const std::string kInfoPath = "static/00_info.txt";
static const std::string kOtherInfoPath = "static/01_info.txt";

TEST(ParserTest, SimpleParse) {
    Parser parser;

    std::fstream info_file(AbsolutePath(kInfoPath));
    ParserStream got_stream = parser.parse(info_file);
    info_file.close();

    ParserStream expected_stream({
        std::make_pair("production", " Италия, Великобритания, США (Fox Searchlight Pictures, Regency Enterprises, Taurus Film)"),
        std::make_pair("genre", " комедия, фэнтези"),
        std::make_pair("director", " Майкл Хоффман"),
        std::make_pair("actor", " Софи Марсо, Кевин Клайн, Мишель Пфайффер, Стэнли Туччи, Руперт Эверетт, Калиста Флокхарт, Доминик Уэст, Кристиан Бэйл, Анна Фрил, Дэвид Стрэтэйрн, Роджер Рис, Сэм Рокуэлл, Грегори Джбара, Билл Ирвин, Макс Райт и др."),
        std::make_pair("description", " По мотивам одноимённой пьесы Шекспира.В чувственном и прекрасном мире низких холмов и бесподобных кулинарных кушаний персонажи Шекспира получают новое право на жизнь, исследуя свой мир на новомодном изобретении — велосипеде. Пролетая по лесным дорогам на двух колёсах, дворяне, любовники и актёры Тосканы оказываются во власти озорных духов, которые правят миром…"),
        std::make_pair("mpaa", " G (нет возрастных ограничений)"),
        std::make_pair("duration", " 01:55:37"),
        std::make_pair("quality", " DVDRemux"),
        std::make_pair("video", " MPEG-2, 720x576@1024x576, ~5500 Kbps"),
        std::make_pair("audio", " AC3, 6 ch, 384 Kbps, английский"),
        std::make_pair("audio", " AC3, 6 ch, 384 Kbps, французский"),
        std::make_pair("audio", " AC3, 6 ch, 384 Kbps, английский"),
        std::make_pair("audio", " AC3, 6 ch, 384 Kbps, испанский"),
        std::make_pair("audio", " AC3, 2 ch, 192 Kbps, русский"),
        std::make_pair("subtitles", " английский, французский, испанский, немецкий"),
    });

    std::pair<std::string, std::string> got, expected;
    std::pair<std::string, std::string> empty;
    while ((got = got_stream.Read()) != empty &&
        (expected = expected_stream.Read()) != empty) {
        
        ASSERT_EQ(got, expected);
        ASSERT_EQ(got, expected);
    }
}

