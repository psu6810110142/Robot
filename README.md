#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Servo.h>

// ตั้งค่าจอ (SH1106)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

Servo myservo;
const int servoPin = 9; // ตรวจสอบว่าเสียบสายสีส้มที่ขา 9 บน Shield ถูกต้อง

void setup() {
  u8g2.begin();
  myservo.attach(servoPin);

  // --- ขั้นตอนที่ 1: เริ่มที่ 0 องศา เป็นเวลา 5 วินาที ---
  myservo.write(0);
  showStatus("Start at 0 deg", 0);
  delay(5000); // รอ 5 วินาที

  // --- ขั้นตอนที่ 2: หมุนไป 180 องศา ---
  myservo.write(180);
  showStatus("Go to 180 deg", 180);
  delay(2000); // รอให้หมุนไปถึง (เผื่อเวลาเดิน)

  // --- ขั้นตอนที่ 3: กลับมาที่ 0 องศา แล้วจบ ---
  myservo.write(0);
  showStatus("Back to 0 & Stop", 0);
  // จบการทำงานใน setup (เซอร์โวจะค้างที่ 0 ยาวๆ)
}

void loop() {
  // ปล่อยว่างไว้ เพราะเราต้องการให้ทำแค่รอบเดียวแล้วหยุด
}

// ฟังก์ชันแสดงผลหน้าจอ
void showStatus(String msg, int angle) {
  u8g2.clearBuffer();
  
  u8g2.drawFrame(0, 0, 128, 64);
  
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(10, 20);
  u8g2.print(msg); // แสดงข้อความสถานะ
  
  u8g2.setFont(u8g2_font_ncenB24_tr);
  u8g2.setCursor(40, 55);
  u8g2.print(angle); // แสดงตัวเลข
  
  u8g2.sendBuffer();
}
