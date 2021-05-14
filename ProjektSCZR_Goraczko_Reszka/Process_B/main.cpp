#include <mqueue.h>
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

    do {
        DataChunk data;
        int bytesRead = mq_receive(mqB, (char *) &data, sizeof(DataChunk), NULL);
        if(bytesRead > 0){
	// tu będzie filtrowanie        
	    int sendResult = mq_send(mqC, (const char *) &data, sizeof(DataChunk), 0);
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
}
