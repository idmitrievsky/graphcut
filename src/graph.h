//
//  graph.h
//  graphcut
//
//  Created by Ivan Dmitrievsky on 20.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __graphcut__graph__
#define __graphcut__graph__

class Graph
{
protected:
    int _nodes;
    double *intencities;
    double **_arcs;
    
public:
    /* Getters */
    int nodes(void);
    double arcs(int i, int j);
    
    /* Constructors and destructors */
    Graph(int _nodes);
    Graph(const Graph &graph);
    ~Graph();
    
    /* Operators */
    Graph & operator = (const Graph &graph);
    
    /* Methods */
    void testFillUp(void);
    void erase(void);
    void print(void);
    void bfs(void);
};

#endif /* defined(__graphcut__graph__) */
