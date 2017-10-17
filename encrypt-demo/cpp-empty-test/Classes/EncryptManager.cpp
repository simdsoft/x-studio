
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

using namespace cocos2d;

class FileUtilsNoEncrypt : public FileUtilsImpl
{
public:
    using FileUtilsImpl::init;
};

class FileUtilsEncrypt : public FileUtilsImpl
{
public:
    FileUtilsEncrypt(EncryptManager& manager) : encryptManager(manager)
    {
    }

    /**
    *  Gets string from a file.
    */
    std::string getStringFromFile(const std::string& filename) override
    {
        auto data = FileUtilsImpl::getStringFromFile(filename);
        if (!data.empty()) {
            crypto::aes::decrypt(data, encryptManager._encryptKey.c_str());
            if (encryptManager._compressed) {
                return crypto::zlib::uncompress(data);
            }
            else {
                return data;
            }
        }
        else {
            return "";
        }
    }

    /**
    *  Creates binary data from a file.
    *  @return A data object.
    */
    Data getDataFromFile(const std::string& filename) override
    {
        auto data = FileUtilsImpl::getDataFromFile(filename);
        if (data.getSize() > 0) {
            size_t size = 0;
            crypto::aes::privacy::mode_spec<>::decrypt(data.getBytes(), data.getSize(), data.getBytes(), size, encryptManager._encryptKey.c_str());

            if (encryptManager._compressed) {
                auto uncomprData = crypto::zlib::abi::_inflate(unmanaged_string((const char*)data.getBytes(), size));
                size = uncomprData.size();

                data.clear();

                data.fastSet((unsigned char*)uncomprData.deatch(), size);
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
        if (data != nullptr && *size > 0) {
            size_t outsize = 0;
            crypto::aes::privacy::mode_spec<>::decrypt(data, *size, data, outsize, encryptManager._encryptKey.c_str());

            if (encryptManager._compressed) {
                auto uncomprData = crypto::zlib::abi::_inflate(unmanaged_string((const char*)data, outsize));
                *size = uncomprData.size();

                free(data);

                return (unsigned char*)uncomprData.deatch();
            }
            else {
                *size = static_cast<ssize_t>(outsize);
                return data;
            }
        }
        return nullptr;
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
    std::string encrpytKey,encryptIvec;

    encrpytKey.resize(32);
    ::memcpy(&encrpytKey.front(), key.c_str(), (std::min)(32, (int)key.size()));

    if (!ivec.empty()) {
        encryptIvec.resize(16);
        ::memcpy(&encryptIvec.front(), ivec.c_str(), (std::min)(16, (int)ivec.size()));
    }
    else {
        encryptIvec = nsc::hex2bin("00234b89aa96fecdaf80fbf178a25621");
    }

    crypto::aes::detail::set_ivec(encryptIvec.c_str(), 16);
    return crypto::aes::decrypt(encryptedData, encrpytKey.c_str());
}

void EncryptManager::setEncryptEnabled(bool bVal, const std::string& key, const std::string& ivec, bool compressed)
{
    if (bVal && !key.empty()) {
        _encryptKey.clear();
        _encryptIvec.clear();

        _encryptKey.resize(32);
        int keysize = key.size();
        if (keysize > 32)
            keysize = 32;

        ::memcpy(&_encryptKey.front(), key.c_str(), keysize);

        if (!ivec.empty()) {
            _encryptIvec.resize(16);
            ::memcpy(&_encryptIvec.front(), ivec.c_str(), (std::min)(16, (int)ivec.size()));
        }
        else {
            _encryptIvec = nsc::hex2bin("00234b89aa96fecdaf80fbf178a25621");
        }

        crypto::aes::detail::set_ivec(_encryptIvec.c_str());

        setupHookFuncs();
        _encryptEnabled = bVal;
        _compressed = compressed;
    }
    else {
        auto fileUtilsNoEncrypt = new FileUtilsNoEncrypt();
        fileUtilsNoEncrypt->init();
        fileUtilsNoEncrypt->setSearchPaths(FileUtils::getInstance()->getSearchPaths());
        FileUtils::setDelegate(fileUtilsNoEncrypt);

        _encryptEnabled = false;
        _compressed = true;
    }
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
    if (format == FileIndexFormat::Binary)
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
    else if (format == FileIndexFormat::Csv)
    {
        const char* endl = buffer.c_str();
        const char* cursor = nullptr;
        do {
            std::string key, value;
            cursor = endl;
            auto counter = 0;
            endl = fastest_csv_parser::csv_parse_line(cursor, [&](const char* v_start, const char* v_end) {
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
        } while ((endl - buffer.c_str()) < buffer.size());
    }
}
