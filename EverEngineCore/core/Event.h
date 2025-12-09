#pragma once

#include "EverEngineCore/platform/Keyboard.h"
#include <functional>
#include <vector>
#include <array>
#include <queue>
#include <mutex>
#include <memory>

/**
 * @enum EventType
 * @brief Перелік типів подій у системі
 * 
 * Визначає всі можливі типи подій, які можуть бути оброблені
 * диспетчером подій. EventCount використовується для визначення
 * розміру масиву обробників.
 */
enum class EventType
{
    WindowResize = 0,      ///< Подія зміни розміру вікна
    WindowClose,           ///< Подія закриття вікна
    KeyPressed,            ///< Подія натискання клавіші
    KeyReleased,           ///< Подія відпускання клавіші
    MouseButtonPressed,    ///< Подія натискання кнопки миші
    MouseButtonReleased,   ///< Подія відпускання кнопки миші
    MouseWheelScroll,      ///< Подія прокрутки коліщатка миші
    MouseMoved,            ///< Подія руху миші
    EventCount,            ///< Кількість типів подій (для внутрішнього використання)
};

/**
 * @struct BaseEvent
 * @brief Базова структура для всіх подій
 * 
 * Абстрактний базовий клас, від якого наслідуються всі конкретні типи подій.
 * Забезпечує поліморфну обробку подій через віртуальні функції.
 */
struct BaseEvent
{
    /**
     * @brief Віртуальний деструктор
     */
    virtual ~BaseEvent() = default;
    
    /**
     * @brief Отримує тип події
     * @return Тип події
     */
    virtual EventType get_type() const = 0;
};

/**
 * @class EventDispatcher
 * @brief Диспетчер подій для централізованої обробки системних повідомлень
 * 
 * EventDispatcher надає механізм для реєстрації обробників подій та
 * їх виконання. Підтримує багатопоточність через чергу подій з мьютексом.
 * Використовує патерн Observer для розсилки подій зареєстрованим слухачам.
 */
class EventDispatcher
{
public:
    /**
     * @brief Додає обробник події до диспетчера
     * 
     * Реєструє callback-функцію, яка буде викликана при надходженні
     * події типу EventT. Один тип події може мати декілька обробників.
     * 
     * @tparam EventT Тип події (має бути похідним від BaseEvent)
     * @param callback Функція-обробник, яка приймає посилання на подію
     * 
     * @code
     * dispatcher.add_event_listener<EventKeyPressed>([](EventKeyPressed& e) {
     *     if (e.key == KeyCode::Esc) {
     *         // Обробка натискання Escape
     *     }
     * });
     * @endcode
     */
    template<typename EventT>
    void add_event_listener(std::function<void(EventT&)> callback)
    {
        const size_t index = static_cast<size_t>(EventT::type);
        
        auto baseCallback = [func = std::move(callback)](BaseEvent& e)
        {
            func(static_cast<EventT&>(e));
        };
        
        m_eventCallbacks[index].push_back(std::move(baseCallback));
    }
    
    /**
     * @brief Додає подію до черги обробки
     * 
     * Потокобезпечно додає подію до черги. Подія буде оброблена
     * при наступному виклику process_event().
     * 
     * @param event Унікальний вказівник на подію
     */
    void post_event(std::unique_ptr<BaseEvent> event)
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_queue.push(std::move(event));
    }
    
    /**
     * @brief Обробляє всі події з черги
     * 
     * Витягує всі події з черги та викликає відповідні обробники.
     * Потокобезпечно копіює чергу у локальну змінну для обробки.
     * Має викликатись один раз на кадр в основному циклі.
     */
    void process_event()
    {
        std::queue<std::unique_ptr<BaseEvent>> localQueue;
        
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            std::swap(localQueue, m_queue);
        }
        
        while (!localQueue.empty())
        {
            auto& event = *localQueue.front();
            dispatch(event);
            localQueue.pop();
        }
    }
    
    /**
     * @brief Розсилає подію всім зареєстрованим обробникам
     * 
     * Викликає всі callback-функції, які були зареєстровані для
     * даного типу події.
     * 
     * @param event Посилання на подію для обробки
     */
    void dispatch(BaseEvent& event)
    {
        const size_t index = static_cast<size_t>(event.get_type());
        
        for (auto& callback : m_eventCallbacks[index])
        {
            callback(event);
        }
    }

private:
    /// Масив векторів обробників для кожного типу події
    std::array<std::vector<std::function<void(BaseEvent&)>>,
        static_cast<size_t>(EventType::EventCount)> m_eventCallbacks {};
    
    std::queue<std::unique_ptr<BaseEvent>> m_queue;  ///< Черга подій для обробки
    std::mutex m_queueMutex;                         ///< Мьютекс для потокобезпечного доступу до черги
};

/**
 * @struct EventWindowClose
 * @brief Подія закриття вікна
 * 
 * Генерується коли користувач намагається закрити вікно
 * (натискає хрестик, Alt+F4 тощо).
 */
struct EventWindowClose : public BaseEvent
{
    static inline const EventType type = EventType::WindowClose;  ///< Тип події
    
    EventType get_type() const override { return type; }
};

/**
 * @struct EventMouseMoved
 * @brief Подія руху миші
 * 
 * Генерується при зміні позиції курсора миші у вікні.
 */
struct EventMouseMoved : public BaseEvent
{
    static inline const EventType type = EventType::MouseMoved;  ///< Тип події
    
    double x;  ///< Координата X курсора в пікселях
    double y;  ///< Координата Y курсора в пікселях
    
    /**
     * @brief Конструктор події руху миші
     * @param nx Нова координата X
     * @param ny Нова координата Y
     */
    EventMouseMoved(double nx, double ny)
        : x(nx), y(ny) {}
    
    EventType get_type() const override { return type; }
};

/**
 * @struct EventWindowResize
 * @brief Подія зміни розміру вікна
 * 
 * Генерується коли користувач змінює розмір вікна.
 */
struct EventWindowResize : public BaseEvent
{
    static inline const EventType type = EventType::WindowResize;  ///< Тип події
    
    unsigned int width;   ///< Нова ширина вікна в пікселях
    unsigned int height;  ///< Нова висота вікна в пікселях
    
    /**
     * @brief Конструктор події зміни розміру
     * @param w Нова ширина
     * @param h Нова висота
     */
    EventWindowResize(unsigned int w, unsigned int h)
        : width(w), height(h) {}
    
    EventType get_type() const override { return type; }
};

/**
 * @struct EventKeyPressed
 * @brief Подія натискання клавіші
 * 
 * Генерується при натисканні клавіші на клавіатурі.
 * Може містити інформацію про автоповтор при утриманні клавіші.
 */
struct EventKeyPressed : public BaseEvent
{
    static const inline EventType type = EventType::KeyPressed;  ///< Тип події
    
    KeyCode key;   ///< Код натиснутої клавіші
    int scancode;  ///< Скан-код клавіші (залежить від платформи)
    int mods;      ///< Біт-маска модифікаторів (Shift, Ctrl, Alt тощо)
    bool repeat;   ///< true якщо це автоповтор при утриманні клавіші
    
    /**
     * @brief Конструктор події натискання клавіші
     * @param key Код клавіші
     * @param scancode Скан-код клавіші
     * @param mods Модифікатори
     * @param repeat Чи це автоповтор
     */
    EventKeyPressed(KeyCode key, int scancode, int mods, bool repeat)
        : key(key), scancode(scancode), mods(mods), repeat(repeat) {}
    
    EventType get_type() const override { return type; }
    
    /**
     * @brief Перевіряє чи є це автоповтором
     * @return true якщо клавіша утримується і це повторне спрацювання
     */
    bool isRepeat() { return repeat; }
};

/**
 * @struct EventKeyReleased
 * @brief Подія відпускання клавіші
 * 
 * Генерується коли користувач відпускає клавішу на клавіатурі.
 */
struct EventKeyReleased : public BaseEvent
{
    static const inline EventType type = EventType::KeyReleased;  ///< Тип події
    
    KeyCode key;   ///< Код відпущеної клавіші
    int scancode;  ///< Скан-код клавіші (залежить від платформи)
    int mods;      ///< Біт-маска модифікаторів (Shift, Ctrl, Alt тощо)
    
    /**
     * @brief Конструктор події відпускання клавіші
     * @param key Код клавіші
     * @param scancode Скан-код клавіші
     * @param mods Модифікатори
     */
    EventKeyReleased(KeyCode key, int scancode, int mods)
        : key(key), scancode(scancode), mods(mods) {}
    
    EventType get_type() const override { return type; }
};