#include <ESP32Servo.h>

const int servoPin = 14; 
Servo myServo360;      // <--- พระเอกของเราอยู่ตรงนี้ครับ

int angleCenter = 0;   // จุดเริ่มต้น (หุบแขนสุด)
int angleMax = 120;    // จุดเป้าหมายที่กางออกไปสุด

// ==========================================
// 1. ฟังก์ชันตั้งค่า
// ==========================================
void setupServo() {
  ESP32PWM::allocateTimer(3); 
  myServo360.setPeriodHertz(50); 
  myServo360.attach(servoPin, 500, 2400); 
  
  myServo360.write(angleCenter); 
}

// ==========================================
// 2. ฟังก์ชันกางแขน
// ==========================================
void spinSmoothDir1() {
  for (int speed = angleCenter; speed <= angleMax; speed++) {
    myServo360.write(speed);
    delay(5); 
  }
}

// ==========================================
// 3. ฟังก์ชันดึงแขนกลับมาท่าเตรียม
// ==========================================
void smoothBrake(int currentSpeed) {
  if (currentSpeed > angleCenter) {
    for (int speed = currentSpeed; speed >= angleCenter; speed--) {
      myServo360.write(speed);
      delay(5);
    }
  } else if (currentSpeed < angleCenter) {
    for (int speed = currentSpeed; speed <= angleCenter; speed++) {
      myServo360.write(speed);
      delay(5);
    }
  }
  myServo360.write(angleCenter); 
}