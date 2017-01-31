#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewTone.h>
#include <Keypad.h>
#include <virtuabotixRTC.h>
#include "ClockSetter.h"
#include "AlarmState.h"

const int SOUND_PIN = 14;
const int PIR_PIN = 10;
const int BUZZ_PIN = 11;
const int LED_YELLOW_PIN = 12;
//const int LED_GREEN_PIN = 15;

const int pinCE = 17;
const int pinIO = 16;
const int pinSLK = 15;


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

int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };
boolean highLastTime = false;

// set LCD address to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// alarm related objects
MFAlarm alarm;
bool displayOnLCD = false;
bool playSounds = true;

// time handling
virtuabotixRTC myRTC(pinSLK, pinIO, pinCE);
MFClockController clock(&myRTC);
bool displayTime = true;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(LED_YELLOW_PIN, OUTPUT);
  //pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(SOUND_PIN, INPUT);

  // initialize PIR pin mode to input
  pinMode(PIR_PIN, INPUT);

  // initialize Buzzer PIN
  pinMode(BUZZ_PIN, OUTPUT);
  //noTone(BUZZ_PIN);

  // clear pins to turn-off the leds 
  digitalWrite(LED_YELLOW_PIN, LOW);
  //digitalWrite(LED_GREEN_PIN, LOW);
  
  Serial.begin(9600);
  
  // initialize LCD
  lcd.begin(16,2);
  
  // print message to LCD
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Czesc ;-)!");
  for (int i = 0; i < 4 ; i++)
  {
    lcd.setCursor(0,1);
    lcd.print(3-i);
    delay(1000);    
  }
  
  randomSeed(analogRead(0));
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
    
    // TODO make sure that this is moved to class flag that will be checked afterwards
    if (alarm.getState() == ARMED) 
    {
      if (playSounds) {
        for (int i = 0; i<3; i++) 
        {
          NewTone(BUZZ_PIN, 2000, 100 + i*50);
          delay(500);  
        }
      }
    } 
    else 
    {
      if (playSounds) {
        NewTone(BUZZ_PIN, 2000, 100);
        for (int i = 0; i<3; i++) {
          NewTone(BUZZ_PIN, 4000, 400 - i*50);  
          delay(500);  
        }
      }
      alarm.pinOk();
    }
    
    
  } else {
    if (playSounds) {
      NewTone(BUZZ_PIN, 6000, 1000);
    }
    alarm.pinNotOk();
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
    if (playSounds) {
      NewTone(BUZZ_PIN, 2000, 100);
    }
    
    if (keypressed == '*') {
      processPin();
    } else if (keypressed == 'A') {
      playSounds = !playSounds;
    } else if (keypressed == 'D') {
      displayTime = !displayTime;
    }
    else {
      addEntryToPin(keypressed);
    }
  }
  
// unsigned long milisec = millis();
// if (timeFromLastKeypress 
// milisec - motionTimer >= motionDelay

}

void handleLCDDisplay() {
  // print to LCD
  lcd.setCursor(0,0);
  String stateAsString;
  alarm.getStateAsString(stateAsString);
  lcd.print(stateAsString);
  
   // print pin
   int lcdIndex = 12;
   for (int index = 0; index < 4; index++) {
     lcd.setCursor(lcdIndex,0);
     lcd.print(pinEntry[index]);
     lcdIndex += 1;
   }
  
   // print time (if needed)
   if (alarm.getState() == GOING_OUT || alarm.getState() == ENTERING)
   {
     lcd.setCursor(14,1);
     String timeAsString;
     alarm.getTimerAsString(timeAsString);
     lcd.print(timeAsString);  
   } 

}

// the loop routine runs over and over again forever:
void loop() {
  
  lcd.clear();
  
  // handle keypad
  processKeypad();
  
  // read data from motion detector
  if (digitalRead(PIR_PIN) == HIGH) {
    alarm.moveDetected();
  }

  if (digitalRead(SOUND_PIN) == LOW) {
    alarm.moveDetected();
  }
  
  //if (analogRead(SOUND_PIN) == HIGH) {
  //  Serial.print("SOUND DETECTED\n");
  //}

  myRTC.updateTime();

  alarm.iterate();
  
  if (alarm.isMoveDetected()) {
    digitalWrite(LED_YELLOW_PIN, HIGH);
  } else {
    digitalWrite(LED_YELLOW_PIN, LOW);
  }

  handleLCDDisplay();
  
  if (displayTime) {
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
  } else {
    
    
    lcd.setCursor(0,1);
    switch (myRTC.dayofweek)
    {
      case 1: 
        lcd.print("PO");
        break;
      case 2:
        lcd.print("WT");
        break;
      case 3: 
        lcd.print("SR");
        break;
      case 4:
        lcd.print("CZ");
        break;
      case 5: 
        lcd.print("PI");
        break;
      case 6:
        lcd.print("SO");
        break;
      case 7: 
        lcd.print("NI");
        break;
      default:
        lcd.print("??");
        break;
    }
    lcd.print(myRTC.dayofweek);
    lcd.setCursor(2,1);
    lcd.print(" ");
    
    lcd.setCursor(3,1);
    lcd.print(myRTC.year);
    lcd.setCursor(7,1);
    lcd.print("-");

    lcd.setCursor(8,1);
    lcd.print(myRTC.month);
    lcd.setCursor(10,1);
    lcd.print("-");

    lcd.setCursor(11,1);
    lcd.print(myRTC.dayofmonth);
    
  }
    
  if (alarm.getState() == ALARM) 
  {
    //digitalWrite(LED_GREEN_PIN, HIGH);
    //digitalWrite(LED_GREEN_PIN, highLastTime ? HIGH : LOW);
    highLastTime = !highLastTime;
    if (playSounds) {
      NewTone(BUZZ_PIN, 4000, 20);
    }
        //for (unsigned long freq = 125; freq <= 15000; freq += 100) { 
    //  NewTone(BUZZ_PIN, freq); // Play the frequency (125 Hz to 15 kHz sweep in 10 Hz steps).
    //delay(1); // Wait 1 ms so you can hear it.
    //}
    //noNewTone(BUZZ_PIN); // Turn off the tone.

  } 
  else 
  {
    //digitalWrite(LED_GREEN_PIN, LOW);
    //digitalWrite(LED_YELLOW_PIN, LOW);
  }
    
  alarm.clearFlags();
  delay(100); 
}


