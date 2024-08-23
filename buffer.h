#include <string>

// MyClass.h
#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H

class TextBuffer {
public:
    TextBuffer();           // constructor
    void displayText();           // method

    void append(const std::string& text);
    void insert(size_t position, const std::string& text);
    void erase(size_t position, size_t length);
    std::string getText() const;
    // Add other necessary methods

private:
    std::string buffer;
};


#endif // TEXTBUFFER_H

