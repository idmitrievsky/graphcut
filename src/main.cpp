//
//  main.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 18.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "network.h"
#include "partition.h"

#define NODESNUM 9

int main(int argc, const char * argv[])
{
    int pixels = 0;
    int i = 0, j = 0;
    double temp;
    
    Network *associatedNetwork;
    
    Partition *currentPartition, *mutated;
    
    cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    
    
    pixels = image.rows * image.cols;
    
    currentPartition = new Partition(pixels);
    mutated = new Partition(pixels);
    
    associatedNetwork = new Network(pixels);
    for (i = 0; i < image.rows; i++)
    {
        for (j = 0; j < image.cols; j++)
        {
            temp = image.at<uchar>(i, j);
            associatedNetwork->setNodeIntencity(i * image.cols + j, image.at<uchar>(i, j));
        }
    }
    
    while (currentPartition->mutate(*associatedNetwork, image.cols, *mutated))
    {
        currentPartition = mutated;
    }
    
    return 0;
}

