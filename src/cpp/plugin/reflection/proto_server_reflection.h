/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef GRPC_INTERNAL_CPP_REFLECTION_PROTO_SERVER_REFLECTION_H
#define GRPC_INTERNAL_CPP_REFLECTION_PROTO_SERVER_REFLECTION_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <grpc++/grpc++.h>

#include "reflection.grpc.pb.h"

namespace grpc {

class ProtoServerReflection final
    : public reflection::v1::ServerReflection::Service {
 public:
  ProtoServerReflection();

  ProtoServerReflection(const Server* server);

  void SetServer(const Server* server);

  void SetSeviceList(const std::vector<grpc::string>* services);

  Status ListService(
      ServerContext* context, const reflection::v1::EmptyRequest* request,
      reflection::v1::ListServiceResponse* response) GRPC_OVERRIDE;

  Status GetFileByName(
      ServerContext* context, const reflection::v1::FileNameRequest* request,
      reflection::v1::FileDescriptorProtoResponse* response) GRPC_OVERRIDE;

  Status GetFileContainingSymbol(
      ServerContext* context, const reflection::v1::SymbolRequest* request,
      reflection::v1::FileDescriptorProtoResponse* response) GRPC_OVERRIDE;

  Status GetFileContainingExtention(
      ServerContext* context, const reflection::v1::ExtensionRequest* request,
      reflection::v1::FileDescriptorProtoResponse* response) GRPC_OVERRIDE;

  Status GetAllExtensionNumbers(
      ServerContext* context, const reflection::v1::TypeRequest* request,
      reflection::v1::ExtensionNumberResponse* response) GRPC_OVERRIDE;

 private:
  const google::protobuf::DescriptorPool* descriptor_pool_;
  const Server* server_;
  const std::vector<string>* services_;
};

}  // namespace grpc

#endif  // GRPC_INTERNAL_CPP_REFLECTION_PROTO_SERVER_REFLECTION_H
