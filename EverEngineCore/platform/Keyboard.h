#pragma once

/**
 * @brief Уніфіковані кейкоди
 * 
 */
enum class KeyCode
{
    Unknown,

    // Функціональні та службові
    Esc,
    f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
    del,
    home,
    end,
    pgUp,
    pgDown,
    insert,
    pause,
    printScreen,
    scrollLock,

    // Цифровий ряд
    Tld,        // ~ `
    k1, k2, k3, k4, k5, k6, k7, k8, k9, k0,
    kSub,       // -
    kEq,        // =
    Backspace,

    // NumPad
    nLock,
    NumDiv,
    NumMul,
    NumSub,
    NumAdd,
    NumEnter,
    NumDot,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

    // Клавіші управління
    Tab,
    CapsLock,
    LShift,
    RShift,
    LCtrl,
    RCtrl,
    LAlt,
    RAlt,
    LSuper,   // Win / Cmd
    RSuper,   // Win / Cmd
    Menu,     // Context menu
    Enter,
    Space,

    // Стрілки
    ArrowUp,
    ArrowDown,
    ArrowLeft,
    ArrowRight,

    // Літерні клавіші
    Q, W, E, R, T, Y, U, I, O, P,
    A, S, D, F, G, H, J, K, L,
    Z, X, C, V, B, N, M,

    // Символи праворуч
    LBracket,      // [
    RBracket,      // ]
    Backslash,     // |
    Semicolon,     // ;
    Apostrophe,    // '
    Comma,         // ,
    Dot,           // .
    Slash,         // /
};
