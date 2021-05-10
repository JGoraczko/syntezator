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
#include <thread>
#include <vector>
#include <fstream>
#include <ctime>

std::vector<double> logs;
std::mutex mutex_save, mutex_logs;
int SAVING_PERIOD;

void addLog(double log)
{
    std::lock_guard<std::mutex> guard(mutex_logs);
    logs.push_back(log);
    if (logs.size() == SAVING_PERIOD) mutex_save.unlock();
}

void saveLogs()
{
    while (true)
    {
        mutex_save.lock();
        mutex_logs.lock();
        std::fstream file;
        file.open("test.txt", std::ios::out);
        if (!file) exit(-1);
        for(int i=0; i<logs.size(); ++i)
            file << logs[i] << std::endl;
        file.close();
        logs.clear();
        mutex_logs.unlock();
    }
}

int main(int argc, char * argv[])
{
    if (argc < 2) return -1;
    SAVING_PERIOD = std::stoi(argv[1]);
    const int SAMPLE_RATE = 44100;
    DataChunk data;

    MusicPlayer player;
    mutex_save.lock();
    std::thread logSaver (saveLogs);
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
            time_t received_time = time(NULL);
            double transport_time = difftime(received_time, data.send_time);
            addLog(transport_time);
        } else {
            fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            perror("Błąd konsumenta");
        }

    } while (1);

    mq_close(mq);
    mq_unlink(FIRST_QUEUE_NAME);

    return 0;
}
