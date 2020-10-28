#include "message_info.h"


namespace info {
std::ostream &operator<<(std::ostream &os, const info::MessageInfo &msgInfo) {
  return os << nlohmann::json(msgInfo);
}

void to_json(nlohmann::json &j, const MessageInfo &msgInfo) {
  j = nlohmann::json{{"channelId", msgInfo.channelId},
                     {"messageId", msgInfo.messageId},
                     {"userId", msgInfo.userId},
                     {"content", msgInfo.content},
                     {"createAt", msgInfo.createAt}};
}
void from_json(const nlohmann::json &j, MessageInfo &msgInfo) {
  j.at("channelId").get_to(msgInfo.channelId);
  j.at("messageId").get_to(msgInfo.messageId);
  j.at("userId").get_to(msgInfo.userId);
  j.at("content").get_to(msgInfo.content);
  j.at("createAt").get_to(msgInfo.createAt);
}


}  // namespace info
