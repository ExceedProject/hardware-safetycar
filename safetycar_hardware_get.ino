#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "vvieww";
const char* password = "qwerty555";
const char* url_get = "https://ecourse.cpe.ku.ac.th/exceed16/api/safety-car/warning";

const int _size = 2*JSON_OBJECT_SIZE(4);
StaticJsonDocument<_size> JSONGet;
char str[100];

//int count=0;
int warning;
int get_status;
int LED_pin=33;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_pin,OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to Wifi...");
    
  }
  Serial.println("Connected to the Wifi network");

  lcd.begin();
  lcd.backlight();
  
  xTaskCreatePinnedToCore(
    _get,
    "get",
    1024*32,
    NULL,
    1,
    NULL,
    0
    );
  
}

void _get(void *parameter){
   while(1){
    get_status = 0;
    if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;
      http.begin(url_get);
      int httpCode = http.GET();
      if(httpCode < 400){
        String payload = http.getString();
        deserializeJson(JSONGet,payload);
        warning = JSONGet["warning"];
        Serial.print("Warning = ");
        Serial.println(warning);
        get_status = 1;
//        count=count+1;
//        Serial.println(count);
      }else{
        Serial.println(httpCode);
        Serial.println("Get Failed");
      }
    }
    vTaskDelay(1000/portTICK_PERIOD_MS);
   }
}

void loop() {
 if(get_status && warning){
// if(count==1){
  lcd.setCursor(6,1);
  digitalWrite(LED_pin,HIGH);
  lcd.print("HELP!!!");
  delay(800);
  lcd.clear();
  digitalWrite(LED_pin,LOW);
//  count=0;
  delay(300);
 }
}
