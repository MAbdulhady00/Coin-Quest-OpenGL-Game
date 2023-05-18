#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <unordered_map>
#include <iostream>
class AudioPlayer
{
private:
    // make constructor private so that no one can create an instance of this class

public:
    AudioPlayer() {}
    std::unordered_map<int, Mix_Chunk *> sounds;
    void init()
    {
        // Initialize SDL
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

        // set volume
        if (Mix_Volume(-1, 64) < 0)
        {
            std::cout << "SDL Mixer volume set failed: " << Mix_GetError() << std::endl;
            return;
        }
    }
    void load(std::string path, int name)
    {

        // Load the sound file
        sounds[name] = Mix_LoadWAV(path.c_str());
        if (!sounds[name])
        {
            std::cout << "Failed to load sound file: " << Mix_GetError() << std::endl;
            return;
        }
    }
    void play(std::string path, int name)
    {
        auto it = sounds.find(name);
        if (it == sounds.end())
        {
            // load the sound
            load(path, name);
        }
        // Play the sound
        if (Mix_PlayChannel(-1, sounds[name], 0) == -1)
        {
            std::cout << "Failed to play sound: " << Mix_GetError() << std::endl;
            return;
        }
    }

    void cleanUp()
    {
        // Clean up resources
        for (auto sound : sounds)
        {
            Mix_FreeChunk(sound.second);
        }
        Mix_CloseAudio();
        SDL_Quit();
    }
    ~AudioPlayer()
    {
        cleanUp();
    }
};