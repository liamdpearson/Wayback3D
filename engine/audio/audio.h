#pragma once

#include "../graphics/graphics.h"


struct SoundHandle
{
    int index = -1;
};

class AudioSource : public Object
{
    private:
        std::string path;
    
    public:
        AudioSource() = default;

        void Compose() override;

        void Play();

        void setPath(const std::string& p) { this->path = p; }
        std::string getPath() { return this->path; }


};

int initAudio();

void playSound2D(const char* path);

void updateAudio(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up);

void uninitAudio();