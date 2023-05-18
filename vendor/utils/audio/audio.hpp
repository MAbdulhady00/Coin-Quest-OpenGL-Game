#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
class AudioPlayer
{
private:
    // make constructor private so that no one can create an instance of this class

public:
    AudioPlayer() {}

    Mix_Chunk *sound;
    void load(std::string path)
    { // Initialize SDL
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            std::cout << "SDL initialization failed: " << SDL_GetError() << std::endl;
            return;
        }

        // Initialize SDL Mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            std::cout << "SDL Mixer initialization failed: " << Mix_GetError() << std::endl;
            return;
        }

        // Load the sound file
        sound = Mix_LoadWAV(path.c_str());
        if (!sound)
        {
            std::cout << "Failed to load sound file: " << Mix_GetError() << std::endl;
            return;
        }
    }
    void play()
    {

        // Play the sound
        if (Mix_PlayChannel(-1, sound, 0) == -1)
        {
            std::cout << "Failed to play sound: " << Mix_GetError() << std::endl;
            return;
        }
    }

    void cleanUp()
    {
        // Clean up resources
        Mix_FreeChunk(sound);
        Mix_CloseAudio();
        SDL_Quit();
    }
    ~AudioPlayer()
    {
        cleanUp();
    }
};