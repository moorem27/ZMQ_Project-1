#include <chrono>
#include <thread>
#include <iostream>
#include "utilities.h"

int main() {
    using namespace std::chrono;
    using namespace util;

    for ( size_t i = 0; i < MAX_MESSAGES; ++i ) {
        std::cout << "work " << i << std::endl;
        // TODO: do work
    }

    return 0;
}

