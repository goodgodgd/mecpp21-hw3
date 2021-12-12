#ifndef PCPMAIN_H
#define PCPMAIN_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/viz/viz3d.hpp>

#include "config.h"
#include "filter.h"

using std::cout;
using std::endl;

// Point Cloud Processing Main
class PCPMain
{
private:
    const std::string prj_path;
    const std::string color_name;
    const std::string depth_name;
    const CameraIntrinsic camera_param = Config::CAMERA_PARAM;
    const float max_noise = Config::MAX_NOISE;
    std::vector<std::unique_ptr<FilterBase>> filters;

    cv::Mat load_image(const std::string &name, int imtype);
    cv::Mat convert_to_point_cloud(cv::Mat depth);
    void show_point_cloud(cv::Mat cloud, cv::Mat color = cv::Mat());
    cv::Mat smooth_filter(const std::unique_ptr<FilterBase> &filter, cv::Mat cloud);
    cv::Mat add_noise_cloud(cv::Mat cloud);
    void evaluate_filter(cv::Mat cloud, cv::Mat smooth_cloud);

public:
    PCPMain(const std::string &prj_path, const std::string &color_name, const std::string &depth_name);
    ~PCPMain() {}
    void main();
};

#endif