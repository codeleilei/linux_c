	LOCAL_PATH := $(call my-dir)
	
	include $(CLEAR_VARS)
	LOCAL_SRC_FILES :=  Ec20_hotdet.cpp
	LOCAL_MODULE_TAGS := optional
	LOCAL_SHARED_LIBRARIES := liblog libcutils libandroid_runtime libnativehelper
	LOCAL_LDLIBS    := -llog 
	LOCAL_MODULE:=Ec20_Service
	include $(BUILD_EXECUTABLE)