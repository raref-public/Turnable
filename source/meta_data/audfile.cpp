// audfile.cpp
#include "audfile.h"
#include <cstring>
#include <cassert>

AudioFile::AudioFile(const std::string& filePath) : filePath(filePath), valid(false) {
    std::ifstream file(this->filePath, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file." << std::endl;
    } else {
        file.read(reinterpret_cast<char*>(&header), sizeof(header));
        if (strncmp(header.chunkID, "RIFF", 4) == 0 && strncmp(header.format, "WAVE", 4) == 0) {
            valid = true;
        } else {
            std::cerr << "Invalid WAV file." << std::endl;
        }
        file.close();
    }
}

AudioFile::~AudioFile() {}

bool AudioFile::isValid() const {
    return valid;
}

double AudioFile::getDuration() const {
    if (valid) {
        // Ensure we have valid header values to prevent division by zero
        assert(header.sampleRate > 0 && "Sample rate must be greater than zero");
        assert(header.numChannels > 0 && "Number of channels must be greater than zero");
        assert(header.bitsPerSample > 0 && "Bits per sample must be greater than zero");
        
        // Calculate duration more accurately using sample rate, number of channels, and bits per sample
        // Duration = (data size in bytes) / (sample rate * num channels * bits per sample / 8)
        double bytesPerSample = header.bitsPerSample / 8.0;
        double bytesPerSecond = header.sampleRate * header.numChannels * bytesPerSample;
        
        // Calculate the duration
        double duration = static_cast<double>(header.subchunk2Size) / bytesPerSecond;
        
        // Assert that the duration is greater than zero
        assert(duration > 0.0 && "WAV file duration must be greater than zero");
        
        return duration;
    }
    return 0.0;
}

void AudioFile::printInfo() const {
    if (valid) {
        std::cout << "Duration: " << getDuration() << " seconds" << std::endl;
    }
}
