//
//  main.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 18.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <iostream>
#include "network.h"

#define NODESNUM 9

int main(int argc, const char * argv[])
{
    double maxFlow = 0;
    Graph undirected(NODESNUM);
    
    Network associatedNetwork(NODESNUM, 0, 8);
    Network minimumCutEdges(NODESNUM, 0, 8);
    
    undirected.testFillUp();

    undirected.print();
    
    associatedNetwork.obduct(undirected, 0, 8);

    associatedNetwork.print();
    
    maxFlow = associatedNetwork.edmondskarp(minimumCutEdges);
    
    minimumCutEdges.print();
    
    return 0;
}

