#include <cstdlib>

#include "ringbuffer.hpp"


int main(int, char**)
{
    example::ring_buffer<int, 512> test_buffer;

    for (int i = 0; i < 1024; ++i)
    {
        test_buffer.emplace(i);
        test_buffer.emplace(i+1);
        test_buffer.try_pop();
        test_buffer.try_pop();
    }

    return EXIT_SUCCESS;
}