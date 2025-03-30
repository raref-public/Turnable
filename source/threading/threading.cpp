#include "threading.h"
#include <functional>
#include <chrono>
#include <iostream>

Threading::Threading() : m_isRunning(true) {
    // Constructor
}

Threading::~Threading() {
    // Destructor - ensure all threads are stopped
    StopAllSounds();
    WaitForAllThreads();
}

void Threading::PlaySoundInThread(const std::string& soundFilePath, int soundIndex) {
    // If soundIndex is provided and valid, ensure we're not already playing that sound
    if (soundIndex >= 0 && soundIndex < m_soundObjects.size()) {
        if (m_soundObjects[soundIndex]->isPlaying()) {
            // If the sound is already playing, stop it
            m_soundObjects[soundIndex]->stop();
            return;
        }
    }
    
    // Create a new sound object
    auto soundObj = std::make_shared<Sounds>(soundFilePath);
    
    // If soundIndex is provided, replace the existing sound object at that index
    if (soundIndex >= 0 && soundIndex < m_soundObjects.size()) {
        // Wait for the existing thread to complete if it's still running
        if (m_threads[soundIndex]->joinable()) {
            m_threads[soundIndex]->join();
        }
        
        // Replace the sound object
        m_soundObjects[soundIndex] = soundObj;
        
        // Create a new thread to play the sound
        m_threads[soundIndex] = std::make_shared<std::thread>([soundObj]() {
            soundObj->play();
        });
    } else {
        // Add a new sound object and thread
        m_soundObjects.push_back(soundObj);
        
        // Create a thread to play the sound
        auto threadObj = std::make_shared<std::thread>([soundObj]() {
            soundObj->play();
        });
        
        // Store the thread
        m_threads.push_back(threadObj);
    }
}

bool Threading::isSoundPlaying(int index) const {
    if (index >= 0 && index < m_soundObjects.size()) {
        return m_soundObjects[index]->isPlaying();
    }
    return false;
}

void Threading::StopSound(int index) {
    if (index >= 0 && index < m_soundObjects.size()) {
        m_soundObjects[index]->stop();
    }
}

void Threading::WaitForAllThreads() {
    // Join all threads (wait for them to complete)
    for (auto& thread : m_threads) {
        if (thread->joinable()) {
            thread->join();
        }
    }
    
    // Clear the vectors after all threads have completed
    m_threads.clear();
    m_soundObjects.clear();
}

void Threading::StopAllSounds() {
    // Set flag to stop all sounds
    m_isRunning = false;
    
    // Stop all sound objects
    for (auto& soundObj : m_soundObjects) {
        soundObj->stop();
    }
}
