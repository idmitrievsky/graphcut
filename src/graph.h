//
//  graph.h
//  graphcut
//
//  Created by Ivan Dmitrievsky on 20.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __graphcut__graph__
#define __graphcut__graph__

#include <forward_list>

#define NEIGHBOURLIST std::vector<Neighbour>

typedef int graphIndex;

struct Neighbour
{
    int nodeNumber;
    double arcWeight;
};

class Graph
{
protected:
    int _nodes;
    
    
public:
    std::vector<NEIGHBOURLIST> _arcs;
    /* Getters */
    int nodes(void);
    double getArcWeight(graphIndex i, graphIndex j, int accurate = -1);
    void setArcWeight(graphIndex i, graphIndex j, double weight, int accurate = -1);
    void removeArc(graphIndex i, graphIndex j);
    
    /* Constructors and destructors */
    Graph(int _nodes = 0, int neighs = 0);
    Graph(const Graph &graph);
    
    /* Operators */
    Graph & operator = (const Graph &graph);
    
    /* Methods */
    int empty(void);
    void init(int nodesNumber, int neighs = 0);
    void testFillUp(void);
    void erase(void);
    void print(void);
};

#endif /* defined(__graphcut__graph__) */
