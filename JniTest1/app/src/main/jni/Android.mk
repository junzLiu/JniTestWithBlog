# 指定当前文件的路径，必须在文件的开头定义
LOCAL_PATH := $(call my-dir)

# 清除几乎全部 LOCAL_XXX 变量
include $(CLEAR_VARS)

# 构建的模块名
LOCAL_MODULE := hello-test

# so文件名
# LOCAL_MODULE_FILENAME := libhellotest.so

# 设置源文件
LOCAL_SRC_FILES := test.cpp

# 帮助系统将所有内容连接到一起
include $(BUILD_SHARED_LIBRARY)