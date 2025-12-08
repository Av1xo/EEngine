#include "Engine.h"
#include "Time.h"

Engine::Engine() {
    m_window = std::make_unique<Window>(1280, 720, "EverEngine");
    Time::init();
}

Engine::~Engine() {}

void Engine::run()
{
    while (!m_window->shouldClose())
    {  
        Time::update();
        m_window->pollEvents();
    }
}