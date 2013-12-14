//
//  main.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 18.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <iostream>
#include "imagenetwork.h"

int main(int argc, const char * argv[])
{
    ImageNetwork image(argv[1]);
    
    while (image.repart());
    
    image.outputPartition(argv[2]);
    
    return 0;
}

