//
// Created by xiang88 on 11/21/17.
//

#include "service/json_util.h"
#include "corgi/base/errors.h"

#include <gtest/gtest.h>


namespace mms {

TEST(JsonUtilTest, Deserilize) {
  const std::string okJson=
      "{\"name\":\"xiang88\", \"age\": 1}";
  auto r = Deserilize(okJson);
  if (r.ok()) {
    std::shared_ptr<corgi::json::Value> v = r.ConsumeValueOrDie();
    LOG(INFO) << (*v)["name"].asString();
  }
  const std::string badJson=
      "{, \"age\":- 1}";
  auto r1 = Deserilize(badJson);
  EXPECT_FALSE(r1.ok());
  LOG(INFO) << r1.status().ToString();
}

TEST(JsonUtilTest, Serialize) {
  corgi::Status ok;
  LOG(INFO) << Serialize(ok);
  LOG(INFO) << Serialize(corgi::errors::InvalidArgument("missing something"));
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
