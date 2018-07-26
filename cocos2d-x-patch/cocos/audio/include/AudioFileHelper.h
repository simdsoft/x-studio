#pragma once

#include "platform/CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <io.h>
#include <direct.h>
#include <fcntl.h>
#else
#include <unistd.h>
#include <errno.h>
#endif

int openAudioFile(const std::string& path);
