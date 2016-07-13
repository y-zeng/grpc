/*
 *
 * Copyright 2016, Google Inc.
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

#include "grpc_tool.h"

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <gflags/gflags.h>
#include <grpc++/channel.h>
#include <grpc++/create_channel.h>
#include <grpc++/grpc++.h>
#include <grpc++/security/credentials.h>
#include <grpc++/support/string_ref.h>
#include <grpc/grpc.h>
#include <grpc/support/log.h>
#include "test/cpp/util/cli_call.h"

#include "test/cpp/util/proto_file_parser.h"
#include "test/cpp/util/proto_reflection_descriptor_database.h"
#include "test/cpp/util/service_describer.h"
#include "test/cpp/util/string_ref_helper.h"
#include "test/cpp/util/test_config.h"

DEFINE_bool(l, false, "Use a long listing format");
DEFINE_bool(enable_ssl, false, "Whether to use ssl/tls.");
DEFINE_bool(use_auth, false, "Whether to create default google credentials.");
DEFINE_bool(remotedb, true, "Use server types to parse and format messages");
DEFINE_string(metadata, "",
              "Metadata to send to server, in the form of key1:val1:key2:val2");
DEFINE_string(proto_path, ".", "Path to look for the proto file.");
DEFINE_string(proto_file, "", "Name of the proto file.");
DEFINE_bool(binary_input, false, "Input in binary format");
DEFINE_bool(binary_output, false, "Output in binary format");
DEFINE_string(infile, "", "Input file (default is stdin)");

namespace grpc {
namespace testing {
namespace {

class GrpcTool {
 public:
  // explicit GrpcTool(const GrpcToolOptions& options);
  explicit GrpcTool();
  virtual ~GrpcTool() {}

  // const GrpcToolOptions& options() const { return options_; }

  bool Help(int argc, const char** argv, OutputCallback callback);
  bool ListServices(int argc, const char** argv, OutputCallback callback);
  bool CallMethod(int argc, const char** argv, OutputCallback callback);
  bool PrintType(int argc, const char** argv, OutputCallback callback);
  bool PrintTypeId(int argc, const char** argv, OutputCallback callback);
  bool ParseMessage(int argc, const char** argv, OutputCallback callback);
  bool ToText(int argc, const char** argv, OutputCallback callback);
  bool ToBinary(int argc, const char** argv, OutputCallback callback);
  // bool RunService(int argc, const char** argv, OutputCallback callback);
  // bool WebTool(int argc, const char** argv, OutputCallback callback);

  void SetPrintCommandMode(int exit_status) {
    print_command_usage_ = true;
    usage_exit_status_ = exit_status;
  }

 private:
  void CommandUsage(const grpc::string& usage) const;

  std::shared_ptr<grpc::Channel> NewChannel(const grpc::string& server_address);

  // bool InitRPC(RPC* rpc) const;
  //
  // bool SetRequestExtensions(const ProtocolDBCreator& db_creator,
  //                           RPC* rpc) const;
  //
  // RPC_Channel* WaitForCanAccept(RPC_Channel* channel, double deadline)
  // const;
  // template <typename T>
  // RPC_Channel* WaitAndReturn(T* channel, const string& address_type,
  //                            const string& address) const;
  // string GetUnqualifiedServiceName(const string& full_service_name,
  //                                  const string* override_service_name)
  //                                  const;
  // vector<ServiceSpec> ParseServiceNames(const char* services) const;
  //
  // bool SendGenericRequest(RPC_Channel* channel,
  //                         const RPC_ServiceDescriptor::Method* method,
  //                         const string& internal_method,
  //                         const proto2::Message* request,
  //                         proto2::Message* reply,
  //                         const ProtocolDBCreator* db_creator,
  //                         ResponseWriter* writer, string* error_text)
  //                         const;
  //
  // void StreamReply(RawMessage* stream, ResponseWriter* writer,
  //                  const MessageHandler* message_handler,
  //                  const ProtocolDescriptor* stream_desc,
  //                  const proto2::Descriptor* stream_desc2,
  //                  const ProtocolDBCreator* db_creator) const;

  // GrpcToolOptions options_;

  // Flag turned on when commands should print usage message
  // instead of executing the specified command.
  bool print_command_usage_;
  int usage_exit_status_;
};

template <typename T>
std::function<bool(GrpcTool*, int, const char**, OutputCallback)> BindWith4Args(
    T&& func) {
  return std::bind(std::forward<T>(func), std::placeholders::_1,
                   std::placeholders::_2, std::placeholders::_3,
                   std::placeholders::_4);
}

template <typename T>
size_t ArraySize(T& a) {
  return ((sizeof(a) / sizeof(*(a))) /
          static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))));
}

void ParseMetadataFlag(
    std::multimap<grpc::string, grpc::string>* client_metadata) {
  if (FLAGS_metadata.empty()) {
    return;
  }
  std::vector<grpc::string> fields;
  const char* delim = ":";
  size_t cur, next = -1;
  do {
    cur = next + 1;
    next = FLAGS_metadata.find_first_of(delim, cur);
    fields.push_back(FLAGS_metadata.substr(cur, next - cur));
  } while (next != grpc::string::npos);
  if (fields.size() % 2) {
    std::cerr << "Failed to parse metadata flag" << std::endl;
    exit(1);
  }
  for (size_t i = 0; i < fields.size(); i += 2) {
    client_metadata->insert(
        std::pair<grpc::string, grpc::string>(fields[i], fields[i + 1]));
  }
}

template <typename T>
void PrintMetadata(const T& m, const grpc::string& message) {
  if (m.empty()) {
    return;
  }
  std::cerr << message << std::endl;
  grpc::string pair;
  for (typename T::const_iterator iter = m.begin(); iter != m.end(); ++iter) {
    pair.clear();
    pair.append(iter->first.data(), iter->first.size());
    pair.append(" : ");
    pair.append(iter->second.data(), iter->second.size());
    std::cerr << pair << std::endl;
  }
}

struct Command {
  const char* command;
  std::function<bool(GrpcTool*, int, const char**, OutputCallback)> function;
  int min_args;
  int max_args;
};

const Command ops[] = {
    {"help", BindWith4Args(&GrpcTool::Help), 0, INT_MAX},
    {"ls", BindWith4Args(&GrpcTool::ListServices), 1, 3},
    {"list", BindWith4Args(&GrpcTool::ListServices), 1, 3},
    {"call", BindWith4Args(&GrpcTool::CallMethod), 2, 3},
    {"type", BindWith4Args(&GrpcTool::PrintType), 2, 2},
    // {"typeid", BindWith4Args(&GrpcTool::PrintTypeId), 2, 2},
    {"parse", BindWith4Args(&GrpcTool::ParseMessage), 2, 3},
    {"totext", BindWith4Args(&GrpcTool::ToText), 2, 3},
    {"tobinary", BindWith4Args(&GrpcTool::ToBinary), 2, 3},
};

void Usage(const grpc::string& msg) {
  fprintf(
      stderr,
      "%s\n"
      "  grpc_tool ls ...         ; List services\n"
      "  grpc_tool call ...       ; Call method\n"
      "  grpc_tool type ...       ; Print type\n"
      "  grpc_tool typeid ...     ; Print typeid\n"
      "  grpc_tool parse ...      ; Parse message\n"
      "  grpc_tool totext ...     ; Convert binary message to text\n"
      "  grpc_tool tobinary ...   ; Convert text message to binary\n"
      // "  grpc_tool run ...        ; Run fake service locally\n"
      // "  grpc_tool web ...        ; Run web tool\n"
      "  grpc_tool help ...       ; Print this message, or per-command usage\n"
      "\n"
      "",
      msg.c_str());

  exit(1);
}

const Command* FindCommand(const grpc::string& name) {
  for (int i = 0; i < (int)ArraySize(ops); i++) {
    if (name == ops[i].command) {
      return &ops[i];
    }
  }
  return NULL;
}
}  // namespace

// GrpcToolOptions GetDefaultOptions() { return GrpcToolOptions(); }

int GrpcToolMainLib(int argc, const char** argv, OutputCallback callback) {
  // TODO(zyc): Implement GetOptionsFromFlags
  //   GrpcToolOptions options = GetDefaultOptions();
  //   return MainLib(argc, argv, options, callback);
  // }
  //
  // int MainLib(int argc, const char** argv, const GrpcToolOptions& options,
  //             OutputCallback callback) {
  if (argc < 2) Usage("No command specified");
  grpc::string command = argv[1];
  argc -= 2;
  argv += 2;

  // GrpcToolOptions o = options;
  // if (o.terse()) {
  //   o.set_print_level(GrpcToolOptions::SHORT);
  // }

  const Command* cmd = FindCommand(command);
  if (cmd != NULL) {
    // GrpcTool grpc_tool(o);
    GrpcTool grpc_tool;
    if (argc < cmd->min_args || argc > cmd->max_args) {
      // Force the command to print its usage message
      fprintf(stderr, "\nWrong number of arguments for %s\n", command.c_str());
      grpc_tool.SetPrintCommandMode(1);
      cmd->function(&grpc_tool, -1, NULL, callback);
      gpr_log(GPR_ERROR, "unreachable");
    }
    const bool ok = cmd->function(&grpc_tool, argc, argv, callback);
    return ok ? 0 : 1;
  } else {
    // Nothing matched
    Usage("Invalid command '" + grpc::string(command.c_str()) + "'");
  }

  return 1;
}

// GrpcTool::GrpcTool(const GrpcToolOptions& options)
//     : options_(options), print_command_usage_(false), usage_exit_status_(0)
//     {}

GrpcTool::GrpcTool() : print_command_usage_(false), usage_exit_status_(0) {}

void GrpcTool::CommandUsage(const grpc::string& usage) const {
  if (print_command_usage_) {
    fprintf(stderr, "\n%s%s\n", usage.c_str(),
            (usage.empty() || usage[usage.size() - 1] != '\n') ? "\n" : "");
    exit(usage_exit_status_);
  }
}

std::shared_ptr<grpc::Channel> GrpcTool::NewChannel(
    const grpc::string& server_address) {
  std::shared_ptr<grpc::ChannelCredentials> creds;
  if (!FLAGS_enable_ssl) {
    creds = grpc::InsecureChannelCredentials();
  } else {
    if (FLAGS_use_auth) {
      creds = grpc::GoogleDefaultCredentials();
    } else {
      creds = grpc::SslCredentials(grpc::SslCredentialsOptions());
    }
  }
  return grpc::CreateChannel(server_address, creds);
}

bool GrpcTool::Help(int argc, const char** argv, OutputCallback callback) {
  CommandUsage(
      "Print help\n"
      "  grpc_tool help [subcommand]\n");

  if (argc == 0) {
    Usage("");
  } else {
    const Command* cmd = FindCommand(argv[0]);
    if (cmd == NULL) {
      Usage("Unknown command '" + grpc::string(argv[0]) + "'");
    }
    SetPrintCommandMode(0);
    cmd->function(this, -1, NULL, callback);
  }
  return true;
}

bool GrpcTool::ListServices(int argc, const char** argv,
                            OutputCallback callback) {
  CommandUsage(
      "List services\n"
      "  grpc_tool ls <address> [<service>[/<method>]]\n"
      "    <address>                ; host:port\n"
      "    <service>                ; Exported service name\n"
      "    <method>                 ; Method name\n"
      "    --l                      ; Use a long listing format\n"
      "    --enable_ssl             ; Set whether to use tls\n"
      "    --use_auth               ; Set whether to create default google"
      " credentials\n");

  grpc::string server_address(argv[0]);
  std::shared_ptr<grpc::Channel> channel = NewChannel(server_address);
  grpc::ProtoReflectionDescriptorDatabase desc_db(channel);
  google::protobuf::DescriptorPool desc_pool(&desc_db);

  std::vector<grpc::string> service_list;
  if (!desc_db.GetServices(&service_list)) {
    return false;
  }

  // If no service is specified, dump the list of services.
  grpc::string output;
  if (argc < 2) {
    // List all services, if --l is passed, then include full description,
    // otherwise include a summarized list only.
    if (FLAGS_l) {
      output = DescribeServiceList(service_list, desc_pool);
    } else {
      for (auto const& service : service_list) {
        output.append(service);
        output.append("\n");
      }
    }
  } else {
    grpc::string service_name;
    grpc::string method_name;
    std::stringstream ss(argv[1]);

    // Remove leading slashes.
    while (ss.peek() == '/') {
      ss.get();
    }

    // Parse service and method names. Support the following patterns:
    //   Service
    //   Service Method
    //   Service.Method
    //   Service/Method
    if (argc == 3) {
      std::getline(ss, service_name, '/');
      method_name = argv[2];
    } else {
      if (std::getline(ss, service_name, '/')) {
        std::getline(ss, method_name);
      }
    }

    const google::protobuf::ServiceDescriptor* service =
        desc_pool.FindServiceByName(service_name);
    if (service != nullptr) {
      if (method_name.empty()) {
        output = FLAGS_l ? DescribeService(service) : SummarizeService(service);
      } else {
        method_name.insert(0, 1, '.');
        method_name.insert(0, service_name);
        const google::protobuf::MethodDescriptor* method =
            desc_pool.FindMethodByName(method_name);
        if (method != nullptr) {
          output = FLAGS_l ? DescribeMethod(method) : SummarizeMethod(method);
        } else {
          fprintf(stderr, "Method %s not found in service %s.\n",
                  method_name.c_str(), service_name.c_str());
          return false;
        }
      }
    } else {
      if (!method_name.empty()) {
        fprintf(stderr, "Service %s not found.\n", service_name.c_str());
        return false;
      } else {
        const google::protobuf::MethodDescriptor* method =
            desc_pool.FindMethodByName(service_name);
        if (method != nullptr) {
          output = FLAGS_l ? DescribeMethod(method) : SummarizeMethod(method);
        } else {
          fprintf(stderr, "Service or method %s not found.\n",
                  service_name.c_str());
          return false;
        }
      }
    }
  }
  return callback(output);
}

bool GrpcTool::PrintType(int argc, const char** argv, OutputCallback callback) {
  CommandUsage(
      "Print type\n"
      "  grpc_tool type <address> <type>\n"
      "    <address>                ; host:port\n"
      "    <type>                   ; Protocol buffer type name\n"
      "    --enable_ssl             ; Set whether to use tls\n"
      "    --use_auth               ; Set whether to create default google"
      " credentials\n");

  grpc::string server_address(argv[0]);
  std::shared_ptr<grpc::Channel> channel = NewChannel(server_address);
  grpc::ProtoReflectionDescriptorDatabase desc_db(channel);
  google::protobuf::DescriptorPool desc_pool(&desc_db);

  grpc::string output;
  const google::protobuf::Descriptor* descriptor =
      desc_pool.FindMessageTypeByName(argv[1]);
  if (descriptor != nullptr) {
    output = descriptor->DebugString();
  } else {
    fprintf(stderr, "Type %s not found.\n", argv[1]);
    return false;
  }
  return callback(output);
}

bool GrpcTool::PrintTypeId(int argc, const char** argv,
                           OutputCallback callback) {
  CommandUsage(
      "Print typeid\n"
      "  grpc_cli typeid <address> <typeid>\n"
      "    <address>                ; host:port\n"
      "    <typeid>                 ; Protocol buffer type id\n"
      "    --enable_ssl             ; Set whether to use tls\n"
      "    --use_auth               ; Set whether to create default google"
      " credentials\n");

  grpc::string server_address(argv[0]);
  std::shared_ptr<grpc::Channel> channel = NewChannel(server_address);
  grpc::ProtoReflectionDescriptorDatabase desc_db(channel);
  google::protobuf::DescriptorPool desc_pool(&desc_db);

  int id = std::stoi(grpc::string(argv[1]), nullptr);
  grpc::string output;
  const google::protobuf::Descriptor* descriptor =
      desc_pool.FindMessageTypeByName(argv[1]);
  if (descriptor != nullptr) {
    output = descriptor->DebugString();
  } else {
    fprintf(stderr, "Type id %d not found.\n", id);
    return false;
  }
  return callback(output);
}

bool GrpcTool::CallMethod(int argc, const char** argv,
                          OutputCallback callback) {
  CommandUsage(
      "Call method\n"
      "  grpc_cli call <address> <service>[.<method>] <request>\n"
      "    <address>                ; host:port\n"
      "    <service>                ; Exported service name\n"
      "    <method>                 ; Method name\n"
      "    <request>                ; Text protobuffer (overrides infile)\n"
      "    --proto_file             ; Comma separated proto files used as a"
      " fallback when parsing request/response\n"
      "    --proto_path             ; The search path of proto files, valid"
      " only when --proto_file is given\n"
      "    --metadata               ; The metadata to be sent to the server\n"
      "    --enable_ssl             ; Set whether to use tls\n"
      "    --use_auth               ; Set whether to create default google"
      " credentials\n"
      "    --infile                 ; Input filename (defaults to stdin)\n"
      "    --outfile                ; Output filename (defaults to stdout)\n"
      "    --binary_input           ; Input in binary format\n"
      "    --binary_output          ; Output in binary format\n");

  std::stringstream output_ss;
  grpc::string request_text;
  grpc::string server_address(argv[0]);
  grpc::string method_name(argv[1]);
  std::unique_ptr<grpc::testing::ProtoFileParser> parser;
  grpc::string serialized_request_proto;

  if (argc == 3) {
    request_text = argv[2];
    if (!FLAGS_infile.empty()) {
      std::cerr << "warning: request given in argv, ignoring --infile"
                << std::endl;
    }
  } else {
    std::stringstream input_stream;
    if (FLAGS_infile.empty()) {
      if (isatty(STDIN_FILENO)) {
        std::cerr << "reading request message from stdin..." << std::endl;
      }
      input_stream << std::cin.rdbuf();
    } else {
      std::ifstream input_file(FLAGS_infile, std::ios::in | std::ios::binary);
      input_stream << input_file.rdbuf();
      input_file.close();
    }
    request_text = input_stream.str();
  }

  std::shared_ptr<grpc::Channel> channel = NewChannel(server_address);
  if (!FLAGS_binary_input || !FLAGS_binary_output) {
    parser.reset(
        new grpc::testing::ProtoFileParser(FLAGS_remotedb ? channel : nullptr,
                                           FLAGS_proto_path, FLAGS_proto_file));
    if (parser->HasError()) {
      return false;
    }
  }

  if (FLAGS_binary_input) {
    serialized_request_proto = request_text;
  } else {
    serialized_request_proto = parser->GetSerializedProtoFromMethod(
        method_name, request_text, true /* is_request */);
    if (parser->HasError()) {
      return false;
    }
    // } else {
    //   std::ifstream input_file(FLAGS_input_binary_file,
    //                            std::ios::in | std::ios::binary);
    //   std::stringstream input_stream;
    //   input_stream << input_file.rdbuf();
    //   serialized_request_proto = input_stream.str();
  }
  std::cerr << "connecting to " << server_address << std::endl;

  grpc::string serialized_response_proto;
  std::multimap<grpc::string, grpc::string> client_metadata;
  std::multimap<grpc::string_ref, grpc::string_ref> server_initial_metadata,
      server_trailing_metadata;
  ParseMetadataFlag(&client_metadata);
  PrintMetadata(client_metadata, "Sending client initial metadata:");
  grpc::Status s = grpc::testing::CliCall::Call(
      channel, parser->GetFormatedMethodName(method_name),
      serialized_request_proto, &serialized_response_proto, client_metadata,
      &server_initial_metadata, &server_trailing_metadata);
  PrintMetadata(server_initial_metadata,
                "Received initial metadata from server:");
  PrintMetadata(server_trailing_metadata,
                "Received trailing metadata from server:");
  if (s.ok()) {
    std::cerr << "Rpc succeeded with OK status" << std::endl;
    if (FLAGS_binary_output) {
      output_ss << serialized_response_proto;
    } else {
      grpc::string response_text = parser->GetTextFormatFromMethod(
          method_name, serialized_response_proto, false /* is_request */);
      if (parser->HasError()) {
        return false;
      }
      output_ss << "Response: \n " << response_text << std::endl;
    }
    // if (!FLAGS_output_binary_file.empty()) {
    //   std::ofstream output_file(FLAGS_output_binary_file,
    //                             std::ios::trunc | std::ios::binary);
    //   output_file << serialized_response_proto;
    // }
  } else {
    std::cerr << "Rpc failed with status code " << s.error_code()
              << ", error message: " << s.error_message() << std::endl;
  }

  return callback(output_ss.str());
}

bool GrpcTool::ParseMessage(int argc, const char** argv,
                            OutputCallback callback) {
  CommandUsage(
      "Parse message\n"
      "  grpc_cli parse <address> <type> [<message>]\n"
      "    <address>                ; host:port\n"
      "    <type>                   ; Protocol buffer type name\n"
      "    <message>                ; Text protobuffer (overrides --infile)\n"
      "    --proto_file             ; Comma separated proto files used as a"
      " fallback when parsing request/response\n"
      "    --proto_path             ; The search path of proto files, valid"
      " only when --proto_file is given\n"
      "    --enable_ssl             ; Set whether to use tls\n"
      "    --use_auth               ; Set whether to create default google"
      " credentials\n"
      "    --infile                 ; Input filename (defaults to stdin)\n"
      "    --outfile                ; Output filename (defaults to stdout)\n"
      "    --binary_input           ; Input in binary format\n"
      "    --binary_output          ; Output in binary format\n");

  std::stringstream output_ss;
  grpc::string message_text;
  grpc::string server_address(argv[0]);
  grpc::string type_name(argv[1]);
  std::unique_ptr<grpc::testing::ProtoFileParser> parser;
  grpc::string serialized_request_proto;

  if (argc == 3) {
    message_text = argv[2];
    if (!FLAGS_infile.empty()) {
      std::cerr << "warning: message given in argv, ignoring --infile"
                << std::endl;
    }
  } else {
    std::stringstream input_stream;
    if (FLAGS_infile.empty()) {
      if (isatty(STDIN_FILENO)) {
        std::cerr << "reading request message from stdin..." << std::endl;
      }
      input_stream << std::cin.rdbuf();
    } else {
      std::ifstream input_file(FLAGS_infile, std::ios::in | std::ios::binary);
      input_stream << input_file.rdbuf();
      input_file.close();
    }
    message_text = input_stream.str();
  }

  if (!FLAGS_binary_input || !FLAGS_binary_output) {
    std::shared_ptr<grpc::Channel> channel = NewChannel(server_address);
    parser.reset(
        new grpc::testing::ProtoFileParser(FLAGS_remotedb ? channel : nullptr,
                                           FLAGS_proto_path, FLAGS_proto_file));
    if (parser->HasError()) {
      return false;
    }
  }

  // if (!message_text.empty()) {
  if (FLAGS_binary_input) {
    serialized_request_proto = message_text;
  } else {
    serialized_request_proto =
        parser->GetSerializedProtoFromMessageType(type_name, message_text);
    if (parser->HasError()) {
      return false;
    }
    // } else {
    //   std::ifstream input_file(FLAGS_input_binary_file,
    //                            std::ios::in | std::ios::binary);
    //   std::stringstream input_stream;
    //   input_stream << input_file.rdbuf();
    //   serialized_request_proto = input_stream.str();
  }

  if (FLAGS_binary_output) {
    output_ss << serialized_request_proto;
  } else {
    grpc::string output_text = parser->GetTextFormatFromMessageType(
        type_name, serialized_request_proto);
    if (parser->HasError()) {
      return false;
    }
    output_ss << output_text << std::endl;
  }
  // if (!FLAGS_output_binary_file.empty()) {
  //   std::ofstream output_file(FLAGS_output_binary_file,
  //                             std::ios::trunc | std::ios::binary);
  //   output_file << serialized_request_proto;
  // }

  return callback(output_ss.str());
}

bool GrpcTool::ToText(int argc, const char** argv, OutputCallback callback) {
  CommandUsage(
      "Convert binary message to text\n"
      "  grpc_cli totext <proto_file> <type>\n"
      "    <proto_file>             ; Comma separated list of proto files\n"
      "    <type>                   ; Protocol buffer type name\n"
      "    --proto_path             ; The search path of proto files\n"
      "    --infile                 ; Input filename (defaults to stdin)\n"
      "    --outfile                ; Output filename (defaults to stdout)\n");

  FLAGS_proto_file = argv[0];
  FLAGS_remotedb = false;
  FLAGS_binary_input = true;
  FLAGS_binary_output = false;
  return ParseMessage(argc, argv, callback);
}

bool GrpcTool::ToBinary(int argc, const char** argv, OutputCallback callback) {
  CommandUsage(
      "Convert text message to binary\n"
      "  grpc_cli tobinary <proto_file> <type> [<message>]\n"
      "    <proto_file>             ; Comma separated list of proto files\n"
      "    <type>                   ; Protocol buffer type name\n"
      "    --proto_path             ; The search path of proto files\n"
      "    --infile                 ; Input filename (defaults to stdin)\n"
      "    --outfile                ; Output filename (defaults to stdout)\n");

  FLAGS_proto_file = argv[0];
  FLAGS_remotedb = false;
  FLAGS_binary_input = false;
  FLAGS_binary_output = true;
  return ParseMessage(argc, argv, callback);
}

}  // namespace testing
}  // namespace grpc
