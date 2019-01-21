LOCAL_PATH := $(call my-dir)

#New AudioEngine
include $(CLEAR_VARS)

LOCAL_MODULE := audio

LOCAL_MODULE_FILENAME := libaudio

LOCAL_SRC_FILES := ../AudioEngineImpl.cpp \
                   ../AudioEngine.cpp \
                   ../AudioDecoder.cpp \
                   ../AudioDecoderOgg.cpp \
                   ../AudioDecoderMp3.cpp \
                   ../AudioDecoderWav.cpp \
                   ../AudioCache.cpp \
                   ../AudioDecoderManager.cpp \
                   ../AudioPlayer.cpp \
                   ../AudioFileStream.cpp \
                   utils/Utils.cpp


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_EXPORT_LDLIBS := -lOpenSLES

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include \
                    $(LOCAL_PATH)/../.. \
                    $(LOCAL_PATH)/../../platform/android \
                    $(LOCAL_PATH)/../../../external/android-specific/OggDecoder/include \
                    $(LOCAL_PATH)/../../../external/android-specific/MP3Decoder/include \
                    $(LOCAL_PATH)/../../../external/android-specific/OpenalSoft/include

LOCAL_SHARED_LIBRARIES += mpg123_shared openal_shared
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ogg_static cocos_vorbis_static cocos_vorbisfile_static
include $(BUILD_STATIC_LIBRARY)

#SimpleAudioEngine
include $(CLEAR_VARS)

LOCAL_MODULE := ccds

LOCAL_MODULE_FILENAME := libccds

LOCAL_SRC_FILES := cddSimpleAudioEngine.cpp \
                   ccdandroidUtils.cpp \
                   jni/cddandroidAndroidJavaEngine.cpp

LOCAL_STATIC_LIBRARIES := audio
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include \
                    $(LOCAL_PATH)/../.. \
                    $(LOCAL_PATH)/../../platform/android

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android-specific/OggDecoder/prebuilt)
$(call import-module,android-specific/MP3Decoder/prebuilt)
$(call import-module,android-specific/OpenalSoft/prebuilt)
