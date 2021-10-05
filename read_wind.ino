#include <Wire.h>
#define ASPD_I2C_ADDRESS 0x28

int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int D1 = 9;
int D2 = 10;
int D3 = 11;
int D4 = 12;

// MS4525D sensor full scale range and units
const int16_t MS4525FullScaleRange = 1;  // 1 psi

// MS4525D Sensor type (A or B) comment out the wrong type assignments
// Type A (10% to 90%)
const int16_t MS4525MinScaleCounts = 1638;
const int16_t MS4525FullScaleCounts = 14746;

const int16_t MS4525Span=MS4525FullScaleCounts-MS4525MinScaleCounts;

//MS4525D sensor pressure style, differential or otherwise. Comment out the wrong one.
//Differential
const int16_t MS4525ZeroCounts=(MS4525MinScaleCounts+MS4525FullScaleCounts)/2;

const int staticPin = A4;
const int dynamicPin = A5;

int offset = 0;
int offset_size = 10;
int veloc_mean_size = 20;
int zero_span = 2;

void setup() {
  // put your setup code here, to run once:
  //Wire.begin(0x28); // join i2c bus with address of sensor
  //Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600); // start serial for output

//  for (int ii=0;ii<offset_size;ii++){
//    offset += analogRead(A4)-(1023/2);
//  }
//  offset /= offset_size;

  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT); 

  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW); 
  //0
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);  


}

void loop() {
  // put your main code here, to run repeatedly:
  //int staticVal = analogRead(staticPin)-(1023/2);
  //int dynamicVal = analogRead(dynamicPin)-(1023/2);

//int dynamicVal = 0;
//  for (int ii=0;ii<offset_size;ii++){
//    dynamicVal += analogRead(A4)-(1023/2);
//  }
//  dynamicVal /= offset_size;

  //int pinAvg = ((staticVal + dynamicVal) / 2);
//  Serial.print("offset = ");
//  Serial.print(offset);
//  Serial.print("\t pin avg = ");
//  Serial.println(dynamicVal); 
//
//  int pressure = offset - dynamicVal;
//  Serial.print("Pressure = ");
//  Serial.println(pressure);
//  if (pressure > 100) {
//    showProgress(4);
//  } else if (pressure > 75) {
//    showProgress(3);
//  } else if (pressure > 50) {
//    showProgress(2);
//  } else if (pressure > 25) {
//    showProgress(1);
//  } else {
//    showProgress(0);
//  }


    // Nothing to do here, so we'll just blink the built-in LED
//    digitalWrite(13,HIGH); delay(300);
//    digitalWrite(13,LOW);  delay(300);

  
  Wire.beginTransmission(ASPD_I2C_ADDRESS); // select device with "beginTransmission()"
  Wire.write(0x04); // select starting register with "write()"
  Wire.endTransmission(false); // end write operation, as we just wanted to select the starting register
  Wire.requestFrom(ASPD_I2C_ADDRESS, 4); // select number of bytes to get from the device (4 bytes in this case)
  byte Press_H= Wire.read(); 
  byte Press_L= Wire.read();
  byte Temp_H= Wire.read();
  byte Temp_L= Wire.read();
  
  Serial.print("byte1 = ");
  Serial.println(Press_H);
  Serial.print("byte2 = ");
  Serial.println(Press_L);
  Serial.print("byte3 = ");
  Serial.println(Temp_H);
  Serial.print("byte4 = ");
  Serial.println(Temp_L);

//  char _status = (Press_H >> 6) & 0x03;
  Press_H = Press_H & 0x3f;
  uint16_t P_dat = (((uint16_t)Press_H) << 8) | Press_L;

  Temp_L = (Temp_L >> 5);
  uint16_t T_dat = (((uint16_t)Temp_H) << 3) | Temp_L;

  Serial.print("Pressure dat : ");
  Serial.println(P_dat);

  Serial.print("Temp dat : ");
  Serial.println(T_dat);

  float psi=(static_cast<float>(static_cast<int16_t>(P_dat)-MS4525ZeroCounts))/static_cast<float>(MS4525Span)*static_cast<float>(MS4525FullScaleRange);

  Serial.print("PSI : ");
  Serial.println(psi);
  
  delay(1000);
  
}

void receiveEvent(int howMany) {
  Serial.print("Receiving...");
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c); // print the character
  }
  int x = Wire.read(); // receive byte as an integer
  Serial.println(x); // print the integer
}

void showProgress(int progress) {
  Serial.print("Showing progress level ");
  Serial.println(progress);
  
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  
  for (int i = 0;i< progress;i++) {
    digitalWrite(i+D1, HIGH);
  }
}
