#include <chrono>
#include <thread>
#include <iostream>
#include "utilities.h"

int main() {
    using namespace std::chrono;
    using namespace util;

    for ( size_t i = 0; i < MAX_MESSAGES; ++i ) {
        // The work done by a constant force of magnitude
        // 'F' on a point that moves a displacement 's'
        // in the direction of the force is the product W = Fs
        std::cout << "work " << i << std::endl;
        // TODO: do work
    }

    return 0;
}

