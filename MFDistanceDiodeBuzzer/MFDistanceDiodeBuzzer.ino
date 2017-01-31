#include <NewTone.h>
#include "HC_SR04.h"

#define TRIG_PIN 3
#define ECHO_PIN 2
#define ECHO_INT 0

#define BUZZER_PIN 8
#define DIODE_PIN 9


HC_SR04 sensor(TRIG_PIN, ECHO_PIN, ECHO_INT);

void setup(){  
  sensor.begin();
  Serial.begin(9600); 
  sensor.start();
}

unsigned int range = 10;

void loop(){
  
  if (sensor.isFinished()) {
    range = sensor.getRange();
    sensor.start();
  }

  Serial.print(sensor.getRange());
  Serial.println("cm");

  if (range < 20) {
    //NewTone(BUZZER_PIN, 4000, 20 + 200/range);
    NewTone(BUZZER_PIN, 4000, 50);
    delay(250 - 200/range);
  } else {
    delay(300);
  } 
  
//  if (range > 20) {
//    digitalWrite(DIODE_PIN, HIGH);
//  } else {
//    digitalWrite(DIODE_PIN, LOW);
//  }

  
}

