#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct CameraIntrinsic
{
    float fx, fy, cx, cy;
    CameraIntrinsic(const float fx, const float fy, const float cx, const float cy) : fx(fx), fy(fy), cx(cx), cy(cy) {}
};

class Config
{
public:
    static const std::string SRC_IMG_PATH;
    static const CameraIntrinsic CAMERA_PARAM;
    static const float MAX_NOISE;
};

#endif // CONFIG_H
