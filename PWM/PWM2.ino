/*===============================================================
      Project: Transmission Shifter
      Author: Scott Hadzik
      Created: Jan 2016
      Arduino IDE: 1.6.6
      Description: Controls Transmission
  ================================================================== */

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 7, 4, 3, 2);
int backLight = 13;

#define SS3 22   // Connect Digital Pin 9 on Relay Module 22 on Arduino
#define SS2 26   // Connect Digital Pin 7 on Relay Module 26 on Arduino
#define TTC 24  // Connect Digital Pin 15 on Relay Module 24 on Arduino
#define SS1 28  // Connect Digital Pin 3 on Relay Module 28 on Arduino
#define DownShiftSignal 48 //Purple
#define UpShiftSignal 50 //Green

#define park -2;
#define reverse -1;
#define neutral 0;
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
#define PCS1 5 //Orange
#define PCS2 6 //White
#define ON LOW
#define OFF HIGH
#define energized HIGH
#define deEnergized LOW

String gearString = "P";
String TCCStatus = "OFF";

int CurrentGear = park;
int downShift = LOW;
int upShift = LOW;

void setupPins() {
  //Setup Digital Pins
  pinMode(SS3, OUTPUT);
  pinMode(SS2, OUTPUT);
  pinMode(TTC, OUTPUT);
  pinMode(SS1, OUTPUT);
  pinMode(DownShiftSignal, INPUT);
  pinMode(UpShiftSignal, INPUT);

  //PWM pins
  //  pinMode(PWM_PIN1, OUTPUT);
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
  digitalWrite(SS3, LOW);
  digitalWrite(SS2, LOW);
  digitalWrite(TTC, LOW);
  digitalWrite(SS1, LOW);

  delay(2000); //Wait 2 seconds before starting sequence
  Serial.begin(9600);
  setGearPark();

}

void deEnergize(int pin) {
  int delayTime = 5; // This will change the frequency
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
  digitalWrite(pin, LOW);
}

void energize(int pin) {
  //  int delayTime = 5; // This will change the frequency
  //  float percentage = 0.00;
  //  while (percentage < 100.00) {
  //    digitalWrite(pin, HIGH);
  //    int onTime = delayTime * percentage;
  //    delay(onTime);
  //    int rem = delayTime - onTime;
  //    digitalWrite(pin, LOW);
  //    delay(rem);
  //    percentage += .005; // This will change the amount of time it takes to fully deenergize
  //  }
  //  digitalWrite(pin, HIGH);
}

void downShiftTrans() {
  switch (CurrentGear) {
    case -2:
      setGearPark();
      break;
    case -1:
      setGearReverse();
      break;
    case 0:
      setGearNeutral();
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
      setGearReverse();
      break;
    case 0:
      setGearNeutral();
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

/*                                        PARK

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State | N or P      |De-energized |De-energized | ON  | ON  | ON  | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/

void setGearPark() {
  gearString = parkString;
  TCCStatus = "OFF";
  digitalWrite(SS1, ON);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);
}

/*                                      Neutral to Reverse

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State | N - R       |De-energizing|  Energizing | ON  | ON  | ON  | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/

void neutralToReverse() {
  digitalWrite(SS1, ON);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, OFF);
  deEnergize(PCS1);
  energize(PCS2);
}

/*                                      Reverse to Neutral

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State | R - N       |De-energizing| Energizing  | ON  | ON  | ON  | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/

void reverseToNeutral() {
  digitalWrite(SS1, ON);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, OFF);
  deEnergize(PCS1);
  energize(PCS2);
}

/*                                      Reverse

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State |     R       | De-energized|   Energized | ON  | ON  | ON  | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/

void setGearReverse() {
  gearString = reverseString;
  TCCStatus = "OFF";
  digitalWrite(SS1, ON);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, energized); 
}

/*                                        Neutral

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State | N or P      |De-energized |De-energized | ON  | ON  | ON  | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/

void setGearNeutral() {
  gearString = neutralString;
  TCCStatus = "OFF";
  digitalWrite(SS1, ON);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);
}

/*                                    Neutral to First

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Upshift      | N - 1       |De-energized | Energized    | ON  | ON  | ON  | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/

void neutralToFirst() {
  digitalWrite(SS1, ON);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, energized);
}

/*                                    First to Neutral

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Down Shift   | 1 - N       |De-energized | Energized    | ON  | ON  | ON  | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/

void firstToNeutral() {
  digitalWrite(SS1, ON);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, energized);
}

/*                                            First

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State |       1     |De-energized | De-energized| OFF | ON  | OFF | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void setGearFirst() {
  gearString = firstString;
  TCCStatus = "OFF";
  digitalWrite(SS1, OFF);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, OFF);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);

}

/*                                    First to Second

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Upshift      | 1 - 2       | Energized   | Energized   | OFF | ON  | OFF | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void firstToSecond() {
  digitalWrite(SS1, OFF);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, OFF);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, energized);
  digitalWrite(PCS2, energized);
}
/*                                    Second to First

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Down Shift   | 2 - 1       | Energized   | Energized   | OFF | ON  | OFF | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void SecondToFirst() {
  digitalWrite(SS1, OFF);
  digitalWrite(SS2, ON);
  digitalWrite(SS3, OFF);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, energized);
  digitalWrite(PCS2, energized);

}

/*                                            Second

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State |       2     |   Energized |  Energized  | OFF | OFF | OFF | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/

void setGearSecond() {
  gearString = secondString;
  TCCStatus = "OFF";
  digitalWrite(SS1, OFF);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, OFF);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, energized);
  digitalWrite(PCS2, energized);
}

/*                                    Second to Third

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Upshift      | 2 - 3       |DE-Energized |DE-Energized | OFF | OFF | OFF | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void secondToThird() {
  digitalWrite(SS1, OFF);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, OFF);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);
}
/*                                    Third to Second

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Down Shift   | 3 - 2       |DE-Energized | DE-Energized| OFF | OFF | OFF | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void ThirdToSecond() {
  digitalWrite(SS1, OFF);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, OFF);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);
}
/*                                            Third

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State |       3     | DE-Energized| DE-Energized| ON  | OFF | OFF | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/

void setGearThird() {
  gearString = thirdString;
  TCCStatus = "OFF";
  digitalWrite(SS1, ON);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, OFF);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);
}
/*                                    Third to Fourth

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Upshift      | 3 - 4       |  Energized  |    Energized| ON  | OFF | OFF | ON  |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void thirdToFourth() {
  digitalWrite(SS1, ON);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, OFF);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, energized);
  digitalWrite(PCS2, energized);
}
/*                                    Fourth to Third

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Down Shift   | 4 - 3       | Energized   | Energized   | ON  | OFF | OFF | OFF |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void fourthToThird() {
  digitalWrite(SS1, ON);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, OFF);
  digitalWrite(TTC, OFF);
  digitalWrite(PCS1, energized);
  digitalWrite(PCS2, energized);
}

/*                                            Fourth

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State |       4     | Energized   |   Energized | ON  | OFF | ON  | ON  |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void setGearFourth() {
  gearString = fourthString;
  TCCStatus = "ON ";
  digitalWrite(SS1, ON);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, ON);
  digitalWrite(PCS1, energized);
  digitalWrite(PCS2, energized);
}

/*                                    Fourth to Fifth

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Upshift      | 4 - 5       |De-Energized | De-Energized| ON  | OFF | ON  | ON  |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void fourthToFifth() {
  digitalWrite(SS1, ON);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, ON);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);
}
/*                                    Fifth to Fourth

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Down Shift   | 4 - 3       | DE-Energized| DE-Energized| ON  | OFF | ON  | ON  |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void fifthToFourth() {
  digitalWrite(SS1, ON);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, ON);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);
}
/*                                            Fifth

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State |       5     | DE-Energize | DE-Energize | OFF | OFF | ON  | ON  |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void setGearFifth() {
  gearString = fifthString;
  TCCStatus = "ON ";
  digitalWrite(SS1, OFF);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, ON);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);
}
/*                                   Fifth to Sixth

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Upshift      | 5 - 6       |   Energized |   Energized | OFF | OFF | ON  | ON  |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void fifthToSixth() {
  digitalWrite(SS1, OFF);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, ON);
  digitalWrite(PCS1, energized);
  digitalWrite(PCS2, energized);
}
/*                                    Sixth to Fifth

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Down Shift   | 6 - 5       | DE-Energize | DE-Energize | OFF | OFF | ON  | ON  |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void sixthToFifth() {
  digitalWrite(SS1, OFF);
  digitalWrite(SS2, OFF);
  digitalWrite(SS3, ON);
  digitalWrite(TTC, ON);
  digitalWrite(PCS1, deEnergized);
  digitalWrite(PCS2, deEnergized);
}
/*                                            Sixth

      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Range Status | Logic State |    PCS1     |    PCS2     | SS1 | SS2 | SS3 | TCC |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
      | Steady State |       6     | Energized   | Energized   | OFF | OFF | ON  | ON  |
      +--------------+-------------+-------------+-------------+-----+-----+-----+-----+
*/
void setGearSixth() {
  gearString = sixthString;
  TCCStatus = "ON";
  digitalWrite(SS1, OFF);  //SS1
  digitalWrite(SS2, OFF); //SS2
  digitalWrite(SS3, ON);  //SS3
  digitalWrite(TTC, ON); //TCC
  digitalWrite(PCS1, energized);
  digitalWrite(PCS2, energized);
}

void loop() {

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(13, 1);

  downShift = digitalRead(DownShiftSignal);
  upShift = digitalRead(UpShiftSignal);

  if (downShift == 0) {
    if (CurrentGear > -2) {
      CurrentGear--;
      downShiftTrans();
    }

  }

  if (upShift == 0) {
    if (CurrentGear < 6) {
      CurrentGear++;
      upShiftTrans();
    }
  }

  lcd.print(gearString);
  lcd.setCursor(15,2);
  lcd.print(TCCStatus);
  Serial.print("Current Gear: ");
  Serial.println(gearString);
  Serial.print("UPshift: ");
  Serial.println(upShift);
  Serial.print("downshift: ");
  Serial.println(downShift);
  delay(200);

}
