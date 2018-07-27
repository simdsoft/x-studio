// Copyright (c) 2018 HALX99.
#include "audio/include/AudioFileHelper.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/CCFileUtils-android.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

AudioFileHelper::AudioFileHelper() : _fd(-1), _start(0), _length(0)
{
}

AudioFileHelper::~AudioFileHelper()
{
    this->close();
}

bool AudioFileHelper::open(const std::string& path)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (path[0] != '/') {
        std::string relativePath;
        size_t position = path.find("assets/");
        if (0 == position)
        {
            // "assets/" is at the beginning of the path and we don't want it
            relativePath = path.substr(strlen("assets/"));
        }
        else
        {
            relativePath = path;
        }
        AAssetManager* asMgr = cocos2d::FileUtilsAndroid::getAssetManager();
        AAsset* asset = AAssetManager_open(asMgr, relativePath.c_str(), AASSET_MODE_UNKNOWN);
        if (asset == nullptr)
            return -1;

        _fd = AAsset_openFileDescriptor(asset, &_start, &_length);
        this->seek(0, SEEK_SET); // seek to start auto, ready to read
        AAsset_close(asset);
        return _fd != -1;
    }
    else {
        _fd = ::open(path.c_str(), O_RDONLY, S_IRUSR);
        return _fd != -1;
    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    _fd = ::_open(path.c_str(), O_BINARY | O_RDONLY, S_IREAD);
    return _fd != -1;
#else
    _fd = ::open(path.c_str(), O_RDONLY, S_IRUSR);
    return _fd != -1;
#endif
}

int AudioFileHelper::close()
{
    int iret = -1;
    if (_fd != -1) {
        iret = ::close(_fd);
        _fd = -1;
    }
    return iret;
}

int AudioFileHelper::seek(long offset, int origin)
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    return ::lseek(_fd, offset, origin);
#else
    long result = 0;

    switch (origin) {
    case SEEK_SET:
        result = ::lseek(_fd, _start + offset, SEEK_SET);
        break;
    case SEEK_CUR:
        result = ::lseek(_fd, offset, SEEK_CUR);
        break;
    case SEEK_END:
        result = ::lseek(_fd, _start + _length + offset, SEEK_SET);
        break;
    }
    
    if (result >= _start) return result - _start;
    return -1;
#endif
}

int AudioFileHelper::read(void* buf, unsigned int size)
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    return ::read(_fd, buf, size);
#else
    long offset = ::lseek(_fd, 0, SEEK_CUR);
    long remain = (_start + _length - offset);
    if (size > remain) {
        size = remain;
    }
    if (size > 0) return ::read(_fd, buf, size);
    return 0;
#endif
}
