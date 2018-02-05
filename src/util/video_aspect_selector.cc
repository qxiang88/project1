//
// Created by wangqixiang on 2018/2/5.
//

#include "util/video_aspect_selector.h"
#include "corgi/strings/str_util.h"
#include "corgi/strings/numbers.h"

namespace mms {

namespace {

VideoAspectSelector::AspectResolutionPair
FindNearlyMatch(const VideoAspectSelector::AspectResolutionList& list,
                const std::string& width, const std::string& height) {
  std::vector<double> aspects;
  std::string size = width+"x"+height;
  VideoAspectSelector::AspectResolutionList l = list;
  l.push_back({width+":"+height, size});

  for (auto& kv : l) {
    if (size == kv.second && kv != l.back()) {
      return kv;
    } else {
      std::vector<std::string> aspect = corgi::str_util::Split(kv.first, ":");
      int w, h;
      DCHECK(aspect.size() == 2) << "Aspect config error: " << kv.first;
      DCHECK(corgi::strings::safe_strto32(aspect[0], &w)) << "Aspect width not legal: " << aspect[0];
      DCHECK(corgi::strings::safe_strto32(aspect[1], &h)) << "Aspect height not legel: " << aspect[1];
      aspects.push_back( (double)(w / h) );
    }
  }

  double delta = 1000000.00;
  int index = -1;
  double orig_apsect = aspects.back();
  for (int i = 0; i < aspects.size()-1; i++) {
    double d = aspects[i] > orig_apsect ? (aspects[i] - orig_apsect) :
               (orig_apsect - aspects[i]);
    if (d < delta) {
      delta = d;
      index = i;
    }
  }
  return list[index];
}

} // namespace

// static
const VideoAspectSelector::AspectResolutionList
    VideoAspectSelector::kLowDefinitionAspectResolutions = {
    {"16:10" , "320x200"},
    {"16:9"  , "480x272"},
    {"4:3"   , "320x240"},
    {"3:2"   , "480x320"}
};

VideoAspectSelector::VideoAspectSelector()
    : ld_aspect_resolutions_(kLowDefinitionAspectResolutions) {}

VideoAspectSelector::AspectResolutionPair
VideoAspectSelector::GetLDAspectResolutionByNearlyMatch(const std::string &width,
                                                        const std::string &height) {
  return FindNearlyMatch(ld_aspect_resolutions_, width, height);
}

} // namespace mms
