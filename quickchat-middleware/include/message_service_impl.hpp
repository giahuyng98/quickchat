#pragma once

#include "logger.hpp"
#include "message.pb.h"
#include "message_service.grpc.pb.h"
#include "message_service.hpp"
#include "message_service.pb.h"
#include <google/protobuf/repeated_field.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/status.h>

class MessageServiceImpl : public MessageService<MessageServiceImpl> {

  std::unique_ptr<srv::MessageService::Stub> stub;
  std::string apikey;

public:
  friend class MessageService<MessageServiceImpl>;

private:
  template <class Config> void startImpl(const Config &config) {
    logger::info("messageservice is started");

    const std::string channelServiceURI =
        config["message-service"]["uri"].template get<std::string>();
    apikey = config["message-service"]["apikey"].template get<std::string>();

    stub = srv::MessageService::NewStub(grpc::CreateChannel(
        channelServiceURI, grpc::InsecureChannelCredentials()));
  }

  auto getLastMessageImpl(const uint64_t channelId) {
    // TODO: should client context stay here?
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::GetLastMessageRequest req;
    req.set_channel_id(channelId);

    srv::GetLastMessageReponse res;
    const auto status = stub->GetLastMessage(&context, req, &res);

    return std::make_pair(status, res.message());
  }

  auto getMessagesImpl(const uint64_t channelId, const uint64_t messageId = 0,
                       const int32_t count = 0) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::GetMessagesRequest req;
    req.set_channel_id(channelId);
    req.set_message_id(messageId);
    req.set_count(count);

    srv::GetMessagesReponse res;
    const auto status = stub->GetMessages(&context, req, &res);
    return std::make_pair(status, res.messages());
  }
};
