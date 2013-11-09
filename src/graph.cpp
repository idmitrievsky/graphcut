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

double Graph::nodeIntencity(int i)
{
    return _intencities[i];
}

double Graph::getArcWeight(int i, int j)
{
    NEIGHBOURLIST::iterator neigh;
    int found = 0;
    
    for (neigh = _arcs[i].begin(); neigh != _arcs[i].end(); neigh++)
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

void Graph::setArcWeight(int i, int j, double weight)
{
    NEIGHBOURLIST::iterator neigh;
    int found = 0;
    Neighbour newNeigh = {j, weight};
    
    for (neigh = _arcs[i].begin(); neigh != _arcs[i].end(); neigh++)
    {
        if (neigh->nodeNumber == j)
        {
            found = 1;
            break;
        }
    }
    
    if (found)
    {
        neigh->arcWeight = weight;
    }
    else
    {
        _arcs[i].push_front(newNeigh);
    }
}

Graph::Graph(int nodesNumber)
{
    int i = 0;
    
    _nodes = nodesNumber;
    
    _arcs = new NEIGHBOURLIST [_nodes];
    _intencities = new double[_nodes];
    
    for (i = 0; i < _nodes; i++)
    {
        _intencities[i] = 0;
    }
}

Graph::Graph(const Graph&graph)
{
    _nodes = graph._nodes;
    
    _arcs = new NEIGHBOURLIST [_nodes];
    _intencities = new double[_nodes];
    
    for (int i = 0; i < _nodes; i++)
    {
        _arcs[i] = graph._arcs[i];
        _intencities[i] = graph._intencities[i];
    }
}

Graph & Graph::operator = (const Graph &graph)
{
    if (this != &graph)
    {
        _nodes = graph._nodes;
        
        for (int i = 0; i < _nodes; i++)
        {
            _arcs[i] = graph._arcs[i];
            _intencities[i] = graph._intencities[i];
        }
    }
    
    return *this;
}


Graph::~Graph(void)
{
    delete [] _arcs;
    delete _intencities;
}

void Graph::testFillUp(void)
{
    int i = 0, j = 0;
    
    setArcWeight(0, 1, 12);
    setArcWeight(0, 2, 15);
    setArcWeight(0, 3, 20);
    setArcWeight(1, 2, 5);
    setArcWeight(1, 5, 5);
    setArcWeight(1, 6, 2);
    setArcWeight(2, 3, 11);
    setArcWeight(2, 4, 3);
    setArcWeight(2, 6, 6);
    setArcWeight(3, 4, 4);
    setArcWeight(3, 7, 8);
    setArcWeight(4, 6, 6);
    setArcWeight(4, 7, 1);
    setArcWeight(5, 6, 9);
    setArcWeight(5, 8, 18);
    setArcWeight(6, 7, 7);
    setArcWeight(6, 8, 13);
    setArcWeight(7, 8, 10);
    
    for (i = 0; i < _nodes; i++)
    {
        for (j = 0; j < i + 1; j++)
        {
            if (getArcWeight(i, j))
            {
                setArcWeight(j, i, getArcWeight(i, j));;
            }
            if (getArcWeight(j, i))
            {
                setArcWeight(i, j, getArcWeight(j, i));;
            }
        }
    }
}

void Graph::erase(void)
{
    int i = 0;
    
    for (i = 0; i < _nodes; i++)
    {
        _arcs[i].clear();
        _intencities[i] = 0;
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

void Graph::removeArc(int i, int j)
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
        _arcs[i].pop_front();
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
        _arcs[i].erase_after(prev);
    }
}

void Graph::bfs(void)
{
    std::vector<int> visitedNodes(_nodes, 0);
    std::queue<int> toVisit;
    int currentNode = 0;
    NEIGHBOURLIST::iterator neigh;
    
    /* Start from the first node */
    visitedNodes[0] = 1;
    toVisit.push(0);
    
    while (!toVisit.empty())
    {
        currentNode = toVisit.front();
        toVisit.pop();
        
        for (neigh = _arcs[currentNode].begin(); neigh != _arcs[currentNode].end(); neigh++)
        {
            /* If <i> is reacheable from <currentNode> */
            if (!visitedNodes[neigh->nodeNumber])
            {
                visitedNodes[neigh->nodeNumber] = 1;
                toVisit.push(neigh->nodeNumber);
            }
        }
    }
}

