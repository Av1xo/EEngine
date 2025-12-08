cmake_minimum_required(VERSION 3.12)

set(SANDBOX_PROJECT_NAME EverEngineSandbox)

add_executable(${SANDBOX_PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${SANDBOX_PROJECT_NAME}
    EverEngineCore
)

target_compile_features(${SANDBOX_PROJECT_NAME} PUBLIC cxx_std_20)

set_target_properties(${SANDBOX_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
    OUTPUT_NAME "main"
)
