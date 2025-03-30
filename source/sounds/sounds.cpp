#include "SFML/Audio.hpp"
#include "sounds.h"
#include <ncurses.h>
#include <meta_data/audfile.h>

// constructor
Sounds::Sounds(const std::string& w) : wavfile(w), m_isPlaying(false) {}

void Sounds::play() {
    // Default sleep time (in case of errors)
    int sleepTime = 2000; // Default to 2 seconds
    
    try {
        // Get the duration of the sound file using AudioFile
        AudioFile audioFile(wavfile);
        
        if (!audioFile.isValid()) {
            printw("Error: Invalid WAV file: %s\n", wavfile.c_str());
            return;
        }
        
        double duration = audioFile.getDuration();
        
        // Convert duration to milliseconds
        sleepTime = static_cast<int>(duration * 1000);
        
        // If the file has zero or negative duration, use a default value
        if (sleepTime <= 0) {
            printw("Warning: Invalid duration (%.2f seconds), using default value.\n", duration);
            sleepTime = 2000; // Default to 2 seconds
        }
    } catch (const std::exception& e) {
        printw("Error processing WAV file: %s\n", e.what());
        return;
    }
    
    sf::SoundBuffer s_buffer;
    if (!s_buffer.loadFromFile(wavfile)) {
        printw("Failed to open file\n");
        return;
    }
    
    sf::Sound sound;
    sound.setBuffer(s_buffer);
    sound.setLoop(true); // Set to true to loop continuously
    
    m_isPlaying = true;
    sound.play();
    
    printw("Playing sound on repeat (press the same button to stop)\n");
    
    // Play the sound until explicitly stopped
    while (m_isPlaying) {
        sf::sleep(sf::milliseconds(100)); // Check status frequently
        
        // If the sound stopped for some reason but we're still supposed to be playing,
        // restart it (this shouldn't happen with loop=true, but just in case)
        if (sound.getStatus() != sf::Sound::Playing && m_isPlaying) {
            sound.play();
        }
    }
    
    // Stop the sound when m_isPlaying is set to false
    sound.stop();
}

void Sounds::stop() {
    m_isPlaying = false;
}

bool Sounds::isPlaying() const {
    return m_isPlaying;
}
