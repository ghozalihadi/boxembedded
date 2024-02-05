#include <Arduino.h>
#include "WiFi.h"
#include "U8g2lib.h"
#include "Wire.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

////////////////////////////////////////////////////////////////////
///////////////////////////* RTOS */////////////////////////////////
TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

void TaskRelay(void *pvParameters);
void TaskOLED(void *pvParameters);
////////////////////////////////////////////////////////////////////

#define LED1 26
#define RELAY 18

////// WiFI /////
const char* ssid = "Ghozali";
const char* pass = "ghadi2021";
//const char* ssid = "15-624";
//const char* pass = "ZavieraElsaftyHadi";

////// OLED 128x64 /////
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

////// QR Scanner /////
#define RXD2 23
#define TXD2 19

////// AWS //////
#define AWS_API_KEY "0nAjSAnXF250jkBug7fGF56b6mF45j5f5kGP3Ekd"

////// Keypad 3x4 /////
const byte ROWS = 4; // rows
const byte COLS = 3; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {5, 2, 4, 13}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 27, 33}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
const String password = "7890"; // change your password here
String input_password;
byte rowScan = 0;
byte startScan = 0;
char key = 'X';

void Keypad(){
  digitalWrite(rowPins[rowScan], LOW);
  delay(1);
  for (byte j=0;j<COLS;j++){
    byte pressed = digitalRead(colPins[j]);
    if (pressed == LOW){
      key = keys[rowScan][j];
      break;
    }
  }
  digitalWrite(rowPins[rowScan], HIGH);
}

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

  ////// OLED 128x64 /////
  u8g2.begin();
  Serial.println("Initiating OLED screen done.");

  ///// QR Scanner //////
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  //Start the program
  Serial.println("Hello World! Box Embedded program just started...");

  //////////////RTOS////////////////////////
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskRelay, "Task1", 2048, NULL, 1, &Task1, 1);
  delay(1000);
  xTaskCreatePinnedToCore(TaskOLED, "Task2", 2048, NULL, 2, &Task2, 1);
  delay(1000);

  ////// Keypad 3x4 /////
  for (byte i=0;i<ROWS;i++){
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  for (byte j=0;j<COLS;j++){
    pinMode(colPins[j], INPUT_PULLUP);
  }
  startScan = 1;
}

void loop() {
  if (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
  if (Serial.available()) {
    Serial2.write(char(Serial.read()));
  }
}

void TaskRelay(void *pvParameters) // This is a task.
{
  (void)pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    ////// Keypad 3x4 /////
    rowScan++;
    if (rowScan>=ROWS) rowScan=0;
    if (startScan==1) Keypad();
    if (key!='X'){
      Serial.print("key pressed: ");
      Serial.println(key);
    }
    key = 'X';
    
    ////// RELAY /////
    digitalWrite(LED1, HIGH);
    delay(100);
    digitalWrite(LED1, LOW);
    delay(100);
  }
}

void TaskOLED(void *pvParameters) // This is a task.
{
  (void)pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.drawStr(0,10,"Hello World!");  // write something to the internal memory
    u8g2.sendBuffer();          // transfer internal memory to the display
    //delay(10); 
  }
}