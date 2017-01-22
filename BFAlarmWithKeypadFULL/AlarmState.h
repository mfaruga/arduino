#ifndef MFALARM
#define MFALARM
 
#include <Arduino.h>

enum ALARM_STATE { NOT_ARMED, GOING_OUT, ARMED, ENTERING, ALARM };

class MFAlarm {
  public:
    MFAlarm();
    // someone entered correct PIN
    void pinOk();
    // someone entered wrong PIN
    void pinNotOk();
    // movement was detected
    void moveDetected();
    // work on timer based events
    void iterate();
    void clearFlags();

    // getters 
    inline bool isWrongPinEntered() { return flag_wrongPinEntered; }
    inline bool isMoveDetected() { return flag_moveDetected; }  
    ALARM_STATE getState();
    inline unsigned long getTimerGoOutInSeconds() { return (TIMEOUT_ENTRY - (millis() - timerGoOut))/1000; }
    inline unsigned long getTimerEntryInSeconds() { return (TIMEOUT_GO_OUT- (millis() - timerEntry))/1000; }
    
    void getStateAsString(String &dest);  
    void getTimerAsString(String &dest);
  private:
    void initializeState(ALARM_STATE targetState);
    static const unsigned long TIMEOUT_ENTRY = 10000;
    static const unsigned long TIMEOUT_GO_OUT = 10000;
    // value of timer when going out 
    unsigned long timerGoOut;
    // value of timer when entering
    unsigned long timerEntry;
    // value of timer when last movement was detected
    unsigned long timerLastMovement;
    // alarm state
    enum ALARM_STATE state;
    // set of flags that will be processed with each iteration
    bool flag_wrongPinEntered;
    bool flag_moveDetected;
};

#endif
