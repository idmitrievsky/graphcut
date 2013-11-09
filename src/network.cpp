//
//  network.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 25.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <queue>
#include <stack>
#include <iostream>
#include "network.h"

double Network::edmondskarp(Network &minimumCut)
{
    double maxFlow = 0, capacity = 0, minCapacity = 0, newCapacity = 0;
    int i = 0, j = 0;
    Network residualNetwork = *this, flowNetwork(_nodes, _source, _sink);
    std::vector<int> path;
    std::vector<int>::iterator it;
    
    while (!(path = residualNetwork.shortestAugmentingPath()).empty())
    {
        /* Start with initial minimum value */
        minCapacity = residualNetwork.getArcWeight(*path.begin(), *(path.begin() + 1));
        /* Skip initial value */
        for (it = path.begin() + 1; it != path.end() - 1; it++)
        {
            if (residualNetwork.getArcWeight(*it, *(it + 1)) < minCapacity)
            {
                minCapacity = residualNetwork.getArcWeight(*it, *(it + 1));
            }
        }
        /* Increase flow as much as possible */
        for (it = path.begin(); it != path.end() - 1; it++)
        {
            newCapacity = flowNetwork.getArcWeight(*it, *(it + 1)) + minCapacity;
            flowNetwork.setArcWeight(*it, *(it + 1), newCapacity);
            flowNetwork.setArcWeight(*(it + 1), *it, -1 * newCapacity);
        }
        /* Decrease capacity in residual network  */
        for (i = 0; i < _nodes; i++)
        {
            for (j = 0; j < _nodes; j++)
            {
                capacity = getArcWeight(i, j) - flowNetwork.getArcWeight(i, j);
                if (capacity > 0)
                {
                    residualNetwork.setArcWeight(i, j, capacity);
                }
                else
                {
                    residualNetwork.removeArc(i, j);
                }
            }
        }
    }
    
    /* Calculate all the flow coming from source */
    for (i = 0; i < _nodes; i++)
    {
        maxFlow += flowNetwork.getArcWeight(_source, i);
    }

    /* Find blocking saturated edges or disconnected pairs of nodes */
    residualNetwork.minimumCut(minimumCut);
    
    /* Filter out disconnected pairs of nodes */
    for (i = 0; i < _nodes; i++)
    {
        for (j = 0; j < _nodes; j++)
        {
            if (getArcWeight(i, j) == 0)
            {
                minimumCut.removeArc(i, j);
            }
        }
    }
    
    return maxFlow;
}

int Network::source(void)
{
    return _source;
}

int Network::sink(void)
{
    return _sink;
}

Network::Network(int nodesNum, int source, int sink):Graph(nodesNum)
{
    _source = source;
    _sink = sink;
}

Network::Network(int pixels):Graph(pixels + 2)
{
    _source = 0;
    _sink = pixels + 1;
}

std::vector<int> Network::shortestAugmentingPath(void)
{
    std::queue<int> toVisit;
    std::vector<int> visitedNodes(_nodes, 0), ancestors(_nodes, 0);
    std::vector<int> reversedPath, path;
    int currentNode = 0;
    NEIGHBOURLIST::iterator neigh;
    
    /* Start from the source */
    toVisit.push(_source);
    visitedNodes[_source] = 1;
    
    /* Source don't have any ancestors */
    ancestors[_source] = -1;
    
    while (!toVisit.empty())
    {
        currentNode = toVisit.front();
        toVisit.pop();
        
        for (neigh = _arcs[currentNode].begin(); neigh != _arcs[currentNode].end(); neigh++)
        {
            /* If <i> is reachable from <currentNode> */
            if (!visitedNodes[neigh->nodeNumber])
            {
                visitedNodes[neigh->nodeNumber] = 1;
                /* Write ancestor for backwards path */
                ancestors[neigh->nodeNumber] = currentNode;
                
                toVisit.push(neigh->nodeNumber);
                if (neigh->nodeNumber == _sink)
                {
                    goto found;
                }
            }
        }
    }
    
    /* Never reached sink, it's blocked */
    if (toVisit.empty())
    {
        return reversedPath;
    }
    
found:
    /* Start from the sink */
    currentNode = _sink;
    /* Make up a path */
    while (currentNode != -1)
    {
        reversedPath.push_back(currentNode);
        currentNode = ancestors[currentNode];
    }
    /* Reverse path */
    std::for_each(reversedPath.rbegin(), reversedPath.rend(), [&path](int val) { path.push_back(val); });

    return path;
}

void Network::obduct(Graph &graph, int src, int snk)
{
    int i = 0, j = 0;
    
    for (i = 0; i < graph.nodes(); i++)
    {
        for (j = 0; j < graph.nodes(); j++)
        {
            if (graph.getArcWeight(i, j))
            {
                setArcWeight(i, j, graph.getArcWeight(i, j));
            }
        }
    }
    
    print();
    
    for (i = 0; i < graph.nodes(); i++)
    {
        removeArc(i, src);
        removeArc(snk, i);
    }

    print();
    _source = src;
    _sink = snk;
}

void Network::minimumCut(Network &minCutEdges)
{
    std::vector<int> visitedNodes(_nodes, 0);
    std::stack<int> toVisit;
    int currentNode = 0;
    int i = 0, j = 0;
    NEIGHBOURLIST::iterator neigh;
    
    /* Start from source */
    visitedNodes[_source] = 1;
    toVisit.push(_source);
    
    while (!toVisit.empty())
    {
        currentNode = toVisit.top();
        toVisit.pop();
        
        for (neigh = _arcs[currentNode].begin(); neigh != _arcs[currentNode].end(); neigh++)
        {
            /* If <i> is reachable from <currentNode> */
            if (!visitedNodes[neigh->nodeNumber])
            {
                visitedNodes[neigh->nodeNumber] = 1;
                toVisit.push(neigh->nodeNumber);

            }
        }
    }
    
    /* If <j> is unreachable from <i> it is either blocked or they are disconnected */
    for (i = 0; i < _nodes; i++)
    {
        for (j = 0; j < _nodes; j++)
        {
            if (visitedNodes[i] && !visitedNodes[j])
            {
                minCutEdges.setArcWeight(i, j, 1);
            }
        }
    }
}