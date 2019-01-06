// Copyright (c) 2018 HALX99.
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
#include <functional>

#include "platform/CCPlatformMacros.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/CCFileUtils-android.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

class CC_DLL AudioFileHelper {
public:
    AudioFileHelper();
    ~AudioFileHelper();

    bool open(const std::string& path, bool readonly = true);
    int close();

    int seek(long offset, int origin);
    int read(void* buf, unsigned int size);

    int write(const void* buf, unsigned int size);

    int getFileDescriptor() const {
        return _fd;
    }
private:
    union {
        int _fd;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        AAsset* _asset;
#endif
    };
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    off_t _start;
    off_t _length;
#endif

    std::function<int(void*, unsigned int)> __read;
    std::function<long(long, int)> __lseek;
    std::function<int()> __close;
};

