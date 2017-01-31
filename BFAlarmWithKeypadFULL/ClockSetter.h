#ifndef MFCLOCKCONTROLLER
#define MFCLOCKCONTROLLER

#include <Arduino.h>
#include <virtuabotixRTC.h>

enum CLOCK_MODE { NONE, ENTER };
enum TIME_ELEMENTS { HOUR, MINUTE, SECOND };
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
    bool handleKeypadEntry(char entry);
    void getDisplayString(String &displayString);
  
  private:
    void getTwoDigitsDisplayString(String &display, int value, bool isBlinking);    
    void getFourDigitsDisplayString(String &display, int value, bool isBlinking);
    int getCurrentEntryAsInt(int maxLength);
    bool changeMode();
    bool modifyDisplay();
    bool handleCurrenyEntry(char entry, int maxLength, int maxValue);
    bool isDigit(char entry);
    void saveTime();
    void setMode(CLOCK_MODE cmode, TIME_ELEMENTS tmode, DATE_ELEMENTS dmode, int curEntryValue);
  private:
    CLOCK_MODE mode;
    virtuabotixRTC *time_pointer;
    TIME_ELEMENTS timeElement;
    DATE_ELEMENTS dateElement;
    bool blinkState;
    bool displayTime;
    String currentEntry;
    
    static const char MODIFY_MODE_CHAR = 'C';
    static const char MODIFY_DISPLAY_CHAR = 'D';
  };

#endif 
