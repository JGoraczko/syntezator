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
    attr.mq_maxmsg = 50;
    attr.mq_msgsize = sizeof(DataChunk);
    attr.mq_curmsgs = 0;


    mq_unlink(FIRST_QUEUE_NAME);
    mq = mq_open(FIRST_QUEUE_NAME, O_CREAT | O_RDONLY | O_EXCL, 0777, &attr);
    if(mq < 0){
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);
        perror("Błąd utworzenia kolejki");
    }

    do {
        sf::SoundBuffer buffer;
        int bytesRead = mq_receive(mq, (char *) &data, sizeof(DataChunk), NULL);
        if(bytesRead > 0){
        std::cout << bytesRead << "\n";
            buffer.loadFromSamples(&data.samples[0], SAMPLE_COUNT, 1, SAMPLE_RATE);
            player.addSamples(buffer);
        } else {
            fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            perror("Błąd konsumenta");
        }

    } while (1);

    mq_close(mq);
    mq_unlink(FIRST_QUEUE_NAME);

    return 0;
}
