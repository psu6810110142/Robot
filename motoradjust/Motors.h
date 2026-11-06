#include <Arduino.h> // จำเป็นต้องใส่เพื่อให้แท็บนี้รู้จักคำสั่ง analogWrite, digitalWrite

// ---------------------------------------------------------
// ตั้งค่าพินมอเตอร์ และ PWM
// ---------------------------------------------------------
const int motorLeft_Fwd = 27;
const int motorLeft_Bwd = 14;
const int motorRight_Fwd = 13; 
const int motorRight_Bwd = 12;
const int pwmLeft = 33;        
const int pwmRight = 32;       

// ---------------------------------------------------------
// ตั้งค่าความเร็วและระบบรักษาเส้นตรง (PID)
// ---------------------------------------------------------
int baseSpeedLeft = 255;  
int baseSpeedRight = 253; // ล้อขวาแรงเกินไป ดรอปความเร็วลง

float targetAngle = 0;       
float Kp = 5.0;            // ลดความแรงในการหักเลี้ยวลง (จากเดิม 10.0)
float Ki = 0.4;            // ลดแรงดันสะสมลงมากๆ เพื่อไม่ให้เกิดการส่ายแบบเลื้อย
float Kd = 5.0;            // (ตัวใหม่) โช้คอัพหน่วงการส่าย ถ้ายิ่งส่ายแรง ให้เพิ่มค่านี้
float integralError = 0;   
float lastError = 0;       // (ตัวใหม่) จำค่าความคลาดเคลื่อนก่อนหน้า
bool isFirstMove = true;

// ---------------------------------------------------------
// ฟังก์ชันควบคุมมอเตอร์
// ---------------------------------------------------------
void initMotors() {
  pinMode(motorLeft_Fwd, OUTPUT);
  pinMode(motorLeft_Bwd, OUTPUT);
  pinMode(motorRight_Fwd, OUTPUT);
  pinMode(motorRight_Bwd, OUTPUT);
  pinMode(pwmRight, OUTPUT);
  pinMode(pwmLeft, OUTPUT);
}

// โยนค่า Yaw_Angle ปัจจุบันจากหน้าหลักเข้ามาในวงเล็บ
void motorWalkStraight(float currentYaw) {
  if (isFirstMove) {
    targetAngle = currentYaw; 
    integralError = 0;        
    lastError = 0;            
    isFirstMove = false;
  }

  float error = targetAngle - currentYaw;
  
  // 1. เพิ่ม Deadband: ถ้าองศาเพี้ยนไม่เกิน 1.0 องศา ให้งดการชดเชย (วิ่งเนียนๆ)
  if (abs(error) < 1.0) {
    error = 0; 
  }

  integralError += error; 
  integralError = constrain(integralError, -10, 10); // บีบค่า I ให้น้อยลงเพื่อลดการสะสมที่ทำให้ส่าย
  
  float derivative = error - lastError;
  
  // ปรับจูน PID ให้นุ่มนวล
  float Kp = 3.0;  // ดึงกลับเบาๆ (ถ้ายังส่ายให้ลดลงอีก)
  float Ki = 0.05; // ใช้น้อยมากๆ หรือปรับเป็น 0 หากไม่จำเป็น
  float Kd = 8.0;  // โช้คอัพ: ยิ่งส่ายตอนกลับมาตรง ให้เพิ่มค่านี้
  
  int correction = (error * Kp) + (integralError * Ki) + (derivative * Kd);
  
  // 2. จำกัดแรงกระชาก ไม่ให้ปรับ PWM บวกลบเกิน 15 แต้มต่อรอบ
  correction = constrain(correction, -15, 15);
  
  lastError = error; 
  
  int speedLeft = baseSpeedLeft - correction;
  int speedRight = baseSpeedRight + correction;
  
  speedLeft = constrain(speedLeft, 0, 255);
  speedRight = constrain(speedRight, 0, 255);
  
  analogWrite(pwmLeft, speedLeft);
  analogWrite(pwmRight, speedRight);
  
  digitalWrite(motorLeft_Fwd, HIGH);  digitalWrite(motorLeft_Bwd, LOW);
  digitalWrite(motorRight_Fwd, HIGH); digitalWrite(motorRight_Bwd, LOW);
}

void motorBackward() {
  analogWrite(pwmLeft, baseSpeedLeft);
  analogWrite(pwmRight, baseSpeedRight);
  digitalWrite(motorLeft_Fwd, LOW);  digitalWrite(motorLeft_Bwd, HIGH);
  digitalWrite(motorRight_Fwd, LOW); digitalWrite(motorRight_Bwd, HIGH);
}

void motorStop() {
  analogWrite(pwmLeft, 0);
  analogWrite(pwmRight, 0);
  digitalWrite(motorLeft_Fwd, LOW); digitalWrite(motorLeft_Bwd, LOW);
  digitalWrite(motorRight_Fwd, LOW); digitalWrite(motorRight_Bwd, LOW);
  
  isFirstMove = true; // รีเซ็ตเป้าหมาย
}