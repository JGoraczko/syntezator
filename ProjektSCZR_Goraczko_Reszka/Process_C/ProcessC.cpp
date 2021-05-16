#include "ProcessC.hpp"

void MusicPlayer::addSamples(const sf::SoundBuffer& buffer)
{
    sem.lock();
    
    if (first_time)
    {
    	initialize(buffer.getChannelCount(), buffer.getSampleRate());
        first_time = false;
        currentSample = 0;
        samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());
        sem.unlock();
        this->play();
        return;
    }

    if(currentSample == samples.size())
    {
        currentSample = 0;
        samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());
    }
    else
    {
        samples.erase(samples.begin(), samples.begin()+currentSample);
        currentSample = 0;
        samples.insert(samples.end(), buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());
    }
    
    sem.unlock();
}

bool MusicPlayer::onGetData(Chunk& data)
{
    std::lock_guard<std::mutex> guard(sem);
    if (samples.size() == 0 || currentSample == samples.size())
    {
        data.samples = &silence[0];
        data.sampleCount = SAMPLES_TO_STREAM;
        return true;
    }
    data.samples = &samples[currentSample];
    if (currentSample + SAMPLES_TO_STREAM <= samples.size())
    {
        data.sampleCount = SAMPLES_TO_STREAM;
        currentSample += SAMPLES_TO_STREAM;
    }
    else
    {
        data.sampleCount = samples.size() - currentSample;
        currentSample = samples.size();            
    }       
    return true;
}
