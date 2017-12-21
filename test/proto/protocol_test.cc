//
// Created by wangqixiang on 2017/11/17.
//
#include "corgi/base/logging.h"
#include "proto/protocol.pb.h"
#include "util/time_util.h"
#include <gtest/gtest.h>

namespace mms {

TEST(MediaKeyPBTest, Serialize) {
  MediaKeyPB media_key_pb;
  media_key_pb.set_key("key12345");
  media_key_pb.set_iv("iv123455");
  std::string str = media_key_pb.SerializeAsString();
  MediaKeyPB key2;
  EXPECT_TRUE(key2.ParseFromString(str));
  LOG(INFO) << "key2: " << key2.DebugString();
}

TEST(HLSMetaPBTest, CtorAndDtor) {
  // HLSMetaPB* outter;
  HLSMetaPB outter;
  {
    MediaKeyPB* media_key_pb = new MediaKeyPB;
    media_key_pb->set_key("key");
    media_key_pb->set_iv("iv");
    HLSMetaPB inner;
    inner.set_allocated_media_key(media_key_pb);
    inner.set_gfs_m3u8_path("/path/to/m3u8");
    // outter = inner.New();
    outter.CopyFrom(inner);
  }
  LOG(INFO) << outter.gfs_m3u8_path();
  LOG(INFO) << outter.media_key().key();
  LOG(INFO) << outter.media_key().iv();
  // delete outter;
}

TEST(HLSMetaPBTest, SerializeAndDeserialize) {
  MediaKeyPB* media_key_pb = new MediaKeyPB;
  media_key_pb->set_key("key_str");
  media_key_pb->set_iv("iv_str");
  HLSMetaPB hls_meta_pb;
  hls_meta_pb.set_uuid("uuid123");
  hls_meta_pb.set_gfs_m3u8_path("/path/to/m3u8");
  hls_meta_pb.set_allocated_media_key(media_key_pb);
  hls_meta_pb.set_create_at(TimeUtil::FormatNow());
  std::string data;
  EXPECT_TRUE(hls_meta_pb.SerializeToString(&data));
  HLSMetaPB r;
  EXPECT_TRUE(r.ParseFromString(data));
  EXPECT_EQ(r.gfs_m3u8_path(), hls_meta_pb.gfs_m3u8_path());
  EXPECT_EQ(r.media_key().key(), hls_meta_pb.media_key().key());
  EXPECT_EQ(r.media_key().iv(), hls_meta_pb.media_key().iv());
  EXPECT_EQ(r.create_at(), hls_meta_pb.create_at());
  EXPECT_EQ(r.uuid(), hls_meta_pb.uuid());
  LOG(INFO) << r.DebugString();
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

