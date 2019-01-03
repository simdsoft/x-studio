//
// Copyright (c) 2014-2019 halx99 - All Rights Reserved
//
#ifndef _CRYPTO_WRAPPER_H_
#define _CRYPTO_WRAPPER_H_
#include <string>
#include <vector>
#include <assert.h>
#include "string_view.hpp"
#include "crypto_utils.h"

#if _HAS_ZLIB
#include <zlib.h>
#if !defined(WINRT)
#pragma comment(lib, "libzlib.lib")
#else
#pragma comment(lib, "zlib.lib")
#endif
#endif

#define AES_DEFAULT_KEY_BITS 256

namespace crypto {

    class mutable_buffer {
    public:
        mutable_buffer() :
            _data(nullptr),
            _size(0),
            _capacity(0)
        {
        }
        mutable_buffer(size_t size, char value)
        {
            resize(size);
            memset(_data, value, size);
        }
        mutable_buffer(mutable_buffer&& rhs)
        {
            _size = rhs._size;
            _capacity = rhs._capacity;
            _data = rhs.detach();
        }
        ~mutable_buffer()
        {
            clear();
            shrink_to_fit();
        }
        void insert(unsigned char* where, unsigned char* start, unsigned char* end)
        {
            size_t count = end - start;
            if (count > 0) {
                auto append = this->end() == where;
                auto offset = size();
                resize(offset + count);
                
                if (append) {
                    memcpy(_data + offset, start, count);
                }
                else if(where != nullptr) {
                    auto to = where + count;
                    memmove(where, to, this->end() - to);
                    memcpy(where, start, count);
                }
            }
        }
        unsigned char& front() {
            return *_data;
        }
        unsigned char* begin()
        {
            return (unsigned char*)_data;
        }
        unsigned char* end() {
            if (!empty()) {
                return (unsigned char*)_data + _size;
            }
            return nullptr;
        }
        void reserve(size_t capacity)
        {
            if (_capacity < capacity) {
                _data = (unsigned char*)realloc(_data, capacity);
                _capacity = capacity;
            }
        }
        void resize(size_t size)
        {
            if (_size > size) {
                _size = size;
            }
            else {
                if (_capacity < size) {
                    _capacity = size * 3 / 2;
                    _data = (unsigned char*)realloc(_data, _capacity);
                }
                _size = size;
            }
        }
        size_t size() const {
            return _size;
        }
        void* data()
        {
            return _data;
        }
        void clear()
        {
            _size = 0;
        }
        bool empty() const
        {
            return _size == 0;
        }
        void shrink_to_fit()
        {
            if (empty()) {
                if (_data != nullptr)
                {
                    free(_data);
                    _data = nullptr;
                }
                _capacity = 0;
            }
            else {
                if (_capacity > _size) {
                    _data = (unsigned char*)realloc(_data, _size);
                    _capacity = _size;
                }
            }
        }
        unsigned char* detach()
        {
            auto rdata = _data;

            _data = nullptr;
            _size = 0;
            _capacity = 0;

            return (unsigned char*)rdata;
        }
    private:
        unsigned char* _data;
        size_t _size;
        size_t _capacity;
    };

    namespace aes {

        /// padding
        namespace privacy {
            template<PaddingMode = PaddingMode::PKCS7>
            struct padding_spec
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return detail::padding::PKCS7(plaintext, blocksize); }
            };

            template<>
            struct padding_spec < PaddingMode::ISO10126>
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return detail::padding::ISO10126(plaintext, blocksize); }
            };

            template<>
            struct padding_spec < PaddingMode::ANSIX923>
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return detail::padding::ANSIX923(plaintext, blocksize); }
            };

            template<>
            struct padding_spec < PaddingMode::Zeros>
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return detail::padding::ZEROS(plaintext, blocksize); }
            };

            template<>
            struct padding_spec < PaddingMode::None>
            {
                template<typename _ByteSeqCont>
                inline static size_t perform(_BYTE_SEQ_CONT& plaintext, size_t blocksize = AES_BLOCK_SIZE) { return 0; }
            };

            template<CipherMode = CipherMode::CBC>
            struct mode_spec
            {
                static void encrypt(const void* in, size_t inlen,
                    void* out, size_t outlen, const void* private_key, int keybits = 256, const void* ivec = nullptr)
                {
                    detail::cbc_encrypt(in, inlen, out, outlen, private_key, keybits, ivec);
                }

                static void decrypt(const void* in, size_t inlen,
                    void* out, size_t& outlen, const void* private_key, int keybits = 256, const void* ivec = nullptr)
                {
                    detail::cbc_decrypt(in, inlen, out, outlen, private_key, keybits, ivec);
                }
            };

            template<>
            struct mode_spec<CipherMode::ECB>
            {
                static void encrypt(const void* in, size_t inlen,
                    void* out, size_t outlen, const void* private_key, int keybits = 256, const void* /*ivec*/ = nullptr)
                {
                    detail::ecb_encrypt(in, inlen, out, outlen, private_key, keybits);
                }

                static void decrypt(const void* in, size_t inlen,
                    void* out, size_t& outlen, const void* private_key, int keybits = 256, const void* /*ivec*/ = nullptr)
                {
                    detail::ecb_decrypt(in, inlen, out, outlen, private_key, keybits);
                }
            };
        };

        /// encrypt decrypt APIs 

        template<CipherMode cipherMode = CipherMode::CBC, PaddingMode paddingMode = PaddingMode::PKCS7, typename _ByteSeqCont = std::string>
        inline _BYTE_SEQ_CONT encrypt(std::string_view in, const void* key = DEFAULT_KEY, int keybits = 256, const void* ivec = nullptr)
        {
            _BYTE_SEQ_CONT out(in.data(), in.length());
            privacy::padding_spec<paddingMode>::perform(out);

            privacy::mode_spec<cipherMode>::encrypt(out.data(),
                out.size(),
                &out.front(),
                out.size(),
                key,
                keybits,
                ivec);
            return out;
        }

        template<CipherMode cipherMode = CipherMode::CBC, typename _ByteSeqCont = std::string>
        inline _BYTE_SEQ_CONT decrypt(std::string_view in, const void* key = DEFAULT_KEY, int keybits = 256, const void* ivec = nullptr)
        {
            size_t outlen = in.size();
            _BYTE_SEQ_CONT out(outlen, 0x0);
            privacy::mode_spec<cipherMode>::decrypt(in.data(),
                in.size(),
                &out.front(),
                outlen,
                key,
                keybits,
                ivec);
            out.resize(outlen);
            return out;
        }

        /// wrappers, strongly encrypt, low speed; false: use ECB: fast speed,  light-weighted encrypt
        namespace overlapped {

            template<CipherMode cipherMode = CipherMode::CBC, PaddingMode paddingMode = PaddingMode::PKCS7, typename _ByteSeqCont = std::string>
            inline void encrypt(_BYTE_SEQ_CONT& inout, const void* key = DEFAULT_KEY, int keybits = 256, const void* ivec = nullptr, size_t offset = 0)
            {
                privacy::padding_spec<paddingMode>::perform(inout);

                privacy::mode_spec<cipherMode>::encrypt(inout.data() + offset,
                    inout.size() - offset,
                    &inout.front() + offset,
                    inout.size() - offset,
                    key,
                    keybits,
                    ivec);
            }

            template<CipherMode cipherMode = CipherMode::CBC, typename _ByteSeqCont = std::string>
            inline void decrypt(_BYTE_SEQ_CONT& inout, const void* key = DEFAULT_KEY, int keybits = 256, const void* ivec = nullptr, size_t offset = 0)
            {
                size_t outlen = 0;
                privacy::mode_spec<cipherMode>::decrypt(inout.data() + offset,
                    inout.size() - offset,
                    &inout.front() + offset,
                    outlen,
                    key,
                    keybits,
                    ivec);

                inout.resize(outlen + offset);
            }
        }
    } /* end of namespace crypto::aes */

#if _HAS_ZLIB
        /*
        ** level values:
        ** Z_NO_COMPRESSION         0
        ** Z_BEST_SPEED             1
        ** Z_BEST_COMPRESSION       9
        ** Z_DEFAULT_COMPRESSION  (-1)
        **
        */
    template<typename _ByteSeqCont>
    _ByteSeqCont zlib_compress(std::string_view in, int level = -1)
    {
        // calc destLen
        auto destLen = ::compressBound(in.size());
        _ByteSeqCont out(destLen, '\0');

        // do compress
        int ret = ::compress2((Bytef*)(&out.front()), &destLen, (const Bytef*)in.data(), in.size(), level);

        if (ret == Z_OK)
        {
            out.resize(destLen);
        }
        return (out);
    }

    template<typename _ByteSeqCont>
    _ByteSeqCont zlib_deflate(std::string_view in, int level = -1)
    {
        int err;
        Bytef buffer[512];
        z_stream d_stream; /* compression stream */

        // strcpy((char*)buffer, "garbage");

        d_stream.zalloc = nullptr;
        d_stream.zfree = nullptr;
        d_stream.opaque = (voidpf)0;

        d_stream.next_in = (Bytef*)in.data();
        d_stream.avail_in = in.size();
        d_stream.next_out = buffer;
        d_stream.avail_out = sizeof(buffer);

        _ByteSeqCont output;

        err = deflateInit(&d_stream, level);
        if (err != Z_OK) // TODO: log somthing
            return (output);

        output.reserve(in.size());

        for (;;)
        {
            err = deflate(&d_stream, Z_FINISH);

            if (err == Z_STREAM_END)
            {
                output.insert(output.end(), buffer, buffer + sizeof(buffer) - d_stream.avail_out);
                break;
            }

            switch (err)
            {
            case Z_NEED_DICT:
                err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                deflateEnd(&d_stream);
                output.clear();
                return (output);
            }

            // not enough buffer ?
            if (err != Z_STREAM_END)
            {
                output.insert(output.end(), buffer, buffer + sizeof(buffer));

                d_stream.next_out = buffer;
                d_stream.avail_out = sizeof(buffer);
            }
        }

        deflateEnd(&d_stream);
        if (err != Z_STREAM_END)
        {
            output.clear();
        }

        return (output);
    }

    template<typename _ByteSeqCont>
    _ByteSeqCont zlib_inflate(std::string_view compr, int expected_size = -1)
    { // inflate
        int err;
        Bytef buffer[512];
        z_stream d_stream; /* decompression stream */

        // strcpy((char*)buffer, "garbage");

        d_stream.zalloc = nullptr;
        d_stream.zfree = nullptr;
        d_stream.opaque = (voidpf)0;

        d_stream.next_in = (Bytef*)compr.data();
        d_stream.avail_in = compr.size();
        d_stream.next_out = buffer;
        d_stream.avail_out = sizeof(buffer);
        _ByteSeqCont output;
        err = inflateInit(&d_stream);
        if (err != Z_OK) // TODO: log somthing
            return output;
        // CHECK_ERR(err, "inflateInit");

        output.reserve(expected_size != -1 ? expected_size : (compr.size() << 2));
        for (;;)
        {
            err = inflate(&d_stream, Z_NO_FLUSH);

            if (err == Z_STREAM_END)
            {
                output.insert(output.end(), buffer, buffer + sizeof(buffer) - d_stream.avail_out);
                break;
            }

            switch (err)
            {
            case Z_NEED_DICT:
                err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                goto  _L_end;
            }

            // not enough memory ?
            if (err != Z_STREAM_END)
            {
                // *out = (unsigned char*)realloc(*out, bufferSize * BUFFER_INC_FACTOR);
                output.insert(output.end(), buffer, buffer + sizeof(buffer));

                d_stream.next_out = buffer;
                d_stream.avail_out = sizeof(buffer);
            }
        }

    _L_end:
        inflateEnd(&d_stream);
        if (err != Z_STREAM_END)
        {
            output.clear();
        }

        return (output);
    }

    // inflate alias
    template<typename _ByteSeqCont>
    _ByteSeqCont zlib_uncompress(std::string_view in, int expected_size = -1)
    {
        return zlib_inflate<_ByteSeqCont>(in, expected_size);
    }

    // gzip
    /*
    reference: http://blog.csdn.net/rainharder/article/details/26342919
    */
    template<typename _ByteSeqCont>
    _ByteSeqCont zlib_gzcompr(std::string_view in, int level = -1)
    {
        int err;
        Bytef buffer[512];
        z_stream d_stream; /* compression stream */

        // strcpy((char*)buffer, "garbage");

        d_stream.zalloc = nullptr;
        d_stream.zfree = nullptr;
        d_stream.opaque = (voidpf)0;

        d_stream.next_in = (Bytef*)in.data();
        d_stream.avail_in = in.size();
        d_stream.next_out = buffer;
        d_stream.avail_out = sizeof(buffer);
        _ByteSeqCont output;
        err = deflateInit2(&d_stream, level, Z_DEFLATED, MAX_WBITS + 16/*well: normaly, gzip is: 16*/, MAX_MEM_LEVEL - 1, Z_DEFAULT_STRATEGY);
        if (err != Z_OK) // TODO: log somthing
            return output;

        for (;;)
        {
            err = deflate(&d_stream, Z_FINISH);

            if (err == Z_STREAM_END)
            {
                output.insert(output.end(), buffer, buffer + sizeof(buffer) - d_stream.avail_out);
                break;
            }

            switch (err)
            {
            case Z_NEED_DICT:
                err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                goto _L_end;
            }

            // not enough buffer ?
            if (err != Z_STREAM_END)
            {
                output.insert(output.end(), buffer, buffer + sizeof(buffer));

                d_stream.next_out = buffer;
                d_stream.avail_out = sizeof(buffer);
            }
        }

    _L_end:
        deflateEnd(&d_stream);
        if (err != Z_STREAM_END)
        {
            output.clear();
        }

        return output;
    }

    template<typename _ByteSeqCont>
    _ByteSeqCont zlib_gzuncompr(std::string_view compr, int expected_size = -1)
    { // inflate
        int err;
        Bytef buffer[512];
        z_stream d_stream; /* decompression stream */

        // strcpy((char*)buffer, "garbage");

        d_stream.zalloc = nullptr;
        d_stream.zfree = nullptr;
        d_stream.opaque = (voidpf)0;

        d_stream.next_in = (Bytef*)compr.data();
        d_stream.avail_in = compr.size();
        d_stream.next_out = buffer;
        d_stream.avail_out = sizeof(buffer);
        _ByteSeqCont output;
        err = inflateInit2(&d_stream, MAX_WBITS + 32/*well: normaly, gzip is: 16*/);
        if (err != Z_OK) // TODO: log somthing
            return output;
        // CHECK_ERR(err, "inflateInit");
        output.reserve(expected_size != -1 ? expected_size : (compr.size() << 2));

        for (;;)
        {
            err = inflate(&d_stream, Z_NO_FLUSH);

            if (err == Z_STREAM_END)
            {
                output.insert(output.end(), buffer, buffer + sizeof(buffer) - d_stream.avail_out);
                break;
            }

            switch (err)
            {
            case Z_NEED_DICT:
                err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                goto _L_end;
            }

            // not enough memory ?
            if (err != Z_STREAM_END)
            {
                // *out = (unsigned char*)realloc(*out, bufferSize * BUFFER_INC_FACTOR);
                output.insert(output.end(), buffer, buffer + sizeof(buffer));

                d_stream.next_out = buffer;
                d_stream.avail_out = sizeof(buffer);
            }
        }

    _L_end:
        inflateEnd(&d_stream);
        if (err != Z_STREAM_END)
        {
            output.clear();
        }

        return output;
    }
#endif

    namespace http {
        std::string b64dec(std::string_view ciphertext);

        std::string b64enc(std::string_view  plaintext);

        std::string urldecode(std::string_view ciphertext);

        std::string urlencode(std::string_view input);
    };

    namespace hash {
#if _HAS_MD5
        std::string md5(std::string_view string);
        std::string md5raw(std::string_view string);
        std::string fmd5(const char* filename);
#endif

#if _HAS_MD6
        std::string md6(std::string_view data, size_t hashByteLen = 64); // small data
        std::string md6raw(std::string_view data, size_t hashByteLen = 64);
        std::string fmd6(const char* filename, int hashByteLen = 64);
#endif
    };

#if _HAS_OPENSSL
    namespace rsa {
        enum PaddingMode {
            PKCS1_PADDING = 1,
            SSLV23_PADDING = 2,
            NO_PADDING = 3,
            PKCS1_OAEP_PADDING = 4,
            X931_PADDING = 5,
            /* EVP_PKEY_ only */
            PKCS1_PSS_PADDING = 6,
        };

        namespace pub {
            // supported PaddingModes : RSA_PKCS1_PADDING, RSA_PKCS1_OAEP_PADDING, RSA_SSLV23_PADDING, RSA_NO_PADDING
            std::string encrypt(std::string_view plaintext, std::string_view keysteam, int paddingMode = PKCS1_OAEP_PADDING);
            std::string decrypt(std::string_view cipertext, std::string_view keysteam, int paddingMode = PKCS1_PADDING);

            // supported PaddingModes : RSA_PKCS1_PADDING, RSA_PKCS1_OAEP_PADDING, RSA_SSLV23_PADDING, RSA_NO_PADDING
            std::string encrypt2(std::string_view plaintext, std::string_view keyfile, int paddingMode = PKCS1_OAEP_PADDING);
            std::string decrypt2(std::string_view cipertext, std::string_view keyfile, int paddingMode = PKCS1_PADDING);
        }

        namespace pri {
            // supported PaddingModes : RSA_PKCS1_PADDING, RSA_X931_PADDING, RSA_NO_PADDING
            std::string encrypt(std::string_view plaintext, std::string_view keysteam, int paddingMode = PKCS1_PADDING);
            std::string decrypt(std::string_view cipertext, std::string_view keysteam, int paddingMode = PKCS1_OAEP_PADDING);

            // supported PaddingModes : RSA_PKCS1_PADDING, RSA_X931_PADDING, RSA_NO_PADDING
            std::string encrypt2(std::string_view plaintext, std::string_view keyfile, int paddingMode = PKCS1_PADDING);
            std::string decrypt2(std::string_view cipertext, std::string_view keyfile, int paddingMode = PKCS1_OAEP_PADDING);
        }
    }
#endif

};

#endif

