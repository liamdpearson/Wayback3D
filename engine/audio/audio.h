#pragma once

#include "../graphics/graphics.h"

class AudioSource : public Object
{
    int index;
};

int initAudio();

void playSound2D(const char* path);

void uninitAudio();