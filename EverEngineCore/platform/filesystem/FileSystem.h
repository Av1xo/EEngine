#pragma once

#include <string>
#include <vector>
#include <functional>
#include <cstdint>

class Path
{
public:
    static std::string normalize(const std::string& path);
    static std::string join(const std::string& a, const std::string& b);
    static std::string getDirectory(const std::string& path);
    static std::string getFilename(const std::string& path);
    static std::string getExtention(const std::string& path);
    static std::string getFilenameWithoutExtention(const std::string& path);
    static bool isAbsolute(const std::string& path);

    static const char separator;
};

class File
{
public:
    static bool exists(const std::string& path);
    static bool isFile(const std::string& path);
    static bool isDirectory(const std::string& path);

    static uint64_t getSize(const std::string& path);
    static uint64_t getLastModifiedTime(const std::string& path);

    static std::vector<uint8_t> readBinary(const std::string& path);
    static std::string readText(const std::string& path);
    static std::vector<std::string> readLines(const std::string& path);

    static bool writeBinary(const std::string& path, const void* data, size_t size);
    static bool writeText(const std::string& path, const std::string& data);
    static bool appendText(const std::string& path, const std::string& data);

    static bool deleteFile(const std::string& path);
    static bool copy(const std::string& from, const std::string& to);
    static bool move(const std::string& from, const std::string& to);
    static bool rename(const std::string& oldPath, const std::string& newPath);
};

class Directory
{
public:
    static bool create(const std::string& path);
    static bool createRecursive(const std::string& path);
    static bool deleteDir(const std::string& path, bool recursive = false);
    static bool exists(const std::string& path);

    static std::vector<std::string> listFiles(const std::string& path);
    static std::vector<std::string> listDirectories(const std::string& path);
    static std::vector<std::string> listAll(const std::string& path);

    static std::string getCurrent();
    static std::string getExecutable();
    static std::string getUserHome();
    static std::string getTemp();
};


class AsyncFile
{
public:
    using ReadCallback = std::function<void(std::vector<uint8_t>)>;
    using ErrorCallback = std::function<void(const std::string&)>;

    static void ReadBinaryAsync(const std::string& path,
        ReadCallback onSuccess,
        ErrorCallback onError = nullptr);
};