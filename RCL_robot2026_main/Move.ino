void Forward() {
  // สั่งล้อซ้ายเดินหน้า
  analogWrite(pwmLeft, speedLeft_Fwd);
  digitalWrite(motorLeft_Fwd, HIGH); 
  digitalWrite(motorLeft_Bwd, LOW);
  
  // สั่งล้อขวาเดินหน้า
  analogWrite(pwmRight, speedRight_Fwd);
  digitalWrite(motorRight_Fwd, HIGH); 
  digitalWrite(motorRight_Bwd, LOW);
}

void Backward() {
  // สั่งล้อซ้ายถอยหลัง
  analogWrite(pwmLeft, speedLeft_Bwd);
  digitalWrite(motorLeft_Fwd, LOW); 
  digitalWrite(motorLeft_Bwd, HIGH);
  
  // สั่งล้อขวาถอยหลัง
  analogWrite(pwmRight, speedRight_Bwd);
  digitalWrite(motorRight_Fwd, LOW); 
  digitalWrite(motorRight_Bwd, HIGH);
}
// แนะนำ: ส่งค่าความเร็วตอนเลี้ยวเข้าไปเลย จะปรับจูนง่ายกว่า
void TurnLeft(int speedTurn) {
  // ล้อซ้ายถอยหลัง ล้อขวาเดินหน้า (หมุนตัวอยู่กับที่)
  analogWrite(pwmLeft, speedTurn); 
  analogWrite(pwmRight, speedTurn);
  
  digitalWrite(motorLeft_Fwd, LOW);  
  digitalWrite(motorLeft_Bwd, HIGH);
  
  digitalWrite(motorRight_Fwd, HIGH); 
  digitalWrite(motorRight_Bwd, LOW);
}

// แนะนำ: ส่งค่าความเร็วตอนเลี้ยวเข้าไปเลย จะปรับจูนง่ายกว่า
void TurnRight(int speedTurn) {
  // ล้อซ้ายเดินหน้า ล้อขวาถอยหลัง (หมุนตัวอยู่กับที่)
  analogWrite(pwmLeft, speedTurn);
  analogWrite(pwmRight, speedTurn);
  
  digitalWrite(motorLeft_Fwd, HIGH);  
  digitalWrite(motorLeft_Bwd, LOW);
  
  digitalWrite(motorRight_Fwd, LOW); 
  digitalWrite(motorRight_Bwd, HIGH);
}

// ---------------------------------------------------------
// ฟังก์ชันจัดทรงรถ (แก้ไขให้เบรกกระชับขึ้น)
// ---------------------------------------------------------
void autoFront() {
  // แนะนำให้ใช้ความเร็วต่ำเวลาเทียบเส้น เพื่อไม่ให้รถพุ่งทะลุเส้น (Overshoot)
  int alignSpeed = 80; // ปรับลดลงนิดนึงเพื่อความแม่นยำสำหรับมอเตอร์ N20

  Serial.println("Start Auto Front Alignment...");

  while (true) {
    int valFrontLeft = digitalRead(sensorFrontLeft_Pin);
    int valFrontRight = digitalRead(sensorFrontRight_Pin);

    if (valFrontLeft == BLACK_LINE && valFrontRight == BLACK_LINE) {
      // กรณีที่ 1: ชนเส้นดำทั้งคู่แล้ว -> รถตั้งตรงสมบูรณ์ ให้หยุด
      Brake(); // เรียกใช้ฟังก์ชัน Hard Brake เพื่อล็อกล้อทันที
      Serial.println("Auto Front: Aligned!");
      break; 
    } 
    else if (valFrontLeft == BLACK_LINE && valFrontRight != BLACK_LINE) {
      // กรณีที่ 2: ล้อซ้ายชนเส้นแล้ว แต่ขวายังไม่ชน
      digitalWrite(motorLeft_Fwd, HIGH); // เบรกซ้ายแบบล็อกล้อเบาๆ
      digitalWrite(motorLeft_Bwd, HIGH);
      analogWrite(pwmLeft, 255);
      
      digitalWrite(motorRight_Fwd, HIGH); // ขวาเดินหน้า
      digitalWrite(motorRight_Bwd, LOW);
      analogWrite(pwmRight, alignSpeed);
    } 
    else if (valFrontRight == BLACK_LINE && valFrontLeft != BLACK_LINE) {
      // กรณีที่ 3: ล้อขวาชนเส้นแล้ว แต่ซ้ายยังไม่ชน
      digitalWrite(motorLeft_Fwd, HIGH); // ซ้ายเดินหน้า
      digitalWrite(motorLeft_Bwd, LOW);
      analogWrite(pwmLeft, alignSpeed);
      
      digitalWrite(motorRight_Fwd, HIGH); // เบรกขวาแบบล็อกล้อเบาๆ
      digitalWrite(motorRight_Bwd, HIGH);
      analogWrite(pwmRight, 255);
    } 
    else {
      // กรณีที่ 4: ยังไม่ชนเส้นทั้งคู่ -> เดินหน้า
      digitalWrite(motorLeft_Fwd, HIGH);
      digitalWrite(motorLeft_Bwd, LOW);
      analogWrite(pwmLeft, alignSpeed);
      
      digitalWrite(motorRight_Fwd, HIGH);
      digitalWrite(motorRight_Bwd, LOW);
      analogWrite(pwmRight, alignSpeed);
    }
  }
}

// ---------------------------------------------------------
// ฟังก์ชันออกตัวนุ่มนวล (Soft Start) - โค้ดเดิมเขียนลอจิกมาดีมากครับ!
// ---------------------------------------------------------
void ForwardSoft() {
  digitalWrite(motorLeft_Fwd, HIGH);  
  digitalWrite(motorLeft_Bwd, LOW);
  digitalWrite(motorRight_Fwd, HIGH); 
  digitalWrite(motorRight_Bwd, LOW);

  int steps = 20; 
  int delayPerStep = 20; 

  for (int i = 1; i <= steps; i++) {
    int currentLeft = (speedLeft_Fwd / steps) * i;
    int currentRight = (speedRight_Fwd / steps) * i;
    
    analogWrite(pwmLeft, currentLeft);
    analogWrite(pwmRight, currentRight);
    
    delay(delayPerStep); 
  }
  analogWrite(pwmLeft, speedLeft_Fwd);
  analogWrite(pwmRight, speedRight_Fwd);
}

void BackwardSoft() {
  digitalWrite(motorLeft_Fwd, LOW);  
  digitalWrite(motorLeft_Bwd, HIGH);
  digitalWrite(motorRight_Fwd, LOW); 
  digitalWrite(motorRight_Bwd, HIGH);

  int steps = 15; 
  int delayPerStep = 20; 

  for (int i = 1; i <= steps; i++) {
    int currentLeft = (speedLeft_Bwd / steps) * i;
    int currentRight = (speedRight_Bwd / steps) * i;
    
    analogWrite(pwmLeft, currentLeft);
    analogWrite(pwmRight, currentRight);
    
    delay(delayPerStep); 
  }
  analogWrite(pwmLeft, speedLeft_Bwd);
  analogWrite(pwmRight, speedRight_Bwd);
}