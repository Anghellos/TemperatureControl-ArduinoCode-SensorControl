//==============================Libraries=============================//

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ThingSpeak.h>

//===============================Set Pin===============================//

const int TEMP_SENSOR_PIN = 2;          // pin DS18B20 sensor in connected to

//============================Set Constants============================//

const int TEMP_CALIBRATION = -1.8;      // temperature calibration unit
//const char* SSID = "FRITZ!Box 3490";    // WiFi-name
//const char* PASS = "04061984";          // WiFi-password
const char* SSID = "Free Wi-Fi";    // WiFi-name
const char* PASS = "jojolinul";          // WiFi-password

const int WIFI_DELAY = 15 * 1000;           // connect to Wifi delay 15 seconds (1 second = 1000 ms)
const int MEASURE_DELAY = 30 * 1000;        // measure temperature delay, every 5 minutes (1 second = 1000 ms)

const char* WRITE_APIKEY = "GYWURSQ0O0WANAL2";    // write APIKey for ThingSpeak MQTT server
const int CHANNEL_NUM = 376531;                   // server channel on ThingSpeak MQTT server
const int WRITE_FIELD = 1;                        // field that temperature is stored on ThingSpeak MQTT Server

//============================Set Variables============================//

//============================Create Objects===========================//

OneWire one_wire_pin (TEMP_SENSOR_PIN);               // create OneWire object with tempSensor pin parameter
DallasTemperature temp_sensor(&one_wire_pin);         // create sensor object
WiFiClient wifi_client;                               // create client object for WiFi connection

//============================Initial Setup============================//

void setup(){
  Serial.begin(9600);
  ThingSpeak.begin(wifi_client);    // ThingSpeak MQTT server setup
  temp_sensor.begin();              // sensor setup
  WiFi.hostname("Wemos D1 R2");     // set WiFi hostname
}

//===============================Main Loop==============================//

void loop(){
  if (!WiFiConnect()){
    Serial.print("Can't connect to WiFi. Retry in 15 seconds!");
    delay(WIFI_DELAY);      // delay if not connected to WiFi
    return;
  }
  
  float temp = ReadTemp();        // temperature store in variable

  Serial.print("Temperature: ");  // temperature print for debugging purpose
  Serial.print(temp);             // temperature print for debugging purpose
  Serial.println(" °C");          // temperature print for debugging purpose

  ThingSpeak.writeField(CHANNEL_NUM, WRITE_FIELD, temp, WRITE_APIKEY);    // upload temperature to ThingSpeak MQTT server
  
  delay(MEASURE_DELAY);     // set delay
}

//=========================Connect WiFi Procedure=======================//

bool WiFiConnect(){
  if (WiFi.status() == WL_CONNECTED) return true;
  
  WiFi.begin(SSID, PASS);       // connect to WiFi with credencials
  return (WiFi.status() == WL_CONNECTED);
}

//=======================Read Temperature Funtion=======================//

float ReadTemp(){
  temp_sensor.requestTemperatures();        // send command to get temperature
  return temp_sensor.getTempCByIndex(0) + TEMP_CALIBRATION;    // function return temperature
}
