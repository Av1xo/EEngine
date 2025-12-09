#pragma once

#include <cstdint>

/**
 * @brief Визначення платформ під час компіляції.
 */
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
    #ifdef _WIN64
        #define PLATFORM_64BIT
    #else
        #define PLATFORM_32BIT
    #endif
#elif defined(__linux__)
    #define PLATFORM_LINUX
    #if defined(__x86_64__) || defined(__aarch64__)
        #define PLATFORM_64BIT
    #else
        #define PLATFORM_32BIT
    #endif
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define PLATFORM_MACOS
    #endif
    #define PLATFORM_64BIT
#else
    #define PLATFORM_UNKNOWN
#endif

/**
 * @brief Визначення компілятора під час компіляції.
 */
#if defined(_MSC_VER)
    #define COMPILER_MSVC
#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_GCC
#elif defined(__clang__)
    #define COMPILER_CLANG
#endif

/**
 * @brief Визначення типу збірки (Debug/Release).
 */
#if defined(_DEBUG) || defined(DEBUG)
    #define BUILD_DEBUG
#else
    #define BUILD_RELEASE
#endif

/**
 * @brief Перелік підтримуваних операційних систем.
 */
enum class OS {
    Windows,   ///< Windows
    Linux,     ///< Linux
    MacOS,     ///< MacOS
    Unknown    ///< Невідомо
};

/**
 * @brief Перелік архітектур процесора.
 */
enum class Architecture {
    x86,    ///< 32-біт
    x64,    ///< 64-біт
    ARM,    ///< ARM 32-біт
    ARM64,  ///< ARM 64-біт
    Unknown ///< Невідомо
};

/**
 * @brief Перелік компіляторів.
 */
enum class Compiler {
    MSVC,    ///< Microsoft Visual C++
    GCC,     ///< GCC
    Clang,   ///< Clang/LLVM
    Unknown  ///< Невідомо
};

/**
 * @brief Тип збірки.
 */
enum class BuildType {
    Debug,    ///< Налагоджувальна збірка
    Release   ///< Релізна збірка
};

/**
 * @brief Клас для отримання інформації про платформу, компілятор і збірку.
 */
class Info
{
public:
    /**
     * @brief Повертає операційну систему.
     * @return OS Значення з enum OS.
     */
    static OS GetOS();

    /**
     * @brief Повертає архітектуру процесора.
     * @return Architecture Значення з enum Architecture.
     */
    static Architecture GetArchitecture();

    /**
     * @brief Повертає компілятор.
     * @return Compiler Значення з enum Compiler.
     */
    static Compiler GetCompiler();

    /**
     * @brief Повертає тип збірки.
     * @return BuildType Значення з enum BuildType.
     */
    static BuildType GetBuildType();

    /**
     * @brief Повертає назву операційної системи у вигляді рядка.
     * @return const char* Ім'я ОС.
     */
    static const char* GetOSName();

    /**
     * @brief Повертає назву архітектури у вигляді рядка.
     * @return const char* Ім'я архітектури.
     */
    static const char* GetArchitectureName();

    /**
     * @brief Повертає назву компілятора у вигляді рядка.
     * @return const char* Ім'я компілятора.
     */
    static const char* GetCompilerName();

    /**
     * @brief Перевіряє, чи ОС є Windows.
     * @return true якщо Windows.
     */
    static bool IsWindows();

    /**
     * @brief Перевіряє, чи ОС є Linux.
     * @return true якщо Linux.
     */
    static bool IsLinux();

    /**
     * @brief Перевіряє, чи ОС є MacOS.
     * @return true якщо MacOS.
     */
    static bool IsMacOS();

    /**
     * @brief Перевіряє, чи збірка є налагоджувальною (Debug).
     * @return true якщо Debug.
     */
    static bool IsDebug();

    /**
     * @brief Перевіряє, чи архітектура є 64-бітною.
     * @return true якщо 64-біт.
     */
    static bool Is64Bit();

    /**
     * @brief Виводить інформацію про ОС, архітектуру, компілятор та тип збірки у консоль.
     */
    static void Print();
};
