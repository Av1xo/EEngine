#include "Engine.h"
#include "Time.h"

Engine::Engine() {}
Engine::~Engine() {}

void Engine::run()
{
    Time::init();
    while (m_running)
    {  
        Time::update();

        m_running = false;
    }
}