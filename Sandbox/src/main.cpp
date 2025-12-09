#include <EverEngineCore/core/Engine.h>
#include <EverEngineCore/core/Log.h>
#include <iostream>
#include <memory>

class SandBox: public Engine
{
public:
    virtual void set_eventCallback() override
    {
        getDispatcher().add_event_listener<EventWindowResize>(
            [](EventWindowResize& event)
            {
                std::cout << "Window resize: " << event.width << "x" << event.height << std::endl; 
            }
        );
        getInput().onKeyPressed(KeyCode::Space, [this]() {
            LOG_INFO("Jump!");
        });
    }
    virtual void on_update() override
    {
        if (getInput().isKeyDown(KeyCode::W))
        {
            LOG_INFO("Moving forward");
        }
        
        if (getInput().wasKeyPressedThisFrame(KeyCode::Esc))
        {
            LOG_INFO("Pause menu");
        }
    }
};

int main()
{
    std::cout << "Engine run" << std::endl;
    auto sandbox = std::make_unique<SandBox>();
    int returnCode = sandbox->init(1024, 720, "SandBox");
    if (returnCode){
        return returnCode;
    }
    returnCode = sandbox->run();

    std::cin.get();
    return returnCode;
}