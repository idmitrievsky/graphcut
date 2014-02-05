//
//  main.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 18.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <iostream>
#include "imagenetwork.h"

#define UNITPX 32

int main(int argc, const char * argv[])
{
    cv::Mat image, imageROI, out, outROI;
    unsigned vn = 0, hn = 0;
    
    image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    out = image;
    
    vn = image.rows / UNITPX;
    hn = image.cols / UNITPX;
    
    for (unsigned x = 0; x < hn; ++x)
    {
        for (unsigned y = 0; y < vn; ++y)
        {
            outROI = imageROI = image(cv::Rect(x * UNITPX, y * UNITPX, UNITPX, UNITPX));
            
            ImageNetwork temp(&imageROI);
            while (temp.repart());
            
            temp.outputPartition(&outROI);
            
            for (unsigned in = 0; in < UNITPX; ++in)
            {
                for (unsigned jn = 0; jn < UNITPX; ++jn)
                {
                    out.at<uchar>(y * UNITPX + jn, x * UNITPX + in) = outROI.at<uchar>(jn, in);
                }
            }
        }
    }
    
    cv::imwrite(argv[2], out);
    
    return 0;
}

