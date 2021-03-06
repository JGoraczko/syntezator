#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <SFML/Audio.hpp>
#include "MidiFile.h"
#include "MidiEvent.h"
#include "MidiMessage.h"
#include "datachunk.h"
#include "SharedMemBuff.h"
#include "test_definitions.h"
#include <mqueue.h>
#include <string.h>
#include <ctime>
#include <unistd.h>

short SineWave (double time, double freq, const int SAMPLE_RATE)
{
    double sin_arg = 2*M_PI*time*freq/SAMPLE_RATE;
    short amplitude = 32767 * sin(sin_arg);
    return amplitude;
}

short SquareWave (double time, double freq, const int SAMPLE_RATE)
{
    short amplitude = 0;
    int ticks_per_cycle = SAMPLE_RATE/freq;
    int cycle_part = int(time)%ticks_per_cycle;
    int half_cycle = ticks_per_cycle/2;
    if (cycle_part < half_cycle) amplitude = 32767;
    return amplitude;
}

short SawWave (double time, double freq, const int SAMPLE_RATE)
{
    double ticks_per_cycle = SAMPLE_RATE/freq;
    short amplitude = 32767;
    amplitude *= 2*((time/ticks_per_cycle) - floor(0.5+time/ticks_per_cycle));
    return amplitude;
}

short TriangleWave (double time, double freq, const int SAMPLE_RATE)
{
    return 2*abs(SawWave(time, freq, SAMPLE_RATE))-1;
}


void GenerateSamplesFromFile(char* filename, int waveform){
    const int SAMPLE_RATE = 44100;
    short (*waveFunction)(double, double, const int);
    switch(waveform){
        case 1:
            waveFunction = &SineWave;
            break;
        case 2:
            waveFunction = &SquareWave;
            break;
        case 3:
            waveFunction = &TriangleWave;
            break;
        case 4:
            waveFunction = &SawWave;
            break;
    }
    smf::MidiFile midiFile;
    mqd_t mq;
    mq = mq_open(PROCESS_B_QUEUE_NAME, O_WRONLY);
    if(mq < 0){
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);
        perror("Błąd otworzenia kolejki B przez A");
    }
    SharedMemBuf sharedMem(SHARED_MEMORY_AB_NAME);
    midiFile.read(filename);
    midiFile.joinTracks();
    midiFile.doTimeAnalysis();
    midiFile.linkNotePairs();
    for(int i = 0; i < midiFile[0].size(); i++ ){
        if(midiFile[0][i].isNoteOn()){
            double duration = midiFile[0][i].getDurationInSeconds();
            DataChunk *data = new DataChunk;
            for (int s = 0; s < SAMPLE_RATE*duration; ++s){
                if(s%SAMPLE_COUNT == SAMPLE_COUNT - 1){
                    data->send_time = std::chrono::high_resolution_clock::now();
		    if(SHARED_MEMORY){
			sharedMem.push(*data);
		    } else {
			int sendResult = mq_send(mq, (const char *) data, sizeof(DataChunk), 0);
                    	if(sendResult < 0){
                            fprintf(stderr, "%s:%d: ", __func__, __LINE__);
                            perror("Błąd producenta A");
                    	}
		    }
                    delete data;
                    data = new DataChunk;
                }
                data->samples[s%SAMPLE_COUNT] = waveFunction(s, 27.5 * pow(2, (midiFile[0][i].getKeyNumber()/12.0)), SAMPLE_RATE);
		
            }
            data->send_time = std::chrono::high_resolution_clock::now();
            mq_send(mq, (const char *) data, sizeof(DataChunk), 0);
        }

    }

     mq_close(mq);
}

int main(int argc, char * argv[])	//argv[1] - nazwa pliku MIDI; argv[2] - numer fali (1 - sinusoidalna, 2 - prostokątna...)
{
	if (argc < 3) return -1;
    char * music_file_name = argv[1];
    int wave_form = std::atoi(argv[2]);
    GenerateSamplesFromFile(music_file_name, wave_form);
    
    return 0;
}
