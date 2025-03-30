#ifndef SOUNDS_H
#define SOUNDS_H

#include <string>
#include <meta_data/audfile.h>
#include <atomic>

class Sounds {
public:
    // constructor
    Sounds(const std::string& w);
    void play();
    void stop();
    bool isPlaying() const;
    
private:
    std::string wavfile;
    std::atomic<bool> m_isPlaying;
};

#endif // SOUNDS_H
