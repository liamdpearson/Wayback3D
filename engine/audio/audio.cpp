#include "audio.h"

#include <miniaudio/miniaudio.h>

#include <glm/glm.hpp>

#include <cstring>


static ma_engine engine;
static bool initialized = false;

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

static int findEmpty()
{
    for (int i = 0; i < MAX_VOICES; ++i)
    {
        if (!voices[i].active) return i;
    }
    return -1;
}

static SoundHandle initSound(const char* path, const glm::vec3& pos, float vol, bool loop)
{
    int i = findEmpty();
    if (i < 0) {
        std::cout << "Voices full\n"; return SoundHandle{};
    }

    SoundHandle sh = SoundHandle{i};
    
    ma_sound& sound = voices[i].sound;

    ma_sound_init_from_file(&engine, path, MA_SOUND_FLAG_DECODE,
                            NULL, NULL, &sound);
    ma_sound_set_position(&sound, pos.x, pos.y, pos.z);
    ma_sound_set_attenuation_model(&sound, ma_attenuation_model_inverse);

    // temp
    ma_sound_set_rolloff(&sound, 10.0f);    
    ma_sound_set_min_distance(&sound, 1.0f);
    ma_sound_set_max_distance(&sound, 40.0f);


    ma_sound_set_volume(&sound, vol);
    ma_sound_set_looping(&sound, loop ? MA_TRUE : MA_FALSE);
    ma_sound_start(&sound);

    return sh;
}

void updateAudio(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up)
{
    ma_engine_listener_set_position(&engine, 0, pos.x, pos.y, pos.z);
    ma_engine_listener_set_direction(&engine, 0, front.x, front.y, front.z);
    ma_engine_listener_set_world_up(&engine, 0, up.x, up.y, up.z);
    
}

void uninitAudio()
{
    ma_engine_uninit(&engine);
}

void AudioSource::Compose()
{

    Object::Compose();
}