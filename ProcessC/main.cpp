#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <cmath>
#include <stdbool.h>
#include "MidiFile.h"
#include "MidiEvent.h"
#include "MidiMessage.h"

#include "ProcessC.hpp"
#include <iostream>

/*
Sposób generacji fali - przykład:
std::vector<sf::Int16> samples;
const int SAMPLE_RATE = 44100;
for (int i = 0; i < 44100; ++s)
        samples.push_back(SineWave(i, 440, SAMPLE_RATE));
*/

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
void PlayFromFile(char* filename){
    const int SAMPLE_RATE = 44100;
    smf::MidiFile midiFile;
    sf::SoundBuffer buffer;
    midiFile.read(filename);
    midiFile.doTimeAnalysis();
    midiFile.linkNotePairs();
    midiFile.deltaTicks();

    bool isPlaying = false;



    midiFile.joinTracks();
    midiFile.linkNotePairs();
    MusicPlayer player;
    for(int i = 0; i < midiFile.getEventCount(0); i++ ){
        smf::MidiEvent midiEvent = midiFile.getEvent(0, i);
        double duration;
        /*
        if(midiEvent.isLinked()){
            duration = midiEvent.getDurationInSeconds();
        }
        if(midiEvent.isNoteOff()){
            std::cout << "note off" << std::endl;
        } else {
            std::cout << "not note" << std::endl;
        }*/
        duration = midiFile.getEvent(0, i + 1).tick/4.0;
        if(midiEvent.isNoteOn() && duration){


            std::cout << duration << " ---------- ";
            std::cout << midiEvent.getKeyNumber() << " ---------- ";
            std::cout << 27.5 * pow(2, (midiEvent.getKeyNumber()/12.0)) << std::endl;

            std::vector<sf::Int16> paczka;
            for (int s = 0; s < SAMPLE_RATE*duration; ++s){
                paczka.push_back(SineWave(s, 27.5 * pow(2, (midiEvent.getKeyNumber()/12.0)), SAMPLE_RATE));

            }
            buffer.loadFromSamples(&paczka[0], paczka.size(), 1, SAMPLE_RATE);
            player.load(buffer);
            if(!isPlaying){
                player.play();
                isPlaying = true;

            }



        }
    }


    std::this_thread::sleep_for(std::chrono::seconds(20));
}

int main()
{
    PlayFromFile("bach.mid");
    /*
    const int SAMPLE_RATE = 44100;
    sf::SoundBuffer buffer, buffer2;
    std::vector<sf::Int16> paczka, paczka2;
    for (int s = 0; s < 44100; ++s)
        paczka.push_back(TriangleWave(s, 440, SAMPLE_RATE));
    for (int s = 0; s < 44100; ++s)
        paczka2.push_back(SawWave(s, 220, SAMPLE_RATE));
    buffer.loadFromSamples(&paczka[0], paczka.size(), 1, SAMPLE_RATE);
    MusicPlayer player;
    player.load(buffer);
    player.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    buffer2.loadFromSamples(&paczka2[0], paczka2.size(), 1, SAMPLE_RATE);
    player.addSamples(buffer2);
    std::this_thread::sleep_for(std::chrono::seconds(6));
    player.addSamples(buffer);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    if (!buffer.loadFromFile("muza.wav"))
       return -1;
    MusicPlayer player;
    player.load(buffer);
    player.play();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    player.addSamples(buffer);
    std::this_thread::sleep_for(std::chrono::seconds(30));
    player.addSamples(buffer);
    std::this_thread::sleep_for(std::chrono::seconds(12)); */
    return 0;
}
