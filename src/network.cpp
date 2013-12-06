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
    int i = 0;
    Network residualNetwork = *this, flowNetwork(_nodes, _source, _sink), blocks(_nodes, _source, _sink);
    std::vector<int> path;
    std::vector<int>::iterator it;
    
    while (!(path = residualNetwork.shortestAugmentingPath()).empty())
    {
        /* Start with initial minimum value */
        minCapacity = residualNetwork.getArcWeight(path[_sink], _sink);
        /* Skip initial value */
        i = path[_sink];
        while (path[i] != -1)
        {
            if (residualNetwork.getArcWeight(path[i], i) < minCapacity)
            {
                minCapacity = residualNetwork.getArcWeight(path[i], i);
            }
            i = path[i];
        }
        /* Increase flow as much as possible */
        i = _sink;
        while (path[i] != -1)
        {
            newCapacity = flowNetwork.getArcWeight(path[i], i) + minCapacity;
            flowNetwork.setArcWeight(path[i], i, newCapacity);
            flowNetwork.setArcWeight(i, path[i], -1 * newCapacity);
            i = path[i];
        }
        /* Decrease capacity in residual network  */
        i = _sink;
        while (path[i] != -1)
        {
            capacity = getArcWeight(path[i], i) - flowNetwork.getArcWeight(path[i], i);
            if (capacity > 0)
            {
                residualNetwork.setArcWeight(path[i], i, capacity);
            }
            else
            {
                residualNetwork.removeArc(path[i], i);
            }
            i = path[i];
        }
    }
    
    /* Calculate all the flow coming from source */
    for (i = 0; i < _nodes; i++)
    {
        maxFlow += flowNetwork.getArcWeight(_source, i);
    }

    /* Find blocking saturated edges or disconnected pairs of nodes */
    residualNetwork.minimumCut(blocks);
    
    /* Filter out disconnected pairs of nodes */
    for (i = 0; i < _nodes; i++)
    {
        for (auto neigh = _arcs[i].begin(); neigh != _arcs[i].end(); neigh++)
        {
            /* If <i> is reachable from <currentNode> */
            if (blocks.getArcWeight(i, neigh->nodeNumber))
            {
                minimumCut.setArcWeight(i, neigh->nodeNumber, 1);
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

Network::Network(void)
{
    _source = 0;
    _sink = 0;
}

std::vector<int> Network::shortestAugmentingPath(void)
{
    std::queue<int> toVisit;
    std::vector<int> visitedNodes(_nodes, 0), ancestors(_nodes, 0);
    std::vector<int> emptyPath, path;
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
        return emptyPath;
    }
    
found:
    return ancestors;
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

int Network::debug(void)
{
    int i = 0, out = 0, in = 0;
    std::vector<graphIndex> neighbours(4);
    std::vector<graphIndex>::iterator it;
    
    for (i = 1; i < _nodes - 1; i++)
    {
        if (getArcWeight(_source, i))
        {
            out += 1;
        };
        if (getArcWeight(i, _sink))
        {
            in += 1;
        };
        
        //        pixelNeighbours(i, neighbours);
        //
        //        for (it = neighbours.begin(); it != neighbours.end(); it++)
        //        {
        //            assoc.setArcWeight(*it, i, boundWeight);
        //            assoc.setArcWeight(i, *it, boundWeight);
        //        }
        //        neighbours.clear();
    }
    
    std::cout << "Out of source: " << out << std::endl << "Into sink: " << in;
    return 0;
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