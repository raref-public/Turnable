#include "SFML/Audio.hpp"
#include "sounds.h"
#include <ncurses.h>


// constructor
Sounds::Sounds(const std::string& w) : wavfile(w) {}

void Sounds::play() {
    sf::SoundBuffer s_buffer;
    if (!s_buffer.loadFromFile(wavfile)) {
        printw("Failed to open file");
    }
    sf::Sound sound;
    sound.setBuffer(s_buffer);
    sound.setLoop(true);
    sound.play();
    while (sound.getStatus() == sf::Sound::Playing) {
        sf::sleep(sf::milliseconds(2000));
        break;
    }
}