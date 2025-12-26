#!/bin/bash

# 超声波测距上位机编译脚本

echo "========================================"
echo "  Ultrasonic Host Build Script"
echo "========================================"

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 检查依赖
echo -e "${YELLOW}[1/4] Checking dependencies...${NC}"

if ! command -v cmake &> /dev/null; then
    echo -e "${RED}Error: CMake not found. Please install CMake 3.16+${NC}"
    exit 1
fi

if ! command -v qmake6 &> /dev/null && ! command -v qmake &> /dev/null; then
    echo -e "${RED}Error: Qt6 not found. Please install Qt6${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Dependencies OK${NC}"

# 清理旧构建
if [ -d "build" ]; then
    echo -e "${YELLOW}[2/4] Cleaning old build...${NC}"
    rm -rf build
fi

# 创建构建目录
echo -e "${YELLOW}[2/4] Creating build directory...${NC}"
mkdir -p build
cd build || exit 1

# 配置项目
echo -e "${YELLOW}[3/4] Configuring project...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ CMake configuration failed${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Configuration successful${NC}"

# 编译
echo -e "${YELLOW}[4/4] Building project...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Build failed${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Build successful${NC}"
echo ""
echo "========================================"
echo -e "${GREEN}Build completed successfully!${NC}"
echo "========================================"
echo ""
echo "To run the application:"
echo "  cd build"
echo "  ./UltrasonicHost"
echo ""
