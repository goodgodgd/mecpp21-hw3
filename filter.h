#ifndef FILTER_H
#define FILTER_H

#include <opencv4/opencv2/opencv.hpp>

#include "matrix.h"

class FilterBase
{
public:
    std::vector<Matrix<float>> points;

    FilterBase(const std::string &name) : name(name) {}
    
    cv::Mat apply(const cv::Mat cloud_)
    {
        auto points = convert_cloud_to_points(cloud_);
        auto points_padding = padding(points, cloud_.rows, cloud_.cols);
        auto filter_points = filter(points_padding, cloud_.rows, cloud_.cols);
        auto cloud = convert_points_to_cloud(filter_points,  cloud_);
        return cloud;

    };
    std::vector<Matrix<float>> convert_cloud_to_points(cv::Mat cloud)
    {

        std::vector<Matrix<float>> mid_points;
        for(int y =0; y< cloud.rows; ++y)
        {
                for(int x =0; x< cloud.cols; ++x)
                {
                    float point_x = cloud.at<cv::Vec3f>(y,x)[0];
                    float point_y = cloud.at<cv::Vec3f>(y,x)[1];
                    float point_z = cloud.at<cv::Vec3f>(y,x)[2];               
                    
                    Matrix<float> point({point_x,point_y,point_z}, 1, 3);
                    mid_points.push_back(point);
                }

        }
        return mid_points;
    }

    std::vector<Matrix<float>> padding(std::vector<Matrix<float>> points_, int rows, int cols)
    {
        Matrix<float> zero_pad({0,0,0},1,3);
        for(int y =0; y< rows; ++y)
        {
            points_.insert(points_.begin()+cols*y,4,zero_pad);
        }
        points_.insert(points_.begin(),cols*2+6,zero_pad);
        points_.insert(points_.end(),cols*2+10,zero_pad);
        return points_;
    }

    virtual std::vector<Matrix<float>> filter(std::vector<Matrix<float>> points_, int rows, int cols){ return points_; }
    cv::Mat convert_points_to_cloud(std::vector<Matrix<float>> points_ ,cv::Mat cloud)
    {
        for(int y = 0; y < cloud.rows; ++y)
        {
            for(int x = 0; x < cloud.cols; ++x)
            {
                int centor_point = cloud.cols*(y+2)+(x+2);
                cloud.at<cv::Vec3f>(y,x)[0] = points_[centor_point].at(0,0);
                cloud.at<cv::Vec3f>(y,x)[1] = points_[centor_point].at(0,1);
                cloud.at<cv::Vec3f>(y,x)[2] = points_[centor_point].at(0,2);
            }
        }
        return cloud;        
    }
    std::string name;
};

class MedianFilter : public FilterBase
{
    public:
    
        MedianFilter(const std::string &name) : FilterBase(name) {}
        virtual std::vector<Matrix<float>> filter(std::vector<Matrix<float>> points_ ,int rows, int cols);
};

#endif // FILTER_H
