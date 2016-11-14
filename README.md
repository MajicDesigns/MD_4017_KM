This is a library to manage the keyswitch matrix implemented using a 4017 as described in the articles at 
* [EDN Design #1](http://www.edn.com/design/power-management/4413677/Connect-a-4-3-matrix-keyboard-to-a-microcontroller-using-two-I-O-pins)
* [EDN Design #2](http://www.edn.com/design/components-and-packaging/4408027/Read-10-or-more-switches-using-only-two-I-O-pins-of-a-microcontroller)
  
Using a 4017 IC, this library implements a method for reading many open/closed switches using only three digital I/O pins (Clock, Reset and DataIn}).

The library
* Allows definition of the size of the keyswitch matrix
* Manages the scanning of the 4017 to read the matrix
* Detects the transition from OFF state to ON state for each key
* Implements software enabled debounce and software auto repeat

Eagle CAD files for a 3x3 version of the matrix are included, as well as the articles referenced in the URL above.