# FFmpegStudy

`FFmpegStudy` 是一个基于 Android NDK / JNI 和 Jetpack Compose 构建的音视频开发学习与实践项目。本项目演示了如何在 Android 平台集成 FFmpeg 动态库，并通过 C++ 封装底层音视频处理能力（如编解码器查询、音视频解复用与无损音频提取等），结合现代化 Compose UI 进行展示与交互。

---

## 🌟 功能特性

- **FFmpeg 动态库集成**：
  - 支持 `arm64-v8a` 和 `armeabi-v7a` 双架构 ABI；
  - 链接 `avutil`、`avcodec`、`avformat`、`avfilter`、`avdevice`、`swresample`、`swscale` 全套核心库。
- **音视频信息与能力查询**：
  - 查询 FFmpeg 版本信息 (`ffmpegVersion`)；
  - 动态遍历当前库支持的所有音频/视频解码器列表 (`getSupportedDecoders`)；
  - 动态遍历当前库支持的所有音频/视频编码器列表 (`getSupportedEncoders`)。
- **音视频解复用与音频提取 (Demuxing & Audio Extraction)**：
  - 基于 FFmpeg 原生 C++ API 快速解析视频容器；
  - 自动寻找最佳音频流并直接无损封装提取音频（无需重新编解码，性能极高）；
  - 支持提取并保存至系统公共音乐目录 (`Environment.DIRECTORY_MUSIC`)。
- **现代化 Android 界面与相册媒体加载**：
  - 基于 Jetpack Compose + Material 3 构建沉浸式响应式 UI；
  - 利用系统 MediaStore API 检索本地视频媒体；
  - 集成 Coil 3 视频帧解码扩展 (`coil-video`) 异步加载与渲染视频缩略图。

---

## 🏗️ 项目架构与目录说明

```text
FFmpegStudy/
├── app/
│   ├── src/
│   │   ├── main/
│   │   │   ├── cpp/                         # C/C++ 原生层
│   │   │   │   ├── include/                 # FFmpeg 头文件 (libavcodec, libavformat 等)
│   │   │   │   ├── CMakeLists.txt           # CMake 构建配置
│   │   │   │   ├── ffmpeg_basic.h           # 通用头文件与日志宏定义
│   │   │   │   ├── ffmpeg_utils.h/.cpp      # FFmpeg 工具与错误处理封装
│   │   │   │   ├── ffmpeg_wrapper.h/.cpp    # FFmpeg 核心业务逻辑 (初始化、提取音频、遍历编解码器)
│   │   │   │   └── native-lib.cpp           # JNI 接口导出与数据类型转换
│   │   │   ├── java/com/wzvideni/ffmpegstudy/
│   │   │   │   ├── data/                    # 数据实体模型 (VideoInfo)
│   │   │   │   ├── expansion/               # 扩展函数 (MediaStore 检索等)
│   │   │   │   ├── ui/theme/                # Compose 主题配置
│   │   │   │   ├── FFmpegNative.kt          # JNI 桥接单例
│   │   │   │   └── MainActivity.kt          # 主界面与 Compose 视图
│   │   │   └── jniLibs/                     # 预编译 FFmpeg 动态库 (.so)
│   │   │       ├── arm64-v8a/
│   │   │       └── armeabi-v7a/
│   │   └── AndroidManifest.xml
│   └── build.gradle.kts
├── gradle/libs.versions.toml                # 版本目录 (Version Catalogs)
├── build.gradle.kts
└── settings.gradle.kts
```

---

## 🛠️ 技术栈与依赖

| 模块 / 组件 | 技术选型 | 版本 / 说明 |
| :--- | :--- | :--- |
| **开发语言** | Kotlin + C++17 | Kotlin 2.2.0 |
| **UI 框架** | Jetpack Compose + Material 3 | Compose BOM 2025.07.00 |
| **媒体加载** | Coil 3 | 3.3.0 (`coil-compose`, `coil-video`) |
| **多媒体库** | FFmpeg | 预编译 7.x 系列 Shared Libraries |
| **Native 构建** | CMake | 3.22.1+ |
| **NDK 版本** | Android NDK | 29.0.14206865 |
| **编译目标** | Android SDK | compileSdk 36 / minSdk 24 / targetSdk 36 |
| **JDK 环境** | Java SE | OpenJDK 17 |

---

## 🚀 编译与运行

### 1. 环境准备
- **Android Studio**：推荐 Android Studio Ladybug / Meerkat 或更高版本；
- **Android SDK & NDK**：在 SDK Manager 中安装 SDK Platform 36 及 NDK (29.x)；
- **CMake**：在 SDK Manager 中安装 CMake 3.22.1 或以上版本；
- **JDK**：配置 Gradle JDK 为 JDK 17。

### 2. 克隆与构建
```bash
git clone <repository_url>
cd FFmpegStudy
```

在 Android Studio 中打开项目，等待 Gradle Sync 和 CMake 配置完成。

### 3. 运行与权限
- 连接 Android 7.0 (API 24) 及以上真机或模拟器；
- 运行应用后，点击右下角刷新按钮授予媒体读取权限 (`READ_MEDIA_VIDEO` / `READ_MEDIA_AUDIO` / `READ_EXTERNAL_STORAGE`)；
- 点击视频缩略图即可调用底层 FFmpeg 进行音频提取，输出文件将保存至设备的 Music 目录。

---

## 📄 License

本项目仅供学习与交流使用。FFmpeg 遵循其各自的开源协议（LGPL / GPL）。
