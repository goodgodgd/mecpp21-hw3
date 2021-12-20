#include "filter.h"

#include <iostream>

cv::Mat FirstFilter::apply(const cv::Mat cloud)
{
    // Step1. cv::Mat cloud를 custom type으로 변환
    //      e.g. std::vector<Matrix<float>> points;
    // Step2. 필터 적용
    // Step3. 1의 custom type을 다시 cv::Mat으로 변환하여 리턴
    auto mask_size = 5;
    auto custom_points = FilterBase::convert_mat_to_vector(cloud);
    auto mask = FilterBase::create_mask(mask_size);
    auto filtered_points = FirstFilter::impl_filter(custom_points, cloud.rows, cloud.cols, mask);
    auto new_cloud = FilterBase::convert_vector_to_mat(filtered_points, cloud);

    
    std::cout << "apply first filter\n";
    return new_cloud;
}

std::vector<std::vector<Matrix<float>>> FirstFilter::impl_filter(std::vector<std::vector<Matrix<float>>> points, const uint32_t cloud_y, const uint32_t cloud_x, Matrix<float> mask)
{
    for(int cy=0; cy<cloud_y; ++cy)
    {
        for(int cx=0; cx<cloud_x; ++cx)
        {
            if(points[cx][cy].at(0, 2)!=0)
            {
                float point_x = 0;
                float point_y = 0;
                float point_z = 0;
                for(int y=0; y<mask.num_rows(); ++y)
                {
                    for(int x=0; x<mask.num_cols(); ++x)
                    {
                        point_x += points[cx+x-2][cy+y-2].at(0, 0) * mask.at(y, x);
                        point_y += points[cx+x-2][cy+y-2].at(0, 1) * mask.at(y, x);
                        point_z += points[cx+x-2][cy+y-2].at(0, 2) * mask.at(y, x);
                    }
                }
                points[cx][cy].at(0, 0) = point_x;
                points[cx][cy].at(0, 1) = point_y;
                points[cx][cy].at(0, 2) = point_z;
            }
        }
    }
    return points;
}


cv::Mat SecondFilter::apply(const cv::Mat cloud)
{
    auto mask_size = 5;
    auto custom_points = FilterBase::convert_mat_to_vector(std::move(cloud));
    auto mask = FilterBase::create_mask(std::move(mask_size));
    auto filtered_points = impl_filter(std::move(custom_points), cloud.rows, cloud.cols, std::move(mask));
    auto new_cloud = FilterBase::convert_vector_to_mat(filtered_points, cloud);

    
    std::cout << "apply second filter\n";
    return new_cloud;
}

std::vector<std::vector<Matrix<float>>> SecondFilter::impl_filter(std::vector<std::vector<Matrix<float>>> points, const uint32_t cloud_y, const uint32_t cloud_x, Matrix<float> mask)
{
    for(int cy=0; cy<cloud_y; ++cy)
    {
        for(int cx=0; cx<cloud_x; ++cx)
        {
            if(points[cx][cy].at(0, 2)!=0)
            {
                float point_x = 0;
                float point_y = 0;
                float point_z = 0;
                for(int y=0; y<mask.num_rows(); ++y)
                {
                    for(int x=0; x<mask.num_cols(); ++x)
                    {
                        point_x += points[cx+x-2][cy+y-2].at(0, 0) * mask.at(y, x);
                        point_y += points[cx+x-2][cy+y-2].at(0, 1) * mask.at(y, x);
                        point_z += points[cx+x-2][cy+y-2].at(0, 2) * mask.at(y, x);
                    }
                }
                points[cx][cy].at(0, 0) = point_x;
                points[cx][cy].at(0, 1) = point_y;
                points[cx][cy].at(0, 2) = point_z;
            }
        }
    }
    return points;
}
