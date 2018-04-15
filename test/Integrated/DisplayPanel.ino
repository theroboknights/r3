/*
 
 This program identifies metal with the help of
 Inductive Proximity Sensor and send signals to 
 microcontroller and notifies Motion sensor
 (Ultrasonic sensor) and a message "Please Recycle!" 
 on LCD display and finally trash bin lid will be
 opened for 4 seconds and then closes.
 
 We used Ananlog pins as Digital pins as some of 
 the digital pins that we used for LCD are needed for
 Ultrasonic sensor also, so we used Analog pins along with
 pinMode(.,.) as Digital pins, to avoid same pin connecting 
 to different components.

  The circuit: 

 * Trigger pin as Analog pin A3
 * Ultrasonic sensor power pin to Analog pin A4
 * echo pin to Analog pin A2
 * Servo pin to Analog pin A5
 * 
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K and 100 ohm resistor:
 * ending to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * 
 * Passive Buzzer for Digital Pin# 8
 *    with 100 ohms resistor to 
 *    lessen the power to the buzzer
 *    
 *    

 Created on February 2018
   by Venya Karri
      Srinidhi Akella
      Harini Devireddy
      Pragna Yalamanchili
      
*/


// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
