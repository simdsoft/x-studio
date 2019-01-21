
set(COCOS_AUDIO_SRC
    audio/AudioEngine.cpp
    )

if(WINDOWS)

    set(COCOS_AUDIO_PLATFORM_HEADER
        audio/win32/MciPlayer.h
        audio/include/AudioDecoderManager.h
        audio/include/AudioDecoder.h
        audio/include/AudioPlayer.h
        audio/include/AudioDecoderOgg.h        
        audio/include/AudioEngineImpl.h
        audio/include/AudioDecoderMp3.h
        audio/include/AudioDecoderWav.h
        audio/include/AudioCache.h
        audio/include/AudioFileStream.h
        )

    set(COCOS_AUDIO_PLATFORM_SRC
        audio/win32/SimpleAudioEngine.cpp
        audio/win32/MciPlayer.cpp
        audio/win32/AudioEngineImpl.cpp
        audio/AudioCache.cpp
        audio/AudioPlayer.cpp
        audio/AudioDecoder.cpp
        audio/AudioDecoderManager.cpp
        audio/AudioDecoderMp3.cpp
        audio/AudioDecoderOgg.cpp
        audio/AudioDecoderWav.cpp
        audio/AudioFileStream.cpp
        )

elseif(ANDROID)

    set(COCOS_AUDIO_PLATFORM_HEADER
        audio/android/ccdandroidUtils.h
        audio/android/jni/cddandroidAndroidJavaEngine.h
        audio/android/utils/Utils.h
        audio/include/AudioEngineImpl.h
        audio/include/AudioCache.h
        audio/include/AudioPlayer.h
        audio/include/AudioDecoder.h
        audio/include/AudioDecoderManager.h
        audio/include/AudioDecoderMp3.h
        audio/include/AudioDecoderOgg.h
        audio/include/AudioDecoderWav.h
        audio/include/AudioFileStream.h
        )

    set(COCOS_AUDIO_PLATFORM_SRC
        audio/android/cddSimpleAudioEngine.cpp
        audio/android/ccdandroidUtils.cpp
        audio/android/jni/cddandroidAndroidJavaEngine.cpp
        audio/android/utils/Utils.cpp
        audio/AudioEngineImpl.cpp
        audio/AudioCache.cpp
        audio/AudioPlayer.cpp
        audio/AudioDecoder.cpp
        audio/AudioDecoderManager.cpp
        audio/AudioDecoderMp3.cpp
        audio/AudioDecoderOgg.cpp
        audio/AudioDecoderWav.cpp
        audio/AudioFileStream.cpp
        )

elseif(LINUX)
    set(COCOS_AUDIO_PLATFORM_HEADER
        audio/include/AudioDecoderManager.h
        audio/include/AudioDecoder.h
        audio/include/AudioPlayer.h
        audio/include/AudioDecoderOgg.h        
        audio/include/AudioEngineImpl.h
        audio/include/AudioDecoderMp3.h
        audio/include/AudioDecoderWav.h
        audio/include/AudioCache.h
        audio/include/AudioFileStream.h
        )

    set(COCOS_AUDIO_PLATFORM_SRC
        audio/linux/SimpleAudioEngine.cpp
        audio/AudioEngineImpl.cpp
        audio/AudioCache.cpp
        audio/AudioPlayer.cpp
        audio/AudioDecoder.cpp
        audio/AudioDecoderManager.cpp
        audio/AudioDecoderMp3.cpp
        audio/AudioDecoderOgg.cpp
        audio/AudioDecoderWav.cpp
        audio/AudioFileStream.cpp
        )

elseif(APPLE)
    # common
    set(COCOS_AUDIO_PLATFORM_HEADER
        audio/apple/AudioDecoder.h
        audio/apple/AudioPlayer.h
        audio/apple/AudioCache.h
        audio/apple/AudioEngineImpl.h
        )
    set(COCOS_AUDIO_PLATFORM_SRC
        audio/apple/AudioCache.mm
        audio/apple/AudioDecoder.mm
        audio/apple/AudioEngineImpl.mm
        audio/apple/AudioPlayer.mm
        )
    # split it in _C and non C
    # because C files needs to be compiled with C compiler and not C++
    # compiler
    if(MACOSX)
        set(COCOS_AUDIO_PLATFORM_HEADER
            ${COCOS_AUDIO_PLATFORM_HEADER}
            audio/mac/CDConfig.h
            audio/mac/SimpleAudioEngine_objc.h
            audio/mac/CDOpenALSupport.h
            audio/mac/CDXMacOSXSupport.h
            audio/mac/CDAudioManager.h
            audio/mac/CocosDenshion.h
            )
        set(COCOS_AUDIO_PLATFORM_SRC_C
            audio/mac/CDAudioManager.m
            audio/mac/CDOpenALSupport.m
            audio/mac/CocosDenshion.m
            audio/mac/SimpleAudioEngine_objc.m
            )
        set(COCOS_AUDIO_PLATFORM_SRC
            ${COCOS_AUDIO_PLATFORM_SRC_C}
            ${COCOS_AUDIO_PLATFORM_SRC}
            audio/mac/SimpleAudioEngine.mm
            audio/mac/CDXMacOSXSupport.mm
            )
    elseif(IOS)
        set(COCOS_AUDIO_PLATFORM_HEADER
            ${COCOS_AUDIO_PLATFORM_HEADER}
            audio/ios/CDConfig.h
            audio/ios/SimpleAudioEngine_objc.h
            audio/ios/CDOpenALSupport.h
            audio/ios/CDAudioManager.h
            audio/ios/CocosDenshion.h
            )
        set(COCOS_AUDIO_PLATFORM_SRC_C
            audio/ios/CDAudioManager.m
            audio/ios/CDOpenALSupport.m
            audio/ios/CocosDenshion.m
            audio/ios/SimpleAudioEngine_objc.m
            )
        set(COCOS_AUDIO_PLATFORM_SRC
            ${COCOS_AUDIO_PLATFORM_SRC_C}
            ${COCOS_AUDIO_PLATFORM_SRC}
            audio/ios/SimpleAudioEngine.mm
            )
    endif()

    set_source_files_properties(
            ${COCOS_AUDIO_PLATFORM_SRC_C}
            PROPERTIES LANGUAGE C
    )
endif()

set(COCOS_AUDIO_HEADER
    audio/include/AudioEngine.h
    audio/include/Export.h
    audio/include/AudioMacros.h
    audio/include/SimpleAudioEngine.h
    ${COCOS_AUDIO_PLATFORM_HEADER}
    )
list(APPEND COCOS_AUDIO_SRC ${COCOS_AUDIO_PLATFORM_SRC})
