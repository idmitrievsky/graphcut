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

Graph::Graph(int _nodes)
{
    nodes = _nodes;
    
    arcs = new double*[nodes];
    
    for (int i = 0; i < nodes; i++)
    {
        arcs[i] = new double[nodes];
        for (int j = 0; j < nodes; j++)
        {
            arcs[i][j] = 0;
        }
    }
    print();
}

Graph::Graph(const Graph&graph)
{
    nodes = graph.nodes;
    
    arcs = new double*[nodes];
    
    for (int i = 0; i < nodes; i++)
    {
        arcs[i] = new double[nodes];
        for (int j = 0; j < nodes; j++)
        {
            arcs[i][j] = graph.arcs[i][j];
        }
    }
}

Graph & Graph::operator = (const Graph &graph)
{
    if (this != &graph)
    {
        nodes = graph.nodes;
        
        arcs = new double*[nodes];
        
        for (int i = 0; i < nodes; i++)
        {
            arcs[i] = new double[nodes];
            for (int j = 0; j < nodes; j++)
            {
                arcs[i][j] = graph.arcs[i][j];
            }
        }
    }
    
    return *this;
}


Graph::~Graph(void)
{
    for (int i = 0; i < nodes; i++)
    {
        delete [] arcs[i];
    }
    
    delete [] arcs;
}

void Graph::test(void)
{
    int i = 0, j = 0;
    
    arcs[0][1] = 12;
    arcs[0][2] = 15;
    arcs[0][3] = 20;
    arcs[1][2] = 5;
    arcs[1][5] = 5;
    arcs[1][6] = 2;
    arcs[2][3] = 11;
    arcs[2][4] = 3;
    arcs[2][6] = 6;
    arcs[3][4] = 4;
    arcs[3][7] = 8;
    arcs[4][6] = 6;
    arcs[4][7] = 1;
    arcs[5][6] = 9;
    arcs[5][8] = 18;
    arcs[6][7] = 7;
    arcs[6][8] = 13;
    arcs[7][8] = 10;
    
    for (i = 0; i < nodes; i++)
    {
        for (j = 0; j < i + 1; j++)
        {
            if (arcs[i][j])
            {
                arcs[j][i] = arcs[i][j];
            }
            if (arcs[j][i])
            {
                arcs[i][j] = arcs[j][i];
            }
        }
    }
}

void Graph::erase(void)
{
    int i, j;
    
    for (i = 0; i < nodes; i++)
    {
        for (j = 0; j < nodes; j++)
        {
            arcs[i][j] = 0;
        }
    }
}

void Graph::print(void)
{
    int i, j;
    
    for (i = 0; i < nodes; i++)
    {
        for (j = 0; j < nodes; j++)
        {
            std::cout << arcs[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}


void Graph::bfs(void)
{
    std::vector<int> visitedNodes(nodes, 0);
    std::queue<int> toVisit;
    int currentNode = 0,
        neighbour   = 0;
    
    visitedNodes[0] = 1;
    
    toVisit.push(0);
    
    while (!toVisit.empty())
    {
        currentNode = toVisit.front();
        toVisit.pop();
        
        for (int i = 0; i < nodes; i++)
        {
            if (arcs[currentNode][i] <= 0)
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

