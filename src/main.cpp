//
//  main.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 18.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <iostream>
#include "imagenetwork.h"

#define NODESNUM 9

int main(int argc, const char * argv[])
{
    ImageNetwork image(argv[1]);
    char outPath[] = "/Users/ivan/.supp/code/graphcut/out.jpg";
    
    while (image.repart());
    
    image.outputPartition(outPath);
    
    return 0;
}

