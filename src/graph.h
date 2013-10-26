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
    int nodes;
    double *intencities;
    double **arcs;
    
public:
    Graph(int nodes);
    Graph(const Graph &graph);
    ~Graph();
    
    Graph & operator = (const Graph &graph);
    
    void test(void);
    void erase(void);
    void print(void);
    void bfs(void);
};

#endif /* defined(__graphcut__graph__) */
