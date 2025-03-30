#include "sound_player_ui.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

SoundPlayerUI::SoundPlayerUI()
    : m_mainWin(nullptr)
    , m_statusWin(nullptr)
    , m_maxY(0)
    , m_maxX(0)
    , m_selectedIndex(0)
    , m_asciiStartY(2)
    , m_asciiStartX(0)
    , m_contentStartY(0)
    , m_controlsStartY(0)
    , m_statusLineY(0)
    , m_currentMode(VimMode::NORMAL)
{
}

SoundPlayerUI::~SoundPlayerUI()
{
    cleanup();
}

void SoundPlayerUI::initialize()
{
    // Initialize ncurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    
    // Check if terminal supports colors
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
    }
    
    // Get screen dimensions
    getmaxyx(stdscr, m_maxY, m_maxX);
    
    // Create windows
    m_mainWin = newwin(m_maxY - 10, m_maxX, 0, 0);
    m_statusWin = newwin(10, m_maxX, m_maxY - 10, 0);
    
    // Enable keypad for main window
    keypad(m_mainWin, TRUE);
    
    // Load ASCII art
    loadAsciiArt("./source/ascii/ascii.txt");
    
    // Initial display
    werase(m_mainWin);
    box(m_mainWin, 0, 0);
    
    // Display ASCII art
    displayAsciiArt();
    
    // Set content start position
    m_contentStartY = m_asciiStartY + m_asciiArt.size() + 2;
}

void SoundPlayerUI::run()
{
    // Display sound files
    displaySoundFiles();
    
    // Display controls
    displayControls();
    
    // Display mode indicator
    displayModeIndicator();
    
    // Update status window
    updateStatusWindow();
    
    // Refresh windows
    wrefresh(m_mainWin);
    wrefresh(m_statusWin);
    
    // Create a thread to update the status window periodically
    bool running = true;
    std::thread status_thread([&]() {
        while (running) {
            updateStatusWindow();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    
    // Main input loop
    int ch;
    while ((ch = wgetch(m_mainWin)) != 'q') {
        if (!handleInput(ch)) {
            break;
        }
        
        // Update status window
        updateStatusWindow();
        
        // Refresh main window
        wrefresh(m_mainWin);
    }
    
    // Clean up before exiting
    running = false;
    if (status_thread.joinable()) {
        status_thread.join();
    }
    
    m_threadManager.StopAllSounds();
    m_threadManager.WaitForAllThreads();
}

bool SoundPlayerUI::loadSoundFiles(const std::string& directory)
{
    try {
        // Check if directory exists
        if (std::filesystem::exists(directory) && std::filesystem::is_directory(directory)) {
            // Iterate through all files in the directory
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                // Check if the file is a WAV file
                if (entry.is_regular_file() && entry.path().extension() == ".wav") {
                    m_soundPaths.push_back(entry.path().string());
                }
            }
            
            // Sort the paths alphabetically for consistent ordering
            std::sort(m_soundPaths.begin(), m_soundPaths.end());
            
            return !m_soundPaths.empty();
        } else {
            mvwprintw(m_mainWin, m_contentStartY, 2, "Error: Audio samples directory not found!");
            wrefresh(m_mainWin);
            return false;
        }
    } catch (const std::exception& e) {
        mvwprintw(m_mainWin, m_contentStartY, 2, "Error scanning audio directory: %s", e.what());
        wrefresh(m_mainWin);
        return false;
    }
}

void SoundPlayerUI::displaySoundFiles()
{
    // Display file durations
    mvwprintw(m_mainWin, m_contentStartY, 2, "Sound file durations:");
    
    for (size_t i = 0; i < m_soundPaths.size(); i++) {
        // Get just the filename from the path
        std::string filename = std::filesystem::path(m_soundPaths[i]).filename().string();
        
        // Highlight the selected row
        if (static_cast<int>(i) == m_selectedIndex) {
            wattron(m_mainWin, A_REVERSE);
        }
        
        try {
            AudioFile audio_file(m_soundPaths[i]);
            
            if (!audio_file.isValid()) {
                mvwprintw(m_mainWin, m_contentStartY + 1 + i, 4, "%d. %s: Invalid WAV file", i + 1, filename.c_str());
            } else {
                double file_duration = audio_file.getDuration();
                
                // Additional check for duration validity
                if (file_duration <= 0.0) {
                    mvwprintw(m_mainWin, m_contentStartY + 1 + i, 4, "%d. %s: Invalid duration (%.2f seconds)", 
                              i + 1, filename.c_str(), file_duration);
                } else {
                    mvwprintw(m_mainWin, m_contentStartY + 1 + i, 4, "%d. %s: %.2f seconds", 
                              i + 1, filename.c_str(), file_duration);
                }
            }
        } catch (const std::exception& e) {
            mvwprintw(m_mainWin, m_contentStartY + 1 + i, 4, "%d. %s: Error - %s", 
                      i + 1, filename.c_str(), e.what());
        }
        
        // Turn off highlighting
        if (static_cast<int>(i) == m_selectedIndex) {
            wattroff(m_mainWin, A_REVERSE);
        }
    }
    
    // Set controls start position
    m_controlsStartY = m_contentStartY + m_soundPaths.size() + 2;
    
    // Set status line position
    m_statusLineY = m_controlsStartY + 8;
}

void SoundPlayerUI::updateStatusWindow()
{
    werase(m_statusWin);
    box(m_statusWin, 0, 0);
    
    // Add title
    mvwprintw(m_statusWin, 1, 2, "Currently Playing Sounds:");
    
    // Display status of each sound
    int playing_count = 0;
    for (size_t i = 0; i < m_soundPaths.size(); i++) {
        bool isPlaying = m_threadManager.isSoundPlaying(i);
        
        // Get just the filename from the path
        std::string filename = std::filesystem::path(m_soundPaths[i]).filename().string();
        
        // Display status with color
        if (isPlaying) {
            wattron(m_statusWin, A_BOLD);
            mvwprintw(m_statusWin, 3 + playing_count, 2, "%d. %s [PLAYING]", i + 1, filename.c_str());
            wattroff(m_statusWin, A_BOLD);
            playing_count++;
        }
    }
    
    if (playing_count == 0) {
        mvwprintw(m_statusWin, 3, 2, "No sounds currently playing");
    }
    
    wrefresh(m_statusWin);
}

bool SoundPlayerUI::handleInput(int ch)
{
    // Display the current mode
    displayModeIndicator();
    
    // Handle input based on the current mode
    switch (m_currentMode) {
        case VimMode::NORMAL:
            return handleNormalModeInput(ch);
        case VimMode::INSERT:
            return handleInsertModeInput(ch);
        case VimMode::VISUAL:
            return handleVisualModeInput(ch);
        default:
            return true;
    }
}

bool SoundPlayerUI::handleNormalModeInput(int ch)
{
    bool selection_changed = false;
    
    // Handle vim motions
    switch(ch) {
        // Mode switching
        case 'i':
            switchToInsertMode();
            break;
            
        case 'v':
            switchToVisualMode();
            break;
            
        // Vim motion: j (down)
        case 'j':
            moveSelectionDown();
            selection_changed = true;
            break;
            
        // Vim motion: k (up)
        case 'k':
            moveSelectionUp();
            selection_changed = true;
            break;
            
        // Vim motion: h (left) - same as k (up) in this context
        case 'h':
            moveSelectionUp();
            selection_changed = true;
            break;
            
        // Vim motion: l (right) - same as j (down) in this context
        case 'l':
            moveSelectionDown();
            selection_changed = true;
            break;
            
        // Enter or Space: Toggle selected sound
        case '\n':
        case ' ':
            toggleSound(m_selectedIndex);
            break;
            
        // Handle number keys 1-9
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            toggleSound(ch - '1');
            break;
            
        case 'w':
            {
                mvwprintw(m_mainWin, m_statusLineY, 2, "Waiting for all sound threads to complete...         ");
                
                // Create a separate thread for waiting to avoid blocking the UI
                std::thread wait_thread([this]() {
                    m_threadManager.WaitForAllThreads();
                    
                    // Update the status message when complete
                    mvwprintw(m_mainWin, m_statusLineY, 2, "All sound threads have completed.                    ");
                    wrefresh(m_mainWin);
                });
                
                // Detach the thread so it can run independently
                wait_thread.detach();
                break;
            }
            
        case 's':
            mvwprintw(m_mainWin, m_statusLineY, 2, "Stopping all sounds...                               ");
            m_threadManager.StopAllSounds();
            break;
            
        case 'q':
            return false;
            
        default:
            mvwprintw(m_mainWin, m_statusLineY, 2, "You pressed: %c                                      ", ch);
            break;
    }
    
    // If selection changed, update the display
    if (selection_changed) {
        displaySoundFiles();
    }
    
    return true;
}

bool SoundPlayerUI::handleInsertModeInput(int ch)
{
    // Handle Escape key to return to normal mode
    if (ch == 27) { // ESC key
        switchToNormalMode();
        return true;
    }
    
    // Placeholder for custom midi instrument keys
    mvwprintw(m_mainWin, m_statusLineY, 2, "Insert mode: Key %c pressed (MIDI placeholder)              ", ch);
    
    return true;
}

bool SoundPlayerUI::handleVisualModeInput(int ch)
{
    // Handle Escape key to return to normal mode
    if (ch == 27) { // ESC key
        switchToNormalMode();
        return true;
    }
    
    bool selection_changed = false;
    
    // Allow navigation in visual mode
    switch(ch) {
        case 'j':
            moveSelectionDown();
            selection_changed = true;
            break;
            
        case 'k':
            moveSelectionUp();
            selection_changed = true;
            break;
            
        case 'h':
            moveSelectionUp();
            selection_changed = true;
            break;
            
        case 'l':
            moveSelectionDown();
            selection_changed = true;
            break;
            
        default:
            mvwprintw(m_mainWin, m_statusLineY, 2, "Visual mode: You pressed: %c                         ", ch);
            break;
    }
    
    // If selection changed, update the display and the visual representation
    if (selection_changed) {
        displaySoundFiles();
        displayWavVisual();
    }
    
    return true;
}

void SoundPlayerUI::switchToNormalMode()
{
    m_currentMode = VimMode::NORMAL;
    mvwprintw(m_mainWin, m_statusLineY, 2, "Switched to NORMAL mode                                  ");
    
    // Update the status window to show playing sounds
    updateStatusWindow();
    
    // Refresh the display
    displayModeIndicator();
    wrefresh(m_mainWin);
    wrefresh(m_statusWin);
}

void SoundPlayerUI::switchToInsertMode()
{
    m_currentMode = VimMode::INSERT;
    mvwprintw(m_mainWin, m_statusLineY, 2, "Switched to INSERT mode (MIDI placeholder)                ");
    
    // Update the display
    displayModeIndicator();
    wrefresh(m_mainWin);
}

void SoundPlayerUI::switchToVisualMode()
{
    m_currentMode = VimMode::VISUAL;
    mvwprintw(m_mainWin, m_statusLineY, 2, "Switched to VISUAL mode                                   ");
    
    // Display the visual representation of the selected WAV file
    displayWavVisual();
    
    // Update the display
    displayModeIndicator();
    wrefresh(m_mainWin);
    wrefresh(m_statusWin);
}

void SoundPlayerUI::displayModeIndicator()
{
    // Display the current mode in the top-right corner of the main window
    std::string mode_text;
    
    switch (m_currentMode) {
        case VimMode::NORMAL:
            mode_text = "-- NORMAL --";
            break;
        case VimMode::INSERT:
            mode_text = "-- INSERT --";
            break;
        case VimMode::VISUAL:
            mode_text = "-- VISUAL --";
            break;
    }
    
    // Calculate position for right-aligned text
    int mode_x = m_maxX - mode_text.length() - 4;
    
    // Display the mode indicator
    mvwprintw(m_mainWin, 1, mode_x, "%s", mode_text.c_str());
    wrefresh(m_mainWin);
}

void SoundPlayerUI::displayWavVisual()
{
    // Clear the status window
    werase(m_statusWin);
    box(m_statusWin, 0, 0);
    
    // Check if we have a valid selection
    if (m_selectedIndex < 0 || m_selectedIndex >= static_cast<int>(m_soundPaths.size())) {
        mvwprintw(m_statusWin, 1, 2, "No valid WAV file selected");
        wrefresh(m_statusWin);
        return;
    }
    
    // Get the selected file path
    std::string filePath = m_soundPaths[m_selectedIndex];
    std::string filename = std::filesystem::path(filePath).filename().string();
    
    // Display the filename
    mvwprintw(m_statusWin, 1, 2, "Visual representation of: %s", filename.c_str());
    
    // Create a WavVisual object for the selected file
    WavVisual wavVisual(filePath);
    
    // Check if the WAV file is valid
    if (!wavVisual.isValid()) {
        mvwprintw(m_statusWin, 3, 2, "Invalid WAV file");
        wrefresh(m_statusWin);
        return;
    }
    
    // Get the dimensions of the status window
    int height, width;
    getmaxyx(m_statusWin, height, width);
    
    // Generate a visual representation of the WAV file
    if (!wavVisual.generateVisual(width - 4, height - 4)) {
        mvwprintw(m_statusWin, 3, 2, "Failed to generate visual representation");
        wrefresh(m_statusWin);
        return;
    }
    
    // Display the visual representation
    wavVisual.displayVisual(m_statusWin, 3, 2);
    
    // Refresh the status window
    wrefresh(m_statusWin);
}

void SoundPlayerUI::toggleSound(int index)
{
    if (index >= 0 && index < static_cast<int>(m_soundPaths.size())) {
        // Toggle sound playback
        if (m_threadManager.isSoundPlaying(index)) {
            mvwprintw(m_mainWin, m_statusLineY, 2, "Stopping sound %d: %s", 
                      index + 1, 
                      std::filesystem::path(m_soundPaths[index]).filename().string().c_str());
            m_threadManager.StopSound(index);
        } else {
            mvwprintw(m_mainWin, m_statusLineY, 2, "Playing sound %d: %s", 
                      index + 1, 
                      std::filesystem::path(m_soundPaths[index]).filename().string().c_str());
            m_threadManager.PlaySoundInThread(m_soundPaths[index], index);
        }
    }
}

void SoundPlayerUI::moveSelectionUp()
{
    if (!m_soundPaths.empty()) {
        m_selectedIndex = (m_selectedIndex - 1 + m_soundPaths.size()) % m_soundPaths.size();
        mvwprintw(m_mainWin, m_statusLineY, 2, "Selected sound %d: %s", 
                  m_selectedIndex + 1, 
                  std::filesystem::path(m_soundPaths[m_selectedIndex]).filename().string().c_str());
    }
}

void SoundPlayerUI::moveSelectionDown()
{
    if (!m_soundPaths.empty()) {
        m_selectedIndex = (m_selectedIndex + 1) % m_soundPaths.size();
        mvwprintw(m_mainWin, m_statusLineY, 2, "Selected sound %d: %s", 
                  m_selectedIndex + 1, 
                  std::filesystem::path(m_soundPaths[m_selectedIndex]).filename().string().c_str());
    }
}

bool SoundPlayerUI::loadAsciiArt(const std::string& filePath)
{
    std::ifstream ascii_file(filePath);
    if (ascii_file.is_open()) {
        std::string line;
        while (std::getline(ascii_file, line)) {
            m_asciiArt.push_back(line);
        }
        ascii_file.close();
        return true;
    }
    return false;
}

void SoundPlayerUI::displayAsciiArt()
{
    if (m_asciiArt.empty()) {
        return;
    }
    
    // Center the ASCII art
    m_asciiStartX = (m_maxX - m_asciiArt[0].length()) / 2;
    
    if (m_asciiStartX < 2) {
        m_asciiStartX = 2; // Ensure minimum margin
    }
    
    for (size_t i = 0; i < m_asciiArt.size(); i++) {
        mvwprintw(m_mainWin, m_asciiStartY + i, m_asciiStartX, "%s", m_asciiArt[i].c_str());
    }
}

void SoundPlayerUI::displayControls()
{
    mvwprintw(m_mainWin, m_controlsStartY, 2, "Controls:");
    
    // Show vim mode controls
    mvwprintw(m_mainWin, m_controlsStartY + 1, 4, "Mode switching: i (insert), v (visual), ESC (normal)");
    
    // Show vim navigation controls
    mvwprintw(m_mainWin, m_controlsStartY + 2, 4, "Vim motions: h (left), j (down), k (up), l (right)");
    mvwprintw(m_mainWin, m_controlsStartY + 3, 4, "Enter/Space: Toggle selected sound");
    
    // Show number keys for all available sound files
    if (!m_soundPaths.empty()) {
        if (m_soundPaths.size() <= 9) {
            // If 9 or fewer sounds, we can use single digit keys
            mvwprintw(m_mainWin, m_controlsStartY + 4, 4, "1-%d: Toggle sound playback", m_soundPaths.size());
        } else {
            // If more than 9 sounds, indicate that only first 9 are accessible
            mvwprintw(m_mainWin, m_controlsStartY + 4, 4, "1-9: Toggle first 9 sound files");
        }
    } else {
        mvwprintw(m_mainWin, m_controlsStartY + 4, 4, "No sound files available");
    }
    
    mvwprintw(m_mainWin, m_controlsStartY + 5, 4, "w: Wait for all sound threads to complete");
    mvwprintw(m_mainWin, m_controlsStartY + 6, 4, "s: Stop all sounds");
    mvwprintw(m_mainWin, m_controlsStartY + 7, 4, "q: Quit");
}

void SoundPlayerUI::cleanup()
{
    if (m_mainWin) {
        delwin(m_mainWin);
        m_mainWin = nullptr;
    }
    
    if (m_statusWin) {
        delwin(m_statusWin);
        m_statusWin = nullptr;
    }
    
    endwin();
}
