// Here I used some “sketch” from the author mentioned below.
// Basically, this code is divided in a part that sets the LCD screen to display characters and a second part that 
// teaches Morse Code to Arduino.
// Unfortunately, when I did this, I didn’t write truly clear comments…
// Omir.



/* About: This is a simple Arduino-compatible "sketch" that is meant to do to things:
 *        1) Convert ASCII charts from the serial port to Morse Code by flashing an
 *           LED (connected to digital pin 13)
 *        2) Interpret presses on button (connected to digital pin 7) as Morse Code
 *           inputs and send as ASCII chars to serial port
 * Author: Aleksandar Gargenta, Marakana
 * License: Apache License, Version 2.0
 * Disclaimer: This is my very first Arduino sketch, so expect bugs :-)
 */
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // create an lcd object and assign the pins
#define DOT_DURATION   200               // Duration of a Morse Code "dot" (in milliseconds)
#define DASH_DURATION  DOT_DURATION * 3  // Duration of a Morse Code "dash" (in milliseconds)
#define SIGNAL_GAP     DOT_DURATION * 4  // Gap between dots/dashes of a single letter (in ms)
#define LETTER_GAP     DOT_DURATION * 20  // Gap between two letters (in milliseconds)
#define WORD_GAP       DOT_DURATION * 30  // Gap between two words (in milliseconds)



#define BUTTON          7                // The diginal connector port to the button

#define DOT             1                // DOT identifier
#define DASH            2                // DASH identifier
#define NONE            0                // Neither DOT nor DASH

boolean buttonWasPressed = false;        // Indicator of whether button was pressed in the last cycle
long lastTimestamp = 0;                  // Last recorded timestamp  (used for mesuring duration)
byte inputSignal[6];                     // Input signal buffer
byte CommandSignal[6];
int inputSignalIndex = 0;                // Index into the input signal buffer
int countletter;

void resetInputSignal() {                // Reset the input signal buffer and index
  inputSignal[0] = NONE;
  inputSignal[1] = NONE; 
  inputSignal[2] = NONE;
  inputSignal[3] = NONE;
  inputSignal[4] = NONE;
  inputSignalIndex = 0;
}

void setup() {
  lcd.begin(16, 2); // Set the display to 16 columns and 2 rows
  pinMode(BUTTON, INPUT);                // Set the button input
  Serial.begin(9600);                    // Init the serial port
  resetInputSignal();                    // Reset input signal buffer
  lcd.print("MORSE TO LCD                            by Omir Antunes"); // print some text
  delay(2000);
  lcd.clear(); // Clear the display
  lcd.cursor(); // enables the cursor
  lcd.setCursor(0,0);
}

// return true if s0-s4 match input signal
boolean matchInputSignal(byte s0, byte s1, byte s2, byte s3, byte s4) {
  return ((inputSignal[0] == s0) && 
          (inputSignal[1] == s1) && 
          (inputSignal[2] == s2) && 
          (inputSignal[3] == s3) &&  
          (inputSignal[4] == s4));
}

// convert input signal to letter or ? if not found
char currentInputSignalToLetter() {  
  if (matchInputSignal(DOT, DASH, NONE, NONE, NONE))  { return 'A'; }
  if (matchInputSignal(DASH, DOT, DOT, DOT, NONE))    { return 'B'; }
  if (matchInputSignal(DASH, DOT, DASH, DOT, NONE))   { return 'C'; }
  if (matchInputSignal(DASH, DOT, DOT, NONE, NONE))   { return 'D'; }
  if (matchInputSignal(DOT, NONE, NONE, NONE, NONE))  { return 'E'; }
  if (matchInputSignal(DOT, DOT, DASH, DOT, NONE))    { return 'F'; }
  if (matchInputSignal(DASH, DASH, DOT, NONE, NONE))  { return 'G'; }
  if (matchInputSignal(DOT, DOT, DOT, DOT, NONE))     { return 'H'; }
  if (matchInputSignal(DOT, DOT, NONE, NONE, NONE))   { return 'I'; }
  if (matchInputSignal(DOT, DASH, DASH, DASH, NONE))  { return 'I'; }
  if (matchInputSignal(DASH, DOT, DASH, NONE, NONE))  { return 'K'; }
  if (matchInputSignal(DOT, DASH, DOT, DOT, NONE))    { return 'L'; }
  if (matchInputSignal(DASH, DASH, NONE, NONE, NONE)) { return 'M'; }
  if (matchInputSignal(DASH, DOT, NONE, NONE, NONE))  { return 'N'; }
  if (matchInputSignal(DASH, DASH, DASH, NONE, NONE)) { return 'O'; }
  if (matchInputSignal(DOT, DASH, DASH, DOT, NONE))   { return 'P'; }
  if (matchInputSignal(DASH, DASH, DOT, DASH, NONE))  { return 'Q'; }
  if (matchInputSignal(DOT, DASH, DOT, NONE, NONE))   { return 'R'; }
  if (matchInputSignal(DOT, DOT, DOT, NONE, NONE))    { return 'S'; }
  if (matchInputSignal(DASH, NONE, NONE, NONE, NONE)) { return 'T'; }
  if (matchInputSignal(DOT, DOT, DASH, NONE, NONE))   { return 'U'; }
  if (matchInputSignal(DOT, DOT, DOT, DASH, NONE))    { return 'V'; }
  if (matchInputSignal(DOT, DASH, DASH, NONE, NONE))  { return 'W'; }
  if (matchInputSignal(DASH, DOT, DOT, DASH, NONE))   { return 'X'; }
  if (matchInputSignal(DASH, DOT, DASH, DASH, NONE))  { return 'Y'; }
  if (matchInputSignal(DASH, DASH, DOT, DOT, NONE))   { return 'Z'; }
  if (matchInputSignal(DOT, DASH, DASH, DASH, DASH))  { return '1'; }
  if (matchInputSignal(DOT, DOT, DASH, DASH, DASH))   { return '2'; }
  if (matchInputSignal(DOT, DOT, DOT, DASH, DASH))    { return '3'; }
  if (matchInputSignal(DOT, DOT, DOT, DOT, DASH))     { return '4'; }
  if (matchInputSignal(DOT, DOT, DOT, DOT, DOT))      { return '5'; }
  if (matchInputSignal(DASH, DOT, DOT, DOT, DOT))     { return '6'; }
  if (matchInputSignal(DASH, DASH, DOT, DOT, DOT))    { return '7'; }
  if (matchInputSignal(DASH, DASH, DASH, DOT, DOT))   { return '8'; }
  if (matchInputSignal(DASH, DASH, DASH, DASH, DOT))  { return '9'; }
  if (matchInputSignal(DASH, DASH, DASH, DASH, DASH)) { return '0'; }
  if (matchInputSignal(DOT, DASH, DOT, DASH, DOT))    { backspace(); return ' ';}
  if (matchInputSignal(DASH, DOT, DOT, DASH, DOT))    { lcd.clear(); }
  return '?';
}


void backspace(){
  if(countletter <16){countletter--;lcd.setCursor(countletter,0);};
  if(countletter >15){countletter--;lcd.setCursor(countletter,1);};
}

void loop() { // loop forever
  long currentTimestamp  = millis(); // get the current timestamp
  long duration = currentTimestamp - lastTimestamp; // get elapsed time
  if(countletter== 16){lcd.setCursor(0,1);}
  if(countletter== 32){lcd.setCursor(0,0);lcd.clear(); countletter = 0;}
  if (digitalRead(BUTTON) == HIGH) { // if the button is pressed
    delay(50);
    if (!buttonWasPressed) { //  if the button was previously not pressed
      buttonWasPressed = true; // remember the button press
      lastTimestamp = currentTimestamp; // record the time of the button press
      if (duration > LETTER_GAP) {
        Serial.print(' ');
        lcd.print(' ');
        countletter++;
      }
    } // end of if (button was not pressed)
  } else { // the button is not pressed
    if (buttonWasPressed) {  // the button was just released
      if (duration < DOT_DURATION) { // if the button was pressed for up to DOT cutoff
        inputSignal[inputSignalIndex] = DOT; // remember the current signal as a DOT
      } else { // if the button was pressed for longer than DOT cutoff
        inputSignal[inputSignalIndex] = DASH; // remember the current signal as a DASH
      }
      inputSignalIndex++; // advance the index to the input signal buffer
      buttonWasPressed = false; // consume previous button press
      lastTimestamp = currentTimestamp; // record the time the button was released
    } else { // the button was not just released
      if (inputSignalIndex > 0) { // if there is data in the input signal buffer
        if (duration > SIGNAL_GAP || inputSignalIndex >= 5) { // if we have a complete letter
          lcd.print(currentInputSignalToLetter());
          Serial.print(currentInputSignalToLetter()); // parse the letter and send it via serial
          countletter++;
          resetInputSignal(); // reset the input signal buffer
        }
      }
    } // end of else (button was not previously pressed)
  } // end of else (button is not pressed)
} // end of loop
