LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos_webview_plugin_static
LOCAL_MODULE_FILENAME := libwebviewplugin

LOCAL_SRC_FILES := \
		WebView.cpp \
		WebViewImpl_android.cpp \
		jni/org_cocos2dx_lib_Cocos2dxWebViewHelper.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../cocos2dx \
					$(LOCAL_PATH)/../../../../cocos2dx/platform \
					$(LOCAL_PATH)/../../../../cocos2dx/platform/android \
					$(LOCAL_PATH)/../ \
					$(LOCAL_PATH)/../../ \
					$(LOCAL_PATH)/ \
					$(LOCAL_PATH)/jni

include $(BUILD_STATIC_LIBRARY)
