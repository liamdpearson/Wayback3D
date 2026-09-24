#include "audio.h"

#include <miniaudio/miniaudio.h>

#include <glm/glm.hpp>


static ma_engine engine;
static bool initialized = false;

// only 32 voices allowed to play at once
const int MAX_VOICES = 32;

struct Voice
{
    ma_sound sound;
    bool active = false;
    bool initialized = false;
};

// cached for volume calculation and muffle checking
static glm::vec3 listener;

static Voice voices[MAX_VOICES];

int initAudio()
{
    if (initialized) return 1; // audio already initialized

    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        std::cout << "initAudio: Audio failed to initialize\n"
                  << ma_result_description(result) << '\n';
        return 0;
    }

    initialized = true;
    ma_engine_set_volume(&engine, 1.0f);

    return 1;
}

// no spacial audio - miniaudio handles ma_sound stuff
void playSound2D(const char* path)
{
    if (initialized) {
        ma_result result = ma_engine_play_sound(&engine, path, NULL);
        if (result != MA_SUCCESS) {
            std::cout << "Failed to play 2D sound " << path << '\n';
        }
    }
}

// finds first empty voice slot and returns index
static int findEmpty()
{
    for (int i = 0; i < MAX_VOICES; ++i)
    {
        if (!voices[i].active) return i;
    }
    return -1;
}

// initalizes sound and returns index to it in voices
static int initSound(const char* path, const glm::vec3& pos, float vol, bool loop)
{
    int i = findEmpty();
    if (i < 0) {
        std::cout << "Voices full\n"; return -1;
    }

    ma_sound& sound = voices[i].sound;

    if (voices[i].initialized) {
        ma_sound_uninit(&sound);
        voices[i].initialized = false;
    }
    
    ma_result result = ma_sound_init_from_file(&engine, path, MA_SOUND_FLAG_DECODE,
                                               NULL, NULL, &sound);                               
    if (result != MA_SUCCESS) {
        std::cout << "Failed to initialize " << path << '\n';
        return -1;
    }

    voices[i].active = true;
    voices[i].initialized = true;
  
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

    listener = pos;

    ma_engine_listener_set_position(&engine, 0, pos.x, pos.y, pos.z);
    ma_engine_listener_set_direction(&engine, 0, front.x, front.y, front.z);
    ma_engine_listener_set_world_up(&engine, 0, up.x, up.y, up.z);
}

// uninits all sounds
void uninitSounds()
{
    for (Voice& v : voices) {
        if (v.initialized) {
            ma_sound_stop(&v.sound);
            ma_sound_uninit(&v.sound);
            v.active = false;
            v.initialized = false;
        }
    }
}

// called when closing game
void uninitAudio()
{
    if (initialized) {
        uninitSounds();
        ma_engine_uninit(&engine);
        initialized = false;
    }

}

void AudioSource::Compose()
{
    Object::ComposeSelf();

    int i = this->getIndex();
    if (i >= 0) {
        ma_sound& sound = voices[i].sound;

        if (ma_sound_at_end(&sound)) {
            this->setIndex(-1);
            voices[i].active = false;
            for (std::unique_ptr<Object>& child : children) child->Compose();
            return;
        }

        glm::vec3 pos = this->getWorld()[3];
        ma_sound_set_position(&sound, pos.x, pos.y, pos.z);
        float dist = glm::length(listener - pos);

        if (falloff <= 0) falloff = 0.1f;
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
    for (std::unique_ptr<Object>& child : children) child->Compose();
}

void AudioSource::Play()
{
    if (this->getIndex() < 0) {
        int i = initSound(getPath().c_str(), this->getWorld()[3], volume, loop);
        if (i < 0) return;
        setIndex(i);
    }

    ma_sound& sound = voices[getIndex()].sound;
    ma_sound_stop(&sound);
    ma_sound_seek_to_pcm_frame(&sound, 0);
    ma_sound_start(&sound);
}

void AudioSource::Stop()
{
    int i = this->getIndex();
    if (i >= 0) {
        ma_sound& sound = voices[i].sound;
        ma_sound_stop(&sound);
        ma_sound_seek_to_pcm_frame(&sound, 0);
        voices[i].active = false;
        this->setIndex(-1);
    }
}

void AudioSource::setLoop(bool loop)
{
    this->loop = loop;
    if (this->getIndex() >= 0) {
        ma_sound_set_looping(&voices[this->getIndex()].sound, loop);
    }
}

AudioSource::~AudioSource()
{
    int i = this->getIndex();
    if (i >= 0 && initialized && voices[i].initialized) {
        ma_sound& sound = voices[i].sound;
        ma_sound_stop(&sound);
        ma_sound_uninit(&sound);
        voices[i].active = false;
        voices[i].initialized = false;
    }
}
