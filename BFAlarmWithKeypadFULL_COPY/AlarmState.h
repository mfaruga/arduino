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
    inline ALARM_STATE getState() { return state; }    
  private:
    void initializeState(ALARM_STATE targetState);
    static const unsigned long TIMEOUT_ENTRY = 20000;
    static const unsigned long TIMEOUT_GO_OUT = 20000;
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
