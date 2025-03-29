// audfile.cpp
#include "audfile.h"
#include <cstring>

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
        return static_cast<double>(header.subchunk2Size) / header.byteRate;
    }
    return 0.0;
}

void AudioFile::printInfo() const {
    if (valid) {
        std::cout << "Duration: " << getDuration() << " seconds" << std::endl;
    }
}