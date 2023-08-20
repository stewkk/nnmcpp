#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>

static const std::unordered_set<std::string> kKeys = {
    "translation",
    "quality",
    "duration",
    "genre",
    "audio",
    "subtitles",
    "director",
    "country",
    "video",
    "actor",
    "age",
    "mpaa",
    "description"
};

static const std::unordered_map<std::string, std::string> kNormalizedKeys = {
    {"перевод", "translation"},
    {"качество", "quality"},
    {"качество видео", "quality"},
    {"продолжительность", "duration"},
    {"длительность", "duration"},
    {"жанр", "genre"},
    {"аудио", "audio"},
    {"субтитры", "subtitles"},
    {"вид субтитров", "subtitles"},
    {"язык субтитров", "subtitles"},
    {"режиссер", "director"},
    {"режиссёр", "director"},
    {"режиссеры", "director"},
    {"режиссёры", "director"},
    {"страна", "country"},
    {"производство", "country"},
    {"размер кадра", "video"},
    {"видео", "video"},
    {"видеодек", "video"},
    {"Перевод", "translation"},
    {"Качество", "quality"},
    {"Качество видео", "quality"},
    {"Продолжительность", "duration"},
    {"Длительность", "duration"},
    {"Жанр", "genre"},
    {"Аудио", "audio"},
    {"Субтитры", "subtitles"},
    {"Вид субтитров", "subtitles"},
    {"Язык субтитров", "subtitles"},
    {"Режиссер", "director"},
    {"Режиссёр", "director"},
    {"Режиссеры", "director"},
    {"Режиссёры", "director"},
    {"Страна", "country"},
    {"Производство", "country"},
    {"Размер кадра", "video"},
    {"Видео", "video"},
    {"Видеодек", "video"},
    {"актеры", "actor"},
    {"Актеры", "actor"},
    {"актёры", "actor"},
    {"Актёры", "actor"},
    {"возраст", "age"},
    {"Возраст", "age"},
    {"Рейтинг MPAA", "mpaa"},
    {"рейтинг MPAA", "mpaa"},
    {"аудио #1", "audio"},
    {"Аудио #1", "audio"},
    {"аудио #2", "audio"},
    {"Аудио #2", "audio"},
    {"аудио #3", "audio"},
    {"Аудио #3", "audio"},
    {"аудио #4", "audio"},
    {"Аудио #4", "audio"},
    {"Аудио #5", "audio"},
    {"Аудио #5", "audio"},
    {"описание", "description"},
    {"Описание", "description"}
};

