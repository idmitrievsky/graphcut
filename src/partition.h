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

enum Label
{
    BACKGROUND,
    FOREGROUND
};

typedef int partitionIndex;

class Partition
{
    int nodesNum;
    Label *labels;
    
public:
    Partition(int nodes);
    Partition(const Partition &partition);
    ~Partition(void);
    
    /* Operators */
    Partition & operator = (const Partition &partition);
    Label operator [](partitionIndex i) const;
    
    Label label(partitionIndex node);
    void setLabel(partitionIndex node, Label label);
};

#endif /* defined(__graphcut__partition__) */
