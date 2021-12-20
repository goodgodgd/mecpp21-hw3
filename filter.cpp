#include "filter.h"
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <math.h>

using Eigen::MatrixXd;


MatrixXd GaussianMask(int dim=5, int sigma=3) noexcept
{
    int x,r,c;
    int dim2 = dim / 2;
    const float PI_F = 3.141592;

    MatrixXd imgMask(5, 5);
    for(r=0; r<imgMask.rows(); r++)
    {
        for(c=0; c<imgMask.cols(); c++)
        {
            x = r - dim2;
            imgMask(r,c) = exp(-(x * x) / (2 * sigma * sigma)) / (sqrt(2 * PI_F) * sigma);
        }
    }
    return imgMask;
}

float Gaussian_calc(const int r, const int c, MatrixXd points_, MatrixXd Gmask_)
{
    float sum, rst;
    for(int r_ = 0; r_ < 5; r_++)
    {
        for(int c_ = 0; c_ < 5; c_++)
        {
            rst = points_(r_+r,c_+c) * Gmask_(r_,c_);
            sum += rst;
        }
        rst = 0.0;
    }
    return sum;
}

float Gaussian_calc_clear_1(const int r, const int c, MatrixXd &points_, MatrixXd &Gmask_)
{
    float sum, rst;
    for(int r_ = 0; r_ < 5; r_++)
    {
        for(int c_ = 0; c_ < 5; c_++)
        {
            rst = points_(r_+r,c_+c) * Gmask_(r_,c_);
            sum += rst;
        }
        rst = 0.0;
    }
    return sum;
}

float Gaussian_calc_clear_2(const int r, const int c, const MatrixXd &points_, const MatrixXd &Gmask_)
{
    float sum;
    for(int r_ = 0; r_ < 5; ++r_)
    {
        for(int c_ = 0; c_ < 5; ++c_)
        {
            sum += (points_(r_+r,c_+c) * Gmask_(r_,c_));
        }
    }
    return sum;
}

MatrixXd cvmat_to_matxd_clear(const cv::Mat &clouds_)
{
    MatrixXd points(clouds_.rows,  clouds_.cols);
    for(int r=0; r<clouds_.rows; ++r)
    {
        for(int c=0; c<clouds_.cols; ++c)
        {
            points(r,c) = clouds_.at<float>(r,c);
        }
    }
    return points;
}

cv::Mat matxd_to_cvmat_clear(const MatrixXd &points_)
{
    cv::Mat Filtered_Point_cv(points_.rows(), points_.cols(), CV_32FC3);
    for(int r=0; r<points_.rows(); ++r)
    {
        for(int c=0; c<points_.cols(); ++c)
        {
            Filtered_Point_cv.at<float>(r,c) = points_(r,c);
        }
    }
    return Filtered_Point_cv;
}


cv::Mat FirstFilter::apply(const cv::Mat cloud)
{
    std::cout << ">> apply first filter\n";

    // Step1. cv::Mat cloud를 custom type으로 변환
    MatrixXd points(cloud.rows,  cloud.cols);
    for(int r=0; r<cloud.rows; r++)
    {
        for(int c=0; c<cloud.cols; c++)
        {
            points(r,c) = cloud.at<float>(r,c);
        }
    }
    
    // Step2. 필터 적용
    MatrixXd GMask = GaussianMask(5,3);
    MatrixXd Filtered_Point(cloud.rows,  cloud.cols);
    float sum,sub;
    for(int r=0; r<cloud.rows-5; r++)
    {
        for(int c=0; c<cloud.cols-5; c++)
        {
            sub = abs(Filtered_Point(r,c) - Filtered_Point(r+1,c+1));
            if (sub >= 5)
                continue;
            else
                Filtered_Point(r,c) = Gaussian_calc(r,c,points,GMask);
        }
    }

    // Step3. 1의 custom type을 다시 cv::Mat으로 변환하여 리턴
    cv::Mat Filtered_Point_cv(cloud.rows, cloud.cols, CV_32FC3);
    for(int r=0; r<cloud.rows; r++)
    {
        for(int c=0; c<cloud.cols; c++)
        {
            Filtered_Point_cv.at<float>(r,c) = Filtered_Point(r,c);
        }
    }
    return Filtered_Point_cv;
}


cv::Mat SecondFilter::apply(const cv::Mat cloud)
{
    std::cout << ">> apply second filter\n";

    MatrixXd points(cloud.rows,  cloud.cols);
    for(int r=0; r<cloud.rows; r++)
    {
        for(int c=0; c<cloud.cols; c++)
        {
            points(r,c) = cloud.at<float>(r,c);
        }
    }

    // Step2. 필터 적용
    MatrixXd GMask = GaussianMask(5,3);
    MatrixXd Filtered_Point(cloud.rows,  cloud.cols);
    float sub;
    for(int r=0; r<cloud.rows-5; r++)
    {
        for(int c=0; c<cloud.cols-5; c++)
        {
            sub = abs(Filtered_Point(r,c) - Filtered_Point(r+1,c+1));
            if (sub >= 5)
                continue;
            else
                Filtered_Point(r,c) = Gaussian_calc_clear_1(r,c,points,GMask);
        }
    }

    // Step3. 1의 custom type을 다시 cv::Mat으로 변환하여 리턴
    cv::Mat Filtered_Point_cv(cloud.rows, cloud.cols, CV_32FC3);
    for(int r=0; r<cloud.rows; r++)
    {
        for(int c=0; c<cloud.cols; c++)
        {
            Filtered_Point_cv.at<float>(r,c) = Filtered_Point(r,c);
        }
    }

    return Filtered_Point_cv;
}


cv::Mat ThirdFilter::apply(const cv::Mat cloud)
{
    std::cout << ">> apply third filter\n";

    // Step1. cv::Mat cloud를 custom type으로 변환
    MatrixXd points = cvmat_to_matxd_clear(cloud); 
    // smart pointer를 활용하여 최적화 시도하였으나, MatrixXd를 포인터로 선언하였을 때 데이터 접근을 하지 못하여,
    // smart pointer를 이용한 최적화 최종 구현하지 못하였습니다.

    // Step2. 필터 적용
    MatrixXd GMask = GaussianMask(5,3);
    MatrixXd Filtered_Point(cloud.rows,  cloud.cols);
    float sub;
    for(int r=0; r<cloud.rows-5; r++)
    {
        for(int c=0; c<cloud.cols-5; c++)
        {
            sub = abs(Filtered_Point(r,c) - Filtered_Point(r+1,c+1));
            if (sub >= 5)
                continue;
            else
                Filtered_Point(r,c) = Gaussian_calc_clear_2(r, c, points, GMask);
        }
    }

    // Step3. 1의 custom type을 다시 cv::Mat으로 변환하여 리턴
    cv::Mat Filtered_Point_cv = matxd_to_cvmat_clear(Filtered_Point);

    return Filtered_Point_cv;
}