#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <cmath>

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

int main()
{
    const int SAMPLE_RATE = 44100;
    sf::SoundBuffer buffer, buffer2;
    std::vector<sf::Int16> paczka, paczka2;
    for (int s = 0; s < 44100; ++s)
        paczka.push_back(SineWave(s, 440, SAMPLE_RATE));
    for (int s = 0; s < 44100; ++s)
        paczka2.push_back(SineWave(s, 880, SAMPLE_RATE));
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
    /*
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
