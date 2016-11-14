/*
  MD_4017_KM.h - Library for CD4017 driven key switch matrix.

  See main header file for information.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
 */

#include "MD_4017_KM.h"

#define DEBUG	0

#if  DEBUG
#define	PRINT(s, v)		{ Serial.print(F(s)); Serial.print(v); }		// Print a string followed by a value (decimal)
#define	PRINTX(s, v)	{ Serial.print(F(s)); Serial.print(v, HEX); }	// Print a string followed by a value (hex)
#define	PRINTS(s)		Serial.print(F(s))								// Print a string
#else
#define	PRINT(s, v)		// Print a string followed by a value (decimal)
#define	PRINTX(s, v)	// Print a string followed by a value (hex)
#define	PRINTS(s)		// Print a string
#endif


MD_4017_KM::MD_4017_KM(uint8_t numKeys, uint8_t pinClk, uint8_t pinKey, uint8_t pinRst) : 
_numKeys(numKeys), _pinClk(pinClk), _pinKey(pinKey), _pinRst(pinRst), _state(0)
{
  setDebounceTime(KEY_DEBOUNCE_TIME);
  setRepeatTime(KEY_REPEAT_TIME);
}

MD_4017_KM::~MD_4017_KM(void) 
{ 
};

void MD_4017_KM::begin(void)
{
  PRINTS("\nbegin");
  digitalWrite(_pinClk, LOW);
  pinMode(_pinClk, OUTPUT);
  pinMode(_pinKey, INPUT);
  if (_pinRst != 0)
  {
    pinMode(_pinRst, OUTPUT);
    digitalWrite(_pinRst, LOW);
  }
}

void MD_4017_KM::reset(void)
{
  PRINTS("\nreset");
  digitalWrite(_pinRst, HIGH);
  delayMicroseconds(1);
  digitalWrite(_pinRst, LOW);
}

void MD_4017_KM::clock(void)
{
  PRINTS(" clock; ");
  digitalWrite(_pinClk, HIGH);
  // delayMicroseconds(1); // may not be needed!
  digitalWrite(_pinClk, LOW);
}

int16_t MD_4017_KM::process(int16_t k)
// Each key is processed for debounce or auto repeat but this will only work if
// more than one key in the matrix is active.
{
  PRINTS(" process ");
  
  // Restart the FSM if we have a different key from last time invoked.
  if (k != _lastKey) 
    _state = 0;
    
  // Now process the key pressed
  switch (_state)
  {
    case 0:		// first transition detection
	  PRINTS(" S0;");
      _state = 1;		// edge detected, initiate debounce
      _timeActive = millis();
      _lastKey = k;
      break;
		
    case 1:		// debounce time
      PRINTS(" S1;");
      k = KEY_NONE;
      if ((millis() - _timeActive) < _timeDebounce)
        break;
		
      // after debounce - possible auto repeat
      _state = 2;
      _timeActive = millis();
      break;
		
    case 2:		// auto repeat
      PRINTS(" S2;");
      if ((millis() - _timeActive) < _timeRepeat)
      {
        k = KEY_NONE;		// no repeat yet
      }
      else
      {
        _timeActive = millis();	// next key repeat starts now
      }
      break;
		
    default:
      _state = 0;
      break;
  }

	return(k);
}

int16_t MD_4017_KM::read(int16_t startKey = 0)
// Return the id [0.._numKeys-1] for the key currently pressed, or KEY_NONE.
// If startKey is specified, then the 4017 is not reset and the checking starts 
// from that key number (ie the 4017 is not reset and it assumes that the last 
// invocation left off at that key) - this allows multiple keys to be detected 
// each pass through the key matrix.
{
  int16_t key;
  bool    b;
  
  if (startKey <= 0)
  {
    reset();
    startKey = 0;
  }
   
  for (int16_t i=startKey; i<_numKeys; i++)
  {
	PRINT("\nKey ", i)
#if DEBUG
	delay(200);
#endif

	// read and advance the counter	
	b = (digitalRead(_pinKey) == HIGH);
    clock();    // advance the 4017 counter

	// now process the input
	if (b)
    {
      key = process(i);
      if (key != KEY_NONE)
	  {
        PRINT(" return ", key);
        return(key);
	  }
    } 
  }
  
  return(KEY_NONE);
}
