#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <thread>
#include "EverEngineCore/platform/filesystem/FileSystem.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <direct.h>
#define getcwd _getcwd
#define mkdir(path) _mkdir(path)
#else
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#endif

#ifdef PLATFORM_WINDOWS
    const char Path::separator = '\\';
#else
    const char Path::separator = '/';
#endif

std::string Path::normalize(const std::string& path) 
{
    std::string result =path;
    std::replace(result.begin(), result.end(),
        separator == '/' ? '\\' : '/',
        separator);

    return result;
}

std::string Path::join(const std::string& a, const std::string& b) 
{
    if (a.empty()) return b;
    if (b.empty()) return a;

    std::string result = a;
    if (result.back() != separator && result.back() != '/' && result.back() != '\\')
    {
        result += separator;
    }
    result += b;

    return normalize(result);
}

std::string Path::getDirectory(const std::string& path)
{
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return "";
    return path.substr(0, pos);
}

std::string Path::getFilename(const std::string& path) 
{
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

std::string Path::getExtention(const std::string& path) 
{
    std::string filename = getFilename(path);
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos) return "";
    return filename.substr(pos);
}

std::string Path::getFilenameWithoutExtention(const std::string& path) 
{
    std::string filename = getFilename(path);
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos) return filename;
    return filename.substr(0, pos);
}

bool Path::isAbsolute(const std::string& path) 
{
#ifdef PLATFORM_WINDOWS
    return path.length() >= 2 && path[1] == ':';
#else
    return !path.empty() && path[0] == '/';
#endif
}


// File

bool File::exists(const std::string& path) 
{
    std::ifstream file(path);
    return file.good();
}

bool File::isFile(const std::string& path) 
{
#ifdef PLATFORM_WINDOWS
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;
    return S_ISREG(st.st_mode);
#endif
}

bool File::isDirectory(const std::string& path) 
{
#ifdef PLATFORM_WINDOWS
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;
    return S_ISDIR(st.st_mode);
#endif
}

uint64_t File::getSize(const std::string& path) 
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return 0;
    return static_cast<uint64_t>(file.tellg());
}

uint64_t File::getLastModifiedTime(const std::string& path) 
{
#ifdef PLATFORM_WINDOWS
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data)) {
        return 0;
    }
    ULARGE_INTEGER ull;
    ull.LowPart = data.ftLastWriteTime.dwLowDateTime;
    ull.HighPart = data.ftLastWriteTime.dwHighDateTime;
    return ull.QuadPart;
#else
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return 0;
    return static_cast<uint64_t>(st.st_mtime);
#endif
}

std::vector<uint8_t> File::readBinary(const std::string& path) 
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        return {};
    }
    
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    
    return buffer;
}

std::string File::readText(const std::string& path) 
{
    std::ifstream file(path);
    if (!file) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::string> File::readLines(const std::string& path) 
{
    std::ifstream file(path);
    if (!file) {
        return {};
    }
    
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

bool File::writeBinary(const std::string& path, const void* data, size_t size) 
{
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;
    
    file.write(static_cast<const char*>(data), size);
    return file.good();
}

bool File::writeText(const std::string& path, const std::string& text) 
{
    std::ofstream file(path);
    if (!file) return false;
    
    file << text;
    return file.good();
}

bool File::appendText(const std::string& path, const std::string& text) 
{
    std::ofstream file(path, std::ios::app);
    if (!file) return false;
    
    file << text;
    return file.good();
}

bool File::deleteFile(const std::string& path) 
{
    return std::remove(path.c_str()) == 0;
}

bool File::copy(const std::string& src, const std::string& dst) 
{
    std::ifstream srcFile(src, std::ios::binary);
    std::ofstream dstFile(dst, std::ios::binary);
    
    if (!srcFile || !dstFile) return false;
    
    dstFile << srcFile.rdbuf();
    return true;
}

bool File::move(const std::string& src, const std::string& dst) 
{
    return std::rename(src.c_str(), dst.c_str()) == 0;
}

bool File::rename(const std::string& oldPath, const std::string& newPath) 
{
    return move(oldPath, newPath);
}


// Directory Implementation

bool Directory::create(const std::string& path) 
{
#ifdef PLATFORM_WINDOWS
    return _mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

bool Directory::createRecursive(const std::string& path) 
{
    if (exists(path)) return true;
    
    std::string parent = Path::getDirectory(path);
    if (!parent.empty() && !exists(parent)) {
        if (!createRecursive(parent)) return false;
    }
    
    return create(path);
}

bool Directory::deleteDir(const std::string& path, bool recursive) 
{
    if (!exists(path)) return false;
    
    if (recursive) {
        auto files = listFiles(path);
        for (const auto& file : files) {
            File::deleteFile(Path::join(path, file));
        }
        
        auto dirs = listDirectories(path);
        for (const auto& dir : dirs) {
            deleteDir(Path::join(path, dir), true);
        }
    }
    
#ifdef PLATFORM_WINDOWS
    return _rmdir(path.c_str()) == 0;
#else
    return rmdir(path.c_str()) == 0;
#endif
}

bool Directory::exists(const std::string& path) 
{
    return File::isDirectory(path);
}

std::vector<std::string> Directory::listFiles(const std::string& path) 
{
    std::vector<std::string> files;
    
#ifdef PLATFORM_WINDOWS
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((path + "\\*").c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                files.push_back(findData.cFileName);
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
#else
    DIR* dir = opendir(path.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string fullPath = Path::join(path, entry->d_name);
            if (File::isFile(fullPath)) {
                files.push_back(entry->d_name);
            }
        }
        closedir(dir);
    }
#endif
    
    return files;
}

std::vector<std::string> Directory::listDirectories(const std::string& path) 
{
    std::vector<std::string> dirs;
    
#ifdef PLATFORM_WINDOWS
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((path + "\\*").c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                strcmp(findData.cFileName, ".") != 0 &&
                strcmp(findData.cFileName, "..") != 0) {
                dirs.push_back(findData.cFileName);
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
#else
    DIR* dir = opendir(path.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            std::string fullPath = Path::join(path, entry->d_name);
            if (File::isDirectory(fullPath)) {
                dirs.push_back(entry->d_name);
            }
        }
        closedir(dir);
    }
#endif
    
    return dirs;
}

std::vector<std::string> Directory::listAll(const std::string& path) 
{
    auto files = listFiles(path);
    auto dirs = listDirectories(path);
    files.insert(files.end(), dirs.begin(), dirs.end());
    return files;
}

std::string Directory::getCurrent() 
{
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return std::string(buffer);
    }
    return "";
}

std::string Directory::getExecutable() 
{
#ifdef PLATFORM_WINDOWS
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    return Path::GetDirectory(buffer);
#else
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        return Path::getDirectory(buffer);
    }
    return getCurrent();
#endif
}

std::string Directory::getUserHome() 
{
#ifdef PLATFORM_WINDOWS
    char* userProfile = getenv("USERPROFILE");
    return userProfile ? userProfile : "";
#else
    char* home = getenv("HOME");
    return home ? home : "";
#endif
}

std::string Directory::getTemp() 
{
#ifdef PLATFORM_WINDOWS
    char buffer[MAX_PATH];
    GetTempPathA(MAX_PATH, buffer);
    return buffer;
#else
    return "/tmp";
#endif
}


// AsyncFile Implementation

void AsyncFile::ReadBinaryAsync(const std::string& path, 
                                ReadCallback onSuccess,
                                ErrorCallback onError) 
{
    std::thread([path, onSuccess, onError]() {
        try {
            auto data = File::readBinary(path);
            if (data.empty() && onError) {
                onError("Failed to read file: " + path);
            } else if (onSuccess) {
                onSuccess(std::move(data));
            }
        } catch (const std::exception& e) {
            if (onError) {
                onError(std::string("Exception: ") + e.what());
            }
        }
    }).detach();
}


