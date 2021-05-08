#ifndef PROCESSC_HPP_INCLUDED
#define PROCESSC_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include <vector>
#include <mutex>

class MusicPlayer : public sf::SoundStream
{
public:

    void load(const sf::SoundBuffer& buffer);
    void addSamples(const sf::SoundBuffer& buffer);

private:

    const int SAMPLES_TO_STREAM = 30000;
    std::vector<sf::Int16> samples;
    std::size_t currentSample;
    std::mutex sem;

    bool onGetData(Chunk& data);
    void onSeek(sf::Time timeOffset) {}
};


#endif // PROCESSC_HPP_INCLUDED
