#ifndef FILTER_H
#define FILTER_H

#include <opencv2/opencv.hpp>

#include "matrix.h"

class FilterBase
{
public:
    FilterBase(const std::string &name) : name(name) {}
    virtual cv::Mat apply(const cv::Mat cloud) { return cloud; }

    std::string name;
};

class FirstFilter : public FilterBase
{
public:
    FirstFilter(const std::string &name) : FilterBase(name) {}
    virtual cv::Mat apply(const cv::Mat cloud);
};

#endif // FILTER_H
