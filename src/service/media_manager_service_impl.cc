//
// Created by xiang88 on 11/21/17.
//
#include "brpc/server.h"
#include "brpc/restful.h"

#include "corgi/strings/str_util.h"
#include "corgi/gtl/map_util.h"
#include "corgi/base/errors.h"

#include "proto/protocol.pb.h"
#include "service/media_manager_service_impl.h"
#include "service/json_util.h"
#include "json2pb/json_to_pb.h"

namespace mms {

///////////////////////// BRPC FileOutputStream

class BRPCFileOutputStream : public FileOutputStream {
 public:
  BRPCFileOutputStream(brpc::ProgressiveAttachment* progressive)
      : writer_(progressive) {}

  virtual ~BRPCFileOutputStream() {}

  virtual corgi::Status Write(const char* data, size_t len) override {
    int r = writer_->Write(data, len);
    if (r != 0) {
      return corgi::errors::DataLoss(strerror(errno));
    }
    return corgi::Status::OK();
  }

 private:
  butil::intrusive_ptr<brpc::ProgressiveAttachment> writer_;
};

/////////////////////////
MediaManagerServiceImpl::MediaManagerServiceImpl(ServiceProxy* proxy)
    : proxy_(proxy) {}

MediaManagerServiceImpl::~MediaManagerServiceImpl() {}

// Global Handler http entry-point
void MediaManagerServiceImpl::Handle(::google::protobuf::RpcController *controller,
                                     const ::mms::MediaHttpRequestPB *request,
                                     ::mms::MediaHttpResponsePB *response,
                                     ::google::protobuf::Closure *done) {
  brpc::ClosureGuard done_guard(done);
  brpc::Controller* bcontroller = static_cast<brpc::Controller*>(controller);
  bcontroller->http_response().set_content_type("text/plain");
  butil::IOBufBuilder builder;

  LOG(INFO) << "Request: " << bcontroller->http_request().uri();

  std::map<std::string, std::string> queries;
  for (brpc::URI::QueryIterator it = bcontroller->http_request().uri().QueryBegin();
      it != bcontroller->http_request().uri().QueryEnd();
      ++it) {
    queries[corgi::str_util::Lowercase(it->first)] = it->second;
  }
  std::string* action = corgi::gtl::FindOrNull(queries, "action");
  if (!action) {
    builder << Serialize(corgi::errors::InvalidArgument("missing `action`"));
    builder.move_to(bcontroller->response_attachment());
    return;
  }

  std::string action_str = corgi::str_util::Lowercase(*action);

  // TODO(xiang88):
  //
  if (action_str == "newtask") {
    builder << HandleNewTask(queries, bcontroller->request_attachment().to_string());
    builder.move_to(bcontroller->response_attachment());
  } else if (action_str == "querytask") {
    builder << HandleQueryTask(queries);
    builder.move_to(bcontroller->response_attachment());
  } else if (action_str == "accessm3u8") {
    BRPCFileOutputStream output_stream(bcontroller->CreateProgressiveAttachment());
    bcontroller->http_response().set_content_type("application/vnd.apple.mpegurl");
    corgi::Status s = HandleAccessM3U8(queries, &output_stream);
    if (!s.ok()) {
      if (corgi::errors::IsUnavailable(s)) {
        bcontroller->http_response().set_status_code(brpc::HTTP_STATUS_MOVE_PERMANENTLY);
      } else {
        bcontroller->http_response().set_status_code(brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR);
      }
    }
  } else if (action_str == "accesskey"){
    builder << HandleAccessHLSKey(queries);
    builder.move_to(bcontroller->response_attachment());
  } else {
    LOG(WARNING) << "Unknown action: " << action_str;
    builder << "Unknown action: " << action_str;
    builder.move_to(bcontroller->response_attachment());
  }
}

// Handles
std::string MediaManagerServiceImpl::HandleNewTask(const std::map<std::string, std::string> &queries,
                                                   const std::string& json_body) {
  TranscodeRequestPB request;
  TranscodeResponsePB response;
  std::string err;
  bool b = json2pb::JsonToProtoMessage(json_body, &request, &err);
  if (!b) {
    return Serialize(corgi::errors::InvalidArgument(err));
  }
  if (!request.has_media_target_format()) {
    request.set_media_target_format("hls");
  }

  corgi::Status s = proxy_->HandleNewTask(&request, &response);
  if (!s.ok()) {
    return Serialize(s);
  }
  corgi::json::Value data;
  data["uuid"] = response.uuid();
  return Serialize(s, data);
}

std::string MediaManagerServiceImpl::HandleQueryTask(const std::map<std::string, std::string>& queries) {
  QueryStateRequestPB request;
  QueryStateResponsePB response;
  const std::string* uuid = corgi::gtl::FindOrNull(queries, "uuid");
  if (!uuid) {
    return Serialize(corgi::errors::InvalidArgument("missing `uuid`"));
  }
  request.set_uuid(*uuid);
  corgi::Status s = proxy_->HandleQueryTask(&request, &response);
  if (!s.ok()) {
    return Serialize(s);
  }
  corgi::json::Value data;
  data["uuid"] = response.uuid();
  data["state"] = response.state();
  return Serialize(s, data);
}

corgi::Status MediaManagerServiceImpl::HandleAccessM3U8(const std::map<std::string, std::string> &queries,
                                                       FileOutputStream *output_stream) {
  AccessM3U8RequestPB request;
  AccessM3U8ResponsePB response;
  const std::string* uuid = corgi::gtl::FindOrNull(queries, "uuid");
  if (!uuid) {
    corgi::errors::InvalidArgument("missing `uuid`");
  }
  request.set_uuid(*uuid);
  corgi::Status s = proxy_->HandleAccessM3U8(&request, &response, output_stream);
  if (!s.ok()) {
    LOG(ERROR) << s.ToString();
    return s;
  }
  return corgi::Status::OK();
}

std::string MediaManagerServiceImpl::HandleAccessHLSKey(const std::map<std::string, std::string> &queries) {
  AccessKeyRequestPB request;
  AccessKeyResponsePB response;
  const std::string* uuid = corgi::gtl::FindOrNull(queries, "uuid");
  const std::string* token= corgi::gtl::FindOrNull(queries, "token");
  if (uuid == nullptr || token == nullptr) {
    return "missing: parameter";
  }
  request.set_uuid(*uuid);
  request.set_token(*token);
  corgi::Status s = proxy_->HandleAccessHLSKey(&request, &response);
  if (!s.ok()) {
    return s.ToString();
  }
  return response.key();
}

} // namespace mms
