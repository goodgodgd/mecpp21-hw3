#include "filter.h"

#include <iostream>

cv::Mat FirstFilter::apply(const cv::Mat cloud)
{
    // Step1. cv::Mat cloud를 custom type으로 변환
    //      e.g. std::vector<Matrix<float>> points;
    // Step2. 필터 적용
    // Step3. 1의 custom type을 다시 cv::Mat으로 변환하여 리턴
    std::cout << "apply first filter\n";
    return cloud;
}
