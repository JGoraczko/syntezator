#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <iostream>

#include "ProcessC.hpp"

int main()
{

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("muza.wav"))
       return -1;
    MusicPlayer player;
    player.load(buffer);
    player.play();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    player.addSamples(buffer);
    std::this_thread::sleep_for(std::chrono::seconds(30));
    player.addSamples(buffer);
    std::this_thread::sleep_for(std::chrono::seconds(12));
    return 0;
}
