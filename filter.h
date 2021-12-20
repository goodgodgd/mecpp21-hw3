#ifndef FILTER_H
#define FILTER_H

#include <opencv4/opencv2/opencv.hpp>

#include "matrix.h"

class FilterBase
{
public:
    FilterBase(const std::string &name) : name(name) {}
    virtual cv::Mat apply(const cv::Mat cloud) { return cloud; }
    virtual std::vector<std::vector<Matrix<float>>> impl_filter(std::vector<std::vector<Matrix<float>>> &points, const uint32_t cloud_y, const uint32_t cloud_x, Matrix<float> &mask)
    { return points; }
    std::vector<std::vector<Matrix<float>>> convert_mat_to_vector(const cv::Mat &cloud)
    {
        std::vector<std::vector<Matrix<float>>> points;
        for(int r=0; r<cloud.rows; ++r)
        {
            for(int c=0; c<cloud.cols; ++c)
            {
                float x = cloud.at<cv::Vec3f>(r, c)[0];
                float y = cloud.at<cv::Vec3f>(r, c)[1];
                float z = cloud.at<cv::Vec3f>(r, c)[2];
                Matrix<float> points_matrix({x, y, z}, 1, 3);
                points.push_back(std::vector<Matrix<float>>());
                points[c].push_back(points_matrix);
            }
        }
        return points;
    }
    cv::Mat convert_vector_to_mat(std::vector<std::vector<Matrix<float>>> &filter_points, const cv::Mat &cloud_) noexcept
    {
        auto new_cloud = cloud_.clone();
        for(int y=0; y<cloud_.rows; ++y)
        {
            for(int x=0; x<cloud_.cols; ++x)
            {
                new_cloud.at<cv::Vec3f>(y, x)[0] = filter_points[x][y].at(0, 0);
                new_cloud.at<cv::Vec3f>(y, x)[1] = filter_points[x][y].at(0, 1);
                new_cloud.at<cv::Vec3f>(y, x)[2] = filter_points[x][y].at(0, 2);
            }
        }
        return new_cloud;
    }
    Matrix<float> create_mask(const int mask_size) noexcept 
    {
        Matrix<float> mask({1, 4, 6, 4, 1,
                            4, 16, 24, 16, 4,
                            6, 24, 36, 24, 6,
                            4, 16, 24, 16, 4,
                            1, 4, 6, 4, 1}, mask_size, mask_size);
        for (int y=0; y<mask_size; ++y)
        {
            for (int x=0; x<mask_size; ++x)
            {
                mask.at(y, x) /= 256;
            }
        }
        return mask;
    }
    std::string name;
};

class FirstFilter : public FilterBase
{
public:
    FirstFilter(const std::string &name) : FilterBase(name) {}
    virtual cv::Mat apply(const cv::Mat cloud);
    virtual std::vector<std::vector<Matrix<float>>> impl_filter(std::vector<std::vector<Matrix<float>>> points, uint32_t cloud_y, uint32_t cloud_x, Matrix<float> mask);
};

#endif // FILTER_H
