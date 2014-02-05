//
//  imagenetwork.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 15.11.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <opencv2/imgproc/imgproc.hpp>
#include "imagenetwork.h"

#define IN_BORDERS(x, y, w, h) (x >= 0 && y >= 0 && x < w && y < h)

static cv::Mat *gradient(cv::Mat *image)
{
    cv::Mat *grad;
    int ddepth = CV_16S;
    
    grad = new cv::Mat();
    
    /// Generate grad_x and grad_y
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    
    /// Gradient X
    cv::Sobel(*image, grad_x, ddepth, 1, 0, 3);
    cv::convertScaleAbs( grad_x, abs_grad_x );
    
    /// Gradient Y
    cv::Sobel(*image, grad_y, ddepth, 0, 1, 3);
    cv::convertScaleAbs( grad_y, abs_grad_y );
    
    /// Total Gradient (approximate)
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, *grad);

    return grad;
}


ImageNetwork::ImageNetwork(const char *imagePath)
{
    int pixels = 0, nodesNumber = 0;
    
    image = new cv::Mat;
    allocatedImage = true;
    *image = cv::imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
    
    grad = gradient(image);
    cv::imwrite("/Users/ivan/.supp/code/graphcut/grad.jpg", *grad);
    
    pixels = image->rows * image->cols;
    
    /* Source and sink aren't pixels */
    nodesNumber = pixels + 2;
    
    /* Only now, when the number of nodes in known, it's time to allocate memory */
    init(nodesNumber, 5);
    _source = 0;
    _sink = pixels + 1;
    _intensities = new double[_nodes];
    
    for (int i = 0; i < image->rows; i++)
    {
        for (int j = 0; j < image->cols; j++)
        {
            _intensities[pixelGraphIndex(i, j)] = image->at<uchar>(i, j);
        }
    }
    
    /* Source and sink aren't pixels */
    _intensities[0] = _intensities[pixels + 1] = 0;
    
    partition = new Partition(pixels);
}

ImageNetwork::~ImageNetwork(void)
{
    if (allocatedImage)
    {
        delete image;
    }
    delete grad;
    delete [] _intensities;
    delete partition;
}

void ImageNetwork::outputPartition(const char *outPath)
{
    cv::Mat out(image->rows, image->cols, CV_8UC1);
    
    for (int i = 0; i < image->rows; i++)
    {
        for (int j = 0; j < image->cols; j++)
        {
            out.at<uchar>(i, j) = 255 * partition->label(pixelPartitionIndex(i, j));
        }
    }
    
    out = out > 127;
    
    cv::imwrite(outPath, out);
}

double ImageNetwork::nodeIntensity(int i)
{
    return _intensities[i];
}

void ImageNetwork::setNodeIntensity(int i, double intensity)
{
    _intensities[i] = intensity;
}

graphIndex ImageNetwork::pixelGraphIndex(int x, int y)
{
    /* Source isn't a pixel, but it's numbered 0 */
    return 1 + x + y * image->cols;
}

partitionIndex ImageNetwork::pixelPartitionIndex(int x, int y)
{
    /* Partition doesn't know about any kind of graph */
    return x + y * image->cols;
}

int ImageNetwork::nodeX(graphIndex index)
{
    return (index - 1) % image->cols;
}

int ImageNetwork::nodeY(graphIndex index)
{
    return (index - 1) / image->cols;
}

void ImageNetwork::pixelNeighbours(graphIndex p, std::vector<graphIndex> &neighbours, bool eight)
{
    int w = 0, h = 0;
    int index = 0;
    
    w = image->cols;
    h = image->rows;
    

    if (IN_BORDERS(nodeX(p) - 1, nodeY(p), w, h))
        neighbours[index++] = (pixelGraphIndex(nodeX(p) - 1, nodeY(p)));
    if (IN_BORDERS(nodeX(p) + 1, nodeY(p), w, h))
        neighbours[index++] = (pixelGraphIndex(nodeX(p) + 1, nodeY(p)));
    if (IN_BORDERS(nodeX(p), nodeY(p) - 1, w, h))
        neighbours[index++] = (pixelGraphIndex(nodeX(p), nodeY(p) - 1));
    if (IN_BORDERS(nodeX(p), nodeY(p) + 1, w, h))
        neighbours[index++] = (pixelGraphIndex(nodeX(p), nodeY(p) + 1));

    if (eight)
    {
        //
    }
}

double ImageNetwork::boundaryTerm(graphIndex p, Label pLabel, graphIndex q, Label qLabel)
{
    /* 1 */
    
//    double weight = 0, eps = 1.0, kapp = 1, alpha = 100;
//    double temp, tempo;
//    
//    if (pLabel == qLabel)
//    {
//        return weight = 0;
//    }
//    else
//    {
//        temp = abs(nodeIntensity(p) - nodeIntensity(q));
//        temp = temp * temp;
//        tempo = abs(nodeIntensity(p) + nodeIntensity(q));
//        tempo = tempo * tempo + eps;
//        weight = alpha * exp(- kapp * (temp / tempo));
//        return weight;
//    }
    
    /* 2 */
    
    double sigma = 30, weight = 0, intensDiff = 0;
    
    intensDiff = nodeIntensity(p) - nodeIntensity(q);
    
    weight = 5 * exp(-(intensDiff * intensDiff) / (2 * sigma * sigma));
    
    return weight;
    
    /* 3 */
    
//    return 80 * fabs(label(p) - label(q));
    
//    if (grad->at<uchar>(nodeY(p), nodeX(p)) > 50 && pLabel == qLabel)
//    {
//        return 1;
//    }
//    if (grad->at<uchar>(nodeY(p), nodeX(p)) < 50 && pLabel != qLabel)
//    {
//        return 1;
//    }
//    return 2000;
}

double ImageNetwork::localTerm(graphIndex p, Label pLabel, Partition *part)
{
    /* 1 */
    
//    double weight = 0;
//    std::vector<graphIndex> neighbours(4);
//    std::vector<graphIndex>::iterator it;
//    int i = 0, hist = 1;
//    
//    pixelNeighbours(i, neighbours);
//    
//    for (it = neighbours.begin(); it != neighbours.end(); it++)
//    {
//        hist += (partition->label(*it - 1) != pLabel ? 1 : 0);
//    }
//    
//    weight = 10 * log(hist);
//    return weight;
    
    /* 2 */
    
//    double weight = 0;
//    
//    switch (pLabel)
//    {
//        case FOREGROUND:
//            weight = - fmax(0, grad->at<uchar>(nodeX(p), nodeY(p)));
//            break;
//        case BACKGROUND:
//            weight = fmax(0, grad->at<uchar>(nodeX(p), nodeY(p)));
//            break;
//        default:
//            break;
//    }
//    
//    return weight;
    
    /* 3 */
    
//    return fmin(fabs(label - intencity), intencity);
    /* 4 */
    
    if (nodeIntensity(p) > 80 && pLabel == BACKGROUND)
    {
        return 2000;
    }
    else if (nodeIntensity(p) < 50 && pLabel == FOREGROUND)
    {
        return 2000;
    }
    else
    {
        double weight = 0;
        std::vector<graphIndex> neighbours(4, -1);
        std::vector<graphIndex>::iterator it;
        int hist = 1;
        
        pixelNeighbours(p, neighbours);
        
        for (it = neighbours.begin(); it != neighbours.end() && *it != -1; ++it)
        {
            hist += (part->label(*it - 1) != pLabel ? 0 : 1);
        }
        
        weight = 10 * log(hist);
        return weight;
    }
}


double ImageNetwork::energy(Partition *p)
{
    double energy = 0;
    std::vector<graphIndex> neighbours(4, -1);
    std::vector<graphIndex>::iterator it;
    
    for (int i = 1; i < _nodes - 1; i++)
    {
        energy += localTerm(i, p->label(i - 1), p);
        
        pixelNeighbours(i, neighbours);
        
        for (it = neighbours.begin(); it != neighbours.end() && *it != -1; it++)
        {
            energy += boundaryTerm(i, p->label(i - 1), *it, p->label(*it - 1));
        }
        neighbours.assign(4, -1);
    }
    
    return energy;
}

bool ImageNetwork::repart(void)
{
    double boundWeight = 0, t;
    std::vector<graphIndex> neighbours(4, -1);
    std::vector<graphIndex>::iterator it;
    Partition *newPart = new Partition(nodes() - 2);
    Network minimumCut(_nodes, _source, _sink, 5), assoc(_nodes, _source, _sink, 5);
    int ind = 0;
    
    assoc._arcs[_source].assign(_nodes, {-1, -1});
    
    for (int i = 1; i < _nodes - 1; i++)
    {
        int acc = 0;
        assoc.setArcWeight(_source, i, localTerm(i, FOREGROUND, partition), ind++);
        assoc.setArcWeight(i, _sink, localTerm(i, BACKGROUND, partition), acc++);
        
        pixelNeighbours(i, neighbours);
        
        for (it = neighbours.begin(); it != neighbours.end() && *it != -1; it++)
        {
            boundWeight = boundaryTerm(i, FOREGROUND, *it, BACKGROUND);
            //assoc.setArcWeight(*it, i, boundWeight, acc++);
            assoc.setArcWeight(i, *it, boundWeight, acc++);
        }
        neighbours.assign(4, -1);
    }
    
    assoc.edmondskarp(minimumCut);
    
    for (int i = 1; i < _nodes - 1; i++)
    {
        if (minimumCut.getArcWeight(assoc.source(), i))
        {
            t = newPart->label(i - 1);
            newPart->setLabel(i - 1, FOREGROUND);
            t = newPart->label(i - 1);
        }
        else if (minimumCut.getArcWeight(i, assoc.sink()))
        {
            newPart->setLabel(i - 1, BACKGROUND);
        }
    }
    
    t = energy(partition);
    t = energy(newPart);
    if (energy(partition) > t)
    {
        delete partition;
        partition = newPart;
        return true;
    }
    
    return false;
}



