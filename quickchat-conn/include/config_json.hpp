#pragma once
#include "config.hpp"
#include "logger.hpp"
#include "util.h"
#include <nlohmann/json_fwd.hpp>
#include <spdlog/spdlog.h>

class ConfigJson : public Config<ConfigJson> {
  friend class Config<ConfigJson>;

public:
  template <class KeyType> auto &operator[](const KeyType &key) const {
    try {
      return config[key];
    } catch (std::exception &ex) {
      logger::error("erro while get key '{}'", key);
      logger::error(ex.what());
      throw;
    }
  }

private:
  void loadFromFileImpl(const std::string_view fileName) {
    try {
      std::ifstream ifs({fileName.data(), fileName.size()});
      logger::info("loading config file: '{}'", fileName);
      ifs >> config;
    } catch (std::exception &ex) {
      logger::error("error loading config file: '{}'", fileName);
      logger::info(ex.what());
      std::exit(EXIT_FAILURE);
    }
  }

  template <class T> T getImpl(const std::string &key) const {
    return config[key].get<T>();
  }

private:
  nlohmann::json config;
};
