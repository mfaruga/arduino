#ifndef MFCLOCKCONTROLLER
#define MFCLOCKCONTROLLER

#include <Arduino.h>
#include <virtuabotixRTC.h>

enum CLOCK_MODE { NONE, ENTER_TIME, ENTER_DATE };
enum TIME_ELEMENTS { HOUR, MINUTE };
enum DATE_ELEMENTS { YEAR, MONTH, DAY };

class MFClockController {
  public:
    MFClockController(virtuabotixRTC * timePointer) {
      this->time_pointer = timePointer;
      this->mode = NONE;
      this->timeElement = HOUR;
      this->dateElement = YEAR;
      this->blinkState = false;
      this->displayTime = true;
    }
    void handleKeypadEntry(char entry);
    void getDisplayString(String &displayString);
  
  private:
    CLOCK_MODE mode;
    virtuabotixRTC *time_pointer;
    TIME_ELEMENTS timeElement;
    DATE_ELEMENTS dateElement;
    bool blinkState;
    bool displayTime;
    
    static const char MODIFY_MODE_CHAR = 'C';
    static const char MODIFY_DISPLAY_CHAR = 'D';
  };

#endif 
