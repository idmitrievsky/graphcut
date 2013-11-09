//
//  partition.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 02.11.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <math.h>
#include "partition.h"

#define FOREGROUND 1
#define BACKGROUND 0

int Partition::label(int node)
{
    return labels[node];
}

void Partition::setLabel(int node, int label)
{
    labels[node] = label;
}

Partition::Partition(int nodes)
{
    int i = 0;
    labels = new int[nodes];
    
    for (i = 0; i < nodes; i++)
    {
        labels[i] = BACKGROUND;
    }
    nodesNum = nodes;
}

Partition::~Partition(void)
{
    delete [] labels;
}

Partition & Partition::operator = (const Partition &partition)
{
    if (this != &partition)
    {
        nodesNum = partition.nodesNum;
        
        for (int i = 0; i < nodesNum; i++)
        {
            labels[i] = partition.labels[i];
        }
    }
    
    return *this;
}

double Partition::boundary(int p, int q)
{
    return 80 * fabs(label(p) - label(q));
}

double Partition::local(int label, double intencity)
{
    return fmin(fabs(label - intencity), 20);
}

double Partition::energy(Network &img, int cols)
{
    int i = 0, neighbour = 0;
    double energy = 0;
    
    for (i = 1; i < img.nodes() - 1; i++)
    {
        energy += local(label(i - 1), img.nodeIntencity(i));
        
        if ((neighbour = i - cols) > 0)
        {
            energy += boundary((i - 1), (neighbour - 1));
        }
        if ((neighbour = i + cols) < img.nodes() - 1)
        {
            energy += boundary((i - 1), (neighbour - 1));
        }
        if ((neighbour = i - 1) > 0 && neighbour % cols != 0)
        {
            energy += boundary((i - 1), (neighbour - 1));
        }
        if ((neighbour = i + 1) < img.nodes() - 1 && neighbour % cols != 1)
        {
            energy += boundary((i - 1), (neighbour - 1));
        }
    }
    
    return energy;
}

int Partition::mutate(Network &img, int cols, Partition &mutated)
{
    int i = 0, neighbour = 0;
    double t;
    Network minimumCut(img.nodes(), img.source(), img.sink());
    
    for (i = 1; i < img.nodes() - 1; i++)
    {
        img.setArcWeight(img.source(), i, local(FOREGROUND, img.nodeIntencity(i)));
        img.setArcWeight(i, img.sink(), local(BACKGROUND, img.nodeIntencity(i)));
        
        if ((neighbour = i - cols) > 0)
        {
            img.setArcWeight(neighbour, i, boundary(i - 1, neighbour - 1));
            img.setArcWeight(i, neighbour, boundary(i - 1, neighbour - 1));
        }
        if ((neighbour = i + cols) < img.nodes() - 1)
        {
            img.setArcWeight(neighbour, i, boundary(i - 1, neighbour - 1));
            img.setArcWeight(i, neighbour, boundary(i - 1, neighbour - 1));
        }
        if ((neighbour = i - 1) > 0 && neighbour % cols != 0)
        {
            img.setArcWeight(neighbour, i, boundary(i - 1, neighbour - 1));
            img.setArcWeight(i, neighbour, boundary(i - 1, neighbour - 1));
        }
        if ((neighbour = i + 1) < img.nodes() - 1 && neighbour % cols != 1)
        {
            img.setArcWeight(neighbour, i, boundary(i - 1, neighbour - 1));
            img.setArcWeight(i, neighbour, boundary(i - 1, neighbour - 1));
        }
    }
    
    t = img.edmondskarp(minimumCut);
    
    for (i = 1; i < img.nodes() - 1; i++)
    {
        if (minimumCut.getArcWeight(img.source(), i))
        {
            t =mutated.labels[i];
            mutated.labels[i] = FOREGROUND;
            t = mutated.labels[i];
        }
        else if (minimumCut.getArcWeight(i, img.sink()))
        {
            mutated.labels[i] = BACKGROUND;
        }
    }
    t = energy(img, cols);
    t = mutated.energy(img, cols);
    if (energy(img, cols) > mutated.energy(img, cols))
    {
        return 1;
    }
    
    return 0;
}


