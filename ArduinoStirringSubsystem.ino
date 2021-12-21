/*

ArduinoStirringSubsystem.ino

Copyright 14 Dec 2020 - Iason Chaimalas

*/

// Initialise libraries
#include <stdio.h>
#include <string.h>
#include <LiquidCrystal.h> // optional library for LCD display of data

// LCD display for the data (setpoint RPM and current system RPM)
// -- for Stirring Subsystem testing and as a physical display
// -- in the project we integrated the Stirring Subsystem with a Python GUI
LiquidCrystal lcd(12, 11, 7, 8, 9, 10); // set up LCD conn to Arduino
	// declare pins - 12 & 11 comm, 7-10 disp. 4-bit inputs

static const int tPin = 5; // pin5 = PWM and stirrer in SimAVR simulator
static const int photointerrupter = 2; // Pin2 on SimAVR
int pwm;
int SetpointRPM = 1000; // manually set RPM here (auto in SimAVR)
	// -- this is the setpoint that the PID drives the motor TrueRPM to
	// -- in full bioreactor GUI this is set by user in real time
double TrueRPM;

unsigned long currentTimePhoto; // timing to get TrueRPM
unsigned long previousTimePhoto = 0; // subtract from currentTimePhoto
// to get time difference since last collection of data over 1000ms
volatile unsigned long ticker = 0, // ticks +1 for every Rising pin2
volatile unsigned long tickNum; // ticks in (currentTimePhoto - previousTimePhoto) time
volatile unsigned long prevTicker = 0; // prev tickNum at last collection

// PID
unsigned long currentTimePID;
unsigned long prevTimePID = 0;
int elapsedPID; // used to be unsigned long but error - Arduino read first bit as sign
double error = 0;
double prevError = 0;
double errorIntegral;
double errorDeriv;

void setup() {
  pinMode(tPin, OUTPUT); // configure pin as PWM output (8bit approx of analog out)
  pinMode(photointerrupter, INPUT);
	// pin receives digital input (high/low voltage) from circuit photointerrupter
	// photointerrupter in physical circuit switches signal between 0 & 1 twice per 1 rotation
	
  Serial.begin(9600); // begin comm. between Arduino and Serial at 9.6kbaud
  Serial.println("\nSystem ready for user input");
 
  attachInterrupt(digitalPinToInterrupt(photointerrupter), tickCount, RISING);
	// interrupts attached to photointerrupter input pin
	// whenever input pin goes LOW->HIGH the tickCount ISR is called
  
  lcd.begin(16, 2);
}

void loop() {
  // LCD:
  lcd.print("Setpoint: ");
  lcd.print(SetpointRPM); // print on LCD 16 segments of 1st row from (0,0) cursor
  lcd.setCursor(0,1); // put cursor on 1st segment of 2nd row
  lcd.print("Current: ");
  lcd.print(TrueRPM); // print on 2nd row
  lcd.setCursor(0,0); // reposition cursor back to (0,0) (will overwrite on next loop)

  currentTimePhoto = millis();

  unsigned long interval = currentTimePhoto - previousTimePhoto;

  if (interval >= 1000) {
    if (interval > 0) {
      TrueRPM = ticker * 30000.0 / interval;
      Serial.print("TrueRPM: "); // for debug
      Serial.print(TrueRPM); // debug
      previousTimePhoto = currentTimePhoto;
      ticker = 0;
    }
    else {
      TrueRPM = 0;
    }
  }

  // PID
  currentTimePID = millis();
  elapsedPID = (int)(currentTimePID - prevTimePID);
  Serial.println(elapsedPID);
  error = SetpointRPM - TrueRPM;

  /* // DEBUG:
  Serial.print("Error:");
  Serial.print(error);
  Serial.print("| elapsedPID:");
  Serial.print(elapsedPID);
  */
  errorIntegral = error * (elapsedPID / 100);
  //Serial.print("| Error Int:");
  //Serial.println(errorIntegral);
  errorDeriv = -(prevError - error)/elapsedPID;

  prevError = error;
  prevTimePID = currentTimePID;

  if (error > 10 || error < -10) {
    /* // DEBUG:
    Serial.print("Error:");
    Serial.print(error);
    Serial.print("| Error Integral:");
    Serial.print(errorIntegral);
    Serial.print("| Error Der:");
    Serial.print(errorDeriv);
    Serial.print("| Elapsed Time:");
    Serial.println(elapsedPID);
    */
	  
    pwm = (int)(0.008*error + 0.009*errorIntegral + 0.03*errorDeriv) + pwm;
  }

  if (pwm >= 255) {pwm = 255;} // saturate pwm output at high cutoff to avoid errors
  if (pwm <= 0) {pwm = 0;} // saturate pwm output at low cutoff to avoid errors

  analogWrite(tPin, pwm);
  //analogWrite(tPin, 50);
  
  Serial.print("Current RPM: ");
  Serial.print(TrueRPM);
  Serial.print(" | Setpoint RPM: ");
  Serial.print(SetpointRPM);
  Serial.print(" | PWM: ");
  Serial.println(pwm);
	// print data to Serial (GUI instead in integrated bioreactor)
  delay(200);
}

void tickCount() {
  ticker++;
}
