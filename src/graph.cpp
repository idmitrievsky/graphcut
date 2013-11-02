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

double Graph::arcs(int i, int j)
{
    return _arcs[i][j];
}

Graph::Graph(int nodesNum)
{
    _nodes = nodesNum;
    
    _arcs = new double*[_nodes];
    
    for (int i = 0; i < _nodes; i++)
    {
        _arcs[i] = new double[_nodes];
        for (int j = 0; j < _nodes; j++)
        {
            _arcs[i][j] = 0;
        }
    }
    print();
}

Graph::Graph(const Graph&graph)
{
    _nodes = graph._nodes;
    
    _arcs = new double*[_nodes];
    
    for (int i = 0; i < _nodes; i++)
    {
        _arcs[i] = new double[_nodes];
        for (int j = 0; j < _nodes; j++)
        {
            _arcs[i][j] = graph._arcs[i][j];
        }
    }
}

Graph & Graph::operator = (const Graph &graph)
{
    if (this != &graph)
    {
        _nodes = graph._nodes;
        
        _arcs = new double*[_nodes];
        
        for (int i = 0; i < _nodes; i++)
        {
            _arcs[i] = new double[_nodes];
            for (int j = 0; j < _nodes; j++)
            {
                _arcs[i][j] = graph._arcs[i][j];
            }
        }
    }
    
    return *this;
}


Graph::~Graph(void)
{
    for (int i = 0; i < _nodes; i++)
    {
        delete [] _arcs[i];
    }
    
    delete [] _arcs;
}

void Graph::testFillUp(void)
{
    int i = 0, j = 0;
    
    _arcs[0][1] = 12;
    _arcs[0][2] = 15;
    _arcs[0][3] = 20;
    _arcs[1][2] = 5;
    _arcs[1][5] = 5;
    _arcs[1][6] = 2;
    _arcs[2][3] = 11;
    _arcs[2][4] = 3;
    _arcs[2][6] = 6;
    _arcs[3][4] = 4;
    _arcs[3][7] = 8;
    _arcs[4][6] = 6;
    _arcs[4][7] = 1;
    _arcs[5][6] = 9;
    _arcs[5][8] = 18;
    _arcs[6][7] = 7;
    _arcs[6][8] = 13;
    _arcs[7][8] = 10;
    
    for (i = 0; i < _nodes; i++)
    {
        for (j = 0; j < i + 1; j++)
        {
            if (_arcs[i][j])
            {
                _arcs[j][i] = _arcs[i][j];
            }
            if (_arcs[j][i])
            {
                _arcs[i][j] = _arcs[j][i];
            }
        }
    }
}

void Graph::erase(void)
{
    int i, j;
    
    for (i = 0; i < _nodes; i++)
    {
        for (j = 0; j < _nodes; j++)
        {
            _arcs[i][j] = 0;
        }
    }
}

void Graph::print(void)
{
    int i, j;
    
    for (i = 0; i < _nodes; i++)
    {
        for (j = 0; j < _nodes; j++)
        {
            std::cout << _arcs[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}


void Graph::bfs(void)
{
    std::vector<int> visitedNodes(_nodes, 0);
    std::queue<int> toVisit;
    int currentNode = 0,
        neighbour   = 0;
    
    /* Start from the first node */
    visitedNodes[0] = 1;
    toVisit.push(0);
    
    while (!toVisit.empty())
    {
        currentNode = toVisit.front();
        toVisit.pop();
        
        for (int i = 0; i < _nodes; i++)
        {
            /* If <i> is unreacheable from <currentNode> */
            if (_arcs[currentNode][i] <= 0)
            {
                continue;
            }
            neighbour = i;
            if (!visitedNodes[neighbour])
            {
                visitedNodes[neighbour] = 1;
                toVisit.push(neighbour);
            }
        }
    }
}

