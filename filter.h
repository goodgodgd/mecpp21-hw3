#ifndef FILTER_H
#define FILTER_H

#include <opencv4/opencv2/opencv.hpp>

#include "matrix.h"

class FilterBase
{
public:
    FilterBase(const std::string &name) : name(name) {}
    virtual cv::Mat apply(const cv::Mat cloud) { return cloud; }

    std::string name;

    // cv::Mat cloud(depth.rows, depth.cols, CV_32FC3);
    std::vector<std::vector<Matrix<float>>> convert_cloud_to_points(cv::Mat cloud)
    {
        std::vector<std::vector<Matrix<float>>> points;
        for (int y=0; y<cloud.rows; ++y)
        {
            for(int x=0; x<cloud.cols; ++x)
            {
                float point_x = cloud.at<cv::Vec3f>(y,x)[0];
                float point_y = cloud.at<cv::Vec3f>(y,x)[1];
                float point_z = cloud.at<cv::Vec3f>(y,x)[2];
                Matrix<float> point_3D({point_x, point_y, point_z}, 1, 3);
                // .push_back <<- segmentation fault (core dumped)
                points[x].push_back(point_3D);
            }
        }
        return points;
    }

    Matrix<float> Average_filter(int ksize)
    {
        // mask size : 5x5, sigma : 5
        Matrix<float> avg_filter_kernel(
            {1, 1, 1, 1, 1,
            1, 1, 1, 1, 1,
            1, 1, 1, 1, 1,
            1, 1, 1, 1, 1,
            1, 1, 1, 1, 1}, ksize, ksize);
        
        // for (int y=0; y<ksize; ++y)
        // {
        //     for(int x=0; x<ksize; ++x)
        //     {
        //         avg_filter_kernel.at(y,x) /= (ksize*ksize);
        //     }
        // }
        return avg_filter_kernel;
    }

    Matrix<float> Gaussian_filter(int ksize)
    {
        // mask size : 5x5, sigma : 5
        Matrix<float> gaussian_filter_kernel(
            {0.0000, 0.0000, 0.0002, 0.0000, 0.0000,
            0.0000, 0.0113, 0.0837, 0.0113, 0.0000,
            0.0002, 0.0837, 0.6187, 0.0837, 0.0002,
            0.0000, 0.0113, 0.0837, 0.0113, 0.0000,
            0.0000, 0.0000, 0.0002, 0.0000, 0.0000,}, ksize, ksize);

        return gaussian_filter_kernel;
    }
    
    float Filter_cal(std::vector<std::vector<Matrix<float>>> points, Matrix<float> &filter_kernel, int y, int x)
    {
        float point_x = 0;
        float point_y = 0;
        float point_z = 0;
        int half_size = filter_kernel.num_rows() / 2;
        float point_wsum = 0;

        for(int ky=0; ky<filter_kernel.num_rows(); ++y)
        {
            for(int kx=0; kx<filter_kernel.num_cols(); ++x)
            {
                point_x += points[x+kx-half_size][y+ky-half_size].at(0,0) * filter_kernel.at(y,x);
                point_y += points[x+kx-half_size][y+ky-half_size].at(0,1) * filter_kernel.at(y,x);
                point_z += points[x+kx-half_size][y+ky-half_size].at(0,2) * filter_kernel.at(y,x);
                point_wsum += filter_kernel.at(y,x);
            }
        }
        point_x /= point_wsum;
        point_y /= point_wsum;
        point_z /= point_wsum;
        return point_x; point_y; point_z;
    }


    cv::Mat convert_points_to_cloud(std::vector<std::vector<Matrix<float>>> points, cv::Mat cloud_origin)
    {
        // cv::Mat cloud(depth.rows, depth.cols, CV_32FC3);
        // clone == Deepcopy
        auto convert_cloud = cloud_origin.clone();
        for (int y=0; y<convert_cloud.rows; ++y)
        {
            for(int x=0; x<convert_cloud.cols; ++x)
            {
                // Matrix<float> point_3D({point_x, point_y, point_z}, 1, 3);
                convert_cloud.at<cv::Vec3f>(y,x)[0] = points[x][y].at(0,0);
                convert_cloud.at<cv::Vec3f>(y,x)[1] = points[x][y].at(0,1);
                convert_cloud.at<cv::Vec3f>(y,x)[2] = points[x][y].at(0,2);
            }
        }
        return convert_cloud;
    }
};

class FirstFilter : public FilterBase
{
public:
    FirstFilter(const std::string &name) : FilterBase(name) {}
    virtual cv::Mat apply(const cv::Mat cloud);
    std::vector<std::vector<Matrix<float>>> apply_filter(std::vector<std::vector<Matrix<float>>> points, uint16_t y, uint16_t x, Matrix<float> kernel);
};

class SecondFilter : public FilterBase
{
public:
    SecondFilter(const std::string &name) : FilterBase(name) {}
    virtual cv::Mat apply(const cv::Mat cloud);
    std::vector<std::vector<Matrix<float>>> apply_filter(std::vector<std::vector<Matrix<float>>> points, uint16_t y, uint16_t x, Matrix<float> kernel);
};

#endif // FILTER_H
