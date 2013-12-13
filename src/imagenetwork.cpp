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
    //cv::Scharr(*image, grad_x, ddepth, 1, 0);
    cv::Sobel(*image, grad_x, ddepth, 1, 0, 3);
    cv::convertScaleAbs( grad_x, abs_grad_x );
    
    /// Gradient Y
    //cv::Scharr(*image, grad_y, ddepth, 0, 1);
    cv::Sobel(*image, grad_y, ddepth, 0, 1, 3);
    cv::convertScaleAbs( grad_y, abs_grad_y );
    
    
//    cv::Mat sq_grad;
//    cv::Mat sq_grad_x, sq_grad_y;
//    cv::multiply(abs_grad_x, abs_grad_x, sq_grad_x);
//    cv::multiply(abs_grad_y, abs_grad_y, sq_grad_y);
//    cv::add(sq_grad_x, sq_grad_y, sq_grad);
//    cv::sqrt(sq_grad, *grad);
    
    /// Total Gradient (approximate)
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, *grad);
    //cv::cartToPolar(abs_grad_x, abs_grad_y, *grad, angle);

    return grad;
}


ImageNetwork::ImageNetwork(const char *imagePath)
{
    int i = 0, j = 0, pixels = 0, nodesNumber = 0;
    
    image = new cv::Mat;
    
    *image = cv::imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
    grad = gradient(image);
    cv::imwrite("/Users/ivan/.supp/code/graphcut/grad.jpg", *grad);
    
    pixels = image->rows * image->cols;
    
    nodesNumber = pixels + 2;
    
    init(nodesNumber, 6);
    _source = 0;
    _sink = pixels + 1;
    _intensities = new double[_nodes];
    
    for (i = 0; i < image->rows; i++)
    {
        for (j = 0; j < image->cols; j++)
        {
            _intensities[pixelGraphIndex(i, j)] = image->at<uchar>(i, j);
        }
    }
    _intensities[0] = _intensities[pixels + 1] = 0;
    
    partition = new Partition(pixels);
}

ImageNetwork::~ImageNetwork(void)
{
    delete image;
    delete [] _intensities;
}

void ImageNetwork::outputPartition(char *outPath)
{
    cv::Mat out(image->rows, image->cols, CV_8UC1);
    int i = 0, j = 0;
    
    for (i = 0; i < image->rows; i++)
    {
        for (j = 0; j < image->cols; j++)
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

int ImageNetwork::pixelGraphIndex(int x, int y)
{
    return 1 + x + y * image->cols;
}

int ImageNetwork::pixelPartitionIndex(int x, int y)
{
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
    
    w = image->cols;
    h = image->rows;
    

    if (IN_BORDERS(nodeX(p) - 1, nodeY(p), w, h))
        neighbours.push_back(pixelGraphIndex(nodeX(p) - 1, nodeY(p)));
    if (IN_BORDERS(nodeX(p) + 1, nodeY(p), w, h))
        neighbours.push_back(pixelGraphIndex(nodeX(p) + 1, nodeY(p)));
    if (IN_BORDERS(nodeX(p), nodeY(p) - 1, w, h))
        neighbours.push_back(pixelGraphIndex(nodeX(p), nodeY(p) - 1));
    if (IN_BORDERS(nodeX(p), nodeY(p) + 1, w, h))
        neighbours.push_back(pixelGraphIndex(nodeX(p), nodeY(p) + 1));

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
    
    double sigma = 30, distX = 0, distY = 0, distance = 0, weight = 0, intensDiff = 0;
    
//    distX = fabs(nodeX(p) - nodeX(q));
//    distY = fabs(nodeY(p) - nodeY(q));
//    
//    distance = sqrt(distX * distX + distY * distY);
    intensDiff = nodeIntensity(p) - nodeIntensity(q);
    
    weight = exp(-(intensDiff * intensDiff) / (2 * sigma * sigma));
    //weight /= distance;
    
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

double ImageNetwork::localTerm(graphIndex p, Label pLabel)
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
    
    if (nodeIntensity(p) < 50 && pLabel == BACKGROUND)
    {
        return 2000;
    }
    else if (nodeIntensity(p) > 100 && pLabel == FOREGROUND)
    {
        return 2000;
    }
    else
    {
        double weight = 0;
        std::vector<graphIndex> neighbours(4);
        std::vector<graphIndex>::iterator it;
        int i = 0, hist = 1;
        
        pixelNeighbours(i, neighbours);
        
        for (it = neighbours.begin(); it != neighbours.end(); it++)
        {
            hist += (partition->label(*it - 1) != pLabel ? 1 : 0);
        }
        
        weight = 10 * log(hist);
        return weight;

    }
}


double ImageNetwork::energy(Partition *p)
{
    int i = 0;
    double energy = 0;
    std::vector<graphIndex> neighbours(4);
    std::vector<graphIndex>::iterator it;
    
    for (i = 1; i < _nodes - 1; i++)
    {
        energy += localTerm(i, p->label(i - 1));
        
        pixelNeighbours(i, neighbours);
        
        for (it = neighbours.begin(); it != neighbours.end(); it++)
        {
            energy += boundaryTerm(i, p->label(i - 1), *it, p->label(*it - 1));
        }
        neighbours.clear();
    }
    
    return energy;
}

int ImageNetwork::repart(void)
{
    int i = 0;
    double boundWeight = 0, t;
    std::vector<graphIndex> neighbours(4);
    std::vector<graphIndex>::iterator it;
    Partition *newPart = new Partition(*partition);
    Network minimumCut(_nodes, _source, _sink), assoc(_nodes, _source, _sink);
    
    for (i = 1; i < _nodes - 1; i++)
    {
        assoc.setArcWeight(_source, i, localTerm(i, FOREGROUND));
        assoc.setArcWeight(i, _sink, localTerm(i, BACKGROUND));
        
        pixelNeighbours(i, neighbours);
        
        for (it = neighbours.begin(); it != neighbours.end(); it++)
        {
            boundWeight = boundaryTerm(i, FOREGROUND, *it, BACKGROUND);
            assoc.setArcWeight(*it, i, boundWeight);
            assoc.setArcWeight(i, *it, boundWeight);
        }
        neighbours.clear();
    }
    
    t = assoc.edmondskarp(minimumCut);
    
    for (i = 1; i < _nodes - 1; i++)
    {
        if (minimumCut.getArcWeight(assoc.source(), i))
        {
            t = newPart->label(i);
            newPart->setLabel(i, FOREGROUND);
            t = newPart->label(i);
        }
        else if (minimumCut.getArcWeight(i, assoc.sink()))
        {
            newPart->setLabel(i, BACKGROUND);
        }
    }
    
    t = energy(partition);
    t = energy(newPart);
    if (energy(partition) > t)
    {
        delete partition;
        partition = newPart;
        return 1;
    }
    
    return 0;
}



