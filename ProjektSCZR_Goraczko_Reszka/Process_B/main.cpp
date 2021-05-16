#include <mqueue.h>
#include "Iir.h"
#include "../datachunk.h"

int main(int argc, char * argv[])
{
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
    const float cutoff_frequency = 10000;
    filter.setup (samplingrate, cutoff_frequency);

    do {
        DataChunk data;
        int bytesRead = mq_receive(mqB, (char *) &data, sizeof(DataChunk), NULL);
        if(bytesRead > 0){
	    DataChunk dataFiltered = data;
	    for(int i = 0; i < SAMPLE_COUNT; ++i){
		dataFiltered.samples[i] = filter.filter(data.samples[i]);
	    }     
	    int sendResult = mq_send(mqC, (const char *) &dataFiltered, sizeof(DataChunk), 0);
            if(sendResult < 0){
            	fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            	perror("Błąd producenta B");
            }
		
            /*std::chrono::duration<float , std::micro> elapsed = std::chrono::high_resolution_clock::now() - data.send_time;
            long long transport_time = elapsed.count();
            addLog(transport_time);*/
        } else {
            fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            perror("Błąd konsumenta B");
        }

    } while (1);
    
    mq_close(mqB);
    mq_close(mqC);
}
