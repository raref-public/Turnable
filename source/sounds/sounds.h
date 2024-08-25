// MyClass.h
#ifndef SOUNDS_H
#define SOUNDS_H

#include <string>

class Sounds {
public:
    // constructor
    Sounds(const std::string& w);
    void play();
private:
    std::string wavfile;
};

#endif // SOUNDS_H
