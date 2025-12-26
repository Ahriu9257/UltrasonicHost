# Windows 平台使用指南

本文档专门针对 Windows 用户，提供详细的安装、编译和使用说明。

## 系统要求

- **操作系统**: Windows 10/11 (64位)
- **开发环境**: Qt 6.2 或更高版本
- **编译器**: MSVC 2019+ 或 MinGW 64-bit
- **磁盘空间**: 至少 5GB（用于 Qt 安装）

## 一、安装依赖

### 方法 1: Qt 官方安装器（推荐）

#### 1.1 下载 Qt 安装器

访问 Qt 官网下载页面：
https://www.qt.io/download-qt-installer

下载 "Qt Online Installer for Windows"

#### 1.2 运行安装器

1. 双击运行下载的安装器
2. 登录或注册 Qt 账号（免费）
3. 选择安装路径（推荐：`C:\Qt`）
4. 在组件选择界面，勾选以下内容：

```
Qt 6.5.x (或更高版本)
├─ Desktop gcc 64-bit (MinGW) 或 MSVC 2019 64-bit
├─ Qt Charts
├─ Qt SerialPort
└─ Qt SQL (SQLite)

开发工具
├─ Qt Creator (IDE)
├─ CMake
├─ Ninja
└─ MinGW 64-bit (如果选择了 MinGW 版本)
```

5. 点击"下一步"开始安装（大约需要 3-10GB 空间）
6. 等待安装完成（可能需要 10-30 分钟，取决于网速）

### 方法 2: MSYS2（命令行爱好者）

#### 2.1 安装 MSYS2

1. 访问 https://www.msys2.org/
2. 下载安装器并运行
3. 安装到 `C:\msys64`（默认路径）

#### 2.2 安装 Qt6 和开发工具

在 MSYS2 MinGW64 终端中执行：

```bash
# 更新包数据库
pacman -Syu

# 安装 Qt6 及相关组件
pacman -S mingw-w64-x86_64-qt6 \
          mingw-w64-x86_64-qt6-charts \
          mingw-w64-x86_64-qt6-serialport \
          mingw-w64-x86_64-cmake \
          mingw-w64-x86_64-ninja \
          mingw-w64-x86_64-gcc
```

---

## 二、编译项目

### 方法 1: 使用 Qt Creator（推荐，适合初学者）

#### 2.1 打开项目

1. 启动 Qt Creator
   - 开始菜单搜索 "Qt Creator" 并打开

2. 打开项目文件
   - 点击 `文件` → `打开文件或项目`
   - 浏览到项目目录，选择 `CMakeLists.txt` 文件
   - 点击"打开"

#### 2.2 配置 Kit

1. 在弹出的配置界面，选择一个 Kit（构建工具链）
   - 推荐：`Desktop Qt 6.x.x MSVC2019 64bit`
   - 或者：`Desktop Qt 6.x.x MinGW 64-bit`
   
2. 点击"Configure Project"（配置项目）

#### 2.3 编译和运行

1. **编译**：点击左下角的 "锤子" 图标（Build）
   - 快捷键：`Ctrl + B`
   - 等待编译完成（首次编译可能需要 1-2 分钟）

2. **运行**：点击绿色的 "三角形" 图标（Run）
   - 快捷键：`Ctrl + R`
   - 程序将自动启动

3. **调试**（可选）：点击左下角的 "虫子" 图标（Debug）
   - 快捷键：`F5`

### 方法 2: 使用命令行编译

#### 2.1 打开 Qt 命令提示符

1. 在开始菜单搜索 "Qt 6.x.x (MSVC 2019 64-bit)"
2. 右键选择"以管理员身份运行"

#### 2.2 使用自动构建脚本

```batch
REM 进入项目目录
cd C:\path\to\chaoshengbo

REM 运行构建脚本
build.bat
```

#### 2.3 手动构建步骤

```batch
REM 进入项目目录
cd C:\path\to\chaoshengbo

REM 创建构建目录
mkdir build
cd build

REM 配置项目（使用 Ninja）
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..

REM 或者使用 NMake（MSVC）
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..

REM 编译
ninja
REM 或者 nmake

REM 运行
UltrasonicHost.exe
```

---

## 三、解决 DLL 缺失问题

### 问题描述

双击运行 `UltrasonicHost.exe` 时，可能出现以下错误：
- "无法启动此程序，因为计算机中丢失 Qt6Core.dll"
- "应用程序无法正常启动(0xc000007b)"

### 解决方法

#### 方法 1: 使用 windeployqt（推荐）

在 Qt 命令提示符中执行：

```batch
cd build
windeployqt UltrasonicHost.exe
```

这会自动复制所有需要的 DLL 到 build 目录。

#### 方法 2: 手动复制 DLL

假设 Qt 安装在 `C:\Qt\6.5.0\msvc2019_64`：

```batch
cd build
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Core.dll .
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Gui.dll .
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Widgets.dll .
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6SerialPort.dll .
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Charts.dll .
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Sql.dll .
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6OpenGL.dll .
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6OpenGLWidgets.dll .
```

#### 方法 3: 在 Qt Creator 中运行（自动处理）

直接在 Qt Creator 中点击运行按钮，Qt Creator 会自动处理依赖。

---

## 四、串口配置

### 4.1 查看串口设备

1. 连接 USB 转串口设备到电脑
2. 打开设备管理器：
   - 右键"此电脑" → "管理" → "设备管理器"
   - 或按 `Win + X`，选择"设备管理器"
3. 展开"端口(COM 和 LPT)"
4. 找到你的设备，如 "USB Serial Port (COM3)"

### 4.2 安装串口驱动

如果设备管理器中显示黄色感叹号，需要安装驱动：

| 芯片型号 | 驱动名称 | 下载地址 |
|---------|---------|---------|
| CH340/CH341 | CH340 驱动 | http://www.wch.cn/downloads/CH341SER_EXE.html |
| CP2102/CP2104 | Silicon Labs CP210x | https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers |
| FT232/FT231X | FTDI VCP 驱动 | https://ftdichip.com/drivers/vcp-drivers/ |

### 4.3 在上位机中连接

1. 打开超声波测距上位机
2. 在"Serial Port Control"区域
3. 点击"Refresh"刷新串口列表
4. 选择对应的 COM 口（如 COM3）
5. 设置波特率为 9600
6. 点击"Connect"按钮

---

## 五、无硬件测试

### 5.1 使用虚拟串口

#### 安装 com0com

1. 访问 https://sourceforge.net/projects/com0com/
2. 下载 `setup_com0com_xxx.exe`
3. 运行安装器（需要管理员权限）
4. 在安装过程中，Windows 可能会警告"未签名驱动"
   - Windows 10: 重启进入"高级启动选项"→"禁用驱动程序强制签名"
   - Windows 11: 在设置中禁用"内存完整性"

#### 配置虚拟串口对

1. 打开 com0com 配置工具
2. 添加端口对：COM10 ↔ COM11
3. 点击"Apply"应用设置

#### 测试方法 1: 使用 Python 脚本

```batch
REM 安装 Python（从 python.org 下载）
REM 安装 pyserial
pip install pyserial

REM 运行模拟器（发送数据到 COM10）
python examples\test_serial_simulator.py COM10

REM 另一个终端：运行上位机（连接到 COM11）
cd build
UltrasonicHost.exe
```

#### 测试方法 2: 使用串口调试助手

1. 下载串口调试助手：
   - SSCOM: https://www.amobbs.com/
   - 友善串口调试助手
   - Serial Port Utility

2. 配置调试助手
   - 串口：COM10
   - 波特率：9600
   - 数据位：8
   - 停止位：1
   - 校验：无

3. 手动发送数据
   ```
   D:125.50
   D:130.25
   D:128.75
   ```

4. 上位机连接到 COM11，观察数据接收

---

## 六、常见问题

### Q1: 安装 Qt 时提示空间不足

**解决**：Qt 完整安装需要约 5-10GB 空间
- 清理磁盘空间
- 或者只安装需要的组件（Qt 6.x.x MSVC 64-bit + Charts + SerialPort）

### Q2: 编译时提示找不到 CMake

**解决**：
- 确保在"Qt 命令提示符"中运行，而不是普通的 CMD
- 或者手动添加 CMake 到系统 PATH：
  ```
  C:\Qt\Tools\CMake_64\bin
  ```

### Q3: 运行时提示 VCRUNTIME140.dll 缺失

**解决**：安装 Visual C++ 运行库
- 下载地址：https://aka.ms/vs/17/release/vc_redist.x64.exe
- 双击安装

### Q4: 串口无法打开（Access Denied）

**解决**：
- 关闭其他占用串口的程序（Arduino IDE, PuTTY 等）
- 以管理员权限运行上位机
- 重新插拔 USB 设备

### Q5: 程序界面显示异常（字体很小或模糊）

**解决**：Windows 高 DPI 缩放问题
- 右键程序 → 属性 → 兼容性
- 勾选"替代高 DPI 缩放行为"
- 选择"系统（增强）"

### Q6: 无法创建虚拟串口（com0com 安装失败）

**解决**：
- 禁用驱动程序强制签名（Windows 10/11）
- 或使用付费工具：Virtual Serial Port Driver

---

## 七、发布程序

如果要将程序分发给其他 Windows 用户，需要创建独立的可执行包。

### 7.1 使用 windeployqt 打包

```batch
REM 在 Qt 命令提示符中
cd build

REM 自动复制依赖
windeployqt UltrasonicHost.exe

REM 创建发布目录
mkdir ..\release
xcopy /E /I /Y * ..\release

REM 打包为 ZIP
cd ..
powershell Compress-Archive -Path release -DestinationPath UltrasonicHost-v1.0-Windows.zip
```

### 7.2 手动打包步骤

1. 创建发布目录
2. 复制以下文件到发布目录：
   - `UltrasonicHost.exe`
   - 所有 Qt DLL（通过 windeployqt 自动复制）
   - `platforms\` 目录（Qt 平台插件）
   - `styles\` 目录（Qt 样式插件）
   - 可选：`examples\` 目录（示例代码）
3. 打包为 ZIP 或使用 Inno Setup 创建安装器

---

## 八、开发建议

### 8.1 推荐的开发环境

- **IDE**: Qt Creator（功能强大，集成调试器）
- **文本编辑器**: VS Code + Qt 扩展
- **版本控制**: Git for Windows

### 8.2 调试技巧

在 Qt Creator 中：
1. 设置断点：点击代码行号左侧
2. 启动调试：按 `F5`
3. 单步执行：
   - `F10` - 单步跳过
   - `F11` - 单步进入
   - `Shift+F11` - 跳出
4. 查看变量：鼠标悬停或在"Locals"窗口查看

---

## 九、更新和维护

### 更新 Qt 版本

1. 运行 Qt 维护工具（MaintenanceTool.exe）
2. 选择"添加或移除组件"
3. 更新到最新版本

### 清理构建缓存

```batch
REM 删除构建目录
rmdir /s /q build

REM 重新构建
build.bat
```

---

## 十、获取帮助

- **项目文档**: 查看 README.md
- **快速开始**: 查看 QUICKSTART.md
- **问题反馈**: 联系开发者或提交 Issue

---

## 附录：常用命令速查

```batch
REM 编译项目
build.bat

REM 清理并重新编译
rmdir /s /q build && build.bat

REM 部署程序（复制 DLL）
cd build && windeployqt UltrasonicHost.exe

REM 运行程序
cd build && UltrasonicHost.exe

REM 查看 Qt 版本
qmake --version

REM 查看 CMake 版本
cmake --version
```

---

**版本**: v1.0.0  
**更新日期**: 2025-12-24  
**适用平台**: Windows 10/11 (64-bit)
