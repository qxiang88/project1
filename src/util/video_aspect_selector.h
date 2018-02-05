//
// Created by wangqixiang on 2018/2/5.
//

#ifndef MEDIA_MANAGE_SYSTEM_VIDEO_ASPECT_SELECTOR_H
#define MEDIA_MANAGE_SYSTEM_VIDEO_ASPECT_SELECTOR_H

#include <string>
#include <vector>
#include <utility>

namespace mms {

class VideoAspectSelector {
 public:
  using AspectResolutionList = std::vector<std::pair<std::string, std::string>>;
  using AspectResolutionPair = std::pair<std::string, std::string>;


  static const AspectResolutionList kLowDefinitionAspectResolutions;
  // TODO(xiang88):
  // 高清&中清


  VideoAspectSelector();
  ~VideoAspectSelector() {}

  void SetLdAspectResolutions(const AspectResolutionList& a) { ld_aspect_resolutions_ = a; }

  AspectResolutionList GetLDAspectResolutions() const {
    return ld_aspect_resolutions_;
  }

  AspectResolutionPair GetLDAspectResolutionByNearlyMatch(const std::string& width, const std::string& height);

  // TODO(xiang88):
  // 高清&中清

 private:
  AspectResolutionList ld_aspect_resolutions_;
};

} // namespace mss
#endif //MEDIA_MANAGE_SYSTEM_VIDEO_ASPECT_SELECTOR_H
