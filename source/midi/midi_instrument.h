#ifndef MIDI_INSTRUMENT_H
#define MIDI_INSTRUMENT_H

#include <string>
#include <vector>

// Forward declaration
class MidiNote;

/**
 * @brief MidiInstrument class for handling MIDI instrument functionality
 * 
 * This is a placeholder class for future implementation of MIDI instrument
 * functionality. It will be used in the INSERT mode of the application.
 */
class MidiInstrument {
public:
    /**
     * @brief Construct a new Midi Instrument object
     * 
     * @param name The name of the instrument
     */
    MidiInstrument(const std::string& name);
    
    /**
     * @brief Destroy the Midi Instrument object
     */
    ~MidiInstrument();
    
    /**
     * @brief Play a note on the instrument
     * 
     * @param note The note to play
     * @param velocity The velocity of the note (0-127)
     * @return true if the note was played successfully
     * @return false if there was an error
     */
    bool playNote(const MidiNote& note, int velocity);
    
    /**
     * @brief Stop a note on the instrument
     * 
     * @param note The note to stop
     * @return true if the note was stopped successfully
     * @return false if there was an error
     */
    bool stopNote(const MidiNote& note);
    
    /**
     * @brief Get the name of the instrument
     * 
     * @return const std::string& The name of the instrument
     */
    const std::string& getName() const;
    
    /**
     * @brief Map a keyboard key to a MIDI note
     * 
     * @param key The keyboard key (ASCII value)
     * @param note The MIDI note to map to
     * @return true if the mapping was successful
     * @return false if there was an error
     */
    bool mapKeyToNote(char key, const MidiNote& note);
    
    /**
     * @brief Get the MIDI note mapped to a keyboard key
     * 
     * @param key The keyboard key (ASCII value)
     * @return const MidiNote* The MIDI note mapped to the key, or nullptr if no mapping exists
     */
    const MidiNote* getNoteForKey(char key) const;
    
private:
    std::string m_name;
    // This would contain the actual MIDI instrument implementation
    // For now, it's just a placeholder
};

/**
 * @brief MidiNote class for representing a MIDI note
 */
class MidiNote {
public:
    /**
     * @brief Construct a new Midi Note object
     * 
     * @param noteNumber The MIDI note number (0-127)
     * @param noteName The name of the note (e.g., "C4")
     */
    MidiNote(int noteNumber, const std::string& noteName);
    
    /**
     * @brief Get the MIDI note number
     * 
     * @return int The MIDI note number (0-127)
     */
    int getNoteNumber() const;
    
    /**
     * @brief Get the name of the note
     * 
     * @return const std::string& The name of the note (e.g., "C4")
     */
    const std::string& getNoteName() const;
    
private:
    int m_noteNumber;
    std::string m_noteName;
};

#endif // MIDI_INSTRUMENT_H
