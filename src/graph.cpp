//
//  graph.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 20.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <vector>
#include <queue>
#include <iostream>
#include "graph.h"

/* Returns number of nodes in a graph */
int Graph::nodes(void)
{
    return _nodes;
}

/* If the exact position of arc is known an <accurate> index can be specified */
/* It allows a much faster lookup */
double Graph::getArcWeight(graphIndex i, graphIndex j, int accurate)
{
    NEIGHBOURLIST::iterator neigh;
    bool found = false;
    
    if (accurate != -1)
    {
        return _arcs[i][accurate].arcWeight;
    }
    
    for (neigh = _arcs[i].begin(); neigh != _arcs[i].end() && neigh->nodeNumber != -1; ++neigh)
    {
        if (neigh->nodeNumber == j)
        {
            found = true;
            break;
        }
    }
    
    if (found)
    {
        return neigh->arcWeight;
    }
    
    return 0;
}

/* If the exact position of arc is known an <accurate> index can be specified */
/* It allows a much faster insertion */
void Graph::setArcWeight(graphIndex i, graphIndex j, double weight, int accurate)
{
    NEIGHBOURLIST::iterator neigh;
    bool found = false;
    Neighbour newNeigh = {j, weight};
    
    if (accurate != -1)
    {
        _arcs[i][accurate] = newNeigh;
        return;
    }
    
    for (neigh = _arcs[i].begin(); neigh != _arcs[i].end(); ++neigh)
    {
        if (neigh->nodeNumber == j || neigh->nodeNumber == -1)
        {
            found = true;
            break;
        }
    }
    
    if (found)
    {
        neigh->nodeNumber = j;
        neigh->arcWeight = weight;
    }
    else
    {
        _arcs[i].push_back(newNeigh);
    }
}

/* <neighs> is an approximate number of neighbours for each node */
/* Specifying this allows fewer memory allocations */
Graph::Graph(int nodesNumber, int neighs):_nodes(nodesNumber), _arcs(nodesNumber, NEIGHBOURLIST(neighs, {-1,-1}))
{
}

/* Graph is empty if nodes number is equal to zero */
int Graph::empty(void)
{
    return !!_nodes;
}

/* Allows to initialize graph with new parameters */
void Graph::init(int nodesNumber, int neighs)
{
    _nodes = nodesNumber;
    
    _arcs.assign(nodesNumber, NEIGHBOURLIST(neighs, {-1, -1}));
}

Graph::Graph(const Graph&graph)
{
    _nodes = graph._nodes;
    _arcs = graph._arcs;
}

Graph & Graph::operator = (const Graph &graph)
{
    if (this != &graph)
    {
        _nodes = graph._nodes;
        
        for (int i = 0; i < _nodes; ++i)
        {
            _arcs[i] = graph._arcs[i];
        }
    }
    
    return *this;
}

/* Isolates each node in a graph */
void Graph::erase(void)
{
    for (int i = 0; i < _nodes; i++)
    {
        _arcs[i].clear();
    }
}

void Graph::print(void)
{
    NEIGHBOURLIST::iterator neigh;
    
    for (int i = 0; i < _nodes; i++)
    {
        for (neigh = _arcs[i].begin(); neigh != _arcs[i].end(); ++neigh)
        {
            std::cout << '(' << i << ", " << neigh->nodeNumber << ", " << neigh->arcWeight << ") ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void Graph::removeArc(graphIndex i, graphIndex j)
{
    NEIGHBOURLIST::iterator neigh, prev;
    bool found = false;

    if (_arcs[i].empty())
    {
        return;
    }
    
    neigh = _arcs[i].begin();

    if (neigh->nodeNumber == j)
    {
        _arcs[i].erase(neigh);
        return;
    }
    
    prev = neigh;
    for (++neigh; neigh != _arcs[i].end(); ++neigh)
    {
        if (neigh->nodeNumber == j)
        {
            found = true;
            break;
        }
        prev = neigh;
    }
    
    if (found)
    {
        _arcs[i].erase(neigh);
    }
}
