#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <cmath>
#include <stdbool.h>
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

void saveLogs(char *file_name)
{
    while (true)
    {
        mutex_save.lock();
        mutex_logs.lock();
        std::fstream file;
        file.open(file_name, std::ios::out);
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
    if (argc < 3) return -1;
    SAVING_PERIOD = std::stoi(argv[2]);
    char * file_name = argv[1];

    const int SAMPLE_RATE = 44100;
    DataChunk data;

    MusicPlayer player;
    mutex_save.lock();
    std::thread logSaver (saveLogs, file_name);
    mqd_t mq;
    mq = mq_open(FIRST_QUEUE_NAME, O_RDONLY);
    if(mq < 0){
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);
        perror("Błąd otworzenia kolejki przez konsumenta");
    }

    do {
        sf::SoundBuffer buffer;
        int bytesRead = mq_receive(mq, (char *) &data, sizeof(DataChunk), NULL);
        if(bytesRead > 0){
            buffer.loadFromSamples(&data.samples[0], SAMPLE_COUNT, 1, SAMPLE_RATE);
            player.addSamples(buffer);
            std::chrono::duration<float , std::micro> elapsed = std::chrono::high_resolution_clock::now() - data.send_time;
            long long transport_time = elapsed.count();
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
