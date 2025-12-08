#pragma once

class Time
{
public:
    static void init();
    static void update();
    static float delta_time();

private:
    static float s_deltaTime;
    static float s_lastFrame;
};