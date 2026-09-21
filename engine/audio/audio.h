#pragma once

#include "../graphics/graphics.h"


// struct SoundHandle
// {
//     int index = -1;
// };

class AudioSource : public Object
{
    private:
        std::string path;
        int index = -1;

        
        bool loop = false;

    public:
        float volume = 1.0f;
        float maxDistance = 50.0f;
        float falloff = 1.0f;


        AudioSource() = default;

        void Compose() override;

        void Play();

        void setLoop(bool loop);
        bool getLoop() const { return this->loop; }

        void setPath(const std::string& p) { this->path = p; }
        std::string getPath() const { return this->path; }

        void setIndex(int i) { this->index = i; }
        int getIndex() const { return this->index; }


};

int initAudio();

void playSound2D(const char* path);

int playSound3D(const char* path, const glm::vec3& pos, float vol, bool loop);

void updateAudio(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up);

void uninitAudio();