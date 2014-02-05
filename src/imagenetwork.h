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
public:
    /* Constructors and destructors */
    ImageNetwork(const char *imagePath);
    ~ImageNetwork(void);
    
    /* Getters and setters */
    double nodeIntensity(int i);
    void setNodeIntensity(int i, double intensity);
    
    graphIndex pixelGraphIndex(int x, int y);
    partitionIndex pixelPartitionIndex(int x, int y);
    int nodeX(graphIndex index);
    int nodeY(graphIndex index);
    
    void pixelNeighbours(graphIndex p, std::vector<graphIndex> &neighbours, bool eight = false);
    bool repart(void);
    double boundaryTerm(graphIndex p, Label pLabel, graphIndex q, Label qLabel);
    double localTerm(graphIndex p, Label pLabel, Partition *part);
    double energy(Partition *p);
    
    void outputPartition(const char *outPath);
    void outputPartition(cv::Mat *image);
private:
    bool allocatedImage;
    cv::Mat *image;
    cv::Mat *grad;
    Partition *partition;
    double *_intensities;
};

#endif /* defined(__graphcut__imagenetwork__) */
