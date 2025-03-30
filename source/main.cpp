#include <ui/sound_player_ui.h>

int main() {
    // Create the sound player UI
    SoundPlayerUI ui;
    
    // Initialize the UI
    ui.initialize();
    
    // Load sound files
    ui.loadSoundFiles("./source/audio_samples");
    
    // Run the UI
    ui.run();
    
    return 0;
}
