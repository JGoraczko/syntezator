#ifndef DATA_CHUNK_INCLUDED
#define DATA_CHUNK_INCLUDED

#include <SFML/Audio.hpp>
#include <ctime>
#include <chrono>
#define SAMPLE_COUNT 1000
#define PROCESS_B_QUEUE_NAME "/process_b_queue"
#define PROCESS_C_QUEUE_NAME "/process_c_queue"

typedef struct DataChunk
{

    sf::Int16 samples[SAMPLE_COUNT];
    std::chrono::high_resolution_clock::time_point send_time;

}DataChunk;


#endif
