#pragma once

#include "EverEngineCore/core/Event.h"
#include "EverEngineCore/platform/Keyboard.h"

#include <functional>
#include <memory>

struct GLFWwindow;

/**
 * @brief Клас, що інкапсулює роботу з вікном GLFW та обробкою подій.
 *
 * Клас відповідає за створення вікна, обробку вводу, виклики callback-функцій,
 * оновлення буферів та завершення роботи.
 */
class Window
{
public:
    /**
     * @brief Тип callback-функції, яка отримує події вікна.
     */
    using EventCallbackFn = std::function<void(std::unique_ptr<BaseEvent>)>;

    /**
     * @brief Створює нове вікно.
     * @param width Ширина вікна у пікселях.
     * @param height Висота вікна у пікселях.
     * @param title Назва вікна.
     */
    Window(const unsigned int width, const unsigned int height, const char* title);

    /**
     * @brief Звільняє ресурси та закриває вікно.
     */
    ~Window();

    /// Заборонено копіювання.
    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    /// Заборонено переміщення.
    Window(Window&& other) = delete;
    Window& operator=(Window&&) = delete;

    /**
     * @brief Реєструє всі необхідні callback-и GLFW.
     */
    void listenCallbacks();

    /**
     * @brief Перевіряє, чи має бути закрите вікно.
     * @return true якщо вікно має бути закрите.
     */
    bool shouldClose() const;

    /**
     * @brief Оновлює стан вікна: обробляє події та перемикає буфери.
     */
    void on_update();

    /**
     * @brief Отримує ширину вікна.
     * @return ширина у пікселях.
     */
    unsigned int get_width() const { return m_data.width; }

    /**
     * @brief Отримує висоту вікна.
     * @return висота у пікселях.
     */
    unsigned int get_height() const { return m_data.height; }

    /**
     * @brief Установлює callback-функцію для отримання всіх подій вікна.
     * @param callback Функція, яка буде викликана при надходженні події.
     */
    void set_event_callback(const EventCallbackFn& callback);

    /**
     * @brief Повертає внутрішній вказівник на GLFWwindow.
     * @return GLFWwindow*
     */
    GLFWwindow* getHandle() const { return m_window; }

    using ProcLoader = void*(*)(const char*);
    ProcLoader getProcLoader();
private:
    /**
     * @brief Структура для зберігання параметрів вікна.
     */
    struct WindowData
    {
        const char* title;               ///< Назва вікна.
        unsigned int width;              ///< Ширина вікна.
        unsigned int height;             ///< Висота вікна.
        EventCallbackFn eventCallbackFn; ///< Функція зворотного виклику подій.
    };

    /**
     * @brief Ініціалізує GLFW, створює вікно та налаштовує контекст.
     * @return 0 при успіху, код помилки при невдачі.
     */
    int init();

    /**
     * @brief Закриває вікно та звільняє всі ресурси GLFW.
     */
    void shutdown();

    /**
     * @brief Конвертує GLFW keycode у внутрішній KeyCode рушія.
     * @param glfwKey Код клавіші GLFW.
     * @return KeyCode відповідний клавіші.
     */
    static KeyCode toKeyCode(int glfwKey);

    /**
     * @brief Обробляє всі черги подій GLFW.
     */
    void pollEvents();

    /**
     * @brief Перемикає буфери при рендерингу.
     */
    void swap();

    GLFWwindow* m_window = nullptr; ///< Вказівник на вікно GLFW.
    WindowData m_data;              ///< Дані вікна.
};
