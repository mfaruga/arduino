#include "ClockSetter.h"

bool MFClockController::handleKeypadEntry(char entry)
{
  if (entry == MODIFY_MODE_CHAR) {
    return changeMode();    
  } else if (mode == NONE && entry == MODIFY_DISPLAY_CHAR) {
    return modifyDisplay();
  } else if (mode != NONE) {
    int numberOfChars = 2;
    int maxValue = 60;
    
    if (displayTime) {
        if (timeElement == HOUR) {
          maxValue = 24;
        }
    } else {
      switch (dateElement) {
        case YEAR:
          numberOfChars = 4;
          maxValue = 9999;
          break;
        case MONTH:
          maxValue = 12;
          break;
        case DAY:
          maxValue = 31;
          break;  
      }
    }
    return handleCurrenyEntry(entry, numberOfChars, maxValue);
  }
  
  return false;
  
}

bool MFClockController::modifyDisplay() 
{
  this->displayTime = !this->displayTime;
  return true;
}

bool MFClockController::handleCurrenyEntry(char entry, int maxLength, int maxValue) 
{
  if (!isDigit(entry)) {
    return false;
  }
  
  // trim if overflow
  if (currentEntry.length() >= maxLength) {
    currentEntry = "";
  }
  
  currentEntry += entry;
  
  // handle numeric overflows
  int currentValue = getCurrentEntryAsInt(maxLength);
  if (currentValue > maxValue) {
    currentEntry = "";
  }
  return true;
}

bool MFClockController::isDigit(char entry) {
  return (entry == '0' || entry == '1' || entry == '2' || entry == '3' || entry == '4' || entry == '5' || entry == '6' || entry == '7' || entry == '8' || entry == '9');
}

void MFClockController::setMode(CLOCK_MODE cmode, TIME_ELEMENTS tmode, DATE_ELEMENTS dmode, int curEntryValue)
{
  this->mode = cmode;
  this->timeElement = tmode;
  this->dateElement = dmode;
  this->currentEntry = "";
  this->currentEntry += curEntryValue;
}

bool MFClockController::changeMode() {
    
  // save time if this is needed
  if (mode != NONE) {
    saveTime();
  }  
  
  // change mode
  if (mode == NONE) {
      setMode(ENTER, HOUR, YEAR, displayTime ? time_pointer->hours : time_pointer->year);
    } else if (displayTime) {
      switch (timeElement) {
        case HOUR:
          setMode(ENTER, MINUTE, YEAR, time_pointer->minutes);
          break; 
        case MINUTE:
          setMode(ENTER, SECOND, YEAR, time_pointer->seconds);
          break;
        case SECOND:
          setMode(NONE, HOUR, YEAR, 0);          
          break;
      }
    } else {
      switch (dateElement) {
        case YEAR:
          setMode(ENTER, HOUR, MONTH, time_pointer->month);
          break;
        case MONTH:
          setMode(ENTER, HOUR, DAY, time_pointer->dayofmonth);
          break;
        case DAY:
          setMode(NONE, HOUR, YEAR, 0);
          break;
      }
    }
    return true;
}

void MFClockController::saveTime() {
  
  int value = 0;
  if (displayTime) 
  {
    switch (timeElement) {
      case HOUR:
          value = getCurrentEntryAsInt(2);
          time_pointer->setDS1302Time(time_pointer->seconds, time_pointer->minutes, value, 1, time_pointer->dayofmonth, time_pointer->month, time_pointer->year);  
        break;
      case MINUTE:
          value = getCurrentEntryAsInt(2);
          time_pointer->setDS1302Time(time_pointer->seconds, value, time_pointer->hours, 1, time_pointer->dayofmonth, time_pointer->month, time_pointer->year);  
        break;
      case SECOND:
          value = getCurrentEntryAsInt(2);
          time_pointer->setDS1302Time(value, time_pointer->minutes, time_pointer->hours, 1, time_pointer->dayofmonth, time_pointer->month, time_pointer->year);  
        break;
    }
  } else 
  {
    switch (dateElement) 
    {
      case YEAR:
        value = getCurrentEntryAsInt(4);
        time_pointer->setDS1302Time(time_pointer->seconds, time_pointer->minutes, time_pointer->hours, 1, time_pointer->dayofmonth, time_pointer->month, value);  
        break;
      case MONTH:
        value = getCurrentEntryAsInt(2);
        time_pointer->setDS1302Time(time_pointer->seconds, time_pointer->minutes, time_pointer->hours, 1, time_pointer->dayofmonth, value, time_pointer->year);  
        break;
      case DAY:
        value = getCurrentEntryAsInt(2);
        time_pointer->setDS1302Time(time_pointer->seconds, time_pointer->minutes, time_pointer->hours, 1, value, time_pointer->month, time_pointer->year);  
        break;
    }
  }
  
}

void MFClockController::getTwoDigitsDisplayString(String &display, int value, bool isBlinking)
{
  if (isBlinking) {
    display = "  ";
  } else {
    if (value < 10) {
      display += "0";
    }
    display += value;
  }
}

void MFClockController::getFourDigitsDisplayString(String &display, int value, bool isBlinking)
{
  if (isBlinking) {
    display = "    ";
  } else {
    if (value < 10) {
      display += "000";
    } else if (value < 100) {
      display += "00";
    } else if (value < 1000) {
      display += "0";
    }
    display += value;
  }
}


int MFClockController::getCurrentEntryAsInt(int maxLength) {
  if (currentEntry.length() == 0 || currentEntry.length() > maxLength) {
    return 0;
  }
  return currentEntry.toInt();
}

void MFClockController::getDisplayString(String &displayString)
{
    
    if (displayTime) {

      String displayHours;
      String displayMinutes;
      String displaySeconds;
      
      int hours = time_pointer->hours;
      if (mode == ENTER && displayTime && timeElement == HOUR) {
        hours = getCurrentEntryAsInt(2);
      } 
      getTwoDigitsDisplayString(displayHours, hours , mode == ENTER && timeElement == HOUR && blinkState);
      
      int minutes = time_pointer->minutes;
      if (mode == ENTER && displayTime && timeElement == MINUTE) {
        minutes = getCurrentEntryAsInt(2);
      }   
      getTwoDigitsDisplayString(displayMinutes, minutes, mode == ENTER && timeElement == MINUTE && blinkState);
      
      int seconds = time_pointer->seconds;
      if (mode == ENTER && displayTime && timeElement == SECOND) {
        seconds = getCurrentEntryAsInt(2);
      } 
      getTwoDigitsDisplayString(displaySeconds, seconds, mode == ENTER && timeElement == SECOND && blinkState);
      
      displayString += displayHours + ":" + displayMinutes + ":" + displaySeconds;
    } 
    else 
    {
      String displayYear;
      String displayMonth;
      String displayDay;
      
      int year = time_pointer->year;
      if (mode == ENTER && !displayTime && dateElement == YEAR) {
        year = getCurrentEntryAsInt(4);
      } 
      getFourDigitsDisplayString(displayYear, year , mode == ENTER && dateElement == HOUR && blinkState);
      
      int month = time_pointer->month;
      if (mode == ENTER && !displayTime && dateElement == MONTH) {
        month = getCurrentEntryAsInt(2);
      }   
      getTwoDigitsDisplayString(displayMonth, month, mode == ENTER && dateElement == MONTH && blinkState);
      
      int days = time_pointer->dayofmonth;
      if (mode == ENTER && !displayTime && dateElement == DAY) {
        days = getCurrentEntryAsInt(2);
      } 
      getTwoDigitsDisplayString(displayDay, days, mode == ENTER && dateElement == SECOND && blinkState);
      
      displayString += displayYear + "-" + displayMonth + "-" + displayDay;    
    }
    
    blinkState = !blinkState;
        
//  } else {
//    
//    
//    lcd.setCursor(0,1);
//    switch (myRTC.dayofweek)
//    {
//      case 1: 
//        lcd.print("PO");
//        break;
//      case 2:
//        lcd.print("WT");
//        break;
//      case 3: 
//        lcd.print("SR");
//        break;
//      case 4:
//        lcd.print("CZ");
//        break;
//      case 5: 
//        lcd.print("PI");
//        break;
//      case 6:
//        lcd.print("SO");
//        break;
//      case 7: 
//        lcd.print("NI");
//        break;
//      default:
//        lcd.print("??");
//        break;
//    }
//    lcd.print(myRTC.dayofweek);
//    lcd.setCursor(2,1);
//    lcd.print(" ");
//    
//    lcd.setCursor(3,1);
//    lcd.print(myRTC.year);
//    lcd.setCursor(7,1);
//    lcd.print("-");
//
//    lcd.setCursor(8,1);
//    lcd.print(myRTC.month);
//    lcd.setCursor(10,1);
//    lcd.print("-");
//
//    lcd.setCursor(11,1);
//    lcd.print(myRTC.dayofmonth);
//    
//  }
  
}
