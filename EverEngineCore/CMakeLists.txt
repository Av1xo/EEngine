cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME EverEngineCore)

# ---------------------
# PUBLIC HEADERS
# ---------------------
set(ENGINE_PUBLIC_INCLUDES 
    core/Engine.h
    core/Log.h
    core/Event.h
    core/Time.h
)

# ---------------------
# PRIVATE HEADERS
# ---------------------
set(ENGINE_PRIVATE_INCLUDES
    platform/Window.h
    platform/Input.h
    platform/Keyboard.h
    platform/filesystem/FileSystem.h
    platform/Platform.h
    rendering/renderer/API/OpenGL/OpenGLRendererAPI.h
    rendering/renderer/API/RendererAPI.h
    rendering/renderer/Renderer.h
    rendering/Shader.h
    rendering/Mesh.h
    scene/Scene.h
    scene/Entity.h
    scene/Component.h
)

# ---------------------
# SOURCE FILES
# ---------------------
set(ENGINE_PRIVATE_SOURCES
    core/Engine.cpp
    core/Time.cpp
    platform/Window.cpp
    platform/filesystem/FileSystem.cpp
    platform/Platform.cpp
    platform/Input.cpp
    rendering/renderer/API/OpenGL/OpenGLRendererAPI.cpp
    rendering/renderer/Renderer.cpp
    rendering/Shader.cpp
    rendering/Mesh.cpp
    scene/Scene.cpp
)

# Створення статичної бібліотеки
add_library(${ENGINE_PROJECT_NAME} STATIC
    ${ENGINE_PUBLIC_INCLUDES}
    ${ENGINE_PRIVATE_INCLUDES}
    ${ENGINE_PRIVATE_SOURCES}
)

# Include directories
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_20)

set_target_properties(${ENGINE_PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)

# ---------------------
# External libs
# ---------------------

# GLFW
find_package(glfw3 QUIET)
if(glfw3_FOUND)
    message(STATUS "Found system GLFW")
    target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC glfw)
else()
    message(STATUS "Using bundled GLFW from external/")
    if(EXISTS ${CMAKE_SOURCE_DIR}/external/glfw)
        add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
        target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC glfw)
    else()
        message(FATAL_ERROR "GLFW not found! Install it or add to external/")
    endif()
endif()

# spdlog
find_package(spdlog QUIET)
if(spdlog_FOUND)
    message(STATUS "Found system spdlog")
    target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC spdlog::spdlog)
else()
    message(STATUS "Using bundled spdlog from external/")
    if(EXISTS ${CMAKE_SOURCE_DIR}/external/spdlog)
        add_subdirectory(${CMAKE_SOURCE_DIR}/external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
        target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC spdlog)
    else()
        message(FATAL_ERROR "spdlog not found! Install it or add to external/")
    endif()
endif()

# fmt (потрібен для spdlog)
find_package(fmt QUIET)
if(fmt_FOUND)
    message(STATUS "Found system fmt")
    target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC fmt::fmt)
endif()

# glad
find_package(glad QUIET)
if(glad_FOUND)
    message(STATUS "Found system glad")
    target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC glad::glad)
else()
    message(STATUS "Using bundled glad from external/")
    if(EXISTS ${CMAKE_SOURCE_DIR}/external/glad)
        add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
        target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC glad)
    else()
        message(FATAL_ERROR "glad not found! Install it or add to external/")
    endif()
endif()

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC OpenGL::GL)

# GLEW
find_package(GLEW REQUIRED)
target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC GLEW::GLEW)

# Threads
find_package(Threads REQUIRED)
target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC Threads::Threads)

# libdl
target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})