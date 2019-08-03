//
//  main.cpp
//  FarmSimulator
//
//  Created by Leo Gould on 8/1/19.
//  Copyright © 2019 Leo Gould. All rights reserved.
//
#ifndef MAINN
#define MAINN

#include "farm.hpp"
#endif

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    FarmSimulator f;
    f.setup();
    f.draw();
    
    return 0;
}
