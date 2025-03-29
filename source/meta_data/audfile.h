// audfile.h
#ifndef AUDFILE_H
#define AUDFILE_H

#include <iostream>
#include <fstream>
#include <cstdint>

#pragma pack(push, 1)

struct WAVHeader {
    char chunkID[4];          // "RIFF"
    uint32_t chunkSize;       // File size - 8
    char format[4];           // "WAVE"
    char subchunk1ID[4];      // "fmt "
    uint32_t subchunk1Size;   // 16 for PCM
    uint16_t audioFormat;     // 1 for PCM
    uint16_t numChannels;     // Number of channels
    uint32_t sampleRate;      // Sample rate
    uint32_t byteRate;        // Byte rate
    uint16_t blockAlign;      // Block align
    uint16_t bitsPerSample;   // Bits per sample
    char subchunk2ID[4];      // "data"
    uint32_t subchunk2Size;   // Size of the data
};

#pragma pack(pop)

class AudioFile {
public:
    AudioFile(const std::string& filePath);
    ~AudioFile();

    bool isValid() const;
    double getDuration() const;
    void printInfo() const;

private:
    std::string filePath;
    WAVHeader header;
    bool valid;
};

#endif // AUDFILE_H
