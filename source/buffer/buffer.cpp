#include <string>
#include <cstring>
#include <ncurses.h>
#include <fstream>
#include "buffer.h"

TextBuffer::TextBuffer() {
    // consturctor
};


void TextBuffer::displayText() {
    // clear();  // Clear the screen

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int y = rows / 6;

    std::ifstream file("ascii.txt");
    if (!file.is_open()) {
        printw("Failed to open file");
        refresh();
    }
    std::string line;
    while (getline(file, line)) {
        const char* text = line.c_str();
        int text_len = strlen(text);
        int x = (cols - text_len) / 2;
        //printw(line.c_str());
        mvprintw(y, x, "%s\n", text);
        y ++;
    }
    file.close();

    refresh();
}

void TextBuffer::centreText() {

}