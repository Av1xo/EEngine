
#pragma once
#include "EverEngineCore/core/Event.h"
#include "EverEngineCore/platform/Input.h"
#include <memory>

/**
 * @class Engine
 * @brief Базовий клас рушія
 * всі застосунки мають наслідуватись від нього
 * 
 */
class Engine
{
private:
    std::unique_ptr<class Window> m_window;  ///< Вказівник на вікно застосунку
    EventDispatcher m_dispatcher;             ///< Диспетчер подій для обробки системних повідомлень
    Input m_input;                            ///< Система вводу для обробки клавіатури та миші

public:
    /**
     * @brief Конструктор за замовчуванням
     * 
     * Ініціалізує базові компоненти рушія
     */
    Engine();
    
    /**
     * @brief Віртуальний деструктор
     * 
     * Забезпечує коректне знищення похідних класів
     */
    virtual ~Engine();

    /**
     * @brief Заборона копіювання
     */
    Engine(const Engine& other) = delete;
    
    /**
     * @brief Заборона присвоєння копіюванням
     */
    Engine& operator=(const Engine& other) = delete;
    
    /**
     * @brief Заборона переміщення
     */
    Engine(Engine&& other) = delete;
    
    /**
     * @brief Заборона присвоєння переміщенням
     */
    Engine& operator=(Engine&& other) = delete;

    /**
     * @brief Ініціалізує рушій та створює вікно
     * 
     * Створює вікно із заданими параметрами, ініціалізує систему часу
     * та систему вводу. Цей метод має бути викликаний перед run().
     * 
     * @param window_width Ширина вікна в пікселях
     * @param window_height Висота вікна в пікселях
     * @param title Заголовок вікна
     * @return 0 у випадку успіху, негативне значення при помилці
     */
    virtual int init(unsigned int window_width, unsigned int window_height, const char* title);

    /**
     * @brief Налаштовує обробники подій
     * 
     * Віртуальний метод, який похідні класи можуть перевизначити для
     * реєстрації власних обробників подій та біндингів клавіш.
     * Викликається автоматично в run() перед початком основного циклу.
     */
    virtual void set_eventCallback();

    /**
     * @brief Оновлення логіки застосунку
     * 
     * Віртуальний метод, який викликається кожен кадр.
     * Похідні класи повинні перевизначити цей метод для реалізації
     * власної логіки оновлення (рух об'єктів, фізика, AI тощо).
     */
    virtual void on_update() {};

    /**
     * @brief Запускає основний цикл движка
     * 
     * Виконує основний цикл застосунку: обробляє події, оновлює логіку,
     * рендерить кадр. Цикл працює доки вікно не буде закрите.
     * 
     * @return 0 у випадку успішного завершення
     */
    int run();
    
    /**
     * @brief Отримує посилання на диспетчер подій
     * 
     * @return Посилання на EventDispatcher для реєстрації обробників подій
     */
    EventDispatcher& getDispatcher() { return m_dispatcher; }
    
    /**
     * @brief Отримує посилання на систему вводу
     * 
     * @return Посилання на Input для обробки клавіатури та миші
     */
    Input& getInput() { return m_input; } 
};