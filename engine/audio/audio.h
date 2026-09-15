#pragma once

#include "../graphics/graphics.h"

// struct VoiceHandle
// {
//     int index;
// };

int initAudio();

void playSound2D(const char* path);

void uninitAudio();