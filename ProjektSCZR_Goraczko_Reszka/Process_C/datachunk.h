#ifndef DATA_CHUNK_INCLUDED
#define DATA_CHUNK_INCLUDED

#include <SFML/Audio.hpp>
#include <ctime>
#define SAMPLE_COUNT 1000
#define FIRST_QUEUE_NAME "/first_queue"

typedef struct DataChunk
{

    sf::Int16 samples[SAMPLE_COUNT];
    std::chrono::high_resolution_clock::time_point send_time;

}DataChunk;


#endif
