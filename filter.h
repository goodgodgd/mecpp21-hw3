#ifndef FILTER_H
#define FILTER_H

#include <opencv4/opencv2/opencv.hpp>

class FilterBase
{
public:
    virtual cv::Mat apply(const cv::Mat cloud) { return cloud; }
};

class FirstFilter : public FilterBase
{
public:
    virtual cv::Mat apply(const cv::Mat cloud);
};

#endif // FILTER_H
