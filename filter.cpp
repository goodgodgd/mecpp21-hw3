#include "filter.h"

#include <iostream>

cv::Mat FirstFilter::apply(const cv::Mat cloud)
{
    std::cout << "apply first filter\n";
    return cloud;
}
