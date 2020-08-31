LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include $(LOCAL_PATH)/../av.mk

LOCAL_SRC_FILES := $(FFFILES)

include $(LOCAL_PATH)/$(ARCH)/Makefile

LOCAL_C_INCLUDES :=        \
    $(LOCAL_PATH)        \
    $(LOCAL_PATH)/..

LOCAL_CFLAGS += $(FFCFLAGS)

LOCAL_LDLIBS := -lz

LOCAL_STATIC_LIBRARIES := $(FFLIBS)

LOCAL_MODULE := $(FFNAME)

include $(BUILD_STATIC_LIBRARY)