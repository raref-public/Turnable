#include <string>
#include <thread>
#include <vector>
#include <memory>
#include <sounds/sounds.h>

#ifndef THREADING_H
#define THREADING_H

class Threading {
public:
    Threading();
    ~Threading();
    
    // Start playing a sound file in a separate thread
    void PlaySoundInThread(const std::string& soundFilePath, int soundIndex = -1);
    
    // Check if a specific sound is playing
    bool isSoundPlaying(int index) const;
    
    // Stop a specific sound
    void StopSound(int index);
    
    void WaitForAllThreads();
    
    // Stop all currently playing sounds
    void StopAllSounds();
    
private:
    std::vector<std::shared_ptr<std::thread>> m_threads;
    std::vector<std::shared_ptr<Sounds>> m_soundObjects;
    bool m_isRunning;
};

#endif // THREADING_H
