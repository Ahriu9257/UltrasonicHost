#!/usr/bin/env python3
"""
串口模拟器 - 用于测试上位机（无需真实硬件）

使用方法:
1. 安装依赖: pip install pyserial
2. 创建虚拟串口对: 
   Linux: socat -d -d pty,raw,echo=0 pty,raw,echo=0
   或使用 com0com (Windows)
3. 运行此脚本: python test_serial_simulator.py /dev/pts/X

功能:
- 模拟超声波测距数据
- 随机生成 10-300cm 的距离值
- 每 100ms 发送一次数据
"""

import serial
import time
import random
import sys

def simulate_ultrasonic(port, baudrate=9600):
    """
    模拟超声波测距数据发送
    """
    try:
        ser = serial.Serial(port, baudrate, timeout=1)
        print(f"已连接到串口: {port} @ {baudrate} baud")
        print("开始发送模拟数据... (Ctrl+C 退出)")
        
        counter = 0
        base_distance = 150.0  # 基准距离
        
        while True:
            # 生成模拟距离数据（带随机波动）
            noise = random.uniform(-20, 20)
            trend = 30 * (random.random() - 0.5)  # 缓慢趋势变化
            distance = base_distance + noise + trend
            
            # 限制范围
            distance = max(10.0, min(400.0, distance))
            
            # 发送数据（格式：D:xxx.xx）
            data = f"D:{distance:.2f}\n"
            ser.write(data.encode())
            
            print(f"[{counter:04d}] 发送: {distance:.2f} cm")
            
            counter += 1
            base_distance = distance  # 平滑过渡
            
            time.sleep(0.1)  # 100ms 间隔
            
    except serial.SerialException as e:
        print(f"串口错误: {e}")
    except KeyboardInterrupt:
        print("\n\n程序已停止")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("串口已关闭")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python test_serial_simulator.py <串口名>")
        print("示例:")
        print("  Linux:   python test_serial_simulator.py /dev/ttyUSB0")
        print("  Windows: python test_serial_simulator.py COM3")
        sys.exit(1)
    
    port = sys.argv[1]
    simulate_ultrasonic(port)
