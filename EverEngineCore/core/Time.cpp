#include "Time.h"
#include <chrono>

float Time::s_deltaTime = 0.0f;
float Time::s_lastFrame = 0.0f;

using clock = std::chrono::steady_clock;

void Time::init()
{
    s_lastFrame = std::chrono::duration<float>(clock::now().time_since_epoch()).count();
}

void Time::update()
{
    float current = std::chrono::duration<float>(clock::now().time_since_epoch()).count();
    s_deltaTime = current - s_lastFrame;
    s_lastFrame = current;
}

float Time::delta_time()
{
    return s_deltaTime;
}