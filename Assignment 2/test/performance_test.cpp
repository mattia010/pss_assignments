#include <benchmark/benchmark.h>

#include "src/CollectionManager.hpp"

static void BM_load_collection(benchmark::State &state)
{
    for (auto _ : state)
    {
        CollectionManager cm;
        cm.loadCollection();
    }
}

BENCHMARK(BM_load_collection);