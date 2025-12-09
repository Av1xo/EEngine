#pragma once

#include "EverEngineCore/platform/Keyboard.h"
#include "EverEngineCore/core/Event.h"
#include <array>
#include <functional>
#include <vector>

/**
 * @brief Клас для обробки введення з клавіатури.
 * 
 * Забезпечує відстеження стану клавіш, підтримку подій натискання,
 * відпускання, повторного натискання та колбеків на клавіші.
 */
class Input
{
public:
    /// Тип колбеку для обробки подій клавіш
    using KeyEventCallback = std::function<void()>;

    /**
     * @brief Ініціалізація Input та підписка на EventDispatcher.
     * @param dispatcher Диспетчер подій, через який будуть приходити події клавіш.
     */
    void init(EventDispatcher& dispatcher);

    /**
     * @brief Додає колбек на подію натискання клавіші.
     * @param code Клавіша.
     * @param callback Функція, яка викликається при натисканні клавіші.
     */
    void onKeyPressed(KeyCode code, KeyEventCallback callback);

    /**
     * @brief Додає колбек на подію відпускання клавіші.
     * @param code Клавіша.
     * @param callback Функція, яка викликається при відпусканні клавіші.
     */
    void onKeyReleased(KeyCode code, KeyEventCallback callback);

    /**
     * @brief Перевіряє, чи клавіша зараз натиснута.
     * @param key Клавіша.
     * @return true, якщо клавіша натиснута.
     */
    bool isKeyDown(KeyCode key) const;

    /**
     * @brief Перевіряє, чи клавіша зараз відпущена.
     * @param key Клавіша.
     * @return true, якщо клавіша відпущена.
     */
    bool isKeyUp(KeyCode key) const;

    /**
     * @brief Перевіряє, чи клавіша була натиснута у поточному кадрі.
     * @param key Клавіша.
     * @return true, якщо клавіша натиснута в цьому кадрі.
     */
    bool wasKeyPressedThisFrame(KeyCode key) const;

    /**
     * @brief Перевіряє, чи клавіша була відпущена у поточному кадрі.
     * @param key Клавіша.
     * @return true, якщо клавіша відпущена в цьому кадрі.
     */
    bool wasKeyReleasedThisFrame(KeyCode key) const;

    /**
     * @brief Перевіряє, чи клавіша була повторно натиснута (repeat).
     * @param key Клавіша.
     * @return true, якщо клавіша повторно натиснута.
     */
    bool isKeyRepeated(KeyCode key) const;

    /**
     * @brief Завершення кадру. Очищає стан натискань/відпускань поточного кадру.
     */
    void endFrame();

private:
    /**
     * @brief Структура для зберігання стану однієї клавіші.
     */
    struct KeyState
    {
        bool down = false;             ///< Клавіша натиснута
        bool pressedThisFrame = false; ///< Натискання відбулося у цьому кадрі
        bool releasedThisFrame = false;///< Відпускання відбулося у цьому кадрі
        bool repeated = false;         ///< Повторне натискання клавіші
    };

    static constexpr size_t KEY_COUNT = static_cast<size_t>(KeyCode::Slash) + 1; ///< Кількість клавіш

    std::array<KeyState, KEY_COUNT> m_keyStates{}; ///< Масив станів клавіш
    
    std::array<std::vector<KeyEventCallback>, KEY_COUNT> m_pressCallbacks;   ///< Колбеки на натискання
    std::array<std::vector<KeyEventCallback>, KEY_COUNT> m_releaseCallbacks; ///< Колбеки на відпускання

    /**
     * @brief Обробка події натискання клавіші.
     * @param e Подія натискання клавіші.
     */
    void handleKeyPressed(EventKeyPressed& e);

    /**
     * @brief Обробка події відпускання клавіші.
     * @param e Подія відпускання клавіші.
     */
    void handleKeyReleased(EventKeyReleased& e);
    
    /**
     * @brief Конвертує KeyCode в індекс для масивів.
     * @param key Клавіша.
     * @return Індекс у масиві.
     */
    size_t toIndex(KeyCode key) const;
};
