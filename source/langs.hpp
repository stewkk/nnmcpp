#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

static const std::unordered_set<std::string> kLangs = {
    "ru", "en", "fr", "es"
};

static const std::unordered_map<std::string, std::string> kNormalizedLangs = {
    {"русский", "ru"},
    {"английский", "en"},
    {"французский", "fr"},
    {"испанский", "es"},
    {"немецкий", "de"}
};

