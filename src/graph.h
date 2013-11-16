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

#define NEIGHBOURLIST std::forward_list<Neighbour>

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
    NEIGHBOURLIST *_arcs;
    
public:
    /* Getters */
    int nodes(void);
    double getArcWeight(graphIndex i, graphIndex j);
    void setArcWeight(graphIndex i, graphIndex j, double weight);
    void removeArc(graphIndex i, graphIndex j);
    
    /* Constructors and destructors */
    Graph(void);
    Graph(int _nodes);
    Graph(const Graph &graph);
    ~Graph();
    
    /* Operators */
    Graph & operator = (const Graph &graph);
    
    /* Methods */
    int isEmpty(void);
    void init(int nodesNumber);
    void testFillUp(void);
    void erase(void);
    void print(void);
    void bfs(void);
};

#endif /* defined(__graphcut__graph__) */
