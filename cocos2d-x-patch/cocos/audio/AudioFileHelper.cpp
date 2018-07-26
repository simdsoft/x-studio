
#include "audio/include/AudioFileHelper.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <io.h>
#include <direct.h>
#include <fcntl.h>
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/CCFileUtils-android.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#endif

int openAudioFile(const std::string& path)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (path[0] != '/') {
        off_t start = 0, length = 0;
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

        int fd = AAsset_openFileDescriptor(asset, &start, &length);
        AAsset_close(asset);
        return fd;
    }
    else {
        return ::open(path.c_str(), O_RDONLY, S_IRUSR);
    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    return ::_open(path.c_str(), O_BINARY | O_RDONLY, S_IREAD);
#else
     return ::open(path.c_str(), O_RDONLY, S_IRUSR);
#endif
}

