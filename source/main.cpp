#include <ncurses.h>
#include <buffer/buffer.h>
#include <sounds/sounds.h>
#include <string>

int main() {
    initscr();              // Initialize the library
    raw();                  // Disable line buffering
    keypad(stdscr, TRUE);   // Enable special keys to be captured
    noecho();               // Do not display typed characters

    TextBuffer text_buffer_obj;
    text_buffer_obj.displayText();

    
    Sounds sound_obj;
    sound_obj.play();

    printw("Press any key to see it. Press 'q' to quit.");
    int ch;
    while ((ch = getch()) != 'q') { // Get user input
        clear();
        text_buffer_obj.displayText();
        printw("\nYou pressed: %c", ch);
        refresh();
    }

    endwin();               // End ncurses mode
    return 0;
}
