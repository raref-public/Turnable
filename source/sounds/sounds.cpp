#include "SFML/Audio.hpp"
#include "sounds.h"
#include <ncurses.h>


//Sounds::Sounds() {
    // consturctor
//};

void Sounds::play() {
    sf::SoundBuffer s_buffer;
    if (!s_buffer.loadFromFile("/home/ivin/git/Turnable/source/audio_samples/dj-airhorn-sound.wav")) {
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