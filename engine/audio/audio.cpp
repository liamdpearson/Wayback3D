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

glm::vec3 listener;

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
    ma_engine_set_volume(&engine, 2.0f);

    return 1;
}

static void releaseVoices()
{
    for (int i = 0; i < MAX_VOICES; ++i)
    {
        if (voices[i].active && ma_sound_at_end(&voices[i].sound))
            voices[i].active = false;
    }
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

static int initSound(const char* path, const glm::vec3& pos, float vol, bool loop)
{
    int i = findEmpty();
    if (i < 0) {
        std::cout << "Voices full\n"; return -1;
    }
    
    ma_sound& sound = voices[i].sound;

    ma_sound_init_from_file(&engine, path, MA_SOUND_FLAG_DECODE,
                            NULL, NULL, &sound);
    ma_sound_set_position(&sound, pos.x, pos.y, pos.z);
    ma_sound_set_attenuation_model(&sound, ma_attenuation_model_linear);
    ma_sound_set_rolloff(&sound, 0.0f);

    ma_sound_set_volume(&sound, vol);
    ma_sound_set_looping(&sound, loop ? MA_TRUE : MA_FALSE);

    return i;
}

void updateAudio(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up)
{

    if (!initialized) return;

    releaseVoices();

    listener = pos;

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
    int i = this->getIndex();
    if (i >= 0) {
        if (ma_sound_at_end(&voices[i].sound)) {
            this->setIndex(-1);
            Object::Compose();
            return;
        }

        ma_sound& sound = voices[i].sound;
        glm::vec3 pos = this->getWorld()[3];
        ma_sound_set_position(&sound, pos.x, pos.y, pos.z);
        float dist = glm::length(listener - pos);
        float vol;
        if (dist < this->maxDistance) {
            float a = 1/(1+(dist/this->falloff)*(dist/this->falloff));
            float b = 1/(1+(this->maxDistance/this->falloff)*(this->maxDistance/this->falloff));
            float denom = 1 - b;

            vol = this->volume * (a - b) / denom;
        } else {
            vol = 0.0f;
        }
        
        ma_sound_set_volume(&sound, vol);
    }
    Object::Compose();
}

void AudioSource::Play()
{
    if (this->getIndex() < 0) {
        int i = initSound(getPath().c_str(), this->getWorld()[3], volume, loop);
        if (i < 0) return;
        setIndex(i);
    }

    ma_sound_start(&voices[getIndex()].sound);
}

void AudioSource::setLoop(bool loop)
{
    this->loop = loop;
    if (this->getIndex() >= 0) {
        ma_sound_set_looping(&voices[this->getIndex()].sound, loop);
    }
}