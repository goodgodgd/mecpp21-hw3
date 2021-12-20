#include "filter.h"

#include <iostream>

cv::Mat gaussian_filter(std::vector <Matrix<float>> points)
{
    cv::Mat img_blur;
    cv::GaussianBlur(points, img_blur, cv::Size(5,5), -1, 10, 5);
    
    return img_blur;
}

cv::Mat FirstFilter::apply(const cv::Mat cloud)
{
    std::vector <Matrix<float>> points;
    std::vector <Matrix<float>> filtered_points;
    for (int y=0; y<cloud.rows; ++y)
    {
        for(int x=0; x<cloud.cols; ++x)
        {
            filtered_points[y*cloud.cols + x] = gaussian_filter(points);
        }
    }
    // Step1. cv::Mat cloud를 custom type으로 변환
    //      e.g. std::vector<Matrix<float>> points;
    // Step2. 필터 적용
    // Step3. 1의 custom type을 다시 cv::Mat으로 변환하여 리턴
    std::cout << "apply first filter\n";
    return cloud;
}
