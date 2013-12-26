//
//  graph.h
//  graphcut
//
//  Created by Ivan Dmitrievsky on 20.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __graphcut__graph__
#define __graphcut__graph__

/* Graph index i matches partition index i - 1 */
typedef int graphIndex;

struct Neighbour
{
    int nodeNumber;
    double arcWeight;
};

typedef std::vector<Neighbour> NEIGHBOURLIST;

/* Each node number A have a neighbourlist NL associated with it */
/* If an instance of Neighbour N occures in NL, then graph contains an arc from A to N.nodeNumber */
/* Its weight is N.arcWeight */

/* If list contains an instance with nodeNumber equal to -1 it indicates the end of this list */

class Graph
{
public:
    /* Neighbour lists associated with each node number */
    std::vector<NEIGHBOURLIST> _arcs;
    
    /* Getters and setters */
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
    void erase(void);
    void print(void);
protected:
    /* Number of nodes in a graph */
    int _nodes;
};

#endif /* defined(__graphcut__graph__) */
