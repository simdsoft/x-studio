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

int openAudioFile(const std::string& path);
