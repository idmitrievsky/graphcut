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

/* Partition index i matches graph index i + 1 */
typedef int partitionIndex;

enum Label
{
    BACKGROUND,
    FOREGROUND
};

class Partition
{
public:
    /* Getters and setters */
    Label label(partitionIndex node);
    void setLabel(partitionIndex node, Label label);
    
    /* Constructors and destructors */
    Partition(int nodes);
    Partition(const Partition &partition);
    ~Partition(void);
    
    /* Operators */
    Partition & operator = (const Partition &partition);
    Label operator [](partitionIndex i) const;
private:
    int nodesNum;
    Label *labels;
};

#endif /* defined(__graphcut__partition__) */
