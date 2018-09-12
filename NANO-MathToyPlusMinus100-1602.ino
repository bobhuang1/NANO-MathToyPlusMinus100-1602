#include <Wire.h>
#include <LiquidCrystal_I2C_Arduino.h>

#define BUTTONPIN  5
#define ALARMPIN 2
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C display(0x27, 16, 2);

int intCurrentStage = 0;
int intOperationType = 1;
int intFirstNumber = 1;
int intSecondNumber = 1;
int intResult1 = 0;
int intResult2 = 0;
int intResult1Position = 0;
int intResult2Position = 0;
int intQuestionCounter = 1;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 30;    // the debounce time; increase if the output flickers

byte multiply[8] = {
  0b00000,
  0b10001,
  0b01010,
  0b00100,
  0b00100,
  0b01010,
  0b10001,
  0b00000
};

byte plus[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b11111,
  0b00100,
  0b00100,
  0b00000,
  0b00000
};

byte minus[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte divide[8] = {
  0b00000,
  0b00100,
  0b00000,
  0b11111,
  0b00000,
  0b00100,
  0b00000,
  0b00000
};

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTONPIN, INPUT);
  pinMode(ALARMPIN, OUTPUT);

  display.begin();
  display.backlight();
  display.createChar(0, plus);
  display.createChar(1, minus);
  display.clear();
  display.setCursor(3, 0);
  display.print("Plus-Minus");
  display.setCursor(4, 1);
  display.print("Practice");
  randomSeed(analogRead(0));
  digitalWrite(ALARMPIN, HIGH);
  delay(1000);
  digitalWrite(ALARMPIN, LOW);
}

void loop()
{
  if (intQuestionCounter > 50)
  {
    intQuestionCounter = 1;
  }
  if (intCurrentStage == 0)
  {
    intOperationType = random(1, 3);
    intFirstNumber = random(1, 100);
    intSecondNumber = random(1, 100);
    if (intFirstNumber < intSecondNumber)
    {
      int intTemp = intFirstNumber;
      intFirstNumber = intSecondNumber;
      intSecondNumber = intTemp;
    }
    intResult1Position = 3;
    if (intFirstNumber < 10)
    {
      intResult1Position = intResult1Position + 1;
    }
    else if (intFirstNumber < 100)
    {
      intResult1Position = intResult1Position + 2;
    }
    else if (intFirstNumber < 1000)
    {
      intResult1Position = intResult1Position + 3;
    }
    intResult1Position = intResult1Position + 1;

    if (intSecondNumber < 10)
    {
      intResult1Position = intResult1Position + 1;
    }
    else if (intSecondNumber < 100)
    {
      intResult1Position = intResult1Position + 2;
    }
    else if (intSecondNumber < 1000)
    {
      intResult1Position = intResult1Position + 3;
    }
    intResult1Position = intResult1Position + 1;
    display.clear();
    display.setCursor(0, 0);
    if (intQuestionCounter < 10)
    {
      display.print("0");
    }
    display.print(intQuestionCounter);
    intQuestionCounter++;
    display.setCursor(3, 0);
    display.print(intFirstNumber);
    if (intOperationType == 1)
    {
      display.write(0);
      intResult1 = intFirstNumber + intSecondNumber;
    }
    else
    {
      display.write(1);
      intResult1 = intFirstNumber - intSecondNumber;
    }
    display.print(intSecondNumber);
    display.write(61);
    display.print("?");
    shortBeep();
    intCurrentStage++;
  }
  else if (intCurrentStage == 2)
  {
    intOperationType = random(1, 3);
    intFirstNumber = random(1, 100);
    intSecondNumber = random(1, 100);
    if (intFirstNumber < intSecondNumber)
    {
      int intTemp = intFirstNumber;
      intFirstNumber = intSecondNumber;
      intSecondNumber = intTemp;
    }

    intResult2Position = 3;
    if (intFirstNumber < 10)
    {
      intResult2Position = intResult2Position + 1;
    }
    else if (intFirstNumber < 100)
    {
      intResult2Position = intResult2Position + 2;
    }
    else if (intFirstNumber < 1000)
    {
      intResult2Position = intResult2Position + 3;
    }
    intResult2Position = intResult2Position + 1;

    if (intSecondNumber < 10)
    {
      intResult2Position = intResult2Position + 1;
    }
    else if (intSecondNumber < 100)
    {
      intResult2Position = intResult2Position + 2;
    }
    else if (intSecondNumber < 1000)
    {
      intResult2Position = intResult2Position + 3;
    }
    intResult2Position = intResult2Position + 1;

    display.setCursor(0, 1);
    if (intQuestionCounter < 10)
    {
      display.print("0");
    }
    display.print(intQuestionCounter);
    intQuestionCounter++;
    display.setCursor(3, 1);
    display.print(intFirstNumber);
    if (intOperationType == 1)
    {
      display.write(0);
      intResult2 = intFirstNumber + intSecondNumber;
    }
    else
    {
      display.write(1);
      intResult2 = intFirstNumber - intSecondNumber;
    }
    display.print(intSecondNumber);
    display.write(61);
    display.print("?");
    shortBeep();
    intCurrentStage++;
  }

  int reading = digitalRead(BUTTONPIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == HIGH)
      {
        if (intCurrentStage == 1)
        {
          display.setCursor(intResult1Position, 0);
          display.print(intResult1);
          shortBeep();
          intCurrentStage++;
        }
        else if (intCurrentStage == 3)
        {
          display.setCursor(intResult2Position, 1);
          display.print(intResult2);
          shortBeep();
          delay(1500);
          intCurrentStage = 0;
        }
      }
    }
  }
  lastButtonState = reading;
}

void shortBeep() {
  digitalWrite(ALARMPIN, HIGH);
  delay(60);
  digitalWrite(ALARMPIN, LOW);
}

