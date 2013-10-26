//
//  main.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 18.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <iostream>
#include "network.h"

int main(int argc, const char * argv[])
{
    Network n(7, 0, 6);
    Network m(7, 0, 6);
    
    n.test();
    n.edmondskarp(&m);
    
    return 0;
}

