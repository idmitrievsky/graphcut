//
//  imagenetwork.h
//  graphcut
//
//  Created by Ivan Dmitrievsky on 15.11.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __graphcut__imagenetwork__
#define __graphcut__imagenetwork__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "network.h"
#include "partition.h"

class ImageNetwork : Network
{
    cv::Mat *image;
    cv::Mat *grad;
    Partition *partition;
    double *_intensities;
    
public:
    ImageNetwork(const char *imagePath);
    ~ImageNetwork(void);
    
    double nodeIntensity(int i);
    void setNodeIntensity(int i, double intensity);
    
    int pixelGraphIndex(int x, int y);
    int pixelPartitionIndex(int x, int y);
    int nodeX(graphIndex index);
    int nodeY(graphIndex index);
    
    void pixelNeighbours(graphIndex p, std::vector<graphIndex> &neighbours, bool eight = false);
    int repart(void);
    double boundaryTerm(graphIndex p, Label pLabel, graphIndex q, Label qLabel);
    double localTerm(graphIndex p, Label pLabel);
    double energy(Partition *p);
};

#endif /* defined(__graphcut__imagenetwork__) */
