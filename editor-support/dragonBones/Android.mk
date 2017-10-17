LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := dragonbones_static

LOCAL_MODULE_FILENAME := libdragonbones

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
animation/Animation.cpp \
model/AnimationData.cpp \
animation/AnimationState.cpp \
armature/Armature.cpp \
model/ArmatureData.cpp \
factories/BaseFactory.cpp \
core/BaseObject.cpp \
armature/Bone.cpp \
CCArmatureDisplay.cpp \
CCFactory.cpp \
CCSlot.cpp \
CCTextureData.cpp \
parsers/DataParser.cpp \
model/DragonBonesData.cpp \
events/EventObject.cpp \
model/FrameData.cpp \
parsers/JSONDataParser.cpp \
armature/Slot.cpp \
textures/TextureData.cpp \
model/TimelineData.cpp \
animation/TimelineState.cpp \
animation/WorldClock.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_STATIC_LIBRARIES := cocos2dx_internal_static

include $(BUILD_STATIC_LIBRARY)
