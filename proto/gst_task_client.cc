#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "gst_task.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using gst_task::GstTask;
using gst_task::StartRequest;
using gst_task::StartReply;

class GstTaskClient {
public:
  GstTaskClient(std::shared_ptr<Channel> channel)
      : stub_(GstTask::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string StartTask(const std::string &file_name) {
    // Data we are sending to the server.
    StartRequest request;
    request.set_file_name(file_name);

    // Container for the data we expect from the server.
    StartReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->StartTask(&context, request, &reply);

    // Act upon its status.
    std::cout << status.ok() << std::endl;
    std::cout << reply.status_code();
    if (status.ok()) {
      return reply.service_id();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<GstTask::Stub> stub_;
};

int main(int argc, char **argv) {
  std::string target_str;
  std::string arg_str("--target");
  if (argc > 1) {
    std::string arg_val = argv[1];
    size_t start_pos = arg_val.find(arg_str);
    if (start_pos != std::string::npos) {
      start_pos += arg_str.size();
      if (arg_val[start_pos] == '=') {
        target_str = arg_val.substr(start_pos + 1);
      } else {
        std::cout << "The only correct argument syntax is --target="
                  << std::endl;
        return 0;
      }
    } else {
      std::cout << "The only acceptable argument is --target=" << std::endl;
      return 0;
    }
  } else {
    target_str = "localhost:50051";
  }
  GstTaskClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.StartTask(user);
  std::cout << "GstTask received: " << reply << std::endl;

  return 0;
}
