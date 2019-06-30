//
// Copyright (c) 2014-2019 x-studio365 - All Rights Reserved
//
#include <stdio.h>
#include <string>
#include "cryptk/endian_portable.h"
#include "cryptk/nsconv.h"
#include "cryptk/fastest_csv_parser.h"
#include "cryptk/cryptk_wrapper.h"

#include "EncryptManager.h"
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "platform/win32/CCFileUtils-win32.h"
typedef cocos2d::FileUtilsWin32 FileUtilsImpl;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/CCFileUtils-android.h"
typedef cocos2d::FileUtilsAndroid FileUtilsImpl;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
#include "platform/winrt/CCFileUtilsWinRT.h"
typedef cocos2d::CCFileUtilsWinRT FileUtilsImpl;
#else /* ios or mac, pitfall you may need change this file name to EncryptManager.mm for xcode */
#import <Foundation/Foundation.h>
#include "platform/apple/CCFileUtils-apple.h"
typedef cocos2d::FileUtilsApple FileUtilsImpl;
#endif


using namespace purelib;

#pragma warning(disable:4996)

using namespace cocos2d;

#if COCOS2D_VERSION >= 0x00031701
#define CCCOMPAT_CONST_QUALIFIER const
#else
#define CCCOMPAT_CONST_QUALIFIER
#endif

namespace cport {
#if defined(_WIN32)
    inline int strcasecmp(const char* s1, const char* s2) {
        return stricmp(s1, s2);
    }
#else
    using ::strcasecmp;
#endif
}

class FileUtilsNoEncrypt : public FileUtilsImpl
{
public:
    using FileUtilsImpl::init; // cocos2d-x older version compatible, such as v3.10
};

class FileUtilsEncrypt : public FileUtilsImpl
{
public:
    /*pitfall:
	    For android, if you are use obb, you should initialize the static member `obbfile` of FileUtilsAndroid
	    only once, and don't delete it at destructor of FileUtilsAndroid.
	*/
    using FileUtilsImpl::init; // cocos2d-x older version compatible, such as v3.10

    /**
    pitfall:
    For cocos2d-x v3.10, you should change FileUtilsWin32's constructor access permission from 'private' to
    'protected' or 'public'
    */
    FileUtilsEncrypt(EncryptManager& manager) : encryptManager(manager)
    {
    }

    /**
    *  Gets string from a file.
    */
    std::string getStringFromFile(const std::string& filename) CCCOMPAT_CONST_QUALIFIER override
    {
#if COCOS2D_VERSION >= 0x00031300
        auto data = FileUtilsImpl::getStringFromFile(filename);
#elif COCOS2D_VERSION >= 0x00031200
        /*
         Because v3.12 original implement of getStringFromFile have: ```truncated s.resize(strlen(s.data()));```
         so we need fix it.
        */
        std::string data;
        FileUtils::getContents(filename, &data);
#else   // COCOS2D_VERSION < V3.11.1, string will truncated by std::string strcpy's behavior.
        Data rawData = FileUtilsImpl::getDataFromFile(filename);
        std::string data;
        if (!rawData.isNull()) {
            data.assign((const char*)rawData.getBytes(), rawData.getSize());
        }
#endif
        EncryptManager::SignInfo info;
        if (!data.empty() && encryptManager.parseSignInfo(data.c_str(), data.size(), &info)) {
            data.resize(data.size() - encryptManager._encryptSignKey.size());
            cryptk::aes::overlapped::decrypt(data, encryptManager._encryptKey.c_str(), AES_DEFAULT_KEY_BITS, encryptManager._encryptIvec.c_str());
            if (info.compressed) {
                return cryptk::zlib_uncompress<std::string>(data, info.original_size);
            }
            else {
                return data;
            }
        }
        return data;
    }

    /**
    *  Creates binary data from a file.
    *  @return A data object.
    */
    Data getDataFromFile(const std::string& filename) CCCOMPAT_CONST_QUALIFIER override
    {
        auto data = FileUtilsImpl::getDataFromFile(filename);
        EncryptManager::SignInfo info;
        if (data.getSize() > 0 && encryptManager.parseSignInfo((char*)data.getBytes(), data.getSize(), &info)) {
            size_t size = 0;
            cryptk::aes::privacy::mode_spec<>::decrypt(data.getBytes(), data.getSize() - encryptManager._encryptSignKey.size(), data.getBytes(), size, encryptManager._encryptKey.c_str(), AES_DEFAULT_KEY_BITS, encryptManager._encryptIvec.c_str());

            if (info.compressed) {
                auto uncomprData = cryptk::zlib_inflate<cryptk::streambuf>(cxx17::string_view((const char*)data.getBytes(), size), info.original_size);
                auto tempData = uncomprData.detach(size);

                data.clear();
                data.fastSet(tempData, size);
            }
            else {
                data.fastSet(data.getBytes(), size);
            }
        }

        return data;
    }

    /**
    *  Gets resource file data
    *
    *  @param[in]  filename The resource file name which contains the path.
    *  @param[in]  mode The read mode of the file.
    *  @param[out] size If the file read operation succeeds, it will be the data size, otherwise 0.
    *  @return Upon success, a pointer to the data is returned, otherwise NULL.
    *  @warning Recall: you are responsible for calling free() on any Non-NULL pointer returned.
    */
    virtual unsigned char* getFileData(const std::string& filename, const char* mode, ssize_t *size) CCCOMPAT_CONST_QUALIFIER override
    {
        auto data = FileUtilsImpl::getFileData(filename, mode, size);
        if (data != nullptr) {
            size_t outsize = 0;
            EncryptManager::SignInfo info;
            if (*size > 0 && encryptManager.parseSignInfo((char*)data, *size, &info)) {
                *size -= static_cast<ssize_t>(encryptManager._encryptSignKey.size());
                cryptk::aes::privacy::mode_spec<>::decrypt(data, *size, data, outsize, encryptManager._encryptKey.c_str(), AES_DEFAULT_KEY_BITS, encryptManager._encryptIvec.c_str());

                if (info.compressed) {
                    auto uncomprData = cryptk::zlib_inflate<cryptk::streambuf>(cxx17::string_view((const char*)data, outsize), info.original_size);

                    free(data);
                    data = (unsigned char*)(uncomprData.detach(*size));
                }
                else {
                    *size = static_cast<ssize_t>(outsize);
                }
            }
        }
        return data;
    }

#if COCOS2D_VERSION <= 0x00031200 && (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    // COCOS2D_VERSION <= v3.12, Apple needed
    virtual ValueMap getValueMapFromFile(const std::string& filename) override
    {
        auto strXml = this->getStringFromFile(filename);
        return getValueMapFromData(strXml.c_str(), static_cast<int>(strXml.length()));
    }
#endif

    EncryptManager& encryptManager;
};

EncryptManager* EncryptManager::getInstance()
{
    static EncryptManager s_EncryptManager;
    return &s_EncryptManager;
}

std::string EncryptManager::decryptData(std::string data)
{
    EncryptManager::SignInfo info;
    if (!data.empty() && parseSignInfo(data.c_str(), data.size(), &info)) {
        data.resize(data.size() - _encryptSignKey.size());
        cryptk::aes::overlapped::decrypt(data, _encryptKey.c_str(), AES_DEFAULT_KEY_BITS, _encryptIvec.c_str());
        if (info.compressed) {
            return cryptk::zlib_uncompress<std::string>(data, info.original_size);
        }
        else {
            return data;
        }
    }
    return data;
}

void EncryptManager::setEncryptEnabled(bool bVal, cxx17::string_view key, cxx17::string_view ivec, int flags)
{
    if (bVal && !key.empty()) {
        _encryptKey.clear();
        _encryptIvec.clear();

        _encryptKey.resize(32);
        int keysize = key.size();
        if (keysize > 32)
            keysize = 32;

        ::memcpy(&_encryptKey.front(), key.data(), keysize);

        if (!ivec.empty()) {
            _encryptIvec.resize(16);
            ::memcpy(&_encryptIvec.front(), ivec.data(), (std::min)(16, (int)ivec.size()));
        }
        else {
            _encryptIvec = nsc::hex2bin("00234b89aa96fecdaf80fbf178a25621");
        }

        setupHookFuncs();
        _encryptEnabled = bVal;
        _encryptFlags = flags;

        if (flags & ENCF_SIGNATURE)
        {
            _encryptSignKey = _encryptIvec;
            int roll = (flags >> 16 & 0xffff); 
            std::string signvec(this->_encryptKey.c_str(), (std::min)(key.size(), ivec.size()));
            do {
                cryptk::aes::detail::ecb_encrypt_block(_encryptSignKey.c_str(),
                    _encryptSignKey.size(),
                    &_encryptSignKey.front(), signvec.c_str(), 128);
                signvec = this->_encryptSignKey;
            } while (--roll > 0);
        }
    }
    else {
        auto fileUtilsNoEncrypt = new FileUtilsNoEncrypt();
        fileUtilsNoEncrypt->init();
        fileUtilsNoEncrypt->setSearchPaths(FileUtils::getInstance()->getSearchPaths());
        FileUtils::setDelegate(fileUtilsNoEncrypt);

        _encryptEnabled = false;
        _encryptFlags = ENCF_NOFLAGS;
    }
}

bool EncryptManager::parseSignInfo(const char* data, size_t len, SignInfo* info) const
{
    if (_encryptFlags & ENCF_SIGNATURE) {
        if (len > 16) {

            char signbuf[16];
            memcpy(signbuf, data + len - 16, 16);
            auto wrptr = signbuf;
            cryptk::aes::detail::ecb_decrypt_block(wrptr,
                16,
                wrptr,
                _encryptSignKey.c_str(),
                128);

            memcpy(&info->mask, wrptr, sizeof(info->mask));
            wrptr += sizeof(info->mask);
            memcpy(&info->sigval, wrptr, sizeof(info->sigval));
            wrptr += sizeof(info->sigval);
            if((info->mask ^ info->sigval) == 0xdeadbeef) {
                info->flags = *((uint8_t*)wrptr++);
                memcpy(&info->original_size, wrptr, sizeof(info->original_size));
                return true;
            }
        }
        return false;
    }

    // No signature, regards all files as encrypted
    info->compressed = this->_encryptFlags & ENCF_COMPRESS;
    return true;
}

void EncryptManager::setupHookFuncs()
{
    auto fileUtilsEncrypt = new FileUtilsEncrypt(*this);
    fileUtilsEncrypt->init();
    fileUtilsEncrypt->setSearchPaths(FileUtils::getInstance()->getSearchPaths());
    FileUtils::setDelegate(fileUtilsEncrypt);

    std::string writablePath = fileUtilsEncrypt->getWritablePath();
    cocos2d::log("Writable Path:%s", writablePath.c_str());
    fileUtilsEncrypt->addSearchPath(writablePath, true);
}
