#include <Arduino.h>
#include "AlarmState.h"

MFAlarm::MFAlarm() 
{
  this->timerGoOut = 0;
  this->timerEntry = 0;
  this->timerLastMovement = 0;
  
  clearFlags();
  initializeState(NOT_ARMED); 
  
}

void MFAlarm::pinOk() 
{
  switch (this->state) {
    
    // if not armed - we're arming
    case NOT_ARMED :
      initializeState(GOING_OUT);
      break;
    // otherwise we're going to unarmed mode
    case GOING_OUT :
    case ENTERING :
    case ARMED :
    case ALARM :
      initializeState(NOT_ARMED);
      break;
    default :
      Serial.print("UNRECOGNIZED SWITCH STATEMENT: alarm_pinOK");
  } 
}

void MFAlarm::pinNotOk() 
{
  switch (this->state) {

    // if alarm is armed and someone passes wrong pin then we're treating this as if someone was entering
    case ARMED :
      initializeState(ENTERING);
      break;    
    // otherwise we do not care (i.e. nothing happens)
    case NOT_ARMED :
    case GOING_OUT :
    case ENTERING :
    case ALARM :
      // do nothing
      break;
    default :
      Serial.print("UNRECOGNIZED SWITCH STATEMENT: MFAlarm::pinNotOk()");
  } 
  
  // set-up flag that will be used to produce some notification to customer
  flag_wrongPinEntered = true;
}

void MFAlarm::moveDetected() 
{
  switch (this->state) {

    // if alarm is armed and move was detected we require pin to be entered
    case ARMED :
      initializeState(ENTERING);
      break;    
    // otherwise we do not care (i.e. nothing happens)
    case NOT_ARMED :
    case GOING_OUT :
    case ENTERING :
    case ALARM :
      // do nothing
      break;
    default :
      Serial.print("UNRECOGNIZED SWITCH STATEMENT: MFAlarm::moveDetected()");
  } 
  
  // store time of last movement
  timerLastMovement = millis();
  
  // set-up flag that will be used to produce some notification to customer
  flag_moveDetected = true;
}

void MFAlarm::clearFlags() 
{
    this->flag_wrongPinEntered = false;
    this->flag_moveDetected = false;
}

void MFAlarm::iterate() 
{
  unsigned long timeDiffGoOut = 0;
  unsigned long timeDiffEntering = 0;
  
  switch (this->state) {
    case GOING_OUT :
      timeDiffGoOut = millis() - timerGoOut;
      if (timeDiffGoOut > TIMEOUT_GO_OUT) {
        initializeState(ARMED);  
      }
      break;
    case ENTERING :
      timeDiffEntering = millis() - timerEntry;
      if (timeDiffEntering > TIMEOUT_ENTRY) {
        initializeState(ALARM);  
      }
      break;    
    // otherwise we do not have any timer based operations
    case NOT_ARMED :
    case ARMED :
    case ALARM :
      // do nothing
      break;
    default :
      Serial.print("UNRECOGNIZED SWITCH STATEMENT: MFAlarm::iterate()");
  } 
}

void MFAlarm::initializeState(ALARM_STATE targetState) 
{
  switch (targetState) {
    
    case NOT_ARMED :
      this->timerGoOut = 0;
      this->timerEntry = 0;
      break;
    case GOING_OUT :
      // read current time to be able to count down
      this->timerGoOut = millis();
      break;
    case ENTERING :
      this->timerEntry = millis();
      break;
    case ARMED :
      break;
    case ALARM :
      this->timerGoOut = 0;
      this->timerEntry = 0;
      break;
    default :
      Serial.print("UNRECOGNIZED SWITCH STATEMENT: MFAlarm::initializeState");
  } 
  
  // set-up target state
  this->state = targetState;
  
}
//
//
//void alarm_pinOK(struct MFAlarm *alarm) {
//   
//}
