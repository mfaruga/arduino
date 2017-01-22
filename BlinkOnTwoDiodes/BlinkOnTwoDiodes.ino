/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int led2 = 12;
int colIndex = -1;
int rowIndex = 0;
//char text[]="hello world";

// set LCD address to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  pinMode(led2, OUTPUT);

  // clear pins to turn-off the leds 
  digitalWrite(led, LOW);
  digitalWrite(led2, LOW);

  // initialize LCD
  lcd.begin(16,2);

  // print message to LCD
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello world!");
  lcd.setCursor(0,1);
  lcd.print("Hello Bartus");
  delay(2000);
}


//  lcd.init();
// Print a message to the LCD.
//  lcd.backlight();
//  lcd.setCursor(3,0);
//  lcd.print("Hello, world!");
//  lcd.setCursor(2,1);
//  lcd.print("SainSmart for UNO");
//   lcd.setCursor(2,2);
//  lcd.print("SainSmart LCM IIC");
//   lcd.setCursor(1,3);
//  lcd.print("Design By SainSmart");

// the loop routine runs over and over again forever:
void loop() {
  lcd.clear();

  colIndex += 1;
  if (colIndex >= 16) {
    colIndex = 0;
    rowIndex += 1;
    if (rowIndex > 1) {
      rowIndex = 0;
    }
  }
  lcd.setCursor(colIndex,rowIndex);
  lcd.print("o");
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led2, LOW);
  //lcd.backlight();
  delay(250);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(led2, HIGH);
  //lcd.noBacklight();
  delay(250);               // wait for a second
}

