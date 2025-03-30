#include "wav_visual.h"
#include <iostream>
#include <cmath>

WavVisual::WavVisual(const std::string& filePath)
    : m_filePath(filePath)
    , m_audioFile(filePath)
    , m_valid(false)
{
    m_valid = m_audioFile.isValid();
}

WavVisual::~WavVisual()
{
}

bool WavVisual::generateVisual(int width, int height)
{
    if (!m_valid) {
        std::cerr << "Cannot generate visual for invalid WAV file: " << m_filePath << std::endl;
        return false;
    }
    
    // For now, just generate a simple waveform
    return generateWaveform(width, height);
}

void WavVisual::displayVisual(WINDOW* win, int startY, int startX)
{
    if (!m_valid || m_visual.empty()) {
        mvwprintw(win, startY, startX, "No valid visual representation available");
        return;
    }
    
    // Display the visual representation
    for (size_t y = 0; y < m_visual.size(); y++) {
        for (size_t x = 0; x < m_visual[y].size(); x++) {
            mvwaddch(win, startY + y, startX + x, m_visual[y][x]);
        }
    }
}

const std::string& WavVisual::getFilePath() const
{
    return m_filePath;
}

bool WavVisual::isValid() const
{
    return m_valid;
}

bool WavVisual::generateWaveform(int width, int height)
{
    // This is a placeholder implementation that generates a simple sine wave
    // In a real implementation, this would analyze the WAV file and generate
    // a more accurate representation
    
    // Resize the visual representation
    m_visual.resize(height);
    for (auto& row : m_visual) {
        row.resize(width, ' ');
    }
    
    // Generate a simple sine wave
    for (int x = 0; x < width; x++) {
        // Calculate the amplitude at this position
        double amplitude = sin(x * 0.2);
        
        // Scale the amplitude to fit the height
        int y = static_cast<int>((amplitude + 1.0) * (height - 1) / 2.0);
        
        // Ensure y is within bounds
        y = std::max(0, std::min(height - 1, y));
        
        // Draw the waveform
        m_visual[y][x] = '*';
    }
    
    return true;
}

bool WavVisual::generateSpectrum(int width, int height)
{
    // This is a placeholder implementation
    // In a real implementation, this would perform a Fourier transform
    // on the WAV file and generate a spectrum visualization
    
    // For now, just return false to indicate that this feature is not implemented
    return false;
}
