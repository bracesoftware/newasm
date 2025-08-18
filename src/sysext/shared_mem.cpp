// SharedMemory.cpp
// For NewASM Virtual Machine
// Credits C++ Community


// doesn' work
#if 0
#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace newasm{namespace _std{
class shared_memory {
public:
    shared_memory() : size_(0), ptr_(nullptr)
#ifdef _WIN32
    , hMap_(nullptr)
#endif
    {}

    ~shared_memory() {
        close();
    }

    size_t get_size() const { return size_; }

    // Kreira novi shared memory region
    bool create(const std::string& name, size_t size) {
        size_ = size;

#ifdef _WIN32
        hMap_ = CreateFileMappingA(
            INVALID_HANDLE_VALUE, // u RAM-u
            nullptr,
            PAGE_READWRITE,
            0,
            static_cast<DWORD>(size_),
            name.c_str()           // <-- ovo je "ključ"
        );
        if (!hMap_) return false;

        ptr_ = MapViewOfFile(hMap_, FILE_MAP_ALL_ACCESS, 0, 0, size_);
        return ptr_ != nullptr;
#else
        int fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666); // <-- ovo je "ključ"
        if (fd < 0) return false;
        if (ftruncate(fd, size_) != 0) return false;

        ptr_ = mmap(nullptr, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        return ptr_ != MAP_FAILED;
#endif
    }

    // Otvara postojeći shared memory region
    bool open(const std::string& name, size_t size) {
        size_ = size;

#ifdef _WIN32
        hMap_ = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name.c_str()); // <-- ključ
        if (!hMap_) return false;

        ptr_ = MapViewOfFile(hMap_, FILE_MAP_ALL_ACCESS, 0, 0, size_);
        return ptr_ != nullptr;
#else
        int fd = shm_open(name.c_str(), O_RDWR, 0666); // <-- ključ
        if (fd < 0) return false;

        ptr_ = mmap(nullptr, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        return ptr_ != MAP_FAILED;
#endif
    }

    // Piše podatke u shared memory (offset u bajtovima)
    bool write(const void* data, size_t bytes, size_t offset = 0) {
        if (!ptr_ || offset + bytes > size_) return false;
        std::memcpy(static_cast<char*>(ptr_) + offset, data, bytes);
        return true;
    }

    // Čita podatke iz shared memory (offset u bajtovima)
    bool read(void* buffer, size_t bytes, size_t offset = 0) {
        if (!ptr_ || offset + bytes > size_) return false;
        std::memcpy(buffer, static_cast<char*>(ptr_) + offset, bytes);
        return true;
    }

    void* get_ptr() const { return ptr_; }

    void close() {
        if (!ptr_) return;

#ifdef _WIN32
        UnmapViewOfFile(ptr_);
        if (hMap_) CloseHandle(hMap_);
        ptr_ = nullptr;
        hMap_ = nullptr;
#else
        munmap(ptr_, size_);
        ptr_ = nullptr;
#endif
    }

private:
    void* ptr_;
    size_t size_;
#ifdef _WIN32
    HANDLE hMap_;
#endif
};
}}
#endif