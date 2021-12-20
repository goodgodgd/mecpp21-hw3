#include "filter.h"

#include <algorithm>
#include <iostream>

std::vector<Matrix<float>> MedianFilter::filter(std::vector<Matrix<float>> points_, int rows, int cols)
{
    for(int y = 0; y < rows; ++y)
    {
        for(int x = 0; x < cols; ++x)
        {
            int centor_point = (cols+4)*(y+2)+(x+2);
            std::vector<float> kernel_x,kernel_y,kernel_z;
            
            float center_x,center_y,center_z;
            for(int i = -2; i <= 2; ++i)
            {
                for(int j = -2; j <= 2; ++j)
                {
                    kernel_x.push_back(points_[(cols+4)*((y+2)+i)+((x+2)+j)].at(0,0));
                    kernel_y.push_back(points_[(cols+4)*((y+2)+i)+((x+2)+j)].at(0,1));
                    kernel_z.push_back(points_[(cols+4)*((y+2)+i)+((x+2)+j)].at(0,2));
                }
            }
            sort(kernel_x.begin(),kernel_x.end());
            sort(kernel_y.begin(),kernel_y.end());
            sort(kernel_z.begin(),kernel_z.end());
            int size = (kernel_x.size() /2) + 1 ;
            center_x = kernel_x.at(size);
            center_y = kernel_y.at(size);
            center_z = kernel_z.at(size);
            Matrix<float> centor({center_x,center_y,center_z},1,3);
            points_[centor_point] = centor;
        }
    }
    return points_;
}


