#include "DHT.h"
//#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const uint8_t BUTTON_PINS_SEAT[8] = {15,2,0,4, 12,14,26,27};
const uint8_t BUTTON_PINS_GROUND[6] = {5,18,19, 25,33,32};
int status_seat[8];
int status_ground[6];

DHT dht(23,DHT11);
int gas = 34;
//float gas_value;
float temp_value;

int status_update;
const char* ssid = "vvieww";
const char* password = "qwerty555";
const char* url_post = "https://ecourse.cpe.ku.ac.th/exceed16/api/safety-car";

const int _size1 = 2*JSON_OBJECT_SIZE(100);
StaticJsonDocument<_size1> JSONPost;

float sensor_volt;
float sensor_value = 0;
float CO;

char str[1000];
void setup() {
  for(int i =0;i<8;i++){
    pinMode(BUTTON_PINS_SEAT[i],INPUT_PULLUP);
  }
  for(int i=0;i<6;i++){
    pinMode(BUTTON_PINS_GROUND[i],INPUT_PULLUP);
  }
  
  Serial.begin(9600);
  dht.begin();

  Wifi_connect();
  xTaskCreatePinnedToCore(
    update_switch,
    "update_switch",
    1024*32,
    NULL,
    1,
    NULL,
    0
    );
    
  xTaskCreatePinnedToCore(
    post,
    "post",
    1024*32,
    NULL,
    2,
    NULL,
    0
    );
}

void update_switch(void *parameter){
  while(1){
    status_update=0;
    for(int i=0;i<8;i++){
      status_seat[i]=digitalRead(BUTTON_PINS_SEAT[i]);
    }
    for(int i=0;i<6;i++){
      status_ground[i]=digitalRead(BUTTON_PINS_GROUND[i]);
    }
    for(int i=0;i<8;i++){
      Serial.print("seat[i]: ");
      Serial.println(status_seat[i]);
    }
    for(int i=0;i<6;i++){
      Serial.print("ground[i]: ");
      Serial.println(status_ground[i]);
    }
    Serial.println("----------------");
    temp_value = dht.readTemperature();
    for(int i = 0;i<100;++i){
      sensor_value = sensor_value + analogRead(gas);
    }

    sensor_value = sensor_value / 100.0;
    Serial.print("sensor_value = ");
    Serial.println(sensor_value);
    sensor_volt = sensor_value * (3.3 /1023.0);
    CO = 3.027*pow(2.718,1.0698*sensor_volt);
    Serial.print("temp: ");
    Serial.println(temp_value);
    Serial.print("gas: ");
    Serial.println(CO);
    status_update=1;
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
void Wifi_connect(){
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
   delay(1000);
   Serial.println("connecting to Wifi..."); 
  }
  Serial.println("Connected to wifi...");
  Serial.print("ip : ");
  Serial.println(WiFi.localIP());
}

void post(void * parameter){
  while(1){
    if(WiFi.status()== WL_CONNECTED && status_update==1){
      for(int i = 0;i<100;++i){
        sensor_value = sensor_value + analogRead(gas);
      }

      sensor_value = sensor_value / 100.0;
      Serial.print("sensor_value = ");
      Serial.println(sensor_value);
      sensor_volt = sensor_value * (3.3 /1023.0);
      CO = 3.027*pow(2.718,1.0698*sensor_volt);
      HTTPClient http;
      http.begin(url_post);
      http.addHeader("Content-type","application/json");
      JSONPost["heat"] = (float)temp_value;
      JSONPost["carbon"] = (float)CO;
      JSONPost["sensor1"] = (int)status_seat[0];
      JSONPost["sensor2"] = (int)status_seat[1];
      JSONPost["sensor3"] = (int)status_seat[2];
      JSONPost["sensor4"] = (int)status_seat[3];
      JSONPost["sensor5"] = (int)status_seat[4];
      JSONPost["sensor6"] = (int)status_seat[5];
      JSONPost["sensor7"] = (int)status_seat[6];
      JSONPost["sensor8"] = (int)status_seat[7];
      JSONPost["sensor9"] = (int)status_ground[0];
      JSONPost["sensor10"] = (int)status_ground[1];
      JSONPost["sensor11"] = (int)status_ground[2];
      JSONPost["sensor12"] = (int)status_ground[3];
      JSONPost["sensor13"] = (int)status_ground[4];
      JSONPost["sensor14"] = (int)status_ground[5];
      
      serializeJson(JSONPost,str);  
      int httpCode = http.POST(str);
      if(httpCode == HTTP_CODE_OK){
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }else{
        Serial.println(httpCode);
        Serial.println("ERROR");
      }
    }else{
      Wifi_connect();
    }
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
}

void loop() {
  
}
