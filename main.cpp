#include <ncurses.h>
#include <buffer.h>
#include <string>

int main() {
    initscr();              // Initialize the library
    raw();                  // Disable line buffering
    keypad(stdscr, TRUE);   // Enable special keys to be captured
    noecho();               // Do not display typed characters
    TextBuffer text_buffer_obj;
    text_buffer_obj.displayText();

    printw("Press any key to see it. Press 'q' to quit.");
    int ch;
    while ((ch = getch()) != 'q') { // Get user input
        printw("\nYou pressed: %c", ch);
        refresh();
    }

    endwin();               // End ncurses mode
    return 0;
}
