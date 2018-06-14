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
 * Inductive Proximity sensor as Metal Detector
 * 
 * DS3231 as Real Time Clock (RTC) and also
 * acts as a Temperature Sensor.
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

//-------------------- SETTINGS ----------------------
#define maxH 15           // working distance. Initially it was 50cm and
                          // 
#define cap_time 4        // time while cap is open, 4 seconds
#define open_angle 10     // open angle
#define close_angle 155   // close angle

#define debug 0           // debug information (0 off, 1 on)
//-------------------- SETTINGS ----------------------

// --- WIRING ---
/*
#define trigPin 3
#define echoPin 2
#define sensorVCC 4
#define MOSFETpin 6
#define servoPin 5
*/

//Ultrasonic Sensor connections
#define echoPin A2
#define trigPin A3
#define sensorVCC A4
#define servoPin A5
#define MOSFETpin A6
// --- WIRING ---


// include the library code:
#include <LiquidCrystal.h> // for LCD module

//#include "LowPower.h" // sleep library
#include <Servo.h>    // servo library,for motion detector module

#include <DS3231.h> // for RTC module

DS3231  rtc(SDA, SCL);
int tempC;
int tempF;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Below are the Metal detector vars
float metal_detected;
int monitoring;
int monitorPin = 1;
int var = 10;
int piezoPin = 8; //for buzzer

Servo servo;

long duration;
byte distance;
unsigned long open_timer;
boolean open_flag;
boolean test_flag = true;

void setup() {

 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
 // lcd.print("Please Recycle!");
  
  Serial.begin(9600); //serial.begin is used for both modules
  if (debug) Serial.println("system start");
  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, 0);
  pinMode(MOSFETpin, OUTPUT);
  digitalWrite(MOSFETpin, 0);
  pinMode(echoPin, INPUT);

  pinMode(sensorVCC, OUTPUT);
  digitalWrite(sensorVCC, 0);     // power off the sensor
 
//  open_cap();                     // open cap with system start


  // Initialize the rtc object
  rtc.begin();
  
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(SUNDAY);     // Set Day-of-Week to SUNDAY
 // rtc.setTime(4, 49, 0);     // Set the time to 12:00:00 (24hr format)
 // rtc.setDate(15, 4, 2018);   // Set the date to April 14TH, 2018 // 14, 4, 2018
  
  
}

void open_cap() {
  if (debug) Serial.println("open");
  digitalWrite(MOSFETpin, 1);     // power up the servo - A6
  delay(1);
  servo.write(open_angle);        // open cap
  delay(1000);                    // wait servo
  digitalWrite(MOSFETpin, 0);     // disable servo - A6
  open_flag = true;                  // remember that cap is open
    Serial.println("Cap Opened");
}
void close_cap() {
  if (debug) Serial.println("close");
  digitalWrite(MOSFETpin, 1);     // power up the servo - A6
  delay(1);
  servo.write(close_angle);       // close cap
  delay(1000);                    // wait servo
  digitalWrite(MOSFETpin, 0);     // disable servo - A6
  open_flag = false;                  // remember that cap is closed
  Serial.println("Cap Closed");

  
}

void loop() {

  displayTime();

  // Below IF is for Metal Detector and
  //ELSE is for Motion Detector
  monitoring= analogRead(monitorPin);
  delay(1000);
  lcd.print("");
    
  if(monitoring < 250)
  {
  
  
      unsigned long StartTime = millis();
  
       // set the cursor to column 0, line 1
       lcd.setCursor(0, 0);
       lcd.print("                      ");
       lcd.setCursor(0, 1);
       lcd.print("                      ");
       lcd.setCursor(0, 0);
       lcd.print("Please Recycle!");
       Serial.println("metal detected.");
       tone(piezoPin, 4000, 2500); // Pin Number, Frequency in Hz, duration in milli seconds
       delay(1000);
       delay(1000);
       delay(1000);
       delay(1000);
       //lcd.setCursor(0, 1);
       //lcd.print("                       ");
       displayTime();
  
  } 
  else 
  {
  
      // The blelow logic is for Motion Detector
      
      // LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);  // sleep 1 second
      Serial.println("system in loop.....distance: " + distance);
      delay(1000);//Instead of LowPower.powerDown(...) we used delay(1000) as LowPower.powerDown(...) function is giving error because of LowPower.h library
      
        digitalWrite(sensorVCC, 1);           // power up range sensor
        measure();                            // first "idle" measure, there are some glitches without it
        distance = measure();                 // get distance
        digitalWrite(sensorVCC, 0);           // disable range sensor
        if (debug) {
          Serial.println(distance);
          delay(500);
        }
        
        //Serial.println("***********************");
        //Serial.println("distance ---- "+ distance);
        //Serial.println("maxH ---- "+ maxH);
        //Serial.println(distance > 5 && distance < maxH);
        //Serial.println("**********  test_flag *************" + test_flag);
        Serial.println("&&&&&&& open_flag value is &&&&&&&&&&&");
        Serial.println(open_flag);
        Serial.println("&&&&&&&$$$$$$$$$$$$$$$$&&&&&&&&&&&");
       
        if (distance >= 5 && distance <= maxH) { // if distance fit the range of 5cm to 15cm
        //  if (distance > 5) {
          Serial.println("testing.... IF...");
          open_timer = 0;                     // reset timer
          if (!open_flag) {                   // if cap is closed
            Serial.println("testing....inside IF -- !open_flag ...");
            tone(piezoPin, 7000, 2500); // Pin Number, Frequency in Hz, duration in milli seconds
            open_cap();
            //delay(1000);
            //delay(1000);
            //delay(1000);
            //delay(1000);
          }
        } else {
          Serial.println("testing....ELSE...");
          if (open_flag) {                    // if cap is open
            Serial.println("testing....inside ELSE -- open_flag ...");
            open_timer++;                     // timer increments by 1 sec each time (open_timer = open_timer + 1)
            if (open_timer > cap_time) {      // if passed more than cap_time after cap was opened
              close_cap();
            }
          }
        }
    
  }
  
  /*Serial.println("monitoring value = " + monitoring);
  Serial.println("testing....");*/
  Serial.println(var);
  var = var + 5; 
  delay(1000);


} //end of loop

// distance measuring custom function using pulseIn and below formula
// (duration / 2) / 29.1 ---> OR (duration / 2) * 0.0344;
byte measure() {
  digitalWrite(trigPin, 0);
  delayMicroseconds(5);
  digitalWrite(trigPin, 1);
  delayMicroseconds(15);
  digitalWrite(trigPin, 0);
  duration = pulseIn(echoPin, HIGH);
  return (duration / 2) / 29.1;
}


//displays time on the LCD
byte displayTime(){
  
  lcd.setCursor(0, 0);
  lcd.print("                      ");
  lcd.setCursor(0, 1);
  lcd.print("                      ");
  lcd.setCursor(0, 0);

  lcd.print(rtc.getDOWStr());
  lcd.setCursor(0, 1);
  lcd.print(rtc.getDateStr());
  lcd.print(" ");
  lcd.print(rtc.getTimeStr());

  
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());

  //to display temperature
  displayTemp(); 
  
  // Wait one second before repeating :)
  delay (1000);
  
}

//displays the temperature inside the trashcan
 byte displayTemp(){ 

  tempC = rtc.getTemp();
  tempF = (tempC * 1.8) + 32.0; // Convert C to F
  lcd.setCursor(12, 0);
  
  lcd.print(tempF);
  lcd.print("F");
  
}


