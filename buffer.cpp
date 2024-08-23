#include <string>
#include <ncurses.h>
#include "buffer.h"

TextBuffer::TextBuffer() {
    // consturctor
};


void TextBuffer::displayText() {
    // clear();  // Clear the screen
    printw(R"(
 ######  ### ###  ######   ##  ###   #####   ######   ###      #######  
 # ## #  ### ###  ### ###  ### ###  ### ###  ### ###  ###      ### ###  
   ##    ### ###  ### ###  #######  ### ###  ### ###  ###      ###      
   ##    ### ###  ######   #######  #######  ######   ###      #####    
   ##    ### ###  ### ##   ### ###  ### ###  ### ###  ###      ###      
   ##    ### ###  ### ###  ### ###  ### ###  ### ###  ###  ##  ### ###  
   ##     #####   ### ###  ### ###  ### ###  ######   #######  #######  
                     THE TERMINAL TURNTABLE
    )");
    refresh();
}