#!/bin/bash

rm gst_task.grpc.pb.h
rm gst_task.grpc.pb.cc
rm gst_task.pb.cc
rm gst_task.pb.h

protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` gst_task.proto
protoc -I . --cpp_out=. gst_task.proto