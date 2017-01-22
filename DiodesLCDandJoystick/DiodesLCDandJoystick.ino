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

int targetX = 8;
int targetY = 1;

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

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
  
  // initialize joystick PINS
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(115200);
  
  // initialize LCD
  lcd.begin(16,2);
  
  // print message to LCD
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello world!");
  for (int i = 0; i < 4 ; i++)
  {
    lcd.setCursor(0,1);
    lcd.print(3-i);
    delay(1000);    
  }
  
  randomSeed(analogRead(0));
}


// the loop routine runs over and over again forever:
void loop() {
  
  lcd.clear();
  
  char toDisplay = 'x';
  boolean moveRight = false;
  boolean moveLeft = false;
  int level = 0;
  boolean showTarget = true;
  
  int pressed = digitalRead(SW_pin);
  if (pressed == 0) {
    toDisplay = 'B';
  } else {
    toDisplay = 'b';
  }
  
  int xAxis = analogRead(X_pin);
  int yAxis = analogRead(Y_pin);

  if (xAxis < 300) {
    moveLeft = true;
    moveRight = false;
  } else if (xAxis > 700) {
    moveLeft = false;
    moveRight = true;
  }
  
  if (yAxis < 300 && rowIndex == 1) {
    rowIndex = 0;
  } else if (yAxis > 700 && rowIndex == 0) {
    rowIndex = 1;
  }

  if (moveLeft && colIndex > 0) {
    colIndex = colIndex - 1;
  }  else if (moveRight && colIndex < 15) {
    colIndex = colIndex + 1;
  }
  
  lcd.setCursor(colIndex,rowIndex);
  lcd.print(toDisplay);

  if (targetX ==  colIndex && targetY == rowIndex) {
    showTarget = false;
    targetX = random(0,16);
    targetY = random(0,2);    
  } else {
    showTarget = true;
  }
  if (showTarget == true) {
    lcd.setCursor(targetX,targetY);
    lcd.print('O');
  }

  if (pressed == 0) {
    
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(led2, LOW);
    //lcd.backlight();
    delay(150);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(led2, HIGH);
    //lcd.noBacklight();
    delay(150);               // wait for a second  
  } else {
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(led2, LOW);
    delay(300);
  }
 
  
}
