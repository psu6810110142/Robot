

void runMission1() {
  // สเต็ปที่ 1: ไฟแดงจะติด
  showStatus(1); 
  BackwardSoft();
  delay(1000); 

  // สเต็ปที่ 2: ไฟแดงดับ ไฟเหลืองจะติดแทน
  Brake();
  delay(500); 

  spinSmoothDir1(); 
  delay(500); 

  // สเต็ปที่ 3: ไฟเหลืองดับ ไฟน้ำเงินจะติดแทน 
  ForwardSoft();
  delay(2000); 

  TurnRight(200);
  delay(500);
  Stop();


  // สเต็ปที่ 4: ไฟน้ำเงินดับ ไฟเขียวจะติดแทน
  Brake();
  smoothBrake(120); 

  ForwardSoft();
  delay(1000);
  Stop();


}

void runMission2(){
  showStatus(2);
  delay(2000);
 
}

void runMission3(){
  showStatus(3);
  delay(2000);

}

void runMission4(){
  showStatus(4);
  delay(2000);
}
