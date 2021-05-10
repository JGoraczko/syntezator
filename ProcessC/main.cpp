#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <cmath>
#include <stdbool.h>
#include "MidiFile.h"
#include "MidiEvent.h"
#include "MidiMessage.h"
#include <mqueue.h>
#include "ProcessC.hpp"
#include "datachunk.h"
#include <iostream>


int main()
{
    const int SAMPLE_RATE = 44100;
    DataChunk data;

    MusicPlayer player;
    mqd_t mq;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(DataChunk);
    attr.mq_curmsgs = 0;
    mq = mq_open(FIRST_QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);

    do {
        sf::SoundBuffer buffer;
        int bytesRead = mq_receive(mq, (char *) &data, sizeof(DataChunk), NULL);
        std::cout << bytesRead << "\n";
        if(bytesRead > 0){
            buffer.loadFromSamples(&data.samples[0], SAMPLE_COUNT, 1, SAMPLE_RATE);
            player.addSamples(buffer);
        }

    } while (1);

    mq_close(mq);
    mq_unlink(FIRST_QUEUE_NAME);

    return 0;
}
