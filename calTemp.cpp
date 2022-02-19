#include <math.h>

int gas = 34;


void setup(){
  Serial.begin(9600);
}

void loop(){
  float sensor_volt;
  float RS_air;
  float R0;
  float sensor_value = 0;
  pinMode(23,INPUT);

  float CO;
  for(int i = 0;i<100;++i){
    sensor_value = sensor_value + analogRead(gas);
  }

  sensor_value = sensor_value / 100.0;
  Serial.print("sensor_value = ");
  Serial.println(sensor_value);
  sensor_volt = sensor_value * (3.3 /1023.0);
  CO = 3.027*pow(2.718,1.0698*sensor_volt);
//  R0 = RS_air/6.5;
//  Serial.print("sensor_volt = ");
//  Serial.print(sensor_volt);
//  Serial.println("V");
//  Serial.print("R0 = ");
//  Serial.println(R0);
  Serial.print("Vrl = ");
  Serial.print(sensor_volt);
  Serial.print(" CO (ppm) = ");
  Serial.println(CO);
  
  delay(1000);
  
}

//void loop(){
//   float sensor_volt;
//  float RS_gas;
//  float ratio; 
//  int sensorValue = analogRead(gas);
//  sensor_volt = (float)sensorValue/(1024*5.0);
//  RS_gas = (5.0-sensor_volt)/sensor_volt;
//  ratio = RS_gas/3.55;
//  Serial.print("sensorValue = ");
//  Serial.print(sensorValue);
//  Serial.print("RS_gas = ");
//  Serial.print(RS_gas);
//  Serial.print(" Ratio = ");
//  Serial.println(ratio);
//  delay(1000);
//  
//}
