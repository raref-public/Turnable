#include "midi_instrument.h"
#include <iostream>
#include <map>

// MidiNote implementation
MidiNote::MidiNote(int noteNumber, const std::string& noteName)
    : m_noteNumber(noteNumber)
    , m_noteName(noteName)
{
}

int MidiNote::getNoteNumber() const
{
    return m_noteNumber;
}

const std::string& MidiNote::getNoteName() const
{
    return m_noteName;
}

// MidiInstrument implementation
MidiInstrument::MidiInstrument(const std::string& name)
    : m_name(name)
{
    std::cout << "Created MIDI instrument: " << name << std::endl;
}

MidiInstrument::~MidiInstrument()
{
    std::cout << "Destroyed MIDI instrument: " << m_name << std::endl;
}

bool MidiInstrument::playNote(const MidiNote& note, int velocity)
{
    // This is a placeholder implementation
    std::cout << "Playing note " << note.getNoteName() 
              << " (MIDI note " << note.getNoteNumber() 
              << ") with velocity " << velocity 
              << " on instrument " << m_name << std::endl;
    return true;
}

bool MidiInstrument::stopNote(const MidiNote& note)
{
    // This is a placeholder implementation
    std::cout << "Stopping note " << note.getNoteName() 
              << " (MIDI note " << note.getNoteNumber() 
              << ") on instrument " << m_name << std::endl;
    return true;
}

const std::string& MidiInstrument::getName() const
{
    return m_name;
}

bool MidiInstrument::mapKeyToNote(char key, const MidiNote& note)
{
    // This is a placeholder implementation
    std::cout << "Mapping key '" << key 
              << "' to note " << note.getNoteName() 
              << " (MIDI note " << note.getNoteNumber() 
              << ") on instrument " << m_name << std::endl;
    return true;
}

const MidiNote* MidiInstrument::getNoteForKey(char key) const
{
    // This is a placeholder implementation
    // In a real implementation, this would look up the note in a map
    std::cout << "Looking up note for key '" << key 
              << "' on instrument " << m_name << std::endl;
    return nullptr;
}
