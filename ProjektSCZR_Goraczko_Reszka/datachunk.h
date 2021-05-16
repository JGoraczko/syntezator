#ifndef DATA_CHUNK_INCLUDED
#define DATA_CHUNK_INCLUDED

#include "test_definitions.h"
#include <SFML/Audio.hpp>
#include <ctime>
#include <chrono>


typedef struct DataChunk
{

    sf::Int16 samples[SAMPLE_COUNT];
    std::chrono::high_resolution_clock::time_point send_time;

}DataChunk;


#endif
