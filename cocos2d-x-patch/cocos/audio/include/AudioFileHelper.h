#pragma once

#include "platform/CCPlatformConfig.h"
#include <string>
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <errno.h>
#endif
#include <fcntl.h>

class AudioFile {
public:
    AudioFile();
    ~AudioFile();

    bool open(const std::string& path);
    int close();

    int seek(long offset, int origin);
    int read(void* buf, unsigned int size);

private:
    int _fd;
    off_t _start;
    off_t _length;
};

