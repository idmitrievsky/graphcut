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

void Network::edmondskarp(Network &minimumCut, double *maxFlow)
{
    double capacity = 0, minCapacity = 0, newCapacity = 0, candidate = 0;
    int i = 0, count = 0;
    std::vector<bool> visitedNodes;
    Network residualNetwork = *this, flowNetwork(_nodes, _source, _sink, 6);
    std::vector<int> path;
    std::vector<int>::iterator it;
    
    /* Source have a lot of outgoing arcs. So allocate them once. */
    flowNetwork._arcs[_source].assign(flowNetwork.nodes(), {-1, -1});
    
    while (residualNetwork.shortestAugmentingPath(path, visitedNodes))
    {
        count++; // debug
        /* Start with initial minimum value */
        minCapacity = residualNetwork.getArcWeight(path[_sink], _sink, 0);
        /* Skip initial value */
        i = path[_sink];
        while (path[i] != _source)
        {
            candidate = residualNetwork.getArcWeight(path[i], i);
            if (candidate < minCapacity)
            {
                minCapacity = candidate;
            }
            i = path[i];
        }
        /* Because of source having so many outgoing arcs, it's much more effective to check it manually */
        candidate = residualNetwork.getArcWeight(_source, i, i - 1);
        if (candidate < minCapacity)
        {
            minCapacity = candidate;
        }

        /* Increase flow as much as possible */
        i = _sink;
        while (path[i] != _source)
        {
            newCapacity = flowNetwork.getArcWeight(path[i], i) + minCapacity;
            flowNetwork.setArcWeight(path[i], i, newCapacity);
            flowNetwork.setArcWeight(i, path[i], -1 * newCapacity);
            i = path[i];
        }
        newCapacity = flowNetwork.getArcWeight(_source, i, i - 1) + minCapacity;
        flowNetwork.setArcWeight(_source, i, newCapacity, i - 1);
        flowNetwork.setArcWeight(i, _source, -1 * newCapacity);
        
        /* Decrease capacity in residual network  */
        i = _sink;
        while (path[i] != _source)
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
        capacity = getArcWeight(_source, i, i - 1) - flowNetwork.getArcWeight(_source, i, i - 1);
        if (capacity > 0)
        {
            residualNetwork.setArcWeight(_source, i, capacity, i - 1);
        }
        else
        {
            residualNetwork._arcs[_source][i - 1].arcWeight = 0;
        }
    }
    
    if (maxFlow)
    {
        /* Calculate all the flow coming from source */
        for (i = 0; i < _nodes; i++)
        {
            *maxFlow += flowNetwork.getArcWeight(_source, i);
        }
    }
    
    /* Filter out disconnected pairs of nodes */
    
    minimumCut._arcs[_source].assign(minimumCut.nodes(), {-1,-1});
    
    for (i = 0; i < _nodes; i++)
    {
        int ind = 0;
        for (auto neigh = _arcs[i].begin(); neigh != _arcs[i].end() && neigh->nodeNumber != -1; ++neigh)
        {
            /* If <i> is reachable from <currentNode> */
            if (visitedNodes[i] && !visitedNodes[neigh->nodeNumber])
            {
                minimumCut.setArcWeight(i, neigh->nodeNumber, 1, ind++);
            }
        }
    }
}

int Network::source(void)
{
    return _source;
}

int Network::sink(void)
{
    return _sink;
}

Network::Network(int nodesNum, int source, int sink, int neighs):Graph(nodesNum, neighs)
{
    _source = source;
    _sink = sink;
}

Network::Network(void)
{
    _source = 0;
    _sink = 0;
}

bool Network::shortestAugmentingPath(std::vector<int> &ancestors, std::vector<bool> &visitedNodes)
{
    std::queue<int> toVisit;
    int currentNode = 0;
    NEIGHBOURLIST::iterator neigh;
    
    /* Clear containers */
    ancestors.assign(_nodes, 0);
    visitedNodes.assign(_nodes, false);
    
    /* Start from the source */
    toVisit.push(_source);
    visitedNodes[_source] = true;
    
    /* Source don't have any ancestors */
    ancestors[_source] = -1;
    
    while (!toVisit.empty())
    {
        currentNode = toVisit.front();
        toVisit.pop();
        
        for (neigh = _arcs[currentNode].begin(); neigh != _arcs[currentNode].end() && neigh->nodeNumber != -1; ++neigh)
        {
            /* If <i> is reachable from <currentNode> */
            if (!visitedNodes[neigh->nodeNumber] && neigh->arcWeight)
            {
                visitedNodes[neigh->nodeNumber] = true;
                
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
    
    return false;
    
found:
    return true;
}

void Network::obduct(Graph &graph, int src, int snk)
{
    erase();
    
    for (int i = 0; i < graph.nodes(); i++)
    {
        for (int j = 0; j < graph.nodes(); j++)
        {
            if (graph.getArcWeight(i, j))
            {
                setArcWeight(i, j, graph.getArcWeight(i, j));
            }
        }
    }
    
    for (int i = 0; i < graph.nodes(); i++)
    {
        removeArc(i, src);
        removeArc(snk, i);
    }
    
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