//
//  network.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 25.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <queue>
#include <iostream>
#include "network.h"

double Network::edmondskarp(Network *minimumCut)
{
    double maxFlow = 0, capacity = 0, minCapacity = 0;
    int i = 0, j = 0;
    Network residualNetwork = *this, flowNetwork(nodes, source, sink);
    std::vector<int> path;
    std::vector<int>::iterator it;
    
    for (i = 0; i < nodes; i++)
    {
        for (j = 0; j < nodes; j++)
        {
            flowNetwork.arcs[i][j] = 0;
        }
    }
    
    while (!(path = residualNetwork.shortestAugmentingPath()).empty())
    {
        minCapacity = residualNetwork.arcs[*path.begin()][*(path.begin() + 1)];
        for (it = path.begin() + 1; it != path.end() - 1; it++)
        {
            if (residualNetwork.arcs[*it][*(it + 1)] < minCapacity)
            {
                minCapacity = residualNetwork.arcs[*it][*(it + 1)];
            }
        }
        for (it = path.begin(); it != path.end() - 1; it++)
        {
            flowNetwork.arcs[*it][*(it + 1)] +=  minCapacity;
            flowNetwork.arcs[*(it + 1)][*it]  = -1 * flowNetwork.arcs[*it][*(it + 1)];
        }
        for (i = 0; i < nodes; i++)
        {
            for (j = 0; j < nodes; j++)
            {
                capacity = arcs[i][j] - flowNetwork.arcs[i][j];
                residualNetwork.arcs[i][j] = capacity > 0 ? capacity : 0;
            }
        }
    }
    
    for (i = 0; i < nodes; i++)
    {
        maxFlow += flowNetwork.arcs[source][i];
        
        for (j = 0; j < nodes; j++)
        {
            if (arcs[i][j] && flowNetwork.arcs[i][j] == arcs[i][j])
            {
                minimumCut->arcs[i][j] = 1;
            }
        }
    }

    return maxFlow;
}

Network::Network(int _nodes, int _source, int _sink):Graph(_nodes)
{
    source = _source;
    sink = _sink;
}

std::vector<int> Network::shortestAugmentingPath(void)
{
    std::queue<int> toVisit;
    std::vector<int> visitedNodes(nodes, 0), ancestors(nodes, 0);
    std::vector<int> reversedPath, path;
    int currentNode = 0,
        neighbour;
    int i = 0;
    
    toVisit.push(source);
    
    visitedNodes[source] = 1;
    ancestors[source] = -1;
    
    while (!toVisit.empty())
    {
        currentNode = toVisit.front();
        toVisit.pop();
        
        for (i = 0; i < nodes; i++)
        {
            if (arcs[currentNode][i] <= 0)
            {
                continue;
            }
            neighbour = i;
            if (!visitedNodes[neighbour])
            {
                visitedNodes[neighbour] = 1;
                ancestors[neighbour] = currentNode;
                
                toVisit.push(neighbour);
                if (neighbour == sink)
                {
                    goto found;
                }
            }
        }
    }
    
    if (toVisit.empty())
    {
        return reversedPath;
    }
    
found:
    currentNode = sink;
    while (currentNode != -1)
    {
        reversedPath.push_back(currentNode);
        currentNode = ancestors[currentNode];
    }
    std::for_each(reversedPath.rbegin(), reversedPath.rend(), [&path](int val) { path.push_back(val); });

    return path;
}