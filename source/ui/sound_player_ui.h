#ifndef SOUND_PLAYER_UI_H
#define SOUND_PLAYER_UI_H

#include <ncurses.h>
#include <string>
#include <vector>
#include <threading/threading.h>
#include <meta_data/audfile.h>
#include <filesystem>

class SoundPlayerUI {
public:
    // Constructor
    SoundPlayerUI();
    
    // Destructor
    ~SoundPlayerUI();
    
    // Initialize the UI
    void initialize();
    
    // Run the main UI loop
    void run();
    
    // Load sound files from a directory
    bool loadSoundFiles(const std::string& directory);
    
    // Display sound files with the current selection highlighted
    void displaySoundFiles();
    
    // Update the status window
    void updateStatusWindow();
    
    // Handle user input
    bool handleInput(int ch);
    
    // Toggle sound playback
    void toggleSound(int index);
    
    // Move selection up
    void moveSelectionUp();
    
    // Move selection down
    void moveSelectionDown();
    
private:
    // Windows
    WINDOW* m_mainWin;
    WINDOW* m_statusWin;
    
    // Screen dimensions
    int m_maxY;
    int m_maxX;
    
    // ASCII art
    std::vector<std::string> m_asciiArt;
    
    // Sound files
    std::vector<std::string> m_soundPaths;
    
    // Threading manager
    Threading m_threadManager;
    
    // Selected index
    int m_selectedIndex;
    
    // Layout coordinates
    int m_asciiStartY;
    int m_asciiStartX;
    int m_contentStartY;
    int m_controlsStartY;
    int m_statusLineY;
    
    // Load ASCII art from a file
    bool loadAsciiArt(const std::string& filePath);
    
    // Display ASCII art
    void displayAsciiArt();
    
    // Display controls
    void displayControls();
    
    // Clean up resources
    void cleanup();
};

#endif // SOUND_PLAYER_UI_H
