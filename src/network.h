//
//  network.h
//  graphcut
//
//  Created by Ivan Dmitrievsky on 25.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __graphcut__network__
#define __graphcut__network__

#include <vector>
#include "graph.h"

class Network : public Graph
{
    int source, sink;
    
public:
    Network(int _nodes, int _source, int _sink);
    double edmondskarp(Network *residualNetwork);
    std::vector<int> shortestAugmentingPath(void);
};

#endif /* defined(__graphcut__network__) */
