#include "audio.h"

#include <miniaudio/miniaudio.h>

#include <glm/glm.hpp>


static ma_engine engine;
static bool initialized = false;

static glm::vec3 listener{0.0f};

const int MAX_VOICES = 32;

struct Voice
{
    ma_sound sound;
    bool active = false;
};

static Voice voices[MAX_VOICES];

int initAudio()
{
    if (initialized) return 0; // audio already initialized

    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        std::cout << "initAudio: Audio failed to initialize\n"
                  << ma_result_description(result) << '\n';
        return 0;
    }

    initialized = true;

    ma_device* device = ma_engine_get_device(&engine);

    return 1;
}

void playSound2D(const char* path)
{
    ma_engine_play_sound(&engine, path, NULL);
}

void uninitAudio()
{
    ma_engine_uninit(&engine);
}