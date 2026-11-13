#include <ESP32Servo.h>
#include <Wire.h>

// ==========================================
// 1. ประกาศตัวแปรพินมอเตอร์ 
// ==========================================
const int pwmLeft = 27; 
const int motorLeft_Fwd = 33; 
const int motorLeft_Bwd = 32; 

// --- ล้อขวา (สลับ 2 กับ 4) ---
const int pwmRight = 13;
const int motorRight_Fwd = 4; 
const int motorRight_Bwd = 2;

// ==========================================
// 2. ประกาศตัวแปรพินเซนเซอร์เส้น (กลุ่ม Input-Only)
// ==========================================
const int sensorFrontLeft_Pin = 34;
const int sensorFrontRight_Pin = 35;
const int BLACK_LINE = HIGH;

// ==========================================
// 3. ตั้งค่าความเร็ว (เปลี่ยนค่าได้ที่นี่ที่เดียว)
// ==========================================
int speedLeft_Fwd = 160;  
int speedRight_Fwd = 160; 
int speedLeft_Bwd = 160;  
int speedRight_Bwd = 160; 

// ==========================================
// 4. ตัวแปรสำหรับ MPU6050 (ไจโร)
// ==========================================
float Yaw_Angle = 0.0;

//==========================================
//ห้ามแก้ส่วนด้านบน
//=========================================

void setup() {
  Serial.begin(115200);
  
  motorSetup();
  sensorSetup(); 
  setupServo();
  setupLED();  
  
  Serial.println("--- System Ready ---");
  delay(2000); 

  runMission1(); 
  runMission2();
  runMission3();
  runMission4();

}

void loop() {
  // พื้นที่สำหรับเขียนลอจิกเดินตามเส้น หรือภารกิจหลัก
}