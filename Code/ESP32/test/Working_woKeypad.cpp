#include <Arduino.h>
#include "WiFi.h"
#include "Adafruit_Keypad.h"
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define LED1 26
#define RELAY 18

////// WiFI /////
const char* ssid = "Ghozali";
const char* pass = "ghadi2021";

// ////// Keypad 3x4 /////
// const byte ROWS = 4; // rows
// const byte COLS = 3; // columns
// //define the symbols on the buttons of the keypads
// char keys[ROWS][COLS] = {
//   {'1', '2', '3'},
//   {'4', '5', '6'},
//   {'7', '8', '9'},
//   {'*', '0', '#'}
// };
// byte rowPins[ROWS] = {19, 18, 5, 17}; //connect to the row pinouts of the keypad
// byte colPins[COLS] = {16, 4, 2}; //connect to the column pinouts of the keypad
// //initialize an instance of class NewKeypad
// Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// const String password = "7890"; // change your password here
// String input_password;

////// OLED 128x64 /////
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  // put your setup code here, to run once:
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(RELAY, OUTPUT);
  
  ////// WiFI /////
  Serial.println(); Serial.println(); Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(""); Serial.println("WiFi connected.");
  Serial.println("IP address: ");Serial.println(WiFi.localIP());
  
  // ////// Keypad 3x4 /////
  // customKeypad.begin();
  // input_password.reserve(32);
  Serial.println("Initiating keypad done.");

  ////// OLED 128x64 /////
  u8g2.begin();
  Serial.println("Initiating OLED screen done.");

  //Start the program
  Serial.println("Hello World! Box Embedded program just started...");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // customKeypad.tick();
  // while(customKeypad.available()){
  //   keypadEvent e = customKeypad.read();
  //   Serial.print((char)e.bit.KEY);
  //   if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
  //   else if(e.bit.EVENT == KEY_JUST_RELEASED) {
  //     Serial.println(" released");
  //     char key = (char)e.bit.KEY;
  //     if (key == '*') {
  //       input_password = ""; // clear input password
  //     } else if (key == '#') {
  //       if (password == input_password) {
  //         Serial.println("The password is correct, ACCESS GRANTED!");
  //         // DO YOUR WORK HERE

  //       } else {
  //         Serial.println("The password is incorrect, ACCESS DENIED!");
  //       }

  //       input_password = ""; // clear input password
  //     } else {
  //       input_password += key; // append new character to input password string
  //     }
  //   }
  // }

  // delay(10);
  

  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,"Hello World!");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000); 
}