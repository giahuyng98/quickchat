#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <memory>
#include "proto/service.grpc.pb.h"
//class UserServiceImpl final : public UserService::AsyncService {
class UserServiceImpl final : public UserService::Service {
  grpc::Status hello(grpc::ServerContext* context, const What* request, What* response) {
    std::cout << "Incoming request\n";
    //request->set_
    return grpc::Status::OK;
  }
};

class Client {
  public:

  Client(std::shared_ptr<grpc::Channel> channel)
      : stub_(UserService::NewStub(channel)) {}

  std::string hello() {
    grpc::ClientContext context;
    What req, res;
    grpc::Status status = stub_->hello(&context, req, &res);
    if (status.ok()) {
      std::cout << status.error_code() << "\n";
      std::cout << res.DebugString() << "\n";
    } else {
      std::cout << status.error_code() << "\n";
      std::cout << status.error_message() << "\n";
    }
    return "";
  }

  private:
  std::unique_ptr<UserService::Stub> stub_;
};

int main() {
  Client client(grpc::CreateChannel("127.0.0.1:9002", grpc::InsecureChannelCredentials()));
  client.hello();

  //server
  UserServiceImpl userService;

  //std::make_unique<int>();

  grpc::EnableDefaultHealthCheckService(true);
  //::protobuf::Reflection::InitProtoReflectionServerBuilderPlugin();

  const std::string address{"127.0.0.1:9002"};
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.RegisterService(&userService);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << address << std::endl;
  server->Wait();

 
}
