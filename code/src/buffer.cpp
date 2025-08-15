#include "buffer.hpp"
#include <cstddef>
#include <iostream>

Buffer::Buffer() { }

Buffer::~Buffer() { }

ssize_t Buffer::size() const { return buffer.size(); }

const char* Buffer::c_str() const { return buffer.c_str(); }

void Buffer::append(const char *_str, size_t _size)
{
    for (int i = 0; i < _size; ++i)
    {
        if (_str[i] == '\0') break;
        buffer.push_back(_str[i]);
    }
}

void Buffer::clear()
{
    buffer.clear();
}

void Buffer::getline()
{
    buffer.clear();
    std::getline(std::cin, buffer);
}
