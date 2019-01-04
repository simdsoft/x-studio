//
// Copyright (c) 2014-2019 x-studio365 - All Rights Reserved
//
#include <stdio.h>
#include <string>

#include "crypto-support/crypto_wrapper.h"

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
#else /* ios or mac */
#include "platform/apple/CCFileUtils-apple.h"
typedef cocos2d::FileUtilsApple FileUtilsImpl;
#endif

#include "EncryptManager.h"
#include "crypto-support/nsconv.h"
#include "crypto-support/fastest_csv_parser.h"
#include "crypto-support/ibinarystream.h"
using namespace purelib;

#pragma warning(disable:4996)

using namespace cocos2d;

class FileUtilsNoEncrypt : public FileUtilsImpl
{
public:
    using FileUtilsImpl::init;
};

class FileUtilsEncrypt : public FileUtilsImpl
{
public:
    /**
    pitfall:
    For cocos2d-x v3.10, you should change FileUtilsWin32's constructor access permission from 'private' to
    'protected'
    */
    FileUtilsEncrypt(EncryptManager& manager) : encryptManager(manager)
    {
    }

    /**
    *  Gets string from a file.
    */
    std::string getStringFromFile(const std::string& filename) override
    {
        auto data = FileUtilsImpl::getStringFromFile(filename);
        EncryptManager::SignInfo info;
        if (!data.empty() && encryptManager.parseSignInfo(&data.front(), data.size(), &info)) {
            data.resize(data.size() - encryptManager._encryptSignKey.size());
            crypto::aes::overlapped::decrypt(data, encryptManager._encryptKey.c_str(), AES_DEFAULT_KEY_BITS, encryptManager._encryptIvec.c_str());
            if (info.compressed) {
                return crypto::zlib_uncompress<std::string>(data, info.expected_size);
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
    Data getDataFromFile(const std::string& filename) override
    {
        auto data = FileUtilsImpl::getDataFromFile(filename);
        EncryptManager::SignInfo info;
        if (data.getSize() > 0 && encryptManager.parseSignInfo((char*)data.getBytes(), data.getSize(), &info)) {
            size_t size = 0;
            crypto::aes::privacy::mode_spec<>::decrypt(data.getBytes(), data.getSize() - encryptManager._encryptSignKey.size(), data.getBytes(), size, encryptManager._encryptKey.c_str(), AES_DEFAULT_KEY_BITS, encryptManager._encryptIvec.c_str());

            if (info.compressed) {
                auto uncomprData = crypto::zlib_inflate<crypto::mutable_buffer>(std::string_view((const char*)data.getBytes(), size), info.expected_size);

                data.clear();
                data.fastSet(uncomprData.detach(), uncomprData.size());
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
    virtual unsigned char* getFileData(const std::string& filename, const char* mode, ssize_t *size) override
    {
        auto data = FileUtilsImpl::getFileData(filename, mode, size);
        if (data != nullptr) {
            size_t outsize = 0;
            EncryptManager::SignInfo info;
            if (*size > 0 && encryptManager.parseSignInfo((char*)data, *size, &info)) {
                *size -= static_cast<ssize_t>(encryptManager._encryptSignKey.size());
                crypto::aes::privacy::mode_spec<>::decrypt(data, *size, data, outsize, encryptManager._encryptKey.c_str(), AES_DEFAULT_KEY_BITS, encryptManager._encryptIvec.c_str());

                if (info.compressed) {
                    auto uncomprData = crypto::zlib_inflate<crypto::mutable_buffer>(std::string_view((const char*)data, outsize), info.expected_size);

                    free(data);
                    *size = uncomprData.size();
                    data = (unsigned char*)uncomprData.detach();
                }
                else {
                    *size = static_cast<ssize_t>(outsize);
                }
            }
        }
        return data;
    }

    std::string fullPathForFilename(const std::string &filename) const override
    {
        auto iter = encryptManager._indexFileMap.find(filename);
        if (iter != encryptManager._indexFileMap.end())
            return iter->second;

        return FileUtilsImpl::fullPathForFilename(filename);
    }

    EncryptManager& encryptManager;
};

EncryptManager* EncryptManager::getInstance()
{
    static EncryptManager s_EncryptManager;
    return &s_EncryptManager;
}

std::string EncryptManager::decryptData(const std::string& encryptedData, const std::string& key, const std::string& ivec)
{
    std::string encrpytKey, encryptIvec;

    encrpytKey.resize(32);
    ::memcpy(&encrpytKey.front(), key.c_str(), (std::min)(32, (int)key.size()));

    if (!ivec.empty()) {
        encryptIvec.resize(16);
        ::memcpy(&encryptIvec.front(), ivec.c_str(), (std::min)(16, (int)ivec.size()));
    }
    else {
        encryptIvec = nsc::hex2bin("00234b89aa96fecdaf80fbf178a25621");
    }

    return crypto::aes::decrypt(encryptedData, encrpytKey.c_str(), AES_DEFAULT_KEY_BITS, encryptIvec.c_str());
}

void EncryptManager::setEncryptEnabled(bool bVal, std::string_view key, std::string_view ivec, int flags)
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
            std::string sign(_encryptKey.c_str(), key.size());
            int roll = (flags >> 16) & 0xffff;
            do {
                crypto::aes::overlapped::encrypt<crypto::aes::ECB, crypto::aes::PaddingMode::None>(_encryptSignKey,
                    sign.c_str(), 128);
                sign = _encryptSignKey;
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

bool EncryptManager::parseSignInfo(char* data, size_t len, SignInfo* info) const
{
    if (_encryptFlags & ENCF_SIGNATURE) {
        if (len > 16) {
            size_t outlen = 0;
            auto signbuf = data + len - 16;
            crypto::aes::detail::ecb_decrypt_block(signbuf,
                16,
                signbuf,
                _encryptSignKey.c_str(),
                128);

            memcpy(&info->mask, signbuf, sizeof(info->mask));
            signbuf += sizeof(info->mask);
            memcpy(&info->sigval, signbuf, sizeof(info->sigval));
            signbuf += sizeof(info->sigval);
            info->flags = *((uint8_t*)signbuf++);
            memcpy(&info->expected_size, signbuf, sizeof(info->expected_size));
            return (info->mask ^ info->sigval) == 0xdeadbeef;
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

void EncryptManager::enableFileIndex(const std::string& indexFile, FileIndexFormat format)
{
    this->_indexFileMap.clear();

    auto buffer = FileUtils::getInstance()->getStringFromFile(indexFile);
    if (buffer.empty()) return;
    if (format == FileIndexFormat::BINARY)
    {
        int fileCount = 0;
        ibinarystream ibs(buffer.c_str(), buffer.size());
        ibs.read_i(fileCount);
        for (auto i = 0; i < fileCount; ++i)
        {
            std::string key, value;
            ibs.read_v(key);
            ibs.read_v(value);
            this->_indexFileMap.emplace(std::move(key), std::move(value));
        }
    }
    else if (format == FileIndexFormat::CSV)
    {
        char* endl = &buffer.front();
        char* cursor = nullptr;
        do {
            std::string key, value;
            cursor = endl;
            auto counter = 0;
            endl = fastest_csv_parser::csv_parse_line(cursor, [&](char* v_start, char* v_end) {
                if (counter == 0)
                {
                    key.assign(v_start, v_end - v_start);
                }
                else {
                    value.assign(v_start, v_end - v_start);
                }
                ++counter;
            });

            _indexFileMap.emplace(std::move(key), std::move(value));
        } while ((endl - buffer.c_str()) < static_cast<int>(buffer.size()));
    }
}
