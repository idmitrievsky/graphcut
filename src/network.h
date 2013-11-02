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
    Network(int nodesNum, int _source, int _sink);
    
    /* Make a valid network from undirected graph*/
    void obduct(Graph &graph, int src, int snk);
    
    /* Find the shortest augmenting path from source to sink */
    std::vector<int> shortestAugmentingPath(void);
    
    /* Find the maximum flow from source to sink and write minimum cut edges to <minCut> */
    double edmondskarp(Network &minCut);
    
    /* Find all blocking saturated edges in residual network */
    void minimumCut(Network &minCutEdges);
};

#endif /* defined(__graphcut__network__) */
