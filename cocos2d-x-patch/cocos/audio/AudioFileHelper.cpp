// Copyright (c) 2018 HALX99.
#include "audio/include/AudioFileHelper.h"

#include <sys/stat.h>

#if defined(_WIN32)
#define O_READ_FLAGS O_BINARY | O_RDONLY, S_IREAD
#define O_WRITE_FLAGS O_CREAT | O_RDWR | O_BINARY, S_IWRITE | S_IREAD
#define posix_open ::_open
#define posix_close ::_close
#define posix_lseek ::_lseek
#define posix_read ::_read
#define posix_write ::_write
#else
#define O_READ_FLAGS O_RDONLY, S_IRUSR
#define O_WRITE_FLAGS O_CREAT | O_RDWR, S_IRWXU
#define posix_open ::open
#define posix_close ::close
#define posix_lseek ::lseek
#define posix_read ::read
#define posix_write ::write
#endif

AudioFileHelper::AudioFileHelper() : _fd(-1)
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
,_start(0), _length(0)
#endif
{   /// Setup default read,lseek,close behavior
    this->__read = [=](void* buf, unsigned int size) {
        return posix_read(_fd, buf, size);
    };
    this->__lseek = [=](long offset, int origin) {
        return posix_lseek(_fd, offset, origin);
    };
    this->__close = [=]() {
        int iret = -1;
        if (_fd != -1) {
            iret = posix_close(_fd);
            _fd = -1;
        }
        return iret;
    };
}

AudioFileHelper::~AudioFileHelper()
{
    this->close();
}

bool AudioFileHelper::open(const std::string& path, bool readonly)
{
    bool succeed = false;
    if (readonly) {
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
                return false;

            _fd = AAsset_openFileDescriptor(asset, &_start, &_length); // Try to openFileDescriptor
            if(_fd != -1) {
                this->__read = [=](void* buf, unsigned int size) {
                    long offset = posix_lseek(_fd, 0, SEEK_CUR);
                    long remain = (_start + _length - offset);
                    if (size > remain) {
                        size = remain;
                    }
                    if (size > 0) return posix_read(_fd, buf, size);
                    return 0;
                };
                this->__lseek = [=](long offset, int origin) {

                    long result = 0;

                    switch (origin) {
                    case SEEK_SET:
                        result = posix_lseek(_fd, _start + offset, SEEK_SET);
                        break;
                    case SEEK_CUR:
                        result = posix_lseek(_fd, offset, SEEK_CUR);
                        break;
                    case SEEK_END:
                        result = posix_lseek(_fd, _start + _length + offset, SEEK_SET);
                        break;
                    }

                    if (result >= _start) return result - _start;
                    return (long)-1;
                };
                this->seek(0, SEEK_SET); // seek to start auto, ready to read
                AAsset_close(asset);
            }
            else {
                _asset = asset;
                 // setup file read/seek/close at here
                this->__read = [=](void* buf, unsigned int size){
                    return AAsset_read(this->_asset, buf, size);
                };
                this->__lseek = [=](long offset, int origin){
                    return AAsset_seek(this->_asset, offset, origin);
                };
                this->__close = [=](){
                    if(this->_asset) {
                        AAsset_close(this->_asset);
                        this->_asset = nullptr;
                    }
                    return 0;
                };
            }
            succeed = true;
        }
        else {
            succeed = (_fd = posix_open(path.c_str(), O_READ_FLAGS)) != -1;
        }
#else
        succeed = (_fd = posix_open(path.c_str(), O_READ_FLAGS)) != -1;
#endif
    }
    else {
        succeed = (_fd = posix_open(path.c_str(), O_WRITE_FLAGS)) != -1;
    }
    return succeed;
}

int AudioFileHelper::close()
{
    return this->__close();
}

int AudioFileHelper::seek(long offset, int origin)
{
    return this->__lseek(offset, origin);
}

int AudioFileHelper::read(void* buf, unsigned int size)
{
    return this->__read(buf, size);
}

int AudioFileHelper::write(const void* buf, unsigned int size)
{
    return posix_write(_fd, buf, size);
}
