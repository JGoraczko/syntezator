#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <SFML/Audio.hpp>
#include "MidiFile.h"
#include "MidiEvent.h"
#include "MidiMessage.h"
#include "datachunk.h"
#include <mqueue.h>
#include <string.h>

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


void GenerateSamplesFromFile(char* filename ){
    const int SAMPLE_RATE = 44100;
    smf::MidiFile midiFile;

    midiFile.read(filename);
    midiFile.joinTracks();
    midiFile.doTimeAnalysis();
    midiFile.linkNotePairs();
    for(int i = 0; i < midiFile[0].size(); i++ ){
        if(midiFile[0][i].isNoteOn()){
            double duration = midiFile[0][i].getDurationInSeconds();

            /*std::cout << duration << " ---------- ";
            std::cout << midiFile[0][i].getKeyNumber() << " ---------- ";
            std::cout << 27.5 * pow(2, (midiFile[0][i].getKeyNumber()/12.0));*/

            std::vector<sf::Int16> paczka;
            for (int s = 0; s < SAMPLE_RATE*duration; ++s){
                paczka.push_back(SineWave(s, 27.5 * pow(2, (midiFile[0][i].getKeyNumber()/12.0)), SAMPLE_RATE));

            }
            sf::SoundBuffer buffer;
            buffer.loadFromSamples(&paczka[0], paczka.size(), 1, SAMPLE_RATE);


        }
    }


    std::this_thread::sleep_for(std::chrono::seconds(20));
}

int main()
{
    GenerateSamplesFromFile("Beethoven.mid");
    mqd_t mq;
    DataChunk data;
    mq = mq_open(FIRST_QUEUE_NAME, O_WRONLY);

    do {

        memset(&data, 0, sizeof(DataChunk));
        mq_send(mq, (const char *) &data, sizeof(DataChunk), 0);

    } while(getchar() != 'q');

    mq_close(mq);

    return 0;
}
