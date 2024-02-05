/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/
  
  Modified for AWS API Gateway by Stephen Borsay teaching AWS IoT on Udemy.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include "WiFi.h"
#include "U8g2lib.h"
#include "Wire.h"
#include <HTTPClient.h>
#include <WiFiManager.h>
#include <Arduino_JSON.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


const char* ssid = "15-624";
const char* password = "ZavieraElsaftyHadi";

//Your Domain name with URL path or IP address with path
String serverName = "https://45elgx4zs2.execute-api.ap-southeast-1.amazonaws.com/Box_control";
const char* API_KEY = "2dw6LIwV9C8knSd79w7Ek9v4KzloU6xFaXyvh0uL"; //Your AWS API-Key

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 1000;
unsigned long Uptime;

String sensorReadings;
float sensorReadingsArr[3];

void setup() {
  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 1 seconds (timerDelay variable), it will take 1 seconds before publishing the first reading.");
}

void loop() {

  int Lock =  random(0,2); //fake number range, adjust as you like
  int Mic =  random(0,2); //fake number range, adjust as you like
  int Cam =  random(0,2);
  int Record =  random(0,2);
  
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;

        String serverPath = serverName + "/?Lock=" + (String) Lock
        + "&Microphone=" + (String) Mic + "&Camera=" + (String) Cam + "&Record=" + (String) Record;
        ////http.addHeader("x-api-key", API_KEY);
        
        // Your Domain name with URL path or IP address with path
        http.begin(serverPath.c_str());
        http.addHeader("x-api-key", API_KEY);
        
        // Send HTTP GET request
        int httpResponseCode = http.GET();
        
        if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println(payload);
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        // Free resources
        http.end();
      }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}