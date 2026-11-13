// ==========================================
// ประกาศขาพิน LED (D1-D4)
// ==========================================
const int ledRed = 19;    // D1
const int ledYellow = 18; // D2
const int ledBlue = 17;   // D3
const int ledGreen = 16;  // D4

// ==========================================
// 1. ฟังก์ชันตั้งค่า LED (นำไปเรียกใน setup ของหน้าหลัก)
// ==========================================
void setupLED() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  
  clearLEDs(); // ปิดไฟทุกดวงตอนเปิดเครื่อง
}

// ==========================================
// 2. ฟังก์ชันปิดไฟทั้งหมด (เอาไว้เคลียร์สถานะ)
// ==========================================
void clearLEDs() {
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledGreen, LOW);
}

// ==========================================
// 3. ฟังก์ชันแสดงสถานะมิชชัน (นำไปเรียกใช้ตอนทำภารกิจ)
// ==========================================
void showStatus(int missionNumber) {
  clearLEDs(); // ดับดวงเก่าก่อนเสมอ
  
  if (missionNumber == 1) {
    digitalWrite(ledRed, HIGH);
  } 
  else if (missionNumber == 2) {
    digitalWrite(ledYellow, HIGH);
  } 
  else if (missionNumber == 3) {
    digitalWrite(ledBlue, HIGH);
  } 
  else if (missionNumber == 4) {
    digitalWrite(ledGreen, HIGH);
  }
  else if (missionNumber == 5) {
    // ถ้ามีมิชชันที่ 5 ให้ติดคู่ (แดง+เหลือง)
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, HIGH);
  }
}