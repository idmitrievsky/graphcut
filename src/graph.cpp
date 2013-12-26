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

int Graph::nodes(void)
{
    return _nodes;
}

double Graph::getArcWeight(graphIndex i, graphIndex j, int accurate)
{
    NEIGHBOURLIST::iterator neigh;
    int found = 0;
    
    if (accurate != -1)
    {
        return _arcs[i][accurate].arcWeight;
    }
    
    for (neigh = _arcs[i].begin(); neigh != _arcs[i].end() && neigh->nodeNumber != -1; neigh++)
    {
        if (neigh->nodeNumber == j)
        {
            found = 1;
            break;
        }
    }
    
    if (found)
    {
        return neigh->arcWeight;
    }
    
    return 0;
}

void Graph::setArcWeight(graphIndex i, graphIndex j, double weight, int accurate)
{
    NEIGHBOURLIST::iterator neigh;
    int found = 0;
    Neighbour newNeigh = {j, weight};
    
    if (accurate != -1)
    {
        _arcs[i][accurate] = newNeigh;
        return;
    }
    
    for (neigh = _arcs[i].begin(); neigh != _arcs[i].end(); neigh++)
    {
        if (neigh->nodeNumber == j || neigh->nodeNumber == -1)
        {
            found = 1;
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

Graph::Graph(int nodesNumber, int neighs):_nodes(nodesNumber), _arcs(nodesNumber, NEIGHBOURLIST(neighs, {-1,-1}))
{

}

int Graph::empty(void)
{
    return !!_nodes;
}

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
        
        for (int i = 0; i < _nodes; i++)
        {
            _arcs[i] = graph._arcs[i];
        }
    }
    
    return *this;
}

void Graph::erase(void)
{
    int i = 0;
    
    for (i = 0; i < _nodes; i++)
    {
        _arcs[i].clear();
    }
}

void Graph::print(void)
{
    int i = 0;
    NEIGHBOURLIST::iterator neigh;
    
    for (i = 0; i < _nodes; i++)
    {
        for (neigh = _arcs[i].begin(); neigh != _arcs[i].end(); neigh++)
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
    int found = 0;

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
    for (neigh++; neigh != _arcs[i].end(); neigh++)
    {
        if (neigh->nodeNumber == j)
        {
            found = 1;
            break;
        }
        prev = neigh;
    }
    
    if (found)
    {
        _arcs[i].erase(neigh);
    }
}
