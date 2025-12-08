#pragma once


#include "EverEngineCore/core/Event.h"
#include <memory>

class Engine
{
private:
    std::unique_ptr<class Window> m_window;
    EventDispatcher m_dispatcher;
public:
    Engine();
    virtual ~Engine();

    Engine(const Engine& other) = delete;
    Engine& operator=(const Engine& other) = delete;
    Engine(Engine&& other) = delete;
    Engine& operator=(Engine&& other) = delete;

    virtual int init(unsigned int window_width, unsigned int window_height, const char* title);

    virtual void set_eventCallback();

    virtual void on_update() {};

    int run();
    EventDispatcher& getDispatcher() { return m_dispatcher; }
};
