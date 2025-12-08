add_library(
    EngineCore STATIC
    EverEngineCore/core/Engine.cpp
    EverEngineCore/Platform/Window.cpp
)

add_executable(Sandbox Sandbox/main.cpp)

target_link_libraries(Sandbox PRIVATE EngineCore)