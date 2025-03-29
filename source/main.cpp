#include <ncurses.h>
#include <buffer/buffer.h>
#include <sounds/sounds.h>
#include <meta_data/audfile.h>
#include <thread>
#include <string>

int main() {
    initscr();              // Initialize the library
    raw();                  // Disable line buffering
    keypad(stdscr, TRUE);   // Enable special keys to be captured
    noecho();               // Do not display typed characters

    // can we rewrite to make it like above syntax ^^^
    TextBuffer text_buffer_obj;
    text_buffer_obj.displayText();

    // can we rewrite to make it like above syntax ^^^
    std::string path = "./source/audio_samples/dj-airhorn-sound.wav";
    // trying out audfile.cpp
    AudioFile audio_file(path);
    double file_duration = audio_file.getDuration();
    printw("Intro starts at %f.\n", file_duration);

    // last commit continues:
    Sounds sound_obj(path);
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
