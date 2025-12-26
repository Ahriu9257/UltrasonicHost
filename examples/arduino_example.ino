/**
 * Arduino 超声波测距示例代码
 * 
 * 硬件连接:
 * - HC-SR04 Trig -> Arduino Pin 9
 * - HC-SR04 Echo -> Arduino Pin 10
 * - VCC -> 5V
 * - GND -> GND
 * 
 * 串口配置:
 * - 波特率: 9600
 * - 数据位: 8
 * - 停止位: 1
 * - 校验位: None
 */

#define TRIG_PIN 9
#define ECHO_PIN 10

void setup() {
  // 初始化串口
  Serial.begin(9600);
  
  // 初始化超声波引脚
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Serial.println("Ultrasonic Distance Sensor Ready");
  delay(1000);
}

void loop() {
  // 测量距离
  float distance = measureDistance();
  
  // 发送数据到上位机（格式：D:xxx.xx）
  Serial.print("D:");
  Serial.println(distance, 2);  // 保留2位小数
  
  // 也可以使用纯数字格式
  // Serial.println(distance, 2);
  
  delay(100);  // 100ms 采样一次
}

/**
 * 测量距离（单位：厘米）
 */
float measureDistance() {
  long duration;
  float distance;
  
  // 发送超声波脉冲
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // 读取回波时间
  duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms 超时
  
  // 计算距离 (声速 340m/s, 往返距离)
  // distance = (duration * 0.034) / 2;
  distance = duration * 0.017;
  
  // 距离有效性检查
  if (distance < 2.0 || distance > 400.0) {
    distance = 0.0;  // 超出范围
  }
  
  return distance;
}
