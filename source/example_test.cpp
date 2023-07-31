#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

using ::testing::Eq;

namespace nnmcpp {

  struct Info {
    std::optional<std::string> title{};
    std::optional<std::string> production{};
  };

  struct Entry {
    std::string key;
    std::string value;
  };

  class Nnmcpp {
  private:
    std::stringstream stream_{};

    std::string ParseTitle() {
      std::string res;
      std::getline(stream_, res);
      return res;
    }

    std::string ParseKey() {
      std::stringstream res;
      for (;stream_.peek() != ':';) {
        if (stream_.peek() == '\n') {
          throw "TODO";
        }
        res << stream_.get();
      }
      return res.str();
    }

    Entry ParseEntry() {
      auto key = ParseKey();
      std::string rest;
      std::getline(stream_, rest);


      return {
          .key = "производство",
          .value = "",
      };
    }

  public:
    Nnmcpp(const std::string& input) : stream_(input) {}
    Info Parse() {
      Info res{};
      res.title = ParseTitle();
      for (; stream_.good();) {
        ParseEntry();
      }
      return res;
    }
  };
}  // namespace nnmcpp

std::string FromFile(std::string relative_path) {
  auto absolute_path = [&relative_path]() {
    std::filesystem::path cur_file = __FILE__;
    return cur_file.replace_filename(relative_path);
  }();
  std::ifstream f(absolute_path);
  std::stringstream buf;
  buf << f.rdbuf();
  return buf.str();
}

static const std::string kInfoPath = "static/00_info.txt";
static const std::string kOtherInfoPath = "static/01_info.txt";

TEST(ParserTest, DISABLED_ParsesTitle) {
  std::string info = FromFile(kInfoPath);
  nnmcpp::Nnmcpp p(info);

  auto got = p.Parse();

  ASSERT_THAT(got.title.value_or(""), Eq("Сон в летнюю ночь / A Midsummer Night's Dream (1999)"));
}

TEST(ParserTest, DISABLED_PartsesOtherTitle) {
  std::string info = FromFile(kOtherInfoPath);
  nnmcpp::Nnmcpp p(info);

  auto got = p.Parse();

  ASSERT_THAT(got.title.value_or(""), Eq("Человек из стали / Man of Steel (2013) BDRip"));
}

TEST(ParserTest, DISABLED_ParsesProduction) {
  std::string info = FromFile(kInfoPath);
  nnmcpp::Nnmcpp p(info);

  auto got = p.Parse();

  ASSERT_THAT(got.production.value_or(""), Eq("Италия, Великобритания, США (Fox Searchlight "
                                              "Pictures, Regency Enterprises, Taurus Film)"));
}
