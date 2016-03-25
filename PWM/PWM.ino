/* ===============================================================
      Project: 4 Channel 5V Relay Module
       Author: Scott C
      Created: 7th Sept 2014
  Arduino IDE: 1.0.5
      Website: http://arduinobasics.blogspot.com.au
  Description: Explore the difference between NC and NO terminals.
  ================================================================== */

/*
  Connect 5V on Arduino to VCC on Relay Module
  Connect GND on Arduino to GND on Relay Module
  Connect GND on Arduino to the Common Terminal (middle terminal) on Relay Module. */
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int backLight = 13;

#define CH9 22   // Connect Digital Pin 9 on Relay Module 22 on Arduino
#define CH7 26   // Connect Digital Pin 7 on Relay Module 26 on Arduino
#define CH15 24  // Connect Digital Pin 15 on Relay Module 24 on Arduino
#define CH3 28  // Connect Digital Pin 3 on Relay Module 28 on Arduino
#define DownShiftSignal 8
#define UpShiftSignal 7

#define park -2;
#define neutral -1;
#define reverse 0;
#define first 1;
#define second 2;
#define third 3;
#define fourth 4;
#define fifth 5;
#define sixth 6;

#define parkString "P";
#define neutralString "N";
#define reverseString "R";
#define firstString "1";
#define secondString "2";
#define thirdString "3";
#define fourthString "4";
#define fifthString "5";
#define sixthString "6";

String gearString = "P";

int CurrentGear = park;
int downShift = LOW;
int upShift = LOW;

byte PWM_PIN1 = 6;

void setupPins() {
  //Setup Digital Pins
  pinMode(CH9, OUTPUT);
  pinMode(CH7, OUTPUT);
  pinMode(CH15, OUTPUT);
  pinMode(CH3, OUTPUT);
  pinMode(DownShiftSignal, INPUT);
  pinMode(UpShiftSignal, INPUT);

  //PWM pins
  pinMode(PWM_PIN1, OUTPUT);
}

void setupLCD() {
  //Setup LCD
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH);
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.setCursor(4, 0);
  lcd.print("Trans Status");
  lcd.setCursor(0, 1);
  lcd.print("Current Gear:");
  lcd.setCursor(0, 2);
  lcd.print("Lockup clutch:");
}

void setup() {

  setupPins();
  setupLCD();

  //Turn OFF any power to the Relay channels
  digitalWrite(CH9, LOW);
  digitalWrite(CH7, LOW);
  digitalWrite(CH15, LOW);
  digitalWrite(CH3, LOW);

  delay(2000); //Wait 2 seconds before starting sequence
  Serial.begin(9600);
}

void deEnergize(int pin) {
  int delayTime = 10; // This will change the frequency
  float percentage = 1.00;
  while (percentage > 0) {
    digitalWrite(pin, HIGH);
    int onTime = delayTime * percentage;
    delay(onTime);
    int rem = delayTime - onTime;
    digitalWrite(pin, LOW);
    delay(rem);
    percentage -= .005; // This will change the amount of time it takes to fully deenergize
  }
}

void energize(int pin) {
  int delayTime = 10; // This will change the frequency
  float percentage = 0.00;
  while (percentage < 100.00) {
    digitalWrite(pin, HIGH);
    int onTime = delayTime * percentage;
    delay(onTime);
    int rem = delayTime - onTime;
    digitalWrite(pin, LOW);
    delay(rem);
    percentage += .005; // This will change the amount of time it takes to fully deenergize
  }
}

void downShiftTrans() {
  switch (CurrentGear) {
    case -2:
      setGearPark();
      break;
    case -1:
      setGearNeutral();
      break;
    case 0:
      setGearReverse();
      break;

    case 1:
      setGearFirst();
      break;
    case 2:
      setGearSecond();
      break;
    case 3:
      setGearThird();
      break;
    case 4:
      setGearFourth();
      break;
    case 5:
      setGearFifth();
      break;
    case 6:
      setGearSixth();
      break;
  }
}

void upShiftTrans() {
  switch (CurrentGear) {
    case -2:
      setGearPark();
      break;
    case -1:
      setGearNeutral();
      break;
    case 0:
      setGearReverse();
      break;

    case 1:
      setGearFirst();
      break;
    case 2:
      setGearSecond();
      break;
    case 3:
      setGearThird();
      break;
    case 4:
      setGearFourth();
      break;
    case 5:
      setGearFifth();
      break;
    case 6:
      setGearSixth();
      break;
  }
}

void setGearPark() { //TODO: NEEDS CHANGED TO REAL VALUES
  digitalWrite(CH3, HIGH);  //SS1 -- HIGH is OFF
  digitalWrite(CH7, LOW); //SS2 -- LOW is ON
  digitalWrite(CH9, HIGH);  //SS3
  digitalWrite(CH15, HIGH); //TCC
  gearString = parkString;
}

void setGearNeutral() { //TODO: NEEDS CHANGED TO REAL VALUES
  digitalWrite(CH3, HIGH);  //SS1 -- HIGH is OFF
  digitalWrite(CH7, LOW); //SS2 -- LOW is ON
  digitalWrite(CH9, HIGH);  //SS3
  digitalWrite(CH15, HIGH); //TCC
  gearString = neutralString;
}

void setGearReverse() { //TODO: NEEDS CHANGED TO REAL VALUES
  digitalWrite(CH3, HIGH);  //SS1 -- HIGH is OFF
  digitalWrite(CH7, LOW); //SS2 -- LOW is ON
  digitalWrite(CH9, HIGH);  //SS3
  digitalWrite(CH15, HIGH); //TCC
  gearString = reverseString;
}

void setGearFirst() {
  digitalWrite(CH3, HIGH);  //SS1 -- HIGH is OFF
  digitalWrite(CH7, LOW); //SS2 -- LOW is ON
  digitalWrite(CH9, HIGH);  //SS3
  digitalWrite(CH15, HIGH); //TCC
  gearString = firstString;
}

void setGearSecond() {
  digitalWrite(CH3, HIGH);  //SS1
  digitalWrite(CH7, HIGH); //SS2
  digitalWrite(CH9, HIGH);  //SS3
  digitalWrite(CH15, HIGH); //TCC
  gearString = secondString;
}

void setGearThird() {
  digitalWrite(CH3, LOW);  //SS1
  digitalWrite(CH7, HIGH); //SS2
  digitalWrite(CH9, HIGH);  //SS3
  digitalWrite(CH15, HIGH); //TCC
  gearString = thirdString;
}

void setGearFourth() {
  digitalWrite(CH3, LOW);  //SS1
  digitalWrite(CH7, HIGH); //SS2
  digitalWrite(CH9, LOW);  //SS3
  digitalWrite(CH15, LOW); //TCC
  gearString = fourthString;
}

void setGearFifth() {
  digitalWrite(CH3, HIGH);  //SS1
  digitalWrite(CH7, HIGH); //SS2
  digitalWrite(CH9, LOW);  //SS3
  digitalWrite(CH15, LOW); //TCC
  gearString = fifthString;
}

void setGearSixth() {
  digitalWrite(CH3, HIGH);  //SS1
  digitalWrite(CH7, HIGH); //SS2
  digitalWrite(CH9, LOW);  //SS3
  digitalWrite(CH15, LOW); //TCC
  gearString = sixthString;
}

void loop() {

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(13, 1);

  downShift = digitalRead(DownShiftSignal);
  upShift = digitalRead(UpShiftSignal);

  if (downShift == 1) {
    if (CurrentGear > -2) {
      CurrentGear--;
      downShiftTrans();
    }

  }

  if (upShift == 1) {
    if (CurrentGear < 6) {
      CurrentGear++;
      upShiftTrans();
    }
  }
  lcd.print(gearString);
  Serial.print("Current Gear: ");
  Serial.println(gearString);
  delay(200);

}
