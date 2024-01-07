#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "gst_task.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using gst_task::GstTask;
using gst_task::StartRequest;
using gst_task::StartReply;

// Logic and data behind the server's behavior.
class GstTaskImpl final : public GstTask::Service {
  Status StartTask(ServerContext *context, const StartRequest *request,
                   StartReply *response) override {
    // std::string prefix("Hello ");
    std::cout << "hello";
    response->set_status_code(10);
    std::cout << "sjdlajdkls";
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  GstTaskImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char **argv) {
  RunServer();

  return 0;
}
