#include "filter.h"

#include <iostream>

cv::Mat FirstFilter::apply(const cv::Mat cloud_origin)
{
    // Step1. cv::Mat cloud를 custom type으로 변환 
    //      e.g. std::vector<Matrix<float>> points;
    std::vector<std::vector<Matrix<float>>> points_origin = FilterBase::convert_cloud_to_points(cloud_origin);

    // Step2. 필터 적용
    int ksize = 5;
    Matrix<float> kernel = FilterBase::Average_filter(ksize);
    std::vector<std::vector<Matrix<float>>> filtered_points = FirstFilter::apply_filter(points_origin, cloud_origin.rows, cloud_origin.cols, kernel);

    // Step3. 1의 custom type을 다시 cv::Mat으로 변환하여 리턴
    cv::Mat cloud_apply = FilterBase::convert_points_to_cloud(filtered_points, cloud_origin);

    std::cout << "apply first filter\n" << std::endl;
    return cloud_apply;
}

cv::Mat SecondFilter::apply(const cv::Mat cloud_origin)
{
    // Step1. cv::Mat cloud를 custom type으로 변환 
    //      e.g. std::vector<Matrix<float>> points;
    std::vector<std::vector<Matrix<float>>> points_origin = FilterBase::convert_cloud_to_points(cloud_origin);

    // Step2. 필터 적용
    int ksize = 5;
    Matrix<float> kernel = FilterBase::Gaussian_filter(ksize);
    std::vector<std::vector<Matrix<float>>> filtered_points = SecondFilter::apply_filter(points_origin, cloud_origin.rows, cloud_origin.cols, kernel);

    // Step3. 1의 custom type을 다시 cv::Mat으로 변환하여 리턴
    cv::Mat cloud_apply = FilterBase::convert_points_to_cloud(filtered_points, cloud_origin);

    std::cout << "apply first filter\n" << std::endl;
    return cloud_apply;
}


std::vector<std::vector<Matrix<float>>> FirstFilter::apply_filter(std::vector<std::vector<Matrix<float>>> points, uint32_t rows, uint32_t cols, Matrix<float> kernel)
{
    for (int y=0; y<rows; ++y)
    {
        for(int x=0; x<cols; ++x)
        {
            if (points[x][y].at(0,2) == 0)
            {
                continue;
            }
            else
            {                  
                float point_x, point_y, point_z= FilterBase::Filter_cal(points, kernel, y, x);
                // apply filter and filtered_points
                points[x][y].at(0,0) = point_x;
                points[x][y].at(0,1) = point_y;
                points[x][y].at(0,2) = point_z;
            }
        }
    }
    return points;
}

std::vector<std::vector<Matrix<float>>> SecondFilter::apply_filter(std::vector<std::vector<Matrix<float>>> points, uint32_t rows, uint32_t cols, Matrix<float> kernel)
{
    for (int y=0; y<rows; ++y)
    {
        for(int x=0; x<cols; ++x)
        {
            if (points[x][y].at(0,2) == 0)
            {
                continue;
            }
            else
            {             
                float point_x, point_y, point_z= FilterBase::Filter_cal(points, kernel, y, x);
                
                // apply filter and filtered_points
                points[x][y].at(0,0) = point_x;
                points[x][y].at(0,1) = point_y;
                points[x][y].at(0,2) = point_z;
            }
        }
    }
    return points;
}