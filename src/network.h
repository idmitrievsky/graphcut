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
public:
    /* Getters */
    int source(void);
    int sink(void);
    
    /* Constructors and destructors */
    Network(int nodesNum, int _source, int _sink, int neighs = 0);
    Network(void);
    
    /* Make a valid network from undirected graph*/
    void obduct(Graph &graph, int src, int snk);
    
    /* Find the shortest augmenting path from source to sink */
    bool shortestAugmentingPath(std::vector<int> &ancestors, std::vector<bool> &visitedNodes);
    
    /* Find the maximum flow from source to sink and write minimum cut edges to <minCut> */
    void edmondskarp(Network &minCut, double *maxFlow = NULL);
    
    int debug(void);
    
protected:
    int _source, _sink;
};

#endif /* defined(__graphcut__network__) */
