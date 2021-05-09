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
#include "datachunk.h"
#include <iostream>


int main()
{
    const int SAMPLE_RATE = 44100;
    DataChunk data;
    sf::SoundBuffer buffer;
    MusicPlayer player;
    buffer.loadFromSamples(&data.samples[0], SAMPLE_COUNT, 1, SAMPLE_RATE);
    player.addSamples(buffer);
    return 0;
}
