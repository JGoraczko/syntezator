#ifndef DATA_CHUNK_INCLUDED
#define DATA_CHUNK_INCLUDED

#include <SFML/Audio.hpp>
#define SAMPLE_COUNT 100
#define FIRST_QUEUE_NAME "/first_queue"

typedef struct DataChunk
{

    sf::Int16 samples[SAMPLE_COUNT];

}DataChunk;


#endif
