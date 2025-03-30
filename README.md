# Turnable - Terminal Audio Player

Turnable is a terminal-based audio player that allows you to play multiple sound files concurrently using threading. It features a vim-like navigation interface and supports WAV file playback.

## Features

- Play multiple sound files concurrently in separate threads
- Vim-like modes and navigation:
  - **Normal mode**: Default mode with hjkl navigation for selecting sound files
  - **Insert mode**: Placeholder for custom MIDI instrument keys (press 'i' to enter)
  - **Visual mode**: Shows a text-based visual representation of the selected WAV file (press 'v' to enter)
  - Press ESC to return to normal mode from any other mode
- Visual highlighting of the currently selected sound
- Real-time status display of currently playing sounds
- Dynamic loading of WAV files from a directory
- Accurate duration calculation for WAV files

## Object-Oriented Design

The application follows object-oriented design principles to improve modularity and maintainability:

### Class Structure

1. **AudioFile** (`meta_data/audfile.h`, `meta_data/audfile.cpp`)
   - Responsible for parsing WAV file headers and calculating durations
   - Validates WAV files and provides error handling
   - Encapsulates WAV file metadata

2. **Sounds** (`sounds/sounds.h`, `sounds/sounds.cpp`)
   - Handles sound playback using SFML
   - Provides methods to play, stop, and check playback status
   - Uses AudioFile for duration calculation

3. **Threading** (`threading/threading.h`, `threading/threading.cpp`)
   - Manages threads for concurrent sound playback
   - Provides methods to start, stop, and wait for threads
   - Handles thread lifecycle and resource management

4. **SoundPlayerUI** (`ui/sound_player_ui.h`, `ui/sound_player_ui.cpp`)
   - Manages the user interface using ncurses
   - Handles user input and navigation
   - Displays sound files, controls, and playback status
   - Coordinates between the UI and sound playback

### Modularity Benefits

1. **Separation of Concerns**
   - Each class has a specific responsibility
   - Changes to one component don't affect others
   - Easier to understand and maintain

2. **Reusability**
   - Components can be reused in other projects
   - For example, the Threading class could be used for other concurrent tasks

3. **Testability**
   - Classes can be tested independently
   - Easier to write unit tests for specific functionality

4. **Extensibility**
   - New features can be added by extending existing classes
   - New UI elements can be added without changing the core functionality

## Usage

1. Build the project using CMake:
   ```
   cd source
   cmake .
   make
   ```

2. Run the application:
   ```
   ./Turnable
   ```

3. Controls:
   - Use `h`, `j`, `k`, `l` (vim motions) to navigate between sound files
   - Press `Enter` or `Space` to toggle playback of the selected sound
   - Press `1-9` to toggle playback of specific sounds
   - Press `s` to stop all sounds
   - Press `w` to wait for all sound threads to complete
   - Press `q` to quit
