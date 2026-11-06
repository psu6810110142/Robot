#include <Wire.h>
#include "Motors.h" // ดึงไฟล์มอเตอร์มาใช้เหมือนเดิม

// ---------------------------------------------------------
// ตั้งค่า Gyroscope
// ---------------------------------------------------------
const int MPU_ADDR = 0x68;
const int16_t OFFSET_GYRO_Z = 86.391; 
float Yaw_Angle = 0;
unsigned long lastGyroTime = 0;

// ---------------------------------------------------------
// ตัวแปรสำหรับคุมการวิ่งอัตโนมัติ
// ---------------------------------------------------------
unsigned long stateStartTime = 0;
int runState = 0; // 0 = รอออกตัว, 1 = กำลังวิ่งตรง, 2 = หยุดนิ่ง

void updateGyro() {
  unsigned long currentTime = millis();
  if (currentTime - lastGyroTime >= 50) { 
    float dt = (currentTime - lastGyroTime) / 1000.0;
    lastGyroTime = currentTime; 

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x47);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 2, true); 
    if(Wire.available() >= 2) {
      int16_t GyZ_Raw = Wire.read()<<8 | Wire.read(); 
      Yaw_Angle += ((GyZ_Raw - OFFSET_GYRO_Z) / 131.0 * dt); 
    }
  }
}

void setup() {
  Serial.begin(115200);

  // 1. เปิดใช้งานไจโร
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  
  Wire.write(0);     
  Wire.endTransmission(true);
  lastGyroTime = millis();

  // 2. เปิดใช้งานพินมอเตอร์
  initMotors();

  // 3. ลำดับการเตรียมตัวก่อนวิ่งอัตโนมัติ
  motorStop();
  delay(3000);             // หน่วงเวลา 3 วินาที ให้คุณมีเวลาวางหุ่นลงกับพื้นก่อน
  Yaw_Angle = 0;           // รีเซ็ตไจโรเป็น 0 ก่อนออกตัว
  stateStartTime = millis(); // เริ่มบันทึกเวลา
  runState = 1;            // สั่งให้เริ่มทำงาน State 1 (วิ่งตรง)
}

void loop() {
  // ควรอ่านค่าไจโรตลอดเวลา ห้ามเอาไปใส่ใน delay
  updateGyro(); 

  // ---------------------------------------------------------
  // State Machine: ลำดับการทำงานอัตโนมัติ
  // ---------------------------------------------------------
  if (runState == 1) {
    // โยนค่าให้ฟังก์ชันใน Motors.h ทำงาน
    motorWalkStraight(Yaw_Angle); 
    
    // ถ้าวิ่งไปครบ 5 วินาที (5000 มิลลิวินาที)
    if (millis() - stateStartTime >= 5000) {
      runState = 2; // ย้ายไป State 2
    }
  } 
  else if (runState == 2) {
    motorStop(); 
    // หุ่นจะจอดนิ่งสนิท ถือว่าจบการทดสอบ
  }
}