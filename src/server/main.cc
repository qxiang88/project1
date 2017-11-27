//
// Created by xiang88 on 11/21/17.
//
#include "corgi/base/logging.h"

#include "server/server.h"

#include <gflags/gflags.h>

DEFINE_string(conf, "/tmp/conf_d.json", "Set conf file path");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  mms::Server server(FLAGS_conf);
  corgi::Status status = server.Init();
  if (!status.ok()) {
    LOG(ERROR) << "Init server error: " << status.ToString();
    return 1;
  }
  status = server.Start();
  if (!status.ok()) {
    LOG(ERROR) << "Start server error: " << status.ToString();
    return 2;
  }

  LOG(INFO) << "Server running:";
  server.Run();
  return 0;
}
