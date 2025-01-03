#include <iostream>
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

int main(int argc, char* argv[])
{
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();

   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}