/****************************************************************************
 Copyright (c) 2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2018 HALX99.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#define LOG_TAG "AudioDecoderWav" 

#include <stddef.h>
#include "audio/include/AudioDecoderWav.h"
#include "audio/include/AudioMacros.h"
#include "platform/CCFileUtils.h"
#include "audio/include/AudioFileHelper.h"

#if !defined(_WIN32)
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;
__inline int IsEqualGUID(const GUID& rguid1, const GUID& rguid2)
{
    return !::memcmp(&rguid1, &rguid2, sizeof(GUID));
}
#endif

static const size_t WAV_FILE_HEADER_SIZE = sizeof(WAV_FILE_HEADER);

// 00000001-0000-0010-8000-00aa00389b71
static const GUID WavSubTypePCM = {
    0x00000001, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
};

// 00000003-0000-0010-8000-00aa00389b71
static const GUID WavSubTypeIEEE_FLOAT = {
    0x00000003, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
};

int wav_open(const std::string& fullPath, WAV_FILE* wavf)
{
    int fd = openAudioFile(fullPath);
    if (fd < 0)
        return -1;

    wavf->PcmDataOffset = 0;

    // Read to BitsPerSample
    int bytesToRead = offsetof(struct WAV_FILE_HEADER, Subchunk2ID) - offsetof(struct WAV_FILE_HEADER, ChunkID);
    ::read(fd, &wavf->FileHeader, bytesToRead);
    wavf->PcmDataOffset += bytesToRead;

    // check somthings
    auto h = &wavf->FileHeader;
     // Read PCM data or extensible data if exists.
    if (h->Subchunk1Size == 16 && h->AudioFormat == 1)
    {
        // PCM
        do {
            // Note: 8-bit samples are stored as unsigned bytes, ranging from 0 to 255. 16-bit samples are stored as 2's-complement signed integers, ranging from -32768 to 32767.
            // data
            ::read(fd, &h->Subchunk2ID, sizeof(uint32_t));    // 0x61746164, "data"
            ::read(fd, &h->Subchunk2Size, sizeof(uint32_t)); 
            wavf->PcmDataOffset += 8;

            if (h->Subchunk2ID == 0x61746164) { // Skip other non "data" chunk
                break;
            }
            else {
                lseek(fd, h->Subchunk2Size, SEEK_CUR);
                wavf->PcmDataOffset += h->Subchunk2Size;
            }
        } while (true);
    }
    else if (h->Subchunk1Size > 16 && h->AudioFormat == 0xFFFE)
    {
        ::read(fd, &wavf->ExtraParamSize, sizeof(uint16_t));
        wavf->PcmDataOffset += 2;

        if (wavf->ExtraParamSize == 22)
        {
            // if cbSize is set to 22 => WAVEFORMATEXTENSIBLE
            ::read(fd, &wavf->Samples, sizeof(uint16_t));

            // DWORD dwChannelMask; which channels are present in stream
            ::read(fd, &wavf->ChannelMask, sizeof(uint32_t));

            GUID GuidSubFormat = { 0 };
            ::read(fd, &GuidSubFormat, sizeof(GUID));

            wavf->PcmDataOffset += 22;

            // Check sub-format.
            if (!IsEqualGUID(GuidSubFormat, WavSubTypePCM)
                && !IsEqualGUID(GuidSubFormat, WavSubTypeIEEE_FLOAT))
            {
                ::close(fd);
                return -1;
            }

            // Find "data" chunk.
            while (!::eof(fd))
            {
                uint32_t chunk;
                ::read(fd, &chunk, sizeof(uint32_t));
                wavf->PcmDataOffset += 4;

                if (chunk == 0x61746164)
                {
                    // "data" chunk
                    h->Subchunk2ID = chunk;  // 0x61746164, "data"
                    ::read(fd, &h->Subchunk2Size, sizeof(uint32_t)); 
                    wavf->PcmDataOffset += 4;
                    break;
                }
                else
                {
                    // Read other non "data" chunks.
                    uint32_t chunkSize;
                    ::read(fd, &chunkSize, sizeof(uint32_t));

                    wavf->PcmDataOffset += 4;

                    ::lseek(fd, chunkSize, SEEK_CUR); // skip only
                    wavf->PcmDataOffset += (int)chunkSize;
                }
            }
        }
        else
        {
            ::close(fd);
            return -1;
        }
    }
    else
    {
        ::close(fd);
        return -1;
    }

    wavf->BytesPerFrame = h->BitsPerSample / 8 * h->NumChannels;
    wavf->FileHandle = fd;

    return 0;
}

int wav_read(WAV_FILE* wavf, char* pcmBuf, size_t bytesToRead)
{
    return ::read(wavf->FileHandle, pcmBuf, bytesToRead);
}

int wav_pcm_seek(WAV_FILE* wavf, int frameOffset)
{
    auto offset = frameOffset * wavf->BytesPerFrame + wavf->PcmDataOffset;
    return ::lseek(wavf->FileHandle, offset, SEEK_SET) >= 0 ? 0 : -1;
}

int wav_pcm_tell(WAV_FILE* wavf)
{
    auto offset = ::lseek(wavf->FileHandle, 0, SEEK_CUR);
    return (offset - wavf->PcmDataOffset) / wavf->BytesPerFrame;
}

int wav_close(WAV_FILE* wavf)
{
    return ::close(wavf->FileHandle);
}

namespace cocos2d { namespace experimental {

    AudioDecoderWav::AudioDecoderWav()
    {
    }

    AudioDecoderWav::~AudioDecoderWav()
    {
        close();
    }

    bool AudioDecoderWav::open(const char* path)
    {
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);

        int iret = wav_open(FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), &_wavf);

        if (0 == iret)
        {
            _sampleRate = _wavf.FileHeader.SampleRate;
            _channelCount = _wavf.FileHeader.NumChannels;
            _bytesPerFrame = _wavf.BytesPerFrame;
            _totalFrames = _wavf.FileHeader.Subchunk2Size / _bytesPerFrame;

            _isOpened = true;
            return true;
        }
        return false;
    }

    void AudioDecoderWav::close()
    {
        if (isOpened())
        {
            wav_close(&_wavf);
            _isOpened = false;
        }
    }

    uint32_t AudioDecoderWav::read(uint32_t framesToRead, char* pcmBuf)
    {
        auto bytesToRead = _bytesPerFrame * framesToRead;
        long bytesRead = wav_read(&_wavf, pcmBuf, bytesToRead);
        return static_cast<uint32_t>(bytesRead / _bytesPerFrame);
    }

    bool AudioDecoderWav::seek(uint32_t frameOffset)
    {
        return wav_pcm_seek(&_wavf, frameOffset) == 0;
    }

    uint32_t AudioDecoderWav::tell() const
    {
        return wav_pcm_tell(&_wavf);
    }

}} // namespace cocos2d { namespace experimental {
