@echo off
REM ========================================
REM   超声波测距上位机编译脚本 (Windows)
REM ========================================

echo ========================================
echo   Ultrasonic Host Build Script (Windows)
echo ========================================
echo.

REM 检查是否在 Qt 环境中
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 CMake。请在 Qt 命令提示符中运行此脚本！
    echo.
    echo 请执行以下步骤:
    echo 1. 在开始菜单搜索 "Qt 6.x.x (MSVC 2019 64-bit)"
    echo 2. 右键点击并选择"以管理员身份运行"
    echo 3. 在打开的终端中再次运行此脚本
    echo.
    pause
    exit /b 1
)

REM 清理旧构建
echo [1/4] 清理旧构建...
if exist build (
    rmdir /s /q build
)

REM 创建构建目录
echo [2/4] 创建构建目录...
mkdir build
cd build

REM 配置项目
echo [3/4] 配置项目...
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
if %errorlevel% neq 0 (
    echo [错误] CMake 配置失败！
    pause
    exit /b 1
)

REM 编译
echo [4/4] 编译项目...
ninja
if %errorlevel% neq 0 (
    echo [错误] 编译失败！
    pause
    exit /b 1
)

echo.
echo ========================================
echo   编译成功！
echo ========================================
echo.
echo 运行程序：
echo   cd build
echo   UltrasonicHost.exe
echo.
echo 或者部署程序（复制所需的 DLL）：
echo   cd build
echo   windeployqt UltrasonicHost.exe
echo.

pause
