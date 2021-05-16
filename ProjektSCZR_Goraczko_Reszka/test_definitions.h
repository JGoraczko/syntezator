#ifndef TEST_DEFINITIONS_H_INCLUDED
#define TEST_DEFINITIONS_H_INCLUDED

#define A_PRIORITY 0
#define B_PRIORITY 0
#define C_PRIORITY 0
#define D_PRIORITY 0

#define PROCESS_B_QUEUE_NAME "/process_b_queue"
#define PROCESS_C_QUEUE_NAME "/process_c_queue"
#define SHARED_MEMORY_AB_NAME "/shared_memory_ab"
#define SHARED_MEMORY_BC_NAME "/shared_memory_bc"

#define SHARED_MEM_BUFF_SIZE 50
#define SAVING_PERIOD 50		//wielkość "paczki" logów zapisywanych do pliku - ta wielkość pośrednio określa okres zapisu logów do plików testowych
#define SAMPLE_COUNT 1000		//ilość próbek przesyłanych w pojedynczym komunikacie

#define MESSAQE_QUEUES 0
#define SHARED_MEMORY 0
#define COMMUNICATION_STRUCTUR 0

#define SAMPLES_TO_STREAM 30000;	//tyle próbek jest cyklicznie pobieranych przez proces C

#endif // TEST_DEFINITIONS_H_INCLUDED
