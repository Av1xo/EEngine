#pragma once

#include <string>
#include <vector>
#include <functional>
#include <cstdint>

/**
 * @brief Утилітний клас для роботи з шляхами файлів та директорій.
 */
class Path
{
public:
    /**
     * @brief Нормалізує шлях (змінює роздільники, видаляє зайві елементи).
     * @param path Вхідний шлях.
     * @return Нормалізований шлях.
     */
    static std::string normalize(const std::string& path);

    /**
     * @brief Об'єднує два шляхи в один.
     * @param a Перший шлях.
     * @param b Другий шлях.
     * @return Об'єднаний шлях.
     */
    static std::string join(const std::string& a, const std::string& b);

    /**
     * @brief Повертає директорію з повного шляху.
     * @param path Вхідний шлях.
     * @return Директорія як рядок.
     */
    static std::string getDirectory(const std::string& path);

    /**
     * @brief Повертає ім'я файлу з шляху.
     * @param path Вхідний шлях.
     * @return Ім'я файлу.
     */
    static std::string getFilename(const std::string& path);

    /**
     * @brief Повертає розширення файлу (без крапки).
     * @param path Вхідний шлях.
     * @return Розширення файлу.
     */
    static std::string getExtention(const std::string& path);

    /**
     * @brief Повертає ім'я файлу без розширення.
     * @param path Вхідний шлях.
     * @return Ім'я файлу без розширення.
     */
    static std::string getFilenameWithoutExtention(const std::string& path);

    /**
     * @brief Перевіряє, чи є шлях абсолютним.
     * @param path Вхідний шлях.
     * @return true, якщо шлях абсолютний, інакше false.
     */
    static bool isAbsolute(const std::string& path);

    /// Символ роздільника шляху (залежить від платформи)
    static const char separator;
};

/**
 * @brief Утилітний клас для роботи з файлами.
 */
class File
{
public:
    /**
     * @brief Перевіряє наявність файлу або директорії.
     * @param path Шлях для перевірки.
     * @return true, якщо існує.
     */
    static bool exists(const std::string& path);

    /**
     * @brief Перевіряє, чи є шлях файлом.
     * @param path Шлях для перевірки.
     * @return true, якщо файл.
     */
    static bool isFile(const std::string& path);

    /**
     * @brief Перевіряє, чи є шлях директорією.
     * @param path Шлях для перевірки.
     * @return true, якщо директорія.
     */
    static bool isDirectory(const std::string& path);

    /**
     * @brief Повертає розмір файлу в байтах.
     * @param path Шлях до файлу.
     * @return Розмір файлу.
     */
    static uint64_t getSize(const std::string& path);

    /**
     * @brief Повертає час останньої модифікації файлу.
     * @param path Шлях до файлу.
     * @return Час останньої модифікації у форматі UNIX timestamp.
     */
    static uint64_t getLastModifiedTime(const std::string& path);

    /**
     * @brief Зчитує бінарні дані з файлу.
     * @param path Шлях до файлу.
     * @return Вектор байтів.
     */
    static std::vector<uint8_t> readBinary(const std::string& path);

    /**
     * @brief Зчитує текст з файлу.
     * @param path Шлях до файлу.
     * @return Рядок з вмістом файлу.
     */
    static std::string readText(const std::string& path);

    /**
     * @brief Зчитує всі рядки файлу у вектор.
     * @param path Шлях до файлу.
     * @return Вектор рядків.
     */
    static std::vector<std::string> readLines(const std::string& path);

    /**
     * @brief Записує бінарні дані у файл.
     * @param path Шлях до файлу.
     * @param data Вказівник на дані.
     * @param size Розмір даних у байтах.
     * @return true, якщо запис успішний.
     */
    static bool writeBinary(const std::string& path, const void* data, size_t size);

    /**
     * @brief Записує текст у файл (перезаписує існуючий вміст).
     * @param path Шлях до файлу.
     * @param data Рядок для запису.
     * @return true, якщо запис успішний.
     */
    static bool writeText(const std::string& path, const std::string& data);

    /**
     * @brief Додає текст у кінець файлу.
     * @param path Шлях до файлу.
     * @param data Рядок для додавання.
     * @return true, якщо додавання успішне.
     */
    static bool appendText(const std::string& path, const std::string& data);

    /**
     * @brief Видаляє файл.
     * @param path Шлях до файлу.
     * @return true, якщо видалення успішне.
     */
    static bool deleteFile(const std::string& path);

    /**
     * @brief Копіює файл.
     * @param from Шлях джерела.
     * @param to Шлях призначення.
     * @return true, якщо копіювання успішне.
     */
    static bool copy(const std::string& from, const std::string& to);

    /**
     * @brief Переміщує файл.
     * @param from Шлях джерела.
     * @param to Шлях призначення.
     * @return true, якщо переміщення успішне.
     */
    static bool move(const std::string& from, const std::string& to);

    /**
     * @brief Перейменовує файл.
     * @param oldPath Старий шлях.
     * @param newPath Новий шлях.
     * @return true, якщо перейменування успішне.
     */
    static bool rename(const std::string& oldPath, const std::string& newPath);
};

/**
 * @brief Утилітний клас для роботи з директоріями.
 */
class Directory
{
public:
    /**
     * @brief Створює директорію.
     * @param path Шлях до директорії.
     * @return true, якщо створення успішне.
     */
    static bool create(const std::string& path);

    /**
     * @brief Рекурсивно створює директорію та усі проміжні шляхи.
     * @param path Шлях до директорії.
     * @return true, якщо створення успішне.
     */
    static bool createRecursive(const std::string& path);

    /**
     * @brief Видаляє директорію.
     * @param path Шлях до директорії.
     * @param recursive Якщо true, видаляє всі вкладені файли та директорії.
     * @return true, якщо видалення успішне.
     */
    static bool deleteDir(const std::string& path, bool recursive = false);

    /**
     * @brief Перевіряє, чи існує директорія.
     * @param path Шлях до директорії.
     * @return true, якщо директорія існує.
     */
    static bool exists(const std::string& path);

    /**
     * @brief Повертає список файлів у директорії.
     * @param path Шлях до директорії.
     * @return Вектор імен файлів.
     */
    static std::vector<std::string> listFiles(const std::string& path);

    /**
     * @brief Повертає список піддиректорій.
     * @param path Шлях до директорії.
     * @return Вектор імен піддиректорій.
     */
    static std::vector<std::string> listDirectories(const std::string& path);

    /**
     * @brief Повертає список усіх об'єктів (файли + директорії).
     * @param path Шлях до директорії.
     * @return Вектор імен файлів та директорій.
     */
    static std::vector<std::string> listAll(const std::string& path);

    /**
     * @brief Повертає поточну робочу директорію.
     * @return Поточна директорія як рядок.
     */
    static std::string getCurrent();

    /**
     * @brief Повертає шлях до виконуваного файлу.
     * @return Шлях до виконуваного файлу.
     */
    static std::string getExecutable();

    /**
     * @brief Повертає домашню директорію користувача.
     * @return Шлях до домашньої директорії.
     */
    static std::string getUserHome();

    /**
     * @brief Повертає тимчасову директорію.
     * @return Шлях до тимчасової директорії.
     */
    static std::string getTemp();
};

/**
 * @brief Асинхронне зчитування файлів.
 */
class AsyncFile
{
public:
    using ReadCallback = std::function<void(std::vector<uint8_t>)>;
    using ErrorCallback = std::function<void(const std::string&)>;

    /**
     * @brief Асинхронне зчитування бінарного файлу.
     * @param path Шлях до файлу.
     * @param onSuccess Колбек, що викликається при успішному зчитуванні.
     * @param onError Колбек, що викликається у випадку помилки.
     */
    static void ReadBinaryAsync(const std::string& path,
        ReadCallback onSuccess,
        ErrorCallback onError = nullptr);
};
