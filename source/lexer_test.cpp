#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "lexer.hpp"

using namespace nnmcpp::parsing;

static std::filesystem::path AbsolutePath(const std::filesystem::path& path) {
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



