#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewTone.h>
#include <Keypad.h>
#include "AlarmState.h"

const int RECV_PIN = 13;
const int PIR_PIN = 10;
const int BUZZ_PIN = 11;
const int LED_YELLOW_PIN = 12;
const int LED_GREEN_PIN = 15;

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]=
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

char pinEntry[4] = {'*','*','*','*'};
char pin[4]= {'1','2','3','4'};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

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
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);

  // initialize PIR pin mode to input
  pinMode(PIR_PIN, INPUT);

  // initialize Buzzer PIN
  pinMode(BUZZ_PIN, OUTPUT);
  //noTone(BUZZ_PIN);

  // clear pins to turn-off the leds 
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
  
  Serial.begin(9600);
  
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

void basicKeypressHandling() {
  char keypressed = myKeypad.getKey();

  if (keypressed != NO_KEY) {
    Serial.print(keypressed);

    NewTone(BUZZ_PIN, 2000, 100);
    
    if (keypressed == '1') {
      alarmArmed = true;
    } else if (keypressed == '2') {
      alarmArmed = false;
    }    
  }
}

void clearPinEntry() {
  for (int i = 0; i < 4 ; i++) {
    pinEntry[i] = '*';
  }
}

void processPin() {
  bool pinCorrect = true;
  for (int i = 0; i < 4 ; i++) {
    if (pinEntry[i] != pin[i]) {
      pinCorrect = false;
      break;
    } 
  }
  
  if (pinCorrect == true) {
    if (alarmArmed) {
      for (int i = 0; i<3; i++) {
        NewTone(BUZZ_PIN, 2000, 100 + i*50);
        delay(500);  
      }
    } else {
      NewTone(BUZZ_PIN, 2000, 100);
      for (int i = 0; i<3; i++) {
        NewTone(BUZZ_PIN, 4000, 400 - i*50);  
        delay(500);  
    }

    }
    alarmArmed = !alarmArmed;
  } else {
  NewTone(BUZZ_PIN, 6000, 1000);
  }
  
  
  
  clearPinEntry();
}

void addEntryToPin(char keypressed) {
  for (int i = 0; i < 3; i++) {
    pinEntry[i] = pinEntry[i+1];
  }
  pinEntry[3] = keypressed;
}

void processKeypad() {

  char keypressed = myKeypad.getKey();
      
  if (keypressed != NO_KEY) {
    // if this is '#' compare PIN with PIN and enable/disable
    NewTone(BUZZ_PIN, 2000, 100);
    if (keypressed == '#') {
      processPin();
    } else {
      addEntryToPin(keypressed);
    }
  }
  
// unsigned long milisec = millis();
// if (timeFromLastKeypress 
// milisec - motionTimer >= motionDelay

}

// the loop routine runs over and over again forever:
void loop() {
  
  lcd.clear();
  
  // handle keypad
  processKeypad();
  
  // read data from motion detector
  unsigned long milisec = millis();
  bool moveDetected = (digitalRead(PIR_PIN) == HIGH);
  
  if (moveDetected) {
    digitalWrite(LED_YELLOW_PIN, HIGH);
  } else {
    digitalWrite(LED_YELLOW_PIN, LOW);
  }
  
  if (moveDetected && !inMotion) {
    inMotion = true;
    motionTimer = milisec;
  } else if (milisec - motionTimer >= motionDelay) {
    inMotion = false;
  }

  // print to LCD
  lcd.setCursor(0,0);
  if (inMotion == true) {
    lcd.print("RUCH");
  } else {
    lcd.print("SPOKOJNIE");
  }
  
  lcd.setCursor(0,1);
  if (alarmArmed == true) {
    lcd.print("Uzbrojony");
  } else {
    lcd.print("Nieuzbrojony");
  }
 
   // print pin
   int lcdIndex = 12;
   for (int index = 0; index < 4; index++) {
     lcd.setCursor(lcdIndex,0);
     lcd.print(pinEntry[index]);
     lcdIndex += 1;
   }
   
  
  if (alarmArmed &&  inMotion) {
    //tone(BUZZ_PIN, 2000, 400);
    //Serial.println("BUZZER");
    //for (unsigned long freq = 125; freq <= 15000; freq += 100) { 
    //  NewTone(BUZZ_PIN, freq); // Play the frequency (125 Hz to 15 kHz sweep in 10 Hz steps).
    //delay(1); // Wait 1 ms so you can hear it.
    //}
    //noNewTone(BUZZ_PIN); // Turn off the tone.
    //digitalWrite(LED_YELLOW_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, HIGH);
  } else {
    //noTone(BUZZ_PIN);
    //Serial.println("NO BUZZER");
    //digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
  }
  delay(100);
  
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
