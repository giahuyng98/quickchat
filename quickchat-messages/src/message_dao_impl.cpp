#include "message_dao_impl.hpp"
#include "message_info.h"

namespace detail {

model::Message makeMessage(const info::MessageInfo &msgInfo) {
  const std::time_t createAt = msgInfo.createAt;
  const std::time_t updateAt = msgInfo.updateAt;
  return model::Message(msgInfo.channelId, msgInfo.messageId, msgInfo.userId,
                        mysqlpp::sql_blob(msgInfo.content),
                        mysqlpp::sql_datetime(msgInfo.createAt),
                        mysqlpp::sql_datetime(msgInfo.updateAt));
}
info::MessageInfo fromMessage(const model::Message &msg) {
  return {.channelId = msg.channelId,
          .messageId = msg.messageId,
          .userId = msg.userId,
          .content = {msg.content.data(), msg.content.size()},
          .createAt = msg.createAt};
}

// TODO: extract datetime
model::Message makeMessage(const msg::Chat &chat, const int64_t id) {
  return model::Message(chat.channel_id(), id, chat.user_id(),
                        mysqlpp::sql_blob(chat.content().SerializeAsString()),
                        mysqlpp::sql_datetime(), mysqlpp::null);
}

} // namespace detail
