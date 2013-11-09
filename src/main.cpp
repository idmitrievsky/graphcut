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
    
    Network *associatedNetwork;
    
    Partition *currentPartition, *mutated;
    
    cv::Mat image = cv::imread(argv[1], 0);
    
    pixels = image.rows * image.cols;
    
    currentPartition = new Partition(pixels);
    mutated = new Partition(pixels);
    
    associatedNetwork = new Network(pixels);
    
    while (currentPartition->mutate(*associatedNetwork, image.cols, *mutated))
    {
        currentPartition = mutated;
    }
    
    return 0;
}

