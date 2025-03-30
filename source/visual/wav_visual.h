#ifndef WAV_VISUAL_H
#define WAV_VISUAL_H

#include <string>
#include <vector>
#include <ncurses.h>
#include <meta_data/audfile.h>

/**
 * @brief WavVisual class for creating visual representations of WAV files
 * 
 * This class is responsible for creating text-based visual representations
 * of WAV files that can be displayed in the terminal. It is used in the
 * VISUAL mode of the application.
 */
class WavVisual {
public:
    /**
     * @brief Construct a new Wav Visual object
     * 
     * @param filePath The path to the WAV file
     */
    WavVisual(const std::string& filePath);
    
    /**
     * @brief Destroy the Wav Visual object
     */
    ~WavVisual();
    
    /**
     * @brief Generate a visual representation of the WAV file
     * 
     * @param width The width of the visual representation
     * @param height The height of the visual representation
     * @return true if the visual representation was generated successfully
     * @return false if there was an error
     */
    bool generateVisual(int width, int height);
    
    /**
     * @brief Display the visual representation in a window
     * 
     * @param win The window to display the visual representation in
     * @param startY The starting Y coordinate
     * @param startX The starting X coordinate
     */
    void displayVisual(WINDOW* win, int startY, int startX);
    
    /**
     * @brief Get the file path
     * 
     * @return const std::string& The file path
     */
    const std::string& getFilePath() const;
    
    /**
     * @brief Check if the visual representation is valid
     * 
     * @return true if the visual representation is valid
     * @return false if the visual representation is not valid
     */
    bool isValid() const;
    
private:
    std::string m_filePath;
    AudioFile m_audioFile;
    std::vector<std::vector<char>> m_visual;
    bool m_valid;
    
    /**
     * @brief Generate a simple waveform representation
     * 
     * @param width The width of the visual representation
     * @param height The height of the visual representation
     * @return true if the waveform was generated successfully
     * @return false if there was an error
     */
    bool generateWaveform(int width, int height);
    
    /**
     * @brief Generate a simple spectrum representation
     * 
     * @param width The width of the visual representation
     * @param height The height of the visual representation
     * @return true if the spectrum was generated successfully
     * @return false if there was an error
     */
    bool generateSpectrum(int width, int height);
};

#endif // WAV_VISUAL_H
