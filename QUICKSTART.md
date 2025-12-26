# 快速开始指南

## 5 分钟快速上手

### Linux / macOS

#### 步骤 1: 编译项目

```bash
cd /home/ahriu/software/chaoshengbo
./build.sh
```

#### 步骤 2: 运行程序

```bash
cd build
./UltrasonicHost
```

#### 步骤 3: 连接硬件

1. 将超声波模块通过 USB 转串口连接到电脑
2. 在上位机界面选择对应的串口（如 `/dev/ttyUSB0`）
3. 设置波特率为 `9600`
4. 点击 "Connect" 按钮

---

### Windows

#### 步骤 1: 安装 Qt6

1. 下载 Qt 在线安装器：https://www.qt.io/download-qt-installer
2. 运行安装器，选择以下组件：
   - Qt 6.5.x (MSVC 2019 64-bit 或 MinGW 64-bit)
   - Qt Charts, Qt SerialPort
   - Qt Creator
   - CMake, Ninja

#### 步骤 2: 编译项目（使用 Qt Creator - 推荐）

1. 双击打开 Qt Creator
2. 点击 `文件` → `打开文件或项目`
3. 选择项目根目录下的 `CMakeLists.txt` 文件
4. 选择 Kit（如 Desktop Qt 6.5.x MSVC2019 64bit）
5. 点击左下角 "锤子" 图标编译
6. 点击绿色 "播放" 按钮运行

#### 步骤 2（备选）: 使用命令行编译

```batch
REM 打开 "Qt 6.x.x (MSVC 2019 64-bit)" 命令提示符
cd C:\path\to\chaoshengbo
mkdir build
cd build
cmake -G "Ninja" ..
ninja
UltrasonicHost.exe
```

#### 步骤 3: 连接硬件

1. 将超声波模块通过 USB 转串口连接到电脑
2. 打开设备管理器查看串口号（如 COM3）
   - 右键 "此电脑" → "管理" → "设备管理器"
   - 展开 "端口(COM 和 LPT)"
3. 在上位机界面选择对应的 COM 口
4. 设置波特率为 `9600`
5. 点击 "Connect" 按钮

---

### 步骤 4: 查看数据（所有平台通用）

- 实时距离显示在 "Real-time Display" 区域
- 波形图自动绘制距离变化曲线
- 数据自动保存（如果勾选 "Auto Save Data"）

## 无硬件测试

如果暂时没有硬件，可以使用模拟器测试：

### Linux: 使用虚拟串口

```bash
# 终端 1: 创建虚拟串口对
socat -d -d pty,raw,echo=0 pty,raw,echo=0
# 会输出类似: /dev/pts/3 <--> /dev/pts/4

# 终端 2: 运行模拟器
cd examples
python3 test_serial_simulator.py /dev/pts/3

# 终端 3: 运行上位机，连接到 /dev/pts/4
cd build
./UltrasonicHost
```

### Windows: 使用虚拟串口

**方法 1: 使用 com0com（推荐）**

1. 下载并安装 com0com
   - 访问：https://sourceforge.net/projects/com0com/
   - 下载 setup_com0com_xxx.exe 并安装

2. 创建虚拟串口对
   - 运行 com0com 配置工具
   - 创建端口对，如：COM10 ↔ COM11

3. 测试数据传输
   ```batch
   REM 终端 1: 运行模拟器发送数据到 COM10
   pip install pyserial
   python examples\test_serial_simulator.py COM10

   REM 终端 2: 运行上位机，连接到 COM11
   cd build
   UltrasonicHost.exe
   ```

**方法 2: 使用串口调试助手**

1. 安装 com0com 创建虚拟串口对（COM10 ↔ COM11）
2. 下载串口调试助手（如 SSCOM, 友善串口调试助手）
3. 使用调试助手向 COM10 发送数据：
   ```
   D:125.50
   D:130.25
   D:128.75
   ```
4. 上位机连接到 COM11 接收数据

### 通用: 使用真实串口回环测试

如果有 USB 转串口模块，可以将 TX 和 RX 短接进行回环测试。

## 常见配置

### 波特率选择
- 9600 (默认，稳定)
- 19200
- 38400
- 57600
- 115200 (高速)

### 数据格式
嵌入式端发送：
```
D:123.45\n
```

上位机会自动解析并显示。

## 界面功能说明

### 串口控制区
- **Port**: 选择串口设备
- **Baud Rate**: 设置波特率
- **Connect/Disconnect**: 连接/断开串口
- **Refresh**: 刷新可用串口列表

### 实时显示区
- **Current Distance**: 当前测距值（大字体）
- **Last Update**: 最后更新时间

### 波形图区
- **Pause/Resume**: 暂停/恢复波形更新
- **Clear Chart**: 清空波形数据

### 数据管理区
- **Auto Save Data**: 自动保存接收数据
- **Save Current**: 手动保存当前值
- **Query All**: 查询最近 20 条记录
- **Export CSV/TXT**: 导出数据
- **Clear All Data**: 清空所有数据
- **Statistics**: 统计信息（总数、平均、最大、最小）

### 日志区
显示操作日志和错误信息

## 下一步

- 查看 `README.md` 了解详细文档
- 查看 `examples/` 目录中的嵌入式端示例代码
- 根据需要修改源代码定制功能

## 需要帮助？

检查项目根目录的 `README.md` 文件，里面有详细的说明和常见问题解答。
