/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <NewTone.h>

int RECV_PIN = 11;

const int PIR_PIN = 8;
const int BUZZ_PIN = 7;

IRrecv irrecv(RECV_PIN);

decode_results results;
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int led2 = 12;
int colIndex = -1;
int rowIndex = 0;
bool inMotion = false;
unsigned long motionDelay = 2000; // Motion Delay Timer
unsigned long motionTimer; // Motion Trigger Timer

int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

bool alarmArmed = false;

// set LCD address to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  pinMode(led2, OUTPUT);

  // initialize PIR pin mode to input
  pinMode(PIR_PIN, INPUT);

  // initialize Buzzer PIN
  pinMode(BUZZ_PIN, OUTPUT);
  //noTone(BUZZ_PIN);

  // clear pins to turn-off the leds 
  digitalWrite(led, LOW);
  digitalWrite(led2, LOW);
  
  Serial.begin(9600);
  
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


// the loop routine runs over and over again forever:
void loop() {
  
  lcd.clear();
  
  // handle IR
  if (irrecv.decode(&results)) {
    lcd.setCursor(0,1);
    lcd.print(results.value);
    
    if (results.value == 16724175) { // 1
      alarmArmed = true;  
    } else if (results.value == 16718055) { // 2
      alarmArmed = false;
    } 
    
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }

  // read data from motion detector
  if (digitalRead(PIR_PIN) == HIGH) {
    inMotion = true;
    motionTimer = millis();
  } else if (millis() - motionTimer >= motionDelay) {
    inMotion = false;
  }

  // print to LCD
  lcd.setCursor(0,0);
  if (inMotion == true) {
    lcd.print("Wykryto ruch");
  } else {
    lcd.print("Brak ruchu");
  }
  
  lcd.setCursor(0,1);
  if (alarmArmed == true) {
    lcd.print("Uzbrojony");
  } else {
    lcd.print("Nieuzbrojony");
  }
  
  if (alarmArmed &&  inMotion) {
    //tone(BUZZ_PIN, 2000, 400);
    Serial.println("BUZZER");
    for (unsigned long freq = 125; freq <= 15000; freq += 10) { 
    NewTone(BUZZ_PIN, freq); // Play the frequency (125 Hz to 15 kHz sweep in 10 Hz steps).
    delay(1); // Wait 1 ms so you can hear it.
  }
  noNewTone(BUZZ_PIN); // Turn off the tone.

  } else {
    //noTone(BUZZ_PIN);
    Serial.println("NO BUZZER");
  }
  delay(500);
  
//  if (alarmArmed && inMotion) {
//   digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
//   digitalWrite(led2, HIGH);
//   //tone(BUZZ_PIN, 2000);
// 
//   //lcd.backlight();
//   delay(100);               // wait for a second
//   digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
//   digitalWrite(led2, LOW);
//   //noTone(BUZZ_PIN);
//   delay(100);
//  } else {
//   digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
//   digitalWrite(led2, LOW);
//    delay(1000);
//  }
 
  
}
