/*
  MD_4017_KM.h - Library for CD4017 driven key switch matrix.
  
  Created by Marco Colli April 2014
	
  Small library to manage the keyswitch matrix managed by a 4017 as 
  described in the articles at 
  http://www.edn.com/design/power-management/4413677/Connect-a-4-3-matrix-keyboard-to-a-microcontroller-using-two-I-O-pins
  http://www.edn.com/design/components-and-packaging/4408027/Read-10-or-more-switches-using-only-two-I-O-pins-of-a-microcontroller
  
  The library
  - Allows definition of the size of the keyswitch matrix
  - Manages the scanning of the 4017 to read the matrix
  - Detects the transition from OFF state to ON state for each key
  - implements software enabled debounce and software auto repeat
  
  Using a 4017 IC, this library implements a method for reading many
  open/closed switches using only three digital I/O pins (Clock, Reset 
  and Data_in). 
  
  Diodes isolate the 4017 counter outputs in the event that two or more 
  switches are closed at the same time. The number of switches connected 
  can be increased by cascading multiple CD4017 or by using a matrix style 
  arrangement. Reliable operation following the initial power-up reset 
  depends on the CD4017 counter’s remaining synchronized with the MCU 
  counter. The Reset pin I/O pin is needed for reliable operation but can 
  be omitted if desired.
  
  Refer to the articles above for a more detailed explanation of the circuit 
  to be implemented.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
 */
#ifndef	MD_4017_KM_H
#define	MD_4017_KM_H

#include <Arduino.h>

// Default values
#define	KEY_DEBOUNCE_TIME	20	// in milliseconds
#define	KEY_REPEAT_TIME		500	// in milliseconds
#define KEY_NONE  -1            // no key detected

class MD_4017_KM
{
public:
  MD_4017_KM(uint8_t numKeys, uint8_t pinClk, uint8_t pinKey, uint8_t pinRst);
  ~MD_4017_KM(void);
		
  void setDebounceTime(uint16_t t) { _timeDebounce = t; };
  void setRepeatTime(uint16_t t)   { _timeRepeat = t; };	

  void    begin(void);
  int16_t read(int16_t startKey); // return -1 if no key
	
protected:
  // Hardware setup and definitions
  uint8_t  _numKeys; // the total number of keys
  uint8_t  _pinClk;  // the 4017 clock pin, LOW to HIGH transition
  uint8_t  _pinRst;  // the 4017 reset pin (0 if not used), LOW to HIGH transition
  uint8_t  _pinKey;  // the key switch output to Arduino, HIGH means key is pressed	

  // Key parameters
  uint16_t _timeDebounce;  // debounce time in milliseconds, default KEY_DEBOUNCE_TIME
  uint16_t _timeRepeat;    // repeat time delay in milliseconds, default KEY_REPEAT_TIME

  // Key detection status and tracking
  int16_t  _lastKey;    // the last key detected
  uint32_t _timeActive; // the millis() time it was last activated
  uint8_t  _state;      // the last state (FSM)
  
  // Internal methods
  void    reset(void);
  void    clock(void);
  int16_t process(int16_t k);
};

#endif