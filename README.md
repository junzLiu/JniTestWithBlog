# JniTestWithBlog

## Android JNI技术的应用

> **JNI**: JNI is the Java Native Interface. It defines a way for managed code (written in the Java programming language) to interact with native code (written in C/C++).

- 目前官方提供了两种应用方案
  - NDK
  - CMake



---
### NDK
> 原生开发工具包 (NDK) 是一组可让您在 Android 应用中利用 C 和 C++ 代码的工具。 可用以从您自己的源代码构建，或者利用现有的预构建库。

目前的绝大多数需要用到JNI技术的第三方都是使用的NDK



#### NDK的配置文件
- [Android.mk](https://developer.android.com/ndk/guides/android_mk.html)  
Android.mk文件位于项目 jni/ 目录的子目录中，用于向构建系统描述源文件和共享库。是构建so的必要配置文件

- [Application.mk](https://developer.android.com/ndk/guides/application_mk.html)

  此文件用于描述应用需要的原生模块。 模块可以是静态库、共享库或可执行文件。



#### 应用流程
- ##### ==创建JNI文件夹==，可以创建系统推荐的JNI文件夹，操作如图
![](http://ww1.sinaimg.cn/mw690/9c096ccbgy1fmlv9o86vvj20nt0j0dic.jpg)

也可以自己指定位置创建文件夹，之后可以在Gradle文件中把JNI路径指向该文件夹
``` gradle
android {
    sourceSets.main {
        jni.srcDir "jniPath"
    }
}
```

- ##### ==创建及编写C++文件及与之关联的Java文件==

C++ 里面jni方法名是由关联的Java文件的包名和类名+方法名组成的

example 1

```java
package com.mark.test

public class TestJni {
    
    public native String getString();
}

```

上面代码中的getString方法在C++中的关联方法名为
```
//Java_包名_类名_方法名
Java_com_mark_test_TestJni_getString
```

那么example 1中的getString()方法对应的C++文件如下
``` c++
#include <jni.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mark_test_TestJni_getString(JNIEnv *env){
    char *str = "hello world";
    return env->NewStringUTF(str);
}
```
c++ 代码说明
> - jstring： jni.h中定义的类型，对应java中的String
> - JNIEXPORT： 在windows中对应__declspec(dllexport)标识，表示将该函数导出在外部可以调用
> - JNICALL：在windows中的值为__stdcall，用于约束函数入栈顺序和堆栈清理的规则。
> - JNIEnv:用于本地线程存储,提供了大量的JNI方法，并且是所有原生方法的第一个参数

- ##### ==配置编译文件==
  - ###### 在JNI文件夹根目录下创建 Android.mk文件

    [Android.mk官方说明文档](https://developer.android.com/ndk/guides/android_mk.html)

    以下是最基本的配置
    ``` 
    # 指定当前文件的路径，必须在文件的开头定义
    LOCAL_PATH := $(call my-dir)
    
    # 清除几乎全部 LOCAL_XXX 变量
    include $(CLEAR_VARS)
    
    # 构建的模块名
    LOCAL_MODULE := hello-test
    
    # 设置源文件
    LOCAL_SRC_FILES := test.cpp
    
    # 帮助系统将所有内容连接到一起
    include $(BUILD_SHARED_LIBRARY)
    
    ```
    
  - ###### 在JNI文件夹根目录下创建 Application.mk文件
    [Application.mk官方说明文档](https://developer.android.com/ndk/guides/application_mk.html)
    
    Application.mk文件是非必须文件，创建后，Gradle构建时会自动搜索并加载
    
    我这里只定义了so文件构建的cpu架构类型，其他的可以看官方文档
    ```
    APP_ABI := armeabi-v7a arm64-v8a
    ```
    
  - 配置Gradle文件
    ``` gradle
    android {
    
        externalNativeBuild {
            ndkBuild {
                //设置Android.mk位置
                path "jni/Android.mk"
            }
        }
        
    }
    ```



> OK,包含JNI的项目完成



---
### CMake
> CMake是一个开源的、夸平台的用于构建、测试、打包软件的工具组

- ##### ==创建JNI文件夹及C++文件编写与NDK一样==

- ##### ==配置编译文件==
  - ###### CMake.txt
    在jni目录下或是项目根目录下创建名字为“CMakeLists.txt”的文件
    
    在文件里写入 ```cmake_minimum_required()``` and ```add_library()```
    
    如下
    ``` text
    # 设置最小编译版本
    cmake_minimum_required(VERSION 3.4.1)

    add_library(
      # 库 名称
      test-cmake
      # 库类型
      SHARED
      # 源码地址
      src/main/jni/test.cpp
    )
    ```
- ##### ==关联Gradle==
``` gradle
 externalNativeBuild {
        cmake {
            path "CMakeLists.txt"
        }
    }
```

> OK,包含JNI的项目完成

##### PS:文章只是JNI构建的最基础配置，更多的配置都在参考资料里给出文档地址，自己添加即可


---
### 参考资料
- [向您的项目添加 C 和 C++ 代码](https://developer.android.com/studio/projects/add-native-code.html#new-project)
- [JNI Tips](https://developer.android.com/training/articles/perf-jni.html)
- [Android.mk](https://developer.android.com/ndk/guides/android_mk.html#basics)
- [Application.mk](https://developer.android.com/ndk/guides/application_mk.html)
- [Android NDK 原生 API](https://developer.android.com/ndk/guides/stable_apis.html)
- [C++ 库支持](https://developer.android.com/ndk/guides/cpp-support.html#li)
- [使用预构建库](https://developer.android.com/ndk/guides/prebuilts.html#dm)
- [CMake官网](https://cmake.org/)
- [CMake Android](https://developer.android.com/ndk/guides/cmake.html#variables)
- [Create a CMake build script](https://developer.android.com/studio/projects/configure-cmake.html#create_script)
- [Link Gradle to Your Native Library](https://developer.android.com/studio/projects/gradle-external-native-builds.html)
