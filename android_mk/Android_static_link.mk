LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/PlaneSceneManager.cpp \
../../Classes/WinLayer.cpp \
../../Classes/LoseLayer.cpp \
../../Classes/Prop.cpp \
../../Classes/FielderSprite.cpp \
../../Classes/BallTest.cpp \
../../Classes/SetLayer.cpp \
../../Classes/HelpLayer.cpp \
../../Classes/OutLayer.cpp \
../../Classes/AchieveLayer.cpp \
../../Classes/DialogLayer.cpp \
../../Classes/BanQiuLayer.cpp \
../../Classes/NewGameLayer.cpp \
../../Classes/BeginLayer.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/HelloWorldScene.cpp \
../../Classes/BanQiuSceneManager.cpp \
../../Classes/OpenLayer.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static 


include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,editor-support/cocostudio)