#pragma once

#include <cstddef>
#include <string>
#include <sys/types.h>

class Buffer
{
    public:
        Buffer();
        ~Buffer();

        ssize_t size() const;
        const char* c_str() const;

        void append(const char *_str, size_t _size);
        void clear();

        void getline();
    
    private:
        std::string buffer;
};
