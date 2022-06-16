#pragma once
#include "libstat.h"

#include <iostream>
using namespace statistics::tests;

void TestNamespace() {
    AggregSum();
    AggregMax();
    AggregMean();
    AggregStandardDeviation();
    AggregMode();
    AggregPrinter();

    std::cout << "Test passed!"sv << std::endl;
}