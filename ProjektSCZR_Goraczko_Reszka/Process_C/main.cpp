#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <cmath>
#include <stdbool.h>
#include <mqueue.h>
#include "ProcessC.hpp"
#include "datachunk.h"
#include "test_definitions.h"
#include "SharedMemBuff.h"
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <ctime>

std::vector<double> logs;
std::mutex mutex_save, mutex_logs;

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

int main(int argc, char * argv[])	//argv[1] - nazwa pliku testowego, do którego zapisują się logi procesu C
{
    if (argc < 2) return -1;
    char * file_name = argv[1];

    const int SAMPLE_RATE = 44100;
    DataChunk data;

    MusicPlayer player;
    mutex_save.lock();
    std::thread logSaver (saveLogs, file_name);
    mqd_t mq;
    
    mq = mq_open(PROCESS_C_QUEUE_NAME, O_RDONLY);
    if(mq < 0){
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);
        perror("Błąd otworzenia kolejki C przez C");
    }
    SharedMemBuf sharedMem(SHARED_MEMORY_BC_NAME);

    do {
        sf::SoundBuffer buffer;
	if(SHARED_MEMORY){
	    data = sharedMem.pop();
	} else {
	    mq_receive(mq, (char *) &data, sizeof(DataChunk), NULL);
	}
	std::chrono::duration<float , std::micro> elapsed = std::chrono::high_resolution_clock::now() - data.send_time;
      	long long transport_time = elapsed.count();
       	addLog(transport_time);
      	buffer.loadFromSamples(&data.samples[0], SAMPLE_COUNT, 1, SAMPLE_RATE);
       	player.addSamples(buffer);

    } while (1);

    mq_close(mq);

    return 0;
}
