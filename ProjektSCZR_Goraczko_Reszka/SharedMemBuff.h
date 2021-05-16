#ifndef SHAREDMEMBUFF_INCLUDED
#define SHAREDMEMBUFF_INCLUDED
#include "test_definitions.h"
#include "datachunk.h"
#include <semaphore.h>
#include <sys/stat.h>
#include <string>

class SharedMemBuf: 
{
public:

    void push(Datachunk* data){
	sem_wait(freeSpace);
	sem_wait(access);


	elementsCount++;
	dataBuffer[end] = &data;
    	end++;
    	end %= SHARED_MEM_BUFF_SIZE;


	sem_post(access);
	sem_post(dataReady);
    }
    Datachunk pop(){
	sem_wait(dataReady);
	sem_wait(access);


	elementsCount--;
    	Datachunk data = dataBuffer[start];
    	start++;
    	start %= SHARED_MEM_BUFF_SIZE;
    
	sem_post(freeSpace);
	sem_post(dataReady);
	return data;	
    }
    SharedMemBuf(std::string name){
        freeSpaceName = name;
        dataReadyName = name
        accessName = name
	freeSpaceName.append('f');
	dataReadyName.append('d');
	accessName.append('a');
	memoryName.append('m');
	freeSpace = sem_open(freeSpaceName.c_str(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, SHARED_MEM_BUFF_SIZE);
        dataReady = sem_open(dataReadyName.c_str(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 0);
        access = sem_open(accessName.c_str(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);

	sharedMemDesc = shm_open(memoryName.c_str(), O_CREAT | O_RDWR, 0777);
        ftruncate(sharedMemDesc, sizeof(Datachunk)*SHARED_MEM_BUFF_SIZE);
        dataBuffer = mmap(NULL, sizeof(Datachunk)*SHARED_MEM_BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    }
    ~SharedMemBuf(){
	sem_unlink(freeSpaceName.c_str());
        sem_unlink(dataReadyName.c_str());
        sem_unlink(accessName.c_str());
	shm_unlink(memoryName.c_str());
    }

private:
    std::string freeSpaceName;
    std::string dataReadyName;
    std::string accessName;
    std::string memoryName;
    sem_t* freeSpace;
    sem_t* dataReady;
    sem_t* access;
    int elementCount = 0;
    int start = 0;
    int end = 0;
    int sharedMemDesc;
    Datachunk dataBuffer[SHARED_MEM_BUFF_SIZE];
};

#endif
