/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <virtuabotixRTC.h>

int RECV_PIN = 11;

const int pinCE = 7;
const int pinIO = 6;
const int pinSLK = 5;

const int tempPin = 0;

IRrecv irrecv(RECV_PIN);

decode_results results;
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int led2 = 12;
int colIndex = -1;
int rowIndex = 0;

int targetX = 8;
int targetY = 1;

char toDisplay = 'x';

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

//char text[]="hello world";

// set LCD address to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

virtuabotixRTC myRTC(pinSLK, pinIO, pinCE);

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  pinMode(led2, OUTPUT);

  // clear pins to turn-off the leds 
  digitalWrite(led, LOW);
  digitalWrite(led2, LOW);
  
  Serial.begin(115200);
  
  // initialize LCD
  lcd.begin(16,2);
  
  // initialize IR
  irrecv.enableIRIn();
  
  // print message to LCD
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello world!");
  for (int i = 0; i < 4 ; i++)
  {
    lcd.setCursor(0,1);
    lcd.print(3-i);
    delay(500);    
  }
  
  randomSeed(analogRead(2));
  
  // SET-UP time (only first time)
  //myRTC.setDS1302Time(00, 35, 13, 1, 2, 1, 2016);
}


//void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
//  analogWrite(redPin, red);
//  analogWrite(greenPin, green);
//  analogWrite(bluePin, blue);
//}

// the loop routine runs over and over again forever:
void loop() {
  
  lcd.clear();
  
/** BEGIN: THIS PART IS GOING WITH RBG LED ONLY */
//  unsigned int rgbColour[3];
//
//  // Start off with red.
//  rgbColour[0] = 255;
//  rgbColour[1] = 0;
//  rgbColour[2] = 0; 
//
//  // Choose the colours to increment and decrement.
//  for (int decColour = 0; decColour < 3; decColour += 1) {
//    int incColour = decColour == 2 ? 0 : decColour + 1;
//
//    // cross-fade the two colours.
//    for(int i = 0; i < 255; i += 1) {
//      rgbColour[decColour] -= 1;
//      rgbColour[incColour] += 1;
//      
//      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
//      delay(5);
//    }
//}

/** END: THIS PART IS GOING WITH RBG LED ONLY */

/** work with time */
myRTC.updateTime();


  int val = analogRead(tempPin);
  float mv = ( val/1024.0)*5000; 
  float cel = mv/10;
  float farh = (cel*9)/5 + 32;

  // print temp to LCD
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.setCursor(6,0);
  lcd.print(cel);

  // print time to LCD
  lcd.setCursor(0,1);
  lcd.print(myRTC.hours);
  lcd.setCursor(2,1);
  lcd.print(":");
  lcd.setCursor(3,1);
  lcd.print(myRTC.minutes);
  lcd.setCursor(5,1);
  lcd.print(":");
  lcd.setCursor(6,1);
  lcd.print(myRTC.seconds);
  
  delay(500);
}
