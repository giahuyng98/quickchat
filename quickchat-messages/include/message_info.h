#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace info {
struct MessageInfo {
  int64_t channelId;
  int64_t messageId;
  int64_t userId;
  std::string content;
  int64_t createAt;
  int64_t updateAt;
  friend std::ostream &operator<<(std::ostream &os, const MessageInfo &msgInfo);
};

void to_json(nlohmann::json &j, const MessageInfo &msgInfo);
void from_json(const nlohmann::json &j, MessageInfo &msgInfo);
}  // namespace info
