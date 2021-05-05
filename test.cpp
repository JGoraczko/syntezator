#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <iostream>

int main()
{

    sf::SoundBuffer buffer;
//if (!buffer.loadFromFile("muza.wav"))
 //       return -1;
    short int y = 10000;
    std::vector<sf::Int16> samples;
    for(int j=0; j<10; ++j)
    for(int i=0; i<2*88200; i+=10)
    {
        if((i>2*44100 && y > 0)||(i<2*44100 && y < 0))
            y = -y;
        samples.push_back(y);
    }
    buffer.loadFromSamples(&samples[0], samples.size(), 2, 44100);
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();
    samples.clear();
    std::this_thread::sleep_for(std::chrono::seconds(6));
    for(int j=0; j<10; ++j)
    for(int i=0; i<2*88200; i+=10)
    {
        if((i>2*44100 && y > 0)||(i<2*44100 && y < 0))
            y = -y;
        samples.push_back(y);
    }
    buffer.loadFromSamples(&samples[0], samples.size(), 2, 44100);
    sound.play();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sound.play();
    std::this_thread::sleep_for(std::chrono::seconds(6));
    return 0;
}
