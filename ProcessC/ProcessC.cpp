#include "ProcessC.hpp"
#include <iostream>

void MusicPlayer::load(const sf::SoundBuffer& buffer)
{
    samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());
    currentSample = 0;
    initialize(buffer.getChannelCount(), buffer.getSampleRate());
}

void MusicPlayer::addSamples(const sf::SoundBuffer& buffer)
{
    std::lock_guard<std::mutex> guard(sem);
    if(this->getStatus() == Stopped)
    {
        currentSample = 0;
        samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());
        this->play();
    }
    else
    {
        samples.erase(samples.begin(), samples.begin()+currentSample);
        currentSample = 0;
        samples.insert(samples.end(), buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());
    }
}

bool MusicPlayer::onGetData(Chunk& data)
{
    std::lock_guard<std::mutex> guard(sem);
    data.samples = &samples[currentSample];
    if (currentSample + SAMPLES_TO_STREAM <= samples.size())
    {
        data.sampleCount = SAMPLES_TO_STREAM;
        currentSample += SAMPLES_TO_STREAM;
        return true;
    }
    else
    {
        data.sampleCount = samples.size() - currentSample;
        currentSample = samples.size();
        return false;
    }
}
