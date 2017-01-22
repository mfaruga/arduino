/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

int RECV_PIN = 11;

const int redPin = 7;
const int greenPin = 6;
const int bluePin = 5;

const int tempPin = 1;

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

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  pinMode(led2, OUTPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  setColourRgb(0,0,0);

  // clear pins to turn-off the leds 
  digitalWrite(led, LOW);
  digitalWrite(led2, LOW);
  
  // initialize joystick PINS
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
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
    delay(1000);    
  }
  
  randomSeed(analogRead(0));
}


void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

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

  boolean moveRight = false;
  boolean moveLeft = false;
  int level = 0;
  boolean showTarget = true;

  // handle IR
  if (irrecv.decode(&results)) {
    lcd.setCursor(0,1);
    lcd.print(results.value);
    
    if (results.value == 16724175) { // 1
      toDisplay = '1';  
    } else if (results.value == 16718055) { // 2
      toDisplay = '2';
    } 
    //else {
    //  toDisplay = 'X';
    //}
    
    //Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }

  
  // handle joystick input
  int pressed = digitalRead(SW_pin);
  //if (pressed == 0) {
  //  toDisplay = 'B';
  //} else {
  //  toDisplay = 'b';
  //}
  
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
  
 
  // print to LCD
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
