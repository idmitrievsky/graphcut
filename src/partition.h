//
//  partition.h
//  graphcut
//
//  Created by Ivan Dmitrievsky on 02.11.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __graphcut__partition__
#define __graphcut__partition__

#include "network.h"

class Partition
{
    int nodesNum;
    int *labels;
    
public:
    Partition(int nodes);
    ~Partition(void);
    
    /* Operators */
    Partition & operator = (const Partition &partition);
    
    int label(int node);
    void setLabel(int node, int label);
    int mutate(Network &image, int cols, Partition &mutated);
    double boundary(int p, int q);
    double local(int p, double intencity);
    double energy(Network &img, int cols);
};

#endif /* defined(__graphcut__partition__) */
