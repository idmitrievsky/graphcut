//
//  partition.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 02.11.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <math.h>
#include "partition.h"

Label Partition::label(partitionIndex node)
{
    return labels[node];
}

void Partition::setLabel(partitionIndex node, Label label)
{
    labels[node] = label;
}

Partition::Partition(int nodes)
{
    int i = 0;
    labels = new Label[nodes];
    
    for (i = 0; i < nodes; i++)
    {
        labels[i] = BACKGROUND;
    }
    nodesNum = nodes;
}

Partition::Partition(const Partition &partition)
{
    int i = 0;
    
    nodesNum = partition.nodesNum;
    labels = new Label[nodesNum];
    
    for (i = 0; i < nodesNum; i++)
    {
        labels[i] = partition.labels[i];
    }
}

Partition::~Partition(void)
{
    delete [] labels;
}

Partition & Partition::operator = (const Partition &partition)
{
    if (this != &partition)
    {
        nodesNum = partition.nodesNum;
        
        for (int i = 0; i < nodesNum; i++)
        {
            labels[i] = partition.labels[i];
        }
    }
    
    return *this;
}

Label Partition::operator [](partitionIndex i) const
{
    return labels[i];
}