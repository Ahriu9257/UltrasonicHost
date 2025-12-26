# 超声波测距上位机系统

基于 C++ 和 Qt6 开发的超声波测距上位机软件，支持实时数据显示、波形绘制、数据保存和查询等功能。

## 功能特性

### 核心功能
- **串口通信**: 支持 UART 串口通信，可配置波特率
- **实时显示**: 实时显示当前测距值
- **波形图**: 动态绘制距离-时间曲线
- **数据保存**: 自动/手动保存测距数据到 SQLite 数据库
- **数据查询**: 支持历史数据查询和统计分析
- **数据导出**: 支持导出为 CSV 和 TXT 格式

### 界面功能
- 串口连接状态指示
- 实时距离显示（大字体）
- 波形图暂停/清空控制
- 数据统计（总数、平均值、最大值、最小值）
- 操作日志记录

## 技术栈

- **语言**: C++17
- **GUI框架**: Qt6 (Widgets, SerialPort, Charts, Sql)
- **构建系统**: CMake 3.16+
- **数据库**: SQLite

## 编译与运行

### 依赖安装

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install -y build-essential cmake git
sudo apt install -y qt6-base-dev qt6-serialport-dev qt6-charts-dev libqt6sql6-sqlite
```

#### Arch Linux
```bash
sudo pacman -S base-devel cmake git
sudo pacman -S qt6-base qt6-serialport qt6-charts
```

#### macOS (Homebrew)
```bash
brew install cmake qt@6
```

#### Windows

**方法 1: 使用 Qt 官方在线安装器（推荐）**

1. 下载 Qt 在线安装器
   - 访问 https://www.qt.io/download-qt-installer
   - 下载 Windows 版本的在线安装器

2. 运行安装器并选择组件
   ```
   Qt 6.5.x (或更高版本)
   ├── Desktop gcc 64-bit / MSVC 2019 64-bit
   ├── Qt Charts
   ├── Qt SerialPort
   └── Qt SQL (SQLite)

   开发工具
   ├── Qt Creator
   ├── CMake
   └── Ninja
   ```

3. 默认安装路径：`C:\Qt\6.x.x\msvc2019_64` 或 `C:\Qt\6.x.x\mingw_64`

**方法 2: 使用 MSYS2（命令行方式）**

```bash
# 1. 下载并安装 MSYS2: https://www.msys2.org/

# 2. 在 MSYS2 终端执行
pacman -Syu
pacman -S mingw-w64-x86_64-qt6 mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
pacman -S mingw-w64-x86_64-qt6-charts mingw-w64-x86_64-qt6-serialport
```

### 编译步骤

#### Linux / macOS

```bash
# 1. 创建构建目录
mkdir build && cd build

# 2. 配置项目
cmake ..

# 3. 编译
make -j$(nproc)

# 4. 运行
./UltrasonicHost
```

或者使用提供的编译脚本：
```bash
chmod +x build.sh
./build.sh
```

#### Windows

**方法 1: 使用 Qt Creator（推荐，图形界面）**

1. 打开 Qt Creator
2. 点击 `File` → `Open File or Project`
3. 选择项目根目录下的 `CMakeLists.txt` 文件
4. 选择 Kit（构建工具链）
   - 推荐：Desktop Qt 6.x.x MSVC2019 64bit
   - 或者：Desktop Qt 6.x.x MinGW 64-bit
5. 点击左下角的 "锤子" 图标（Build）编译项目
6. 点击绿色 "三角形" 图标（Run）运行程序

**方法 2: 使用命令行（CMD 或 PowerShell）**

```batch
REM 1. 打开 "Qt 6.x.x (MSVC 2019 64-bit)" 命令提示符
REM    或者在开始菜单搜索 "Qt" 找到对应的终端

REM 2. 进入项目目录
cd C:\path\to\chaoshengbo

REM 3. 创建构建目录
mkdir build
cd build

REM 4. 配置项目
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..

REM 5. 编译
ninja

REM 6. 运行
UltrasonicHost.exe
```

**方法 3: 使用 MSYS2 终端**

```bash
# 在 MSYS2 MinGW64 终端中执行
cd /c/path/to/chaoshengbo
mkdir build && cd build
cmake -G "Ninja" ..
ninja
./UltrasonicHost.exe
```

## 使用说明

### 1. 串口配置

**Linux**
- 串口名称：`/dev/ttyUSB0`, `/dev/ttyACM0`, `/dev/ttyS0` 等
- 在"Serial Port Control"区域选择串口
- 设置波特率（默认 9600）
- 点击"Connect"连接设备

**Windows**
- 串口名称：`COM1`, `COM2`, `COM3` 等
- 查看设备管理器确认串口号：
  1. 右键 "此电脑" → "管理" → "设备管理器"
  2. 展开 "端口(COM 和 LPT)"
  3. 找到对应的 USB Serial Port (COMx)
- 在上位机界面选择对应的 COM 口
- 如果遇到权限问题，使用管理员权限运行程序

### 2. 数据接收协议

上位机支持以下数据格式：

**格式 1**: 带前缀
```
D:123.45\n
DIST:123.45\n
```

**格式 2**: 纯数字
```
123.45\n
```

- 数据单位：厘米 (cm)
- 有效范围：0-500 cm
- 结束符：`\n` 或 `\r\n`

### 3. 数据保存
- 勾选"Auto Save Data"可自动保存接收到的数据
- 点击"Save Current"手动保存当前显示的值
- 数据存储在 `ultrasonic_data.db` SQLite 数据库中

### 4. 数据查询与导出
- 点击"Query All"查看最近 20 条记录
- 点击"Export CSV"导出为 CSV 格式
- 点击"Export TXT"导出为文本报告格式

### 5. 波形图控制
- "Pause"暂停/恢复波形更新
- "Clear Chart"清空波形图数据

## 项目结构

```
.
├── CMakeLists.txt           # CMake 配置文件
├── README.md                # 项目说明
├── build.sh                 # 编译脚本
└── src/
    ├── main.cpp             # 程序入口
    ├── mainwindow.h/cpp     # 主窗口（UI 整合）
    ├── serialport.h/cpp     # 串口通信模块
    ├── datamanager.h/cpp    # 数据管理模块
    └── chartwidget.h/cpp    # 波形图显示模块
```

## 模块说明

### SerialPortHandler
- 负责串口通信
- 自动解析接收数据
- 支持多种数据格式
- 错误处理和状态通知

### DataManager
- SQLite 数据库管理
- 数据增删查改
- 统计分析（平均值、最大值、最小值）
- CSV/TXT 导出

### ChartWidget
- 基于 Qt Charts 的实时波形图
- 自动滚动显示
- 可暂停/恢复/清空
- 可配置显示点数和坐标轴范围

### MainWindow
- 整合所有功能模块
- 提供完整的用户界面
- 信号槽连接和事件处理

## 硬件接口

### 嵌入式端（STM32/Arduino）示例代码

```c
// Arduino 示例
float distance = measureDistance();  // 你的测距函数
Serial.print("D:");
Serial.println(distance);
delay(100);
```

```c
// STM32 HAL 示例
char buffer[32];
float distance = measureDistance();
sprintf(buffer, "D:%.2f\r\n", distance);
HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
HAL_Delay(100);
```

## 常见问题

### Q1: 无法找到串口

**Linux**
- 检查设备是否正确连接
- 检查用户权限：`sudo usermod -a -G dialout $USER`（需重新登录）
- 查看串口设备：`ls /dev/tty*`

**Windows**
- 检查设备管理器中是否有设备（右键"此电脑"→"管理"→"设备管理器"）
- 如果有黄色感叹号，需要安装驱动程序
- CH340/CH341 芯片：下载 CH340 驱动
- CP2102 芯片：下载 Silicon Labs CP210x 驱动
- FT232 芯片：下载 FTDI VCP 驱动
- 点击上位机的"Refresh"按钮刷新串口列表

### Q2: 数据接收异常
- 检查波特率设置是否与硬件一致
- 检查数据格式是否符合协议（需要换行符 `\n` 或 `\r\n`）
- 查看日志区域的错误信息
- Windows 下确保没有其他程序占用串口（如 Arduino IDE 的串口监视器）

### Q3: 编译错误

**Linux**
- 确保 Qt6 版本 >= 6.2
- 检查 CMake 版本 >= 3.16
- 确保所有 Qt 组件都已安装
- 缺少 Charts 模块：`sudo apt install qt6-charts-dev`

**Windows**
- 确保使用正确的 Qt 命令提示符（包含 Qt 环境变量）
- 检查 CMake 生成器选择正确（`-G "Ninja"` 或 `-G "MinGW Makefiles"`）
- 使用 MSVC 编译器时，需要安装 Visual Studio 2019 或更高版本
- 如果遇到 "ninja: error"，确保在 Qt 安装时选择了 Ninja 工具

### Q4: Windows 下程序无法运行（缺少 DLL）

**问题现象**
- 提示缺少 `Qt6Core.dll`, `Qt6Widgets.dll` 等文件

**解决方法 1**: 将 Qt DLL 复制到程序目录
```batch
REM 假设 Qt 安装在 C:\Qt\6.5.0\msvc2019_64
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Core.dll build\
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Widgets.dll build\
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Gui.dll build\
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6SerialPort.dll build\
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Charts.dll build\
copy C:\Qt\6.5.0\msvc2019_64\bin\Qt6Sql.dll build\
```

**解决方法 2**: 使用 Qt 部署工具（推荐）
```batch
REM 在 Qt 命令提示符中执行
cd build
windeployqt UltrasonicHost.exe
```

**解决方法 3**: 在 Qt Creator 中运行（自动处理依赖）

### Q5: Windows 下虚拟串口测试

如果没有真实硬件，可以使用虚拟串口工具测试：

**推荐工具**
- **com0com**: 免费开源虚拟串口驱动
  - 下载：https://sourceforge.net/projects/com0com/
  - 创建虚拟串口对（如 COM10 ↔ COM11）

- **Virtual Serial Port Driver**: 商业软件，功能强大
  - 提供试用版

**使用步骤**
1. 安装 com0com 并创建虚拟串口对（如 COM10 和 COM11）
2. 使用串口调试助手（如 SSCOM）向 COM10 发送数据
3. 上位机连接到 COM11 接收数据

或者使用 Python 脚本模拟：
```batch
REM 安装 Python 和 pyserial
pip install pyserial

REM 运行模拟器（需要修改串口号为 Windows 格式）
python examples\test_serial_simulator.py COM10
```

## 开发者信息

- **开发工具**: Qt Creator / Visual Studio Code
- **调试**: 使用 Qt Creator 的图形调试器
- **代码风格**: 遵循 Qt 编码规范

## 许可证

本项目仅供学习和研究使用。

## 更新日志

### v1.0.0 (2025-12-24)
- 初始版本发布
- 完整的串口通信功能
- 实时波形图显示
- 数据保存和查询
- 数据导出（CSV/TXT）
