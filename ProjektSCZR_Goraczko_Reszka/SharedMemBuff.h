#ifndef SHAREDMEMBUFF_INCLUDED
#define SHAREDMEMBUFF_INCLUDED
#include "test_definitions.h"
#include "datachunk.h"
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>

class SharedMemBuf {
public:

    void push(DataChunk data){
	sem_wait(freeSpace);
	sem_wait(access);

	buffer->elementsCount++;
	buffer->data[buffer->end] = data;
    	buffer->end++;
    	buffer->end %= SHARED_MEM_BUFF_SIZE;

        //std::cout << "włożono, zostało " << buffer->elementsCount << "\n";
	sem_post(access);
	sem_post(dataReady);
    }
    DataChunk pop(){
	sem_wait(dataReady);
	sem_wait(access);

	buffer->elementsCount--;
    	DataChunk data = buffer->data[buffer->start];
    	buffer->start++;
    	buffer->start %= SHARED_MEM_BUFF_SIZE;
        //std::cout << "zdjęto, zostało " << buffer->elementsCount << "\n";
	sem_post(access);
	sem_post(freeSpace);
	return data;	
    }
    void clean(){
	sem_unlink(freeSpaceName.c_str());
        sem_unlink(dataReadyName.c_str());
        sem_unlink(accessName.c_str());
	shm_unlink(memoryName.c_str());

    }
    SharedMemBuf(std::string name){
        freeSpaceName = name;
        dataReadyName = name;
        accessName = name;
	memoryName = name;
	freeSpaceName.append("f");
	dataReadyName.append("d");
	accessName.append("a");
	memoryName.append("m");
	//std::cout << freeSpaceName << "\n" << dataReadyName << "\n" << accessName << "\n" << memoryName << "\n";
	freeSpace = sem_open(freeSpaceName.c_str(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, SHARED_MEM_BUFF_SIZE);
	if(freeSpace == SEM_FAILED){
            fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            perror("Błąd otwarcia semafora 1");
        }
        dataReady = sem_open(dataReadyName.c_str(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 0);
	if(dataReady == SEM_FAILED){
            fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            perror("Błąd otwarcia semafora 2");
        }
        access = sem_open(accessName.c_str(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
	if(access == SEM_FAILED){
            fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            perror("Błąd otwarcia semafora 3");
        }

	sharedMemDesc = shm_open(memoryName.c_str(), O_CREAT | O_RDWR, 0777);
	if(sharedMemDesc < 0){
            fprintf(stderr, "%s:%d: ", __func__, __LINE__);
            perror("Błąd otwarcia pamięci współdzielonej");
        }
        ftruncate(sharedMemDesc, sizeof(DataChunk)*SHARED_MEM_BUFF_SIZE);
        buffer = (struct buffer*)mmap(NULL, sizeof(struct buffer), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemDesc, 0);
    }
    ~SharedMemBuf(){
	clean();
    }

private:
    struct buffer {
	int elementsCount;
    	int start;
    	int end;
    	DataChunk data[SHARED_MEM_BUFF_SIZE];
    };
    struct buffer *buffer;
    std::string freeSpaceName;
    std::string dataReadyName;
    std::string accessName;
    std::string memoryName;
    sem_t* freeSpace;
    sem_t* dataReady;
    sem_t* access;
    int sharedMemDesc;
    
};

#endif
