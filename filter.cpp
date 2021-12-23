#include "filter.h"
using namespace cv;
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

#include "matrix.h"

cv::Mat FirstFilter::apply(const cv::Mat cloud)
{
    // Step1. cv::Mat cloud를 custom type으로 변환
    //      e.g. std::vector<Matrix<float>> points;
    // Step2. 필터 적용
    // Step3. 1의 custom type을 다시 cv::Mat으로 변환하여 리턴
    std::cout << "[Test]apply first filter\n";
    std::vector<Matrix<float>> points[cloud.rows][cloud.cols];
    cv::Mat average_img;
    std::cout << "[Here is filter.cpp] shape=" << cloud.rows << " x " << cloud.cols ;
    cv::Mat avg_kernel = Mat::ones(5, 5, CV_32F) / 25;
    filter2D(cloud, average_img, -1, avg_kernel, Point(-1, -1), (0,0), 16);
    
    cv::Mat img_gaussian;
    cv::GaussianBlur(cloud, img_gaussian, Size(5,5), 1);
    //imshow("img average test", cloud);
    //waitKey();

    int mask[5][5] = {
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1}
    };
    float pix, sum;
    
    
    for (int y = 0; y < (cloud.rows-1); ++y)
    {   
        
        for (int x = 0; x < (cloud.cols -1); ++x)
        {
            sum =0;
            for (int m = 0; m < 5; ++m)
            {
                for (int n = 0; n< 5; ++n)
                {
                    sum+= (cloud.ptr<uchar>(y-1+m)[x-1+n] * mask[m][n]);
                }
            }
            //points[y][x] = sum / 25;
        }
    }

    return cloud;
}

