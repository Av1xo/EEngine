#include "EverEngineCore/core/Engine.h"
#include "EverEngineCore/core/Time.h"
#include "EverEngineCore/platform/Window.h"

Engine::Engine() {}

Engine::~Engine() {}

int Engine::init(unsigned int window_width, unsigned int window_height, const char* title) {
    m_window = std::make_unique<Window>(window_width, window_height, title);
    Time::init();
    return 0;
}

void Engine::set_eventCallback()
{
    
}


int Engine::run()
{
    set_eventCallback();
    m_window->set_event_callback(
        [&](std::unique_ptr<BaseEvent> event){
            m_dispatcher.post_event(std::move(event));
        }
    );

    while (!m_window->shouldClose())
    {  
        Time::update();
        m_dispatcher.process_event();
        m_window->on_update();
        on_update();
    }
    m_window = nullptr;

    return 0;
}