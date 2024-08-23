#include <string>
#include <ncurses.h>
#include <fstream>
#include "buffer.h"

TextBuffer::TextBuffer() {
    // consturctor
};


void TextBuffer::displayText() {
    // clear();  // Clear the screen
    std::ifstream file("ascii.txt");
    if (!file.is_open()) {
        printw("Failed to open file");
        refresh();
    }
    std::string line;
    while (getline(file, line)) {
        printw(line.c_str());
        printw("\n");
    }
    file.close();

    refresh();
}

void TextBuffer::centreText() {

}