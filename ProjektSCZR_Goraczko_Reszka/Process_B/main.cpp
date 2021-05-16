#include <mqueue.h>
#include "Iir.h"
#include "datachunk.h"
#include <thread>
#include <mutex>
#include <fstream>

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

int main(int argc, char * argv[])	//TO DO: argv[1] - włączenie/wyłączenie filtrowania; argv[2] - częstotliwość odcinania; argv[3] - nazwa pliku testowego, do którego zapisują się logi procesu B
{
	if (argc < 4) return -1;
	bool ifFiltrSamples = std::atoi(argv[1]);
	const float cutoff_frequency = std::atoi(argv[2]);
	char * file_name = argv[3];
	mutex_save.lock();
    std::thread logSaver (saveLogs, file_name);
    
    mqd_t mqB = mq_open(PROCESS_B_QUEUE_NAME, O_RDONLY);
    if(mqB < 0){
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);
        perror("Błąd otworzenia kolejki B przez B");
    }
    mqd_t mqC = mq_open(PROCESS_C_QUEUE_NAME, O_WRONLY);
    if(mqB < 0){
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);
        perror("Błąd otworzenia kolejki C przez B");
    }

    const int order = 4; 
    Iir::Butterworth::LowPass<order> filter;
    const float samplingrate = 44100;
    filter.setup (samplingrate, cutoff_frequency);
    std::chrono::high_resolution_clock::time_point receive_time, send_time;

    do {
        DataChunk data;
        int bytesRead = mq_receive(mqB, (char *) &data, sizeof(DataChunk), NULL);
        if(bytesRead > 0){
        receive_time = std::chrono::high_resolution_clock::now();
        send_time = data.send_time;
	    DataChunk dataFiltered = data;
	    if (ifFiltrSamples)
	    	for(int i = 0; i < SAMPLE_COUNT; ++i)
				dataFiltered.samples[i] = filter.filter(data.samples[i]);
	    data.send_time = std::chrono::high_resolution_clock::now();     
	    int sendResult = mq_send(mqC, (const char *) &dataFiltered, sizeof(DataChunk), 0);
            if(sendResult < 0){
            	fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            	perror("Błąd producenta B");
            }
            std::chrono::duration<float , std::micro> elapsed = receive_time - send_time;
            long long transport_time = elapsed.count();
            addLog(transport_time);
        } else {
            fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            perror("Błąd konsumenta B");
        }

    } while (true);
       
    mq_close(mqB);
    mq_close(mqC);
}
